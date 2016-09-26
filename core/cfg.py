import ConfigParser

__DEFAULT_SECTION = 'global'

__cfg = ConfigParser.RawConfigParser()
__cfg.add_section(__DEFAULT_SECTION)

# Set some defaults

# Pseudo-methods
def readFrom(cfgfile):
    __cfg.read(cfgfile)

def get(option, section = __DEFAULT_SECTION):
    return __cfg.get(section, option)

def getInt(option, section = __DEFAULT_SECTION):
    return __cfg.getint(__DEFAULT_SECTION, option)

def getBoolean(option, section = __DEFAULT_SECTION):
    return __cfg.getboolean(section, option)

def set(option, value, section = __DEFAULT_SECTION):
    return __cfg.set(section, option, value)

