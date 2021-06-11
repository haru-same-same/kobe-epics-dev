#include <cafe.h>
#include "callbacks.h"

#define PV_AI1 "root:ai1"
#define PV_AI2 "root:ai2"

#define HANDLES 2

#define __METHOD__ "cafeTest.cc"

int main()
{
    using namespace std;
    
    string pvArray[HANDLES] = {PV_AI1, PV_AI2};
    unsigned int hArray[HANDLES];
    
    vector<unsigned int> hV;
    vector<string> pvV;
    vector<unsigned short> stateV;
    
    unsigned int handle;
    int status;
    
    CAFE *cafe = new CAFE();
    cafe->channelOpenPolicy.setTimeout(0.5);
    
    //Open PVs
    try {
        cafe->openPrepare();		 		 
        cafe->open(pvArray,  hArray, HANDLES);					
        
        cafe->openNowAndWait(0.5);  //wait for specified time, here 0.5 seconds
        //Are all channels connected? Returns true/false
        bool ifYes= cafe->allChannelsConnected();
        				
        if(!ifYes) {
            cafe->printDisconnectedHandles();
        }
    }
    catch(CAFEException_open & e) {
        cout << e.what() << endl;
    }
    
    //Get PV values
    unsigned short val_ai1;
    status = cafe->get(hArray[0], val_ai1);
    
    if(status != ICAFE_NORMAL) {
        cout << "Status = " << status << "; indicates an error at " << __METHOD__ << "//" << __LINE__ << endl;
        cafe->getCafeStatus().report(status);	
        if (cafe->getCafeStatus().isTimeout(status)) {
            cout << "Timeout error" << endl;	
        }
    } else {
        cout << "Value of ai1 = " << val_ai1 << endl;
    }
    
    //Termination
    cafe->terminate();
    return 0;
}
