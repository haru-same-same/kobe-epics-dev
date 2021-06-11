//
//  A simple example
//
#include <cafe.h>

#include "callbacks.h" //ca callback funtions

// To run this example at PSI
// type on the command line: cao

// Test with 6 PVS
// PV_X1,PV_Y1 are scalar numerics
// PV_WF is a vector numeric
// PV_MBBI is an mbbi record with a number of enum vakues
// PV_X1_DESC is of data type dbr_string_t 
// PV_JOKE is a non-existant channel (used to illustrate CAFE response is such cases)

#define PV_X1      "ARIDI-BPM-01LE:X-AVG"
#define PV_Y1      "ARIDI-BPM-01LE:Y-AVG"
#define PV_WF      "ARIDI-BPM-01LE:WF-INT-1"   //256 elements
#define PV_MBBI    "ARIDI-BPM-01LE:GET-ENABLE" //mmbi
#define PV_X1_DESC "ARIDI-BPM-01LE:X-AVG.DESC" //dbr_string_t
#define PV_JOKE    "PV:JOKE"

#define NHANDLES 6 // should match number of pvs above that we are to open.

#define __METHOD__ "cafeTest.cc"


//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//  (1) Establishing connections to EPICS Process Variables (PVs) 
//  (2) Simple single channel operations (and the PVData Object)
//      Understanding the Status Error Code 
//  (3) Waveforms and Arrays
//  (4) Handling Enumerated Types
//  (5) Control system parameters, i.e., operating limits, engineerimg units
//  (6) Retrieve channel medadata, e.g., data type, native no. elements, connection state, read/write access, etc.
//  (7) Monitors, either with or without user supplied callbacks (in essence, matlab scripts)
//  (8:END) Gracefully terminate CAFE
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%



int main( int argc, char *argv[] )
{
	using namespace std;


	string pvArray[NHANDLES]={PV_X1,PV_Y1,PV_WF,PV_MBBI,PV_X1_DESC,PV_JOKE};
	unsigned int hArray [NHANDLES];

	vector<unsigned int>   hV;    //vector of handles
	vector<string>         pvV;	  //corresponding vector of pvs
	vector<unsigned short> stateV;//corresponding vector of connection states 
			
	unsigned int handle;
	         int status;
					 
	//Instantiate CAFE
	CAFE * cafe = new CAFE();


	//------------------------------------------------------------
	//(1) Establishing connections to EPICS Process Variables (PVs)
	//------------------------------------------------------------
	
	cout << "---------------------------------------------------------------------" << endl;
	cout << "START: (1) Establishing connections to EPICS Process Variables (PVs) " << endl;
	cout << "---------------------------------------------------------------------" << endl;
	
		
	// An open channel operation will pend the ioc for a default amount of time given by:
	// cafe->channelOpenPolicy.getDefaultTimeout() 
	// else otherwise specified.	
	cafe->channelOpenPolicy.setTimeout(0.1); //pend 0.1 seconds
				
	// Open one channel
	// An exception is ***not*** thrown if the channel does not connect!	 
	  
	// The open method returns a handle as an output argument
	// Subsequent cafe method invocations on a channel may be made on a per handle or pv basis
		
	try {       		
		cafe->open(pvArray[0].c_str(), handle);			
	}
	catch(CAFEException_open & e) {
		cout << e.what() << endl;
		exit(1);
	}
	
	try{  
		//For multiple invocations of cafe->open it is more efficient and less of a
		//time sink to first prepare the messages before flushing the message buffer 
		//once at the end.
		//This can be done by configuring the channelopenPolicy class accordingly.
		//See policies.h
		//Set open policy so as to create virtual circuits in one step
		//cafe->channelOpenPolicy.setWhenToFlushSendBuffer(FLUSH_DESIGNATED_TO_CLIENT);
		//cafe->channelOpenPolicy.setFlushSendBufferKind(WITH_PEND_EVENT);
		//cafe->channelOpenPolicy.setTimeout(1.0);
		//cafe->open(...)			
		//cafe->openNow() // Now send messages to IOC 
		  
		//However, for convenience open policy has been conveniently packaged so that 
		//only the openPrepare method need be called a priori 
		
		cafe->openPrepare();		 		 
		cafe->open(pvArray,  hArray, NHANDLES);					
		
		// Vector interfaces also exist
		// vector<string> pvVector
		// vector<unsigned int> hVector
		// cafe->open(pvVector,  hVector);		
			
		//Only after the following method is invoked are the open requests made
		//or if the ca message buffer becomes full and is consequently flushed

		//cafe->openNow() ; // and wait for cafe->channelOpenPolicy.getTimeout()
		//or
		cafe->openNowAndWait(0.5);  //wait for specified time, here 0.5 seconds
						
		//Is the given channel connected? Returns true/false
		bool ifYes= cafe->isChannelConnected(hArray[0]); 
			
		//Are all channels connected? Returns true/false
		ifYes= cafe->allChannelsConnected();
						
		if (!ifYes) {
			cafe->printDisconnectedHandles();
		}
		 
		//Handle helper functions also provide information on PV handles and their connection states
		//hV and pvV vectors are used further down this code.
		
		status=cafe->getHandleHelper().getDisconnectedHandles(hV, pvV); //output args return handles and their pv names
		status=cafe->getHandleHelper().getHandles(hV, pvV); //output args return handles and their pv names
		//output args return handles, their pv names, and connection states
		status=cafe->getHandleHelper().getHandleStates(hV, pvV,stateV);  
								
	}
	catch(CAFEException_open & e) {
		cout << e.what() << endl;
	}
		
		
	// To close channels within a given ca_client_context
		
	// To close a single Channel
	// cafe->close(hArray[0]);
		
	// To close an array of channels
	// cafe->closeChannels(hArray, NHANDLES);
	
	// To close all channels
	// cafe->closeChannels();
			
	// If you wish to close channels that are in other threads 
	// (i.e., with other ca_client_context)
	// use instead the following methods:	
	// To close a single Channel irrespective of ca_client_context
	// cafe->closeHandle(hArray[0[);
		
	// To close an array of handles 
	// cafe->closeHandles(hArray, NHANDLES);
	// 
	
	// To close all handles
	// cafe->closeHandles();
			
		
	// Methods also exist that close a vector of handles - not shown
		
		
	cout << "---------------------------------------------------------------------" << endl;
	cout << "STOP:  (1) Establishing connections to EPICS Process Variables (PVs) " << endl;
	cout << "---------------------------------------------------------------------" << endl;
	
	
	//------------------------------------------------------------
	//(2) Basic Single Channel Operations
	//------------------------------------------------------------
	
	cout << "------------------------------------------------------------" << endl;
	cout << "START: (2) Basic Single Channel Operations " << endl;
	cout << "------------------------------------------------------------" << endl;
		
		
		
	//------------------------------------------------------------
	//Data retrieval methods returning a scalar value
	//------------------------------------------------------------
	
	double d;
	//get PV value by handle
	status=cafe->get(hArray[0], d);
			
	if (status != ICAFE_NORMAL) {
		cout << "Status = " << status << "; indicates an error at " << __METHOD__ << "//" << __LINE__ << endl;
		cafe->getCafeStatus().report(status);	
		//or
		cafe->printStatus(handle,status);	
						
		//To explicitly check on timeout error
		if (cafe->getCafeStatus().isTimeout(status)) {
			cout << "Above is a timeout error" << endl;	
		}
			
	}
	else {
		cout << "Value d=" << d << endl; 
	}
			
	//get PV value, alarm, timeStamp by handle	
	float f; short alarmStatus, alarmSeverity; epicsTimeStamp ets;
	status=cafe->get(handle, f, alarmStatus, alarmSeverity, ets);
			
	if (status != ICAFE_NORMAL) {
		cout << "Status = " << status << "; indicates an error at " << __METHOD__ << "//" << __LINE__ << endl;
		cafe->getCafeStatus().report(status);	
		//or
		cafe->printStatus(hArray[0],status);	
	}
	else {
		cout << "Value f=" << f << endl;
		cout << "AlarmStatus= " << cafe->getEpicsAlarmCondition().asString(alarmStatus) << endl;
		cout << " AlarmSeverity=" << cafe->getEpicsAlarmSeverity().asString(alarmSeverity) << endl;
		cout << " TimeStamp= " << ets.secPastEpoch << " sec. " << ets.nsec << " nsec " << endl;
	}
		
	d=0;
	//get by pvname
	status=cafe->get(pvArray[0].c_str(), d);
	if (status != ICAFE_NORMAL) {
		cout << "Status = " << status << "; indicates an error at " << __METHOD__ << "//" << __LINE__ << endl;
		cafe->getCafeStatus().report(status);	
		//or
		cafe->printStatus(handle,status);	
			
		//To explicitly check on timeout error
		if (cafe->getCafeStatus().isTimeout(status)) {
			cout << "Above is a timeout error" << endl;	
		}
	}	
	else {
		cout << "Value d=" << d << endl; 
	}
		
		
	//Handle and PV Mappring, i.e., to interchange between handle and pvName
	             handle = cafe->getHandleFromPV(pvArray[0].c_str());
	const char * pvName = cafe->getPVFromHandle(handle);
		  
		
	//------------------------------------------------------------	
	//Set data
	//The set method is able to intrepret all data types, and can 
	//cater for scalar values and arrays
	//------------------------------------------------------------
	
	//by pvName
	status=cafe->set(PV_X1, 0.02453);
	
	//by Handle
	status=cafe->set(handle, 0.02453);
	
	if (status!=ICAFE_NORMAL) {
		cafe->getCafeStatus().report(status);
	}
	//or to print an error with pv name (handle as input argument)
	//cafe->printStatusIfError(handle,status);
	//or
	//cafe->printStatusIfError(cafe->getHandleFromPV(PV_X1),status);
	//or another way to print an error with pv name 
	cafe->printStatusIfError(PV_X1,status);
		
		
	//------------------------------------------------------------	
	//Data retrieval methods returning structured data 
	//and set with PvDataHolder object
	//------------------------------------------------------------
	
		
	//This example acts on a waveform
				
	PVDataHolder pvd;
	//if wf, ***must*** allocate memory as follows:
	pvd.setNelem(cafe->getNelemNative(hArray[2]));
	//status=cafe->get(pvArray[2].c_str(), pvd);
	status=cafe->get(hArray[2], pvd);
		
		
	if (status==ICAFE_NORMAL) {
		
		pvd.print(10); //Show only 10 elements of the waveform
		
		//The PVDataHolder
		cout << "pvd.getAsString()" << endl;
		for (int i=0; i<min(5,(int) cafe->getNelemRequest(hArray[2])); ++i) {
			cout  << pvd.getAsString(i) << " [" << i << "] ";
		}	
		cout << endl;
		//First Element Only
		cout << "pvd.getAsDbr_string_t() = " << pvd.getAsDbr_string_t() << endl;
		cout << "pvd.getAsDouble()       = " << pvd.getAsDouble() << endl;
		cout << "pvd.getAsFloat()        = " << pvd.getAsFloat() << endl;
		cout << "pvd.getAsChar()         = " << (unsigned short) pvd.getAsChar() << endl;
		cout << "pvd.getAsShort()        = " << pvd.getAsShort() << endl;
		cout << "pvd.getAsUShort()       = " << pvd.getAsUShort() << endl;		
		cout << "pvd.getAsLong()         = " << pvd.getAsLong() << endl;
		cout << "pvd.getAsULong()        = " << pvd.getAsULong() << endl;
		cout << "pvd.getAsLongLong()     = " << pvd.getAsLongLong() << endl;
		cout << "pvd.getAsULongLong()    = " << pvd.getAsULongLong() << endl;
		
		boost::shared_ptr<vector<double> > spVd = pvd.getAsVDouble();		
					
		cout << "pvd.getAsVDouble()      = " << spVd.get()[0][0] << " [0] " ;
		if (spVd.get()[0].size()>1) {
			cout << spVd.get()[0][1] << " [1] ";
		}			
		cout << endl;
		vector<double> * vd= spVd.get();
		cout << "pvd.getAsVDouble()      = " << vd[0][0] << " [0] " ;
		if (vd[0].size()>1 ) {
			cout << vd[0][1] << " [1] ";	
		}
		cout << endl;
		boost::shared_ptr<vector<float> >  spVf = pvd.getAsVFloat();
		cout << "pvd.getAsVFloat()       = " << spVf.get()[0][0] << " [0] " ;
		if (spVf.get()[0].size()>1) {
			cout <<  spVf.get()[0][1] << " [1] ";
		}
		cout << endl;
		 
		vector<float> * vf= spVf.get();
		cout << "pvd.getAsVFloat()       = " << vf[0][0] << " [0] " ;
		if (vf[0].size()>1) {
			cout << vf[0][1] << " [1] ";	
		}
		cout << endl;
		
		cout << "pvd.getPVName()         = " << pvd.getPV() << endl;
		cout << "pvd.getPVAlias()        = " << pvd.getPVAlias() << endl; 
		cout << "pvd.getDevice()         = " << pvd.getDevice() << endl;
		cout << "pvd.getAttribute()      = " << pvd.getAttribute() << endl; 
		cout << "pvd.getNelem()          = " << pvd.getNelem() << endl;	
		cout << "pvd.getSize()           = " << pvd.getSize() << endl; //Size of PVDataHolder container
		cout << "pvd.getVal()            = " << pvd.getVal() << endl; //CAFE_DATATYPE_UNION_SEQ type
		cout << "pvd.getVal()[0].f       = " << pvd.getVal()[0].f << endl; // extract assuming a float
		cout << "pvd.getDataType()       = " << pvd.getDataType() << endl; // CAFE_DATATYPE
		
		cout << "pvd.getStatus()         = " << pvd.getStatus() << endl;
		
		cout << "pvd.getHasAlarm         = " << pvd.getHasAlarm() << endl; // true if Alarm/Severity data returned 
		cout << "pvd.getAlarmStatus()    = " << pvd.getAlarmStatus() << endl;
		cout << "pvd.getAlarmSeverity()  = " << pvd.getAlarmSeverity() << endl; 		
		cout << "pvd.getHasTS            = " << pvd.getHasTS() << endl; // true if Epics Timestamps data returned 
		ets= pvd.getEpicsTimeStamp(); 
		cout << "epicsTimeStamp          = " << ets.secPastEpoch << " " << ets.nsec << endl;

		//cout << "pvd.getEnumIntegerValueAsString()  = " << pvd.getEnumIntegerValueAsString() << endl; //for mbbi/o records
		cout << "pvd.concatToString()    = " << pvd.concatToString() << endl; // for waveforesm with data type dbr_chart_t
		cout << "(Does same as above)    = " << pvd.getWFAsString()  << endl; // for waveforesm with data type dbr_chart_t
		
		 
		//------------------------------------------------------------	
		//Alarm, status/severity values 
		//------------------------------------------------------------
		
		//Alarm status and severities have value/name (int/string) pairs.
		//The string equivalent of a given value may be gotten as shown 				
		
		cout << "Alarm Status    = " << cafe->getEpicsAlarmCondition().asString(pvd.getAlarmStatus()) << endl;
		cout << "Alarm Severity  = " << cafe->getEpicsAlarmSeverity().asString(pvd.getAlarmSeverity()) << endl;
		
		//Information concerning the meaning of the status of the cafe operation
		cout << cafe->getCafeStatus().severity(pvd.getStatus()) << endl; //Whether WARNING, ERROR, OR INFO
		cout << cafe->getCafeStatus().asString(pvd.getStatus()) << endl;
		cout << cafe->getCafeStatus().code(pvd.getStatus()) << endl;
		cout << cafe->getCafeStatus().info(pvd.getStatus()) << endl;
		cout << cafe->getCafeStatus().message(pvd.getStatus()) << endl;
			
		//print summary
		cout << "<---- SUMMARY --> " << endl;
		cafe->getCafeStatus().report(pvd.getStatus());
		cout << "<---- END SUMMARY --> " << endl;
				
		} // if status=ICAFE_NORMAL	
		else  {
			cout << "Status = " << status << "; indicates an error at " << __METHOD__ << "//" << __LINE__ << endl;
			cafe->getCafeStatus().report(status);		
		}
	
	//Get with user supplied callback
	ChannelRequestPolicy channelRequestPolicyLocal;	
	//Supply user defined handle	
	channelRequestPolicyLocal.setHandler(handlerGet);
	channelRequestPolicyLocal.setPolicy(CAFENUM::FLUSH_AFTER_EACH_MESSAGE,
									  CAFENUM::WAIT, CAFENUM::WITH_CALLBACK_USER_SUPPLIED); 
	//Set this callback function for one handle								
	status=cafe->getPolicyHelper().setChannelRequestPolicyGet(hArray[0], channelRequestPolicyLocal);
	//Set this callback for ALL handles
	//status=cafe->getPolicyHelper().setChannelRequestPolicyGet(channelRequestPolicyLocal);
		
	d=0;
	status=cafe->get(hArray[0], d);
	if (status != ICAFE_NORMAL) {
		cout << "Status = " << status << "; indicates an error at " << __METHOD__ << "//" << __LINE__ << endl;
		cafe->getCafeStatus().report(status);	
		//or
		cafe->printStatus(handle,status);	
			
		//To explicitly check on timeout error
		if (cafe->getCafeStatus().isTimeout(status)) {
			cout << "Above is a timeout error" << endl;	
		}		
	}	
	else {
		cout << "This print statement will actually appear before that in callback " << endl;
		cout << "USER_SUPPLIED_CALLBACK: Value d=" << d << endl; 
	}
		
	cout << "------------------------------------------------------------" << endl;
	cout << "END: (2) Basic Single Channel Operations " << endl;
	cout << "------------------------------------------------------------" << endl;
				
	//------------------------------------------------------------
	//(3) Waveforms and Arrays
	//------------------------------------------------------------
		
	cout << "------------------------------------------------------------" << endl;
	cout << "START: (3) Waveforms and Arrays  " << endl;
	cout << "------------------------------------------------------------" << endl;
						
	//------------------------------------------------------------	
	//Data retrieval methods returning a waveform array
	//------------------------------------------------------------
	
	
	//inwf array of 20 doubles previous defined
	double inwf [20] = {1.1,2.2,3.3,4.4,5.5,6.6,7.7,8.8,9.9,10, 11, 12, 13,14,15,16,17,18,19,20};
		
	//copy to vector to illustrate other possibilities
			
	vector<double> dvv (inwf, inwf+20);			
	//Be sure to assign memory
	cafe->getHandleHelper().setNelem(hArray[2], 20);
	//Actually set method with vectors, will check the size of the vector
	//and adjust the setNelem accordingly if required
		
	//Methods invoking vectors are by Handle
	status=cafe->set(cafe->getHandleFromPV(PV_WF), dvv);
	//or
	//status=cafe->set(hArray[2], inwf);
	if (status != ICAFE_NORMAL) {
		cout << "Status = " << status << "; indicates an error at " << __METHOD__ << "//" << __LINE__ << endl;
		cafe->getCafeStatus().report(status);		
	}
	
	//reset Number of elements to native value for get operation
	cafe->getHandleHelper().setNelem(hArray[2], cafe->getNelemNative(hArray[2]) );
			
	//Allocate	
	double * dwf = new double[cafe->getNelemNative(hArray[2])]; 
		
	status=cafe->get(PV_WF, dwf);
		
	if (status != ICAFE_NORMAL) {
		cout << "Status = " << status << "; indicates an error at " << __METHOD__ << "//" << __LINE__ << endl;
		cafe->getCafeStatus().report(status);		
	}
	else {
		cout << PV_WF << " successfully read out " << endl;
		cout << "First five elements have values: " << endl;
		for (int i=0; i<std::min(10,(int) cafe->getNelemRequest(hArray[2])); ++i) {
			cout << dwf[i] << " [" << i << "] ";
		}
		cout << endl;
	}
				
	
	//One may also set the number of elements to retrieve an offset
	//int nelem  =cafe->getHandleHelper().setNelemToNative(hArray[2]);
	int nelem  = cafe->getHandleHelper().setNelem (hArray[2], 10); //returns max(10, cafe->getNelemNative(handle))
	//and offset
	int offset = cafe->getHandleHelper().setOffset(hArray[2], 2);
	//by Handle
	status=cafe->get(hArray[2], dwf);
	if (status != ICAFE_NORMAL) {
		cout << "Status = " << status << "; indicates an error at " << __METHOD__ << "//" << __LINE__ << endl;
		cafe->getCafeStatus().report(status);		
	}
	else {
		cout << PV_WF << " sucessfully read out " << endl;
		cout << "Elements 2 to 12 of WF (recall offset=2, nelem=10) have values:" << endl;
		for (int i=0; i<nelem; ++i) {
			cout << dwf[i] << " [" << i << "] ";
		}
		cout << endl;
	}
	//reset offset for read and the nelemtonative
	cout << "Resetting nelem for wf to native value: " << cafe->getHandleHelper().setNelemToNative(hArray[2]) 
	     << " and resetting offset to: " <<  cafe->getHandleHelper().setOffset(hArray[2], 0) << endl;
	
	//by handle	
	status=cafe->set(cafe->getHandleFromPV(PV_WF), dvv);
	
	if (status != ICAFE_NORMAL) {
		cout << "Status = " << status << "; indicates an error at " << __METHOD__ << "//" << __LINE__ << endl;
		cafe->getCafeStatus().report(status);		
	}
	delete  dwf;
		
			
	//------------------------------------------------------------	
	//Data retrieval methods returning a std::vector
	//------------------------------------------------------------
	vector<double> dV;
	dbr_short_t alarmSta; dbr_short_t alarmSe; epicsTimeStamp tsta;
	
		
	cout << "set nelem to 16, get: " << cafe->getHandleHelper().setNelem(hArray[2],16) << endl;;				
	cout << "set nelem cache to 10, get: " << cafe->getHandleHelper().setNelemToRetrieveFromCache(hArray[2],10) << endl;
		
		
	//Will retrieve 16 elements	
	status=cafe->get(hArray[2], dV);
		
	if (status != ICAFE_NORMAL) {
		cout << "Status = " << status << "; indicates an error at " << __METHOD__ << "//" << __LINE__ << endl;
		cafe->getCafeStatus().report(status);		
	}
	else {
		cout << "dV.size will be 16 -> " << dV.size() << endl;
	}
		
	//Will retrieve 12 elements from cache
	status=cafe->getCache(hArray[2], dV,   alarmSta, alarmSe, tsta);	
		
	cout << "dV.size will be 10 -> " << dV.size() << endl;	
	if (status != ICAFE_NORMAL) {
		cout << "Status = " << status << "; indicates an error at " << __METHOD__ << "//" << __LINE__ << endl;
		cafe->getCafeStatus().report(status);		
	}
	else {
		cout << PV_WF << " successfully read out " << endl;
		cout << "Elements 1-5 of WF  have values:" << endl;
		for (int i=0; i<min(20, (int) dV.size()); ++i) {
			cout << dV[i] << " [" << i << "] ";
		}
		cout << endl;
	}
								
	cout << "------------------------------------------------------------" << endl;
	cout << "END: (3) Waveforms and Arrays " << endl;
	cout << "------------------------------------------------------------" << endl;
	
		
	cout << "------------------------------------------------------------" << endl;
	cout << "START: (4) Handling Enumerated Types " << endl;
	cout << "------------------------------------------------------------" << endl;
	
	//GET INFORMATION OF ENUM PV
	PVCtrlHolder pvCtrl;
	status=cafe->getCtrl(PV_MBBI, pvCtrl);
	
	vector<string> enums= pvCtrl.getEnumStrings();
	cout << "------------------------" << endl;
	cout << "ENUM NAME/VALUE PAIRS ARE:" << endl;
	for (int i=0; i<enums.size(); ++i) {
		cout << i << " : " << enums[i] << endl;
	}
	cout << "-------------------------" << endl;
	
	
	string enumStrVal;
	status= cafe->get(PV_MBBI, enumStrVal);	 
	cout << "VALUE as string:   " << enumStrVal << endl;
			
	cout << "getEnumFromString:  " << pvCtrl.getEnumFromString(enumStrVal) << endl;
		
	unsigned short enumShortVal;
	status= cafe->get(PV_MBBI, enumShortVal);	 
	cout << "VALUE as int:       " << enumShortVal << endl;
	
	cout << "getStringFromEnum: " << pvCtrl.getStringFromEnum(enumShortVal) << endl;
	
		
	PVDataHolder pvDat;
	status= cafe->get(PV_MBBI, pvDat);	 
	cout << "VALUE as string:          " << pvDat.getAsString() << endl;
	cout << "VALUE as int:             " << pvDat.getAsUShort() << endl;
	cout << "Integer Value as String:  " << pvDat.getEnumIntegerValueAsString() << endl;
		
	//Each of these 3 methods will set the pv value to 1, i.e., "on"
	status = cafe->set(PV_MBBI, "on");
	status = cafe->set(PV_MBBI, 1);
	status = cafe->set(PV_MBBI, " 1 "); //leading and trailing spaces are ignored
		
	//try to set an illegal value
	status = cafe->set(PV_MBBI, "invalid enum name");
	cafe->printStatus(PV_MBBI,status);


	cout << "------------------------------------------------------------" << endl;
	cout << "END: (4) Handling Enumerated Types " << endl;
	cout << "------------------------------------------------------------" << endl;
	

		
	cout << "------------------------------------------------------------" << endl;
	cout << "START: (5) Control System Parameters " << endl;
	cout << "------------------------------------------------------------" << endl;
	
	// An example of a single get that retrieves control display parameters
	// Get ctrl limits
	PVCtrlHolder pvc;

	status=cafe->getCtrl(hArray[0], pvc);
		
	if (status==ICAFE_NORMAL) {
		pvc.print(); //just print
		// Access a members explicitly
		cout << "---------------------------------------" << endl;
		cout << "Control Parameters                     " << endl;
		cout << "---------------------------------------" << endl;
		cout << "pv             = "      << pvc.getPV()   << endl; 
		cout << "val (asString) = "      << pvc.getAsString() << endl;
		cout << "units          = "      << pvc.getUnits() << endl; // as char *
		//cout << "units         = "      << pvc.getUnitsAsString() << endl; // as string
		cout << "precision      = " << pvc.getPrecision() << endl;
		vector<std::string> strV = pvc.getEnumStrings();
		for (unsigned int i=0; i< strV.size(); ++i) {
			cout << strV[0] << " " ;				
		}
		if (strV.size() >0) {cout << endl;}
		cout << "AsDouble: " << endl;
		cout << "upperDispLimit   = " <<  pvc.getUpperDispLimit_AsDouble() << endl;
		cout << "lowerDispLimit   = " <<  pvc.getLowerDispLimit_AsDouble() << endl;
		cout << "upperAlarmLimit  = " <<  pvc.getUpperAlarmLimit_AsDouble() << endl;
		cout << "upperWarningLimit= " <<  pvc.getUpperWarningLimit_AsDouble() << endl;
		cout << "lowerWarningLimit= " <<  pvc.getLowerWarningLimit_AsDouble() << endl;
		cout << "lowerAlarmLimit  = " <<  pvc.getLowerAlarmLimit_AsDouble() << endl;
		cout << "upperCtrlLimit   = " <<  pvc.getUpperCtrlLimit_AsDouble() << endl;
		cout << "lowerCtrlLimit   = " <<  pvc.getLowerCtrlLimit_AsDouble() << endl;
		cout << "AsString: " << endl;
		cout << "upperDispLimit   = " <<  pvc.getUpperDispLimit_AsString() << endl;
		cout << "lowerDispLimit   = " <<  pvc.getLowerDispLimit_AsString() << endl;
		cout << "upperAlarmLimit  = " <<  pvc.getUpperAlarmLimit_AsString() << endl;
		cout << "upperWarningLimit= " <<  pvc.getUpperWarningLimit_AsString() << endl;
		cout << "lowerWarningLimit= " <<  pvc.getLowerWarningLimit_AsString() << endl;
		cout << "lowerAlarmLimit  = " <<  pvc.getLowerAlarmLimit_AsString() << endl;
		cout << "upperCtrlLimit   = " <<  pvc.getUpperCtrlLimit_AsString() << endl;
		cout << "lowerCtrlLimit   = " <<  pvc.getLowerCtrlLimit_AsString() << endl;
		cout << "---------------------------------------\n" << endl;
								
	}
	else {
		cafe->printStatusIfError(hArray[0],status);
	}	
				
	cout << "------------------------------------------------------------" << endl;
	cout << "  END: (5) Control System Parameters" << endl;
	cout << "------------------------------------------------------------" << endl;

	cout << "------------------------------------------------------------" << endl;
	cout << "START: (6) Channel Information" << endl;
	cout << "------------------------------------------------------------" << endl;
	
	
	
	ChannelRegalia chInfo;
	CAFEDataTypeCode cdt;
	status=cafe->getChannelInfo(hArray[0], chInfo);
		
	cout << "---------------------------------------" << endl;
	cout << "Channel Information                    " << endl;
	cout << "---------------------------------------" << endl;
	cout << "channelID   = " << chInfo.getChannelID() << endl;
	cout << "channelID   = " << cafe->getChannelIDAsString(chInfo.getChannelID()) << endl;
	cout << "connectFlag = " << chInfo.getConnectFlag() << endl; //bool
	cout << "hostName    = " << chInfo.getHostName() << endl; //char *
	cout << "hostName    = " << chInfo.getHostNameAsString() << endl; //string
	cout << "dataType    = " << chInfo.getDataType() << endl;
	cout << "dataType    = " << cdt.asString( chInfo.getDataType() ) << endl;  
	cout << "className   = " << chInfo.getClassName() << endl; //char *
	cout << "className   = " << chInfo.getClassNameAsString() << endl; //string	
	cout << "readAccess  = " << chInfo.getReadAccess() << endl;
	cout << "writeAccess = " << chInfo.getWriteAccess() << endl;
	cout << "nElem       = " << chInfo.getNelem() << endl;
	cout << "connectionState = " << chInfo.getConnectionState() << endl;
	cout << "connectionState = " << chInfo.getConnectionStateAsString() << endl;
	cout << "cafeConnectionState = " << chInfo.getCafeConnectionState() << endl;
	cout << "cafeConnectionState = " << chInfo.getCafeConnectionStateAsString() << endl;
	cout << "---------------------------------------\n" << endl;
		
			
	cout << "------------------------------------------------------------" << endl;
	cout << "  END: (6) Channel Information" << endl;
	cout << "------------------------------------------------------------" << endl;
		
			
		
	cout << "------------------------------------------------------------" << endl;
	cout << "START: (7) Monitors, either with or without user supplied callbacks " << endl;
	cout << "------------------------------------------------------------" << endl;
	
	//Simple Monitor  
	cafe->monitorStart(hArray[0]);
	//or
	//cafe->monitorStart(cafe->getInfo().getHandleFromPV(PV_X1));
	
			
	MonitorPolicy mp;		
	mp.setHandler(callbackHandlerMonitor);
			
	cafe->monitorStart(hArray[0], mp);	
					
	cafe->set(hArray[0], 1.0123456789);
	usleep(100000); // 0.1s just about long enough to trigger an extra monitor
		
	//get latest value from cache
	status=cafe->getCache(hArray[0],d);  
		
	cout << "latest value " << d << endl;
		
	status=cafe->getCache(hArray[0],pvd);
		
	cout << "pvd " << pvd.getAsString() << endl;
				
	cafe->monitorStop(hArray[0], mp);
		   		
	cout << "------------------------------------------------------------" << endl;
	cout << "  END: (7) Monitors, either with or without user supplied callbacks " << endl;
	cout << "------------------------------------------------------------" << endl;	
							
	cout << "------------------------------------------------------------" << endl;
	cout << "START: (8) Gracefully terminate CAFE " << endl;
	cout << "------------------------------------------------------------" << endl;
			
	cafe->terminate();
					
	cout << "------------------------------------------------------------" << endl;
	cout << "  END: (8) Gracefully terminate CAFE " << endl;
	cout << "------------------------------------------------------------" << endl;
  return 0;	
	
}		
