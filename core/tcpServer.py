from twisted.internet.protocol import Factory
from twisted.protocols.basic import LineReceiver
from serialClient import serialClient
import re

def displaymatch(match):
    if match is None:
        return None
    return '<Match: %r, groups=%r>' % (match.group(), match.groups())


class tcpServer(LineReceiver):

    def __init__(self):
        self.regex0 = re.compile(r"^(QUIT)$", re.IGNORECASE)
        self.regex1 = re.compile(r"^(HELP)$", re.IGNORECASE)
        self.regex2 = re.compile(r"^(PRINT)$", re.IGNORECASE)
        self.regex3 = re.compile(r"^(PULSEON|PULSEOFF|SAMPLE)\s+(\d)$", re.IGNORECASE)

    def connectionMade(self):
        ip = self.transport.getPeer()
        print "New connection from ", ip.host

    def sendLine(self, cmd):
        buf = cmd.strip() + "\r\n"
        self.transport.write(buf)

    # Called when timeout occurs
    def lineReceived(self, line):

        ip = self.transport.getPeer()
        print "Data: ", repr(line), " received from ", ip.host

        # Parse & verify 
        # Execute and return response
        # 
        # Allowed commands:
        # ---------------
        # print                 : prints a list of managed devices
        # sample #              : returns a single reading from the device #
        # pulseon #             : sends a 2 second ON pulse to device #
        # pulseoff #            : sends a 2 second OFF pulse to device #

        cmd = line.strip().upper()

        if cmd:

            res = self.regex0.match(cmd)
            # QUIT command received
            if res is not None:
                self.transport.loseConnection()
                return

            res = self.regex1.match(cmd)
            # HELP command received
            if res is not None:
                self.sendLine("HELP: short help text")
                self.sendLine("QUIT: close connection")
                self.sendLine("PRINT: list connected machines")
                self.sendLine("PULSEON [N]: send on pulse to machine N")
                self.sendLine("PULSEOFF [N]: send off pulse to machine N")
                self.sendLine("SAMPLE [N]: sample data from machine N")
                return

            res = self.regex2.match(cmd)
            # PRINT command received
            if res is not None:
                serialClient.listAll(self)

            else:
                res = self.regex3.match(cmd)
                if res is not None:
                    serialClient.notify(self, int(res.groups()[1]), res.groups()[0])
                else:
                    self.sendLine("Invalid command")

    def connectionLost(self, reason):
        ip = self.transport.getPeer()
        print "Connection lost from ", ip.host

class tcpServerFactory(Factory):
    protocol = tcpServer

    def __init__(self):
        pass
