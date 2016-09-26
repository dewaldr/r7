#-- Global modules
from twisted.python import log
import cfg, db

#-- Global symbols
_appname = 'r7core'
_appversion = '0.1'

#-- Signal handlers
def _stopDaemon(sig, frame):
    raise SystemExit('terminated by signal %d' % int(sig))

#-- Server startup
def main():
    from twisted.internet import reactor
    from twisted.internet.serialport import SerialPort
    import serial                                           # for SerialException
    from twisted.python.logfile import DailyLogFile
    from serialClient import serialClient
    from tcpServer import tcpServer, tcpServerFactory

    machine_ids = db.getMachineIdsForBaseStation(cfg.get('basestation'))

    for machine_id in machine_ids:
        sport = db.getSerialPortForMachine(machine_id)
        #id, name, path, baudrate
        name = sport[1].encode('ascii', 'ignore')
        print "Machine found: ", sport[0], name

        try:
            SerialPort(serialClient(int(sport[0]),name), sport[2], reactor, baudrate=sport[3])
        except serial.SerialException, errortext:
            print("Unable to open serial port: %s" % errortext)

    reactor.listenTCP(cfg.getInt('port'), tcpServerFactory(), interface=cfg.get('interface'))
    reactor.run()

#-- Command line detection
if __name__ == "__main__":
    import argparse, daemon, sys
    from twisted.python.logfile import DailyLogFile
    from setproctitle import setproctitle

    parser = argparse.ArgumentParser(description="Robot7 core server")
    parser.add_argument('-d','--daemon',help='Detach from console',action='store_true')
    parser.add_argument('-v','--verbose',help='Be more verbose when running in console',action='store_true')
    parser.add_argument('-c','--cfgfile',help='Read configuration from file',default='/etc/monitor/mcore.cfg',type=str)
    args = parser.parse_args()

    cfg.readFrom(args.cfgfile)

    # Set process name
    setproctitle(_appname)

    # Start logging
    if (args.verbose and not args.daemon):
        log.startLogging(sys.stdout)
        f = DailyLogFile.fromFullPath(cfg.get('logfile'))
        log.addObserver(log.FileLogObserver(f).emit)
    else:
        log.startLogging(DailyLogFile.fromFullPath(cfg.get('logfile')))

    # Connect to database
    db.setDataSource(cfg.get('datasource', 'database'))

    # Invoke the application
    if (args.daemon):
        with daemon.DaemonContext(): main()
    else:
        main()

