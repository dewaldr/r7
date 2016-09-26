from twisted.python import usage

class coreUsage(usage.Options):
    optFlags = [
        ['help', 'h', 'Usage help message'],
    ]
    optParameters = [
        ['cfgfile', 'c', '/etc/robot7/r7core.conf', 'robot7 core server configuration file']
    ]

class coreConfig(object):
    def __init__(self):
        self.config = {}

    def readFrom(self, filename):
        # TODO: check for file existance, readability
        execfile(filename, self.config)  

    def get(self, key):
        return self.config[key]

    def getAll(self):
        return self.config

