import sys

from twisted.python import log, usage
from twisted.internet.protocol import ClientFactory
from twisted.protocols.policies import TimeoutMixin
from twisted.protocols.basic import LineOnlyReceiver
from twisted.internet import reactor
import rrdtool

_ANSWER_TIMEOUT        = 4;
_BETWEENSAMPLES_TIME   = 60;

class statsProtocol(LineOnlyReceiver, TimeoutMixin):

    def __init__(self):
        self.sampling = False

    def lineReceived(self, data):
        print "Sample data received: ", repr(data)
        # cancel read timeout
        self.sampling = False
        self.setTimeout(_BETWEENSAMPLES_TIME)
        # parse and write response to RRD

    def sendLine(self):
        buf = "SAMPLE 0\r\n"
        # set timeout
        self.sampling = True
        self.setTimeout(_ANSWER_TIMEOUT)
        # write command to serial connection
        self.transport.write(buf)
        print "Command sent:", buf

    # Called when timeout occurs
    def timeoutConnection(self):
        if self.sampling:
            # Quit sampling
            self.sampling = False
            self.setTimeout(_BETWEENSAMPLES_TIME)
        else:
            self.sendLine()

    def connectionMade(self):
        print "Connection successful"
        # Start sample cycle
        self.sendLine()

    def connectionLost(self, reason):
        pass


class statsClientFactory(ClientFactory):

    protocol = statsProtocol

    def __init__(self):
        pass



def main():
    copts = CoreOptions()

    try:
        copts.parseOptions()
    except usage.UsageError, errortext:
        print '%s: %s' % (sys.argv[0], errortext)
        print '%s: Try -h for usage details.' % (sys.argv[0])
        raise SystemExit, 1

    sf = copts.opts['statsfile']
    reactor.connectTCP('localhost', 10010, statsClientFactory())
    reactor.run()

if __name__ == '__main__':
    main()
