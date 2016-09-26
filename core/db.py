from twisted.python import log
from pysqlite2 import dbapi2 as sqlite3

class dB(object):

    def __init__(self, dbfile = None):
        # TODO: check for db existence & validity
        # TODO: handle db errors
        # TODO: implement more db types (or use SQLAlchemy)

        if (dbfile):
            self.setDSN(dbfile)

        self.queries = {'getMachineIdsForBaseStation' :
                            "SELECT A.id FROM machine A, basestation B \
                            WHERE A.is_active=1 \
                            AND A.basestation_id=B.id AND B.name=?",

                        'getSerialPortForMachine' :
                            "SELECT A.id, A.name, B.port, B.baudrate \
                            FROM machine A, serialport B \
                            WHERE A.id=? AND A.control='serialport' \
                            AND A.is_active='1' AND A.id=B.machine_id"}

    def setDSN(self, dbfile):
        log.msg("Setting data source to: %s" % dbfile)
        self.con = sqlite3.connect(dbfile)
        self.con.text_factory = sqlite3.OptimizedUnicode

    def getMachineIdsForBaseStation(self, basestation):
        cur = self.con.cursor()
        cur.execute(self.queries['getMachineIdsForBaseStation'], [basestation])
        return cur.fetchall()

    def getSerialPortForMachine(self, machine_id):
        cur = self.con.cursor()
        cur.execute(self.queries['getSerialPortForMachine'], (machine_id))
        return cur.fetchone()


__db = dB()

def setDataSource(dbfile):
    __db.setDSN(dbfile)
    return

def getMachineIdsForBaseStation(basestation):
    return __db.getMachineIdsForBaseStation(basestation)

def getSerialPortForMachine(machine_id):
    return __db.getSerialPortForMachine(machine_id)

