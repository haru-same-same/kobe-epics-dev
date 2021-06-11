#include <cafe.h>
#include "callbacks.h"

#define PV_AI1 "abes:ai1"
#define PV_AI2 "abes:ai2"

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
    
    /*
    try {
        cafe->open(pvArray[0].c_str(), handle);
    }
    catch(CAFEException_open &e) {
        cout << e.what() << endl;
        exit(1);
    }
    */
    
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
    
}
