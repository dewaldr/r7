from twisted.protocols.policies import TimeoutMixin
from twisted.protocols.basic import LineOnlyReceiver

_TIMEOUT = 3;

class serialClient(LineOnlyReceiver, TimeoutMixin):
    instances = {}

    def __init__(self, key, name):
        self.key = key
        self.name = name
        self.consumer = None

    def connectionMade(self):
        serialClient.instances[self.key] = self
        print "Serial connection successful: ", self.key

    def connectionLost(self, reason):
        del serialClient.instances[self.key]
        print "Serial connection lost: ", self.key, reason

    def lineReceived(self, data):
        print "Serial line received: ", repr(data)
        # cancel timeout
        self.setTimeout(None)
        # write response to commanding TCP connection
        self.consumer.sendLine(data)
        self.consumer = None

    def sendLine(self, cmd):
        buf = cmd.strip() + "\r\n"
        # set timeout
        self.setTimeout(_TIMEOUT)
        # write command to serial connection
        self.transport.write(buf)
        print "Serial line sent:", buf

    # Called when timeout occurs
    def timeoutConnection(self):
        self.setTimeout(None)
        self.consumer = None
        print "Serial connection timed out"

    @staticmethod
    def listAll(consumer):
        print "List serial clients from: ", repr(consumer)
        buf = "Serial client(s): "
        for key in serialClient.instances.keys():
            buf += str(key) + ":" + serialClient.instances[key].name + " "
        consumer.sendLine(buf)

    @staticmethod
    def notify(consumer, key, data):
        if serialClient.instances.has_key(key):
            if serialClient.instances[key].consumer == None:
                serialClient.instances[key].consumer = consumer
                serialClient.instances[key].sendLine(data)
                print "Command sent: ", repr(data)
            else:
                consumer.sendLine("Resource busy")
        else:
            consumer.sendLine("Resource unknown")

