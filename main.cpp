#include <iostream>
#include <iomanip>

#include "listLib.h"
#include "requestLib.h"
#include "dbLib.h"

using namespace std;

bool processRequest(VRequest& request, L1List<VRecord>& recList, void* pGData);
bool initVGlobalData(void** pGData);
void releaseVGlobalData(void* pGData);

int main(int narg, char** argv) {
    L1List<VRequest>  reqList;
    L1List<VRecord>   recDB;// database of records
    void*             pGData;
    L1List<VRecord>   DataTest;

    loadRequests(argv[1], reqList);
    loadVDB(argv[2], recDB);
    //cout << reqList.at(5).code << endl;
    //cout << recDB.getpHead()->data.y << endl;
    //cout << distanceVR(106.75340, 10.80271, 106.75289, 10.80485) << endl;
    cout << fixed << setprecision(12);// preset for floating point numbers
    if (initVGlobalData(&pGData)) {
        clog << "Successfully initialize data\n";
    }
    else {
        clog << "Failed to initialize data\nExiting...\n";
        return -1;
    }
    clock_t start = clock();
    while (!reqList.isEmpty()) {
        if (!processRequest(reqList[0], recDB, pGData))
            cout << "Failed to process the request\n";
        reqList.removeHead();
    }
    clock_t end = clock();
    cout << (double)(end - start) / 1000 << endl;
    cout << resetiosflags(ios::showbase) << setprecision(-1);
    /// Release any global allocaed data
    releaseVGlobalData(pGData);
    
    //system("pause");
    return 0;
}