#include "tclap/CmdLine.h"
#include <wiringPi.h>
#include <iostream>
#include <string>

#include <boost/interprocess/sync/scoped_lock.hpp>
#include <boost/interprocess/sync/named_mutex.hpp>

using namespace TCLAP;
using namespace std;
using namespace boost::interprocess;

bool _verbose;
string _command;

int _onPin = 0;
int _offPin = 1;

void parseOptions(int argc, char** argv);

int main(int argc, char** argv)
{
	 struct mutex_remove
     {
		mutex_remove() { named_mutex::remove("pumpctl_named_mutex"); }
		~mutex_remove(){ named_mutex::remove("pumpctl_named_mutex"); }
	} remover;

	try {

		//Open or create the named mutex
		named_mutex mutex(open_or_create, "pumpctl_named_mutex");
		//Ensure exclusive execution
		scoped_lock<named_mutex> lock(mutex);

		parseOptions(argc,argv);

		wiringPiSetup();
		pinMode( _onPin, OUTPUT );
		pinMode( _offPin, OUTPUT );

		if ( _command.compare( "on" ) == 0 ) {

			if ( _verbose == true ) cout << "Switching on pump -> "; cout << flush;

			// Close NO relay connected to _onPin for 2 seconds	
		    digitalWrite( _onPin, HIGH ); 
		    delay( 2000 );
		    digitalWrite( _onPin, LOW );

			if ( _verbose == true ) cout << "done" << endl;
		}
		else if ( _command.compare( "off" ) == 0 ) {

			if ( _verbose == true ) cout << "Switching off pump -> "; cout << flush;

			// Open NC relay connected to _offPin for 2 seconds	
		    digitalWrite( _offPin, HIGH ); 
		    delay( 2000 );
		    digitalWrite( _offPin, LOW );

			if ( _verbose == true ) cout << "done" << endl;
		}
		else if ( _command.compare( "status" ) == 0 ) {
		}
	}
	catch (interprocess_exception &ex) {
		std::cout << ex.what() << std::endl;
		return 1;
	}

	return 0;
}


void parseOptions(int argc, char** argv)
{
	try {
	    // Define the command line object.
    	CmdLine cmd("Command line pump controller", ' ', "0.1");

    	// Verbose flag
    	SwitchArg verboseArg( "v", "verbose", "Print additional information", false );
    	cmd.add( verboseArg );

    	// Pump command: Values - on, off, status
    	vector<string> allowed;
    	allowed.push_back( "on" );
    	allowed.push_back( "off" );
    	allowed.push_back( "status" );
    	ValuesConstraint<string> allowedVals( allowed );

    	UnlabeledValueArg<string> commandArg( "command", "Command to execute", true, "status", &allowedVals, false );
    	cmd.add( commandArg );

    	// Parse the args.
    	cmd.parse( argc, argv );

    	// Get the value parsed by each arg.
    	_verbose = verboseArg.getValue(); 
    	_command = commandArg.getValue();

	} 
	catch ( ArgException& e) { 
		cerr << "error: " << e.error() << " for arg " << e.argId() << endl; 
	}
}

