/*
 * =========================================================================================
 * Name        : processData.cpp
 * Description : student code for Assignment 1 - Data structures and Algorithms - Spring 2018
 * =========================================================================================
 */
#include "requestLib.h"
#include "dbLib.h"

#define MAX_LENGTH_TIMESTAMP    20
#define ZERO                    0e-10
#define STOP                    5e-3

struct Data {
    L1List<L1List<VRecord>> NewDataList;
    int flag;
};

/// Initialize and Finalize any global data that you use in the program
bool _CopyLinkedList(L1List<VRecord>& recList, L1List<VRecord>& CopyList) {
    if(recList.getpHead() == NULL) {
        return false;
    }
    else {
        L1Item<VRecord>* pCurr = recList.getpHead();
        while(pCurr != NULL) {
            VRecord CopyData = pCurr->data;
            L1Item<VRecord>* CopyNode = new L1Item<VRecord>(CopyData);
            CopyList.insertHead(CopyNode);
            pCurr = pCurr->pNext;
        }
        CopyList.reverse();
        return true;
    }
}
bool _CCCCreateLinkedListID(L1List<VRecord>& recList, L1List<L1List<VRecord>> &NewDataList) {
    if(recList.getpHead() == NULL) {
        return false;
    }
    while(recList.getSize() > 0) {
        L1Item<VRecord>* Node = recList.getFirst();
        L1Item<L1List<VRecord>>* pCurr = NewDataList.getpHead();
        if(pCurr == NULL) {
            L1List<VRecord>* ListChild = new L1List<VRecord>();
            ListChild->insertHead(Node);
            L1Item<L1List<VRecord>>* Data = new L1Item<L1List<VRecord>>((*ListChild));
            NewDataList.insertHead(Data);
            pCurr = NewDataList.getpHead();
        }
        else {
            int flag = 0;
            while(pCurr != NULL) {
                if(strcmp(pCurr->data.getpHead()->data.id, Node->data.id) == 0) {
                    pCurr->data.insertLast(Node);
                    flag = 1;
                    break;
                }
                if(pCurr->pNext == NULL) {
                    break;
                }
                pCurr = pCurr->pNext;
            }
            if(pCurr->pNext == NULL && flag == 0) {
                L1List<VRecord>* db = new L1List<VRecord>();
                db->insertHead(Node);
                L1Item<L1List<VRecord>>* Data = new L1Item<L1List<VRecord>>((*db));
                pCurr->pNext = Data;
                NewDataList.increaseSize();
            }
        }
    }
}

bool _CreateLinkedListID(L1List<VRecord>& recList, L1List<L1List<VRecord>> &NewDataList) {
    if(recList.getpHead() == NULL) {
        return false;
    }
    while(recList.getSize()) {
        L1List<VRecord>* db = new L1List<VRecord>();
        L1Item<VRecord>* p = recList.getFirst();
        db->insertHead(p);

        int flag = 0;
        L1Item<VRecord>* pPrev = NULL;
        L1Item<VRecord>* pCurr = recList.getpHead();
        while(pCurr != NULL) {
            if(strcmp(pCurr->data.id, p->data.id) == 0) {
                L1Item<VRecord>* pTemp = NULL;
                if(flag == 0) {
                    pCurr = pCurr->pNext;
                    pTemp = recList.getFirst();
                }
                else {
                    pTemp = pCurr;
                    pPrev->pNext = pCurr->pNext;
                    pCurr = pCurr->pNext;
                    recList.decreaseSize();
                    flag++;
                }
                pTemp->pNext = NULL;
                db->insertHead(pTemp);
            }
            else {
                pPrev = pCurr;
                pCurr = pCurr->pNext;
                flag++;
            }
        }
        db->reverse();
        L1Item<L1List<VRecord>>* New = new L1Item<L1List<VRecord>>((*db));
        NewDataList.insertHead(New);
    }
    NewDataList.reverse();
    return true;
}

bool initVGlobalData(void** pGData) {
    // TODO: allocate global data if you think it is necessary.
    /// pGData contains the address of a pointer. You should allocate data for the global data
    /// and then assign its address to *pGData
    static Data* pGList = new Data;
    pGList->flag = 0;
    *pGData = pGList;
    return true;
}

void releaseVGlobalData(void* pGData) {
    // TODO: release the data if you finish using it
}
bool _processRequest(VRequest& request, L1List<L1List<VRecord>> &DoubleLinkedList, L1List<VRecord>& recList);
bool _Check(L1List<L1List<VRecord>> &DoubleLinkedList, L1List<VRecord>& recList);
bool processRequest(VRequest& request, L1List<VRecord>& recList, void* pGData) {
    // TODO: Your code comes here

    /// NOTE: The output of the request will be printed on one line
    /// end by the end-line '\n' character.

    Data* pGList = (Data*) pGData;
    L1List<VRecord> CopyLinkedList;
    
    if(pGList->flag == 0) {
        //_CopyLinkedList(recList, CopyLinkedList);
        _CreateLinkedListID(recList, pGList->NewDataList);
        pGList->flag++;
    }
    //_processRequest(request, pGList->NewDataList, recList);
    _Check(pGList->NewDataList, recList);
    return true;

}
bool _Check(L1List<L1List<VRecord>> &DoubleLinkedList, L1List<VRecord>& recList) {
    cout << DoubleLinkedList.getSize() << endl;
    return true;
}


bool _processRequest(VRequest& request, L1List<L1List<VRecord>> &DoubleLinkedList, L1List<VRecord>& recList) {
    
    if (!strcmp(request.code, "CNV"))
        cout << DoubleLinkedList.getSize() << endl;

    else if (!strcmp(request.code, "CNR"))
        cout << recList.getSize() << endl;

    else if (!strcmp(request.code, "CNS")){
        int Num = 0;
        L1Item<L1List<VRecord>>* pCurr = DoubleLinkedList.getpHead();
        while(pCurr != NULL) {
            int Check = 0;
            if(pCurr->data.getSize() > 1) {
                L1Item<VRecord>* p = pCurr->data.getpHead();
                L1Item<VRecord>* pp = p->pNext;
                while(pp != NULL) {
                    if(distanceVR(p->data.y,p->data.x, pp->data.y, pp->data.x) == 0) {
                        Check = 1;
                        break;
                    }
                    p = pp;
                    pp = pp->pNext;
                }
                if(Check == 0) {
                    Num = Num + 1;
                }
            }
            pCurr = pCurr->pNext;
        }
        cout << Num << endl;
    }

    else if (!strcmp(request.code, "CAS")){}

    else if (!strcmp(request.code, "VFF")){
        cout << recList.getpHead()->data.id << endl;
    }

    else if (!strcmp(request.code, "VFL")){
        L1Item<VRecord>* p = recList.getpHead();
        while(p->pNext != NULL) {
            p = p->pNext;
        }
        cout << p->data.id << endl;
    }

    else if (!strncmp(request.code, "VFY", 3)) {
        char* ID = getRequestID(request);
        
        L1Item<L1List<VRecord>>* pCurr = DoubleLinkedList.getpHead();
        while(pCurr != NULL) {
            if(strcmp(pCurr->data.getpHead()->data.id, ID) == 0) {
                cout << pCurr->data.getpHead()->data.y << endl;
                break;
            }
            pCurr = pCurr->pNext;
        }
        if(pCurr == NULL) {
            cout << "VFY" << ID << " : False" << endl;
        }
    }

    else if (!strncmp(request.code, "VFX", 3)) {
        char* ID = getRequestID(request);
        
        L1Item<L1List<VRecord>>* pCurr = DoubleLinkedList.getpHead();
        while(pCurr != NULL) {
            if(strcmp(pCurr->data.getpHead()->data.id, ID) == 0) {
                cout << pCurr->data.getpHead()->data.x << endl;
                break;
            }
            pCurr = pCurr->pNext;
        }
        if(pCurr == NULL) {
            cout << "VFX" << ID << " : False" << endl;
        }
    }
    
    else if (!strncmp(request.code, "VFT", 3)) {
        char* ID = getRequestID(request);
        char Buffer[MAX_LENGTH_TIMESTAMP];
        
        L1Item<L1List<VRecord>>* pCurr = DoubleLinkedList.getpHead();
        while(pCurr != NULL) {
            if(strcmp(pCurr->data.getpHead()->data.id, ID) == 0) {
                strPrintTime(Buffer, pCurr->data.getpHead()->data.timestamp);
                cout << Buffer << endl;
                break;
            }
            pCurr = pCurr->pNext;
        }
        if(pCurr == NULL) {
            cout << "VFT<" << ID << " : False" << endl;
        }
    }

    else if (!strncmp(request.code, "VFS", 3)) {
        char* ID = getRequestID(request);
        int Num_ZERO = 0;
        L1Item<L1List<VRecord>>* pCurr = DoubleLinkedList.getpHead();
        while(pCurr != NULL) {
            if(strcmp(pCurr->data.getpHead()->data.id, ID) == 0) {
                if(pCurr->data.getSize() > 1) {
                    L1Item<VRecord>* p = pCurr->data.getpHead();
                    L1Item<VRecord>* pp = p->pNext;
                    while(pp != NULL) {
                        if(distanceVR(p->data.y,p->data.x, pp->data.y, pp->data.x) == 0) {
                            Num_ZERO = 1;
                            break;
                        }
                    }
                    if(pp != NULL) {
                        cout << "(" << pp->data.x << " " << pp->data.y << ")" << endl;
                    }
                }
                break;
            }
            pCurr = pCurr->pNext;
        }
        if(pCurr == NULL || Num_ZERO == 0) {
            cout << "VFS" << ID << " : False" << endl;
        }
    }

    else if (!strncmp(request.code, "VLY", 3)) {
        char* ID = getRequestID(request);

        L1Item<L1List<VRecord>>* pCurr = DoubleLinkedList.getpHead();
        while(pCurr != NULL) {
            if(strcmp(pCurr->data.getpHead()->data.id, ID) == 0) {
                L1Item<VRecord>* p = pCurr->data.getpHead();
                while(p->pNext != NULL) {
                    p = p->pNext;
                }
                cout << p->data.y << endl;
                break;
            }
            pCurr = pCurr->pNext;
        }
        if(pCurr == NULL) {
            cout << "VLY" << ID << " : False" << endl;
        }
    }

    else if (!strncmp(request.code, "VLX", 3)) {
        char* ID = getRequestID(request);
        L1Item<L1List<VRecord>>* pCurr = DoubleLinkedList.getpHead();
        while(pCurr != NULL) {
            if(strcmp(pCurr->data.getpHead()->data.id, ID) == 0) {
                L1Item<VRecord>* p = pCurr->data.getpHead();
                while(p->pNext != NULL) {
                    p = p->pNext;
                }
                cout << p->data.x << endl;
                break;
            }
            pCurr = pCurr->pNext;
        }
        if(pCurr == NULL) {
            cout << "VLX" << ID << " : False" << endl;
        }
    }

    else if (!strncmp(request.code, "VLT", 3)) {
        char* ID = getRequestID(request);
        char Buffer[MAX_LENGTH_TIMESTAMP];

        L1Item<L1List<VRecord>>* pCurr = DoubleLinkedList.getpHead();
        while(pCurr != NULL) {
            if(strcmp(pCurr->data.getpHead()->data.id, ID) == 0) {
                L1Item<VRecord>* p = pCurr->data.getpHead();
                while(p->pNext != NULL) {
                    p = p->pNext;
                }
                strPrintTime(Buffer, p->data.timestamp);
                cout << Buffer << endl;
                break;
            }
            pCurr = pCurr->pNext;
        }
        if(pCurr == NULL) {
            cout << "VLT" << ID << " : False" << endl;
        }
    }

    else if (!strncmp(request.code, "VCR", 3)) {
        char* ID = getRequestID(request);
        L1Item<L1List<VRecord>>* pCurr = DoubleLinkedList.getpHead();
        while(pCurr != NULL) {
            if(strcmp(pCurr->data.getpHead()->data.id, ID) == 0) {
                cout << pCurr->data.getSize() << endl;
                break;
            }
            pCurr = pCurr->pNext;
        }
        if(pCurr == NULL) {
            cout << "VCR" << ID << " : False" << endl;
        }
    }

    else if (!strncmp(request.code, "VCL", 3)) {
        char* ID = getRequestID(request);
        double S = ZERO;
        int Num_Record = 0;

        L1Item<L1List<VRecord>>* pCurr = DoubleLinkedList.getpHead();
        while(pCurr != NULL) {
            if(strcmp(pCurr->data.getpHead()->data.id, ID) == 0) {
                Num_Record = pCurr->data.getSize();
                L1Item<VRecord>* p = pCurr->data.getpHead();
                L1Item<VRecord>* pp = p->pNext;
                while(pp != NULL) {
                    double Sdd = distanceVR(p->data.y, p->data.x, pp->data.y, pp->data.x);
                    if(Sdd != ZERO) {
                        S = S + Sdd;
                    }
                    p = pp;
                    pp = pp->pNext;
                }
                break;
            }
            pCurr = pCurr->pNext;
        }
        if(pCurr == NULL || Num_Record == 1) {
            cout << "VCL" << ID << " : False" << endl;
        }
        else {
            cout << S << endl;
        }
    }

    else if (!strncmp(request.code, "VMT", 3)) {
        char* ID = getRequestID(request);
        int Time = 0;
        int Num_Record = 0;

        L1Item<L1List<VRecord>>* pCurr = DoubleLinkedList.getpHead();
        while(pCurr != NULL) {
            if(strcmp(pCurr->data.getpHead()->data.id, ID) == 0) {
                Num_Record = pCurr->data.getSize();
                L1Item<VRecord>* p = pCurr->data.getpHead();
                L1Item<VRecord>* pp = p->pNext;
                while(pp != NULL) {
                    double Sdd = distanceVR(p->data.y, p->data.x, pp->data.y, pp->data.x);
                    if(Sdd != ZERO) {
                        Time = Time + difftime(p->data.timestamp, pp->data.timestamp);
                    }
                    p = pp;
                    pp = pp->pNext;
                }
                break;
            }
            pCurr = pCurr->pNext;
        }
        if(pCurr == NULL || Num_Record == 1) {
            cout << "VMT" << ID << " : False" << endl;
        }
        else {
            cout << Time << endl;
        }
    }

    else if (!strncmp(request.code, "VMS", 3)) {
        char* ID = getRequestID(request);
        int Time_ZERO = 0;
        int TimeCurrent = 0;
        int Num_Record = 0;

        L1Item<L1List<VRecord>>* pCurr = DoubleLinkedList.getpHead();
        while(pCurr != NULL) {
            if(strcmp(pCurr->data.getpHead()->data.id, ID) == 0) {
                Num_Record = pCurr->data.getSize();
                L1Item<VRecord>* p = pCurr->data.getpHead();
                L1Item<VRecord>* pp = p->pNext;
                while(pp != NULL) {
                    double Sdd = distanceVR(p->data.y, p->data.x, pp->data.y, pp->data.x);
                    if(Sdd == ZERO) {
                        TimeCurrent = TimeCurrent + difftime(p->data.timestamp, pp->data.timestamp);
                    }
                    else {
                        TimeCurrent = 0;
                    }
                    if(Time_ZERO < TimeCurrent) {
                        Time_ZERO = TimeCurrent;
                    }
                    p = pp;
                    pp = pp->pNext;
                }
                break;
            }
            pCurr = pCurr->pNext;
        }
        if(pCurr == NULL || Num_Record == 1) {
            cout << "VMS" << ID << " : False" << endl;
        }
        else {
            cout << Time_ZERO << endl;
        }
    }

    else if (!strncmp(request.code, "VMS", 3)) {
        char* ID = getRequestID(request);
        int Time_ZERO = 0;
        int TimeCurrent = 0;
        int Num_Record = 0;

        L1Item<L1List<VRecord>>* pCurr = DoubleLinkedList.getpHead();
        while(pCurr != NULL) {
            if(strcmp(pCurr->data.getpHead()->data.id, ID) == 0) {
                Num_Record = pCurr->data.getSize();
                L1Item<VRecord>* p = pCurr->data.getpHead();
                L1Item<VRecord>* pp = p->pNext;
                while(pp != NULL) {
                    double Sdd = distanceVR(p->data.y, p->data.x, pp->data.y, pp->data.x);
                    if(Sdd == ZERO) {
                        TimeCurrent = TimeCurrent + difftime(p->data.timestamp, pp->data.timestamp);
                    }
                    else {
                        TimeCurrent = 0;
                    }
                    if(Time_ZERO < TimeCurrent) {
                        Time_ZERO = TimeCurrent;
                    }
                    p = pp;
                    pp = pp->pNext;
                }
                break;
            }
            pCurr = pCurr->pNext;
        }
        if(pCurr == NULL || Num_Record == 1) {
            cout << "VMS" << ID << " : False" << endl;
        }
        else {
            cout << Time_ZERO << endl;
        }
    }

    else if (!strcmp(request.code, "MST")) {
        int check = 0;
        int Time_ZERO = 0;
        int Num_Record = 0;

        L1Item<L1List<VRecord>>* pCurr = DoubleLinkedList.getpHead();
        while(pCurr != NULL) {
            int TimeCurrent = 0;
            Num_Record = pCurr->data.getSize();
            if(Num_Record > 1) {
                check = 1;
                //int TimeCurrent = 0;
                L1Item<VRecord>* p = pCurr->data.getpHead();
                L1Item<VRecord>* pp = p->pNext;
                while(pp != NULL) {
                    double Sdd = distanceVR(p->data.y, p->data.x, pp->data.y, pp->data.x);
                    if(Sdd == ZERO) {
                        TimeCurrent = TimeCurrent + difftime(p->data.timestamp, pp->data.timestamp);
                    }
                    p = pp;
                    pp = pp->pNext;
                }
            }
            if(Time_ZERO < TimeCurrent) {
                Time_ZERO = TimeCurrent;
            }
            pCurr = pCurr->pNext;
        }
        if(check == 0 || (strlen(request.code) != 3)) {
            cout << "MST : False" << endl;
        }
        else {
            cout << Time_ZERO << endl;
        }
    }

    else if (!strcmp(request.code, "MRV")) {
        int Num_Record = 0;
        L1Item<L1List<VRecord>>* pCurr = DoubleLinkedList.getpHead();
        while(pCurr != NULL) {
            int Num_Record_ID = 0;
            Num_Record_ID = pCurr->data.getSize();
            if(Num_Record < Num_Record_ID) {
                Num_Record = Num_Record_ID;
            }
            pCurr = pCurr->pNext;
        }
        if(strlen(request.code) != 3) {
            cout << "MRV : False" << endl;
        }
        else {
            cout << Num_Record << endl;
        }
    }

    else if (!strcmp(request.code, "MTV")) {
        int Time_Move_MAX = 0;
        L1Item<L1List<VRecord>>* pTemp = NULL;
        L1Item<L1List<VRecord>>* pCurr = DoubleLinkedList.getpHead();
        while(pCurr != NULL) {
            int Num_Record_ID = pCurr->data.getSize();
            if(Num_Record_ID > 1) {
                int Time_Move_ID = 0;
                L1Item<VRecord>* p = pCurr->data.getpHead();
                L1Item<VRecord>* pp = p->pNext;
                while(pp != NULL) {
                    //if(distanceVR(p->data.y, p->data.x, pp->data.y, pp->data.x) != 0) {
                    if(p->data.y == pp->data.y && p->data.x == pp->data.x) {
                        Time_Move_ID = Time_Move_ID + difftime(p->data.timestamp, pp->data.timestamp);
                    }
                    p = pp;
                    pp = pp->pNext;
                }
                if(Time_Move_MAX < Time_Move_ID) {
                    Time_Move_MAX = Time_Move_ID;
                    pTemp = pCurr;
                }
            }
            pCurr = pCurr->pNext;
        }
        if(strlen(request.code) != 3 || Time_Move_MAX == 0) {
            cout << "MTV : False" << endl;
        }
        else {
            cout << pTemp->data.getpHead()->data.id << "   " << Time_Move_MAX << endl;
        }
    }

    else if (!strcmp(request.code, "MVV")) {
        //int Num_Record = 0;
        double MAX_V = ZERO;
        //int Time_Move_MAX = 0;
        L1Item<L1List<VRecord>>* pTemp = NULL;
        L1Item<L1List<VRecord>>* pCurr = DoubleLinkedList.getpHead();
        while(pCurr != NULL) {
            int Num_Record_ID = pCurr->data.getSize();
            if(Num_Record_ID > 1) {
                double S_Move = ZERO;
                int Time_Move_ID = 0;
                L1Item<VRecord>* p = pCurr->data.getpHead();
                L1Item<VRecord>* pp = p->pNext;
                while(pp != NULL) {
                    if(distanceVR(p->data.y, p->data.x, pp->data.y, pp->data.x) != 0) {
                        S_Move = S_Move + distanceVR(p->data.y, p->data.x, pp->data.y, pp->data.x);
                        Time_Move_ID = Time_Move_ID + difftime(p->data.timestamp, pp->data.timestamp);
                    }
                    p = pp;
                    pp = pp->pNext;
                }
                if(Time_Move_ID != 0.) {
                    double V_Move_ID = S_Move / Time_Move_ID;
                    if(MAX_V < V_Move_ID) {
                        MAX_V = V_Move_ID;
                        pTemp = pCurr;
                    }
                }
            }
            pCurr = pCurr->pNext;
        }
        if(strlen(request.code) != 3 || MAX_V == 0) {
            cout << "MVV : False" << endl;
        }
        else {
            cout << pTemp->data.getpHead()->data.id << endl;
        }
    }

    else if (!strcmp(request.code, "LRV")) {
        L1Item<L1List<VRecord>>* pCurr = DoubleLinkedList.getpHead();
        L1Item<L1List<VRecord>>* pTemp = pCurr;
        int Num_Record = pCurr->data.getSize();
        pCurr = pCurr->pNext;
        while(pCurr != NULL) {
            int Num_Record_ID = pCurr->data.getSize();
            if(Num_Record > Num_Record_ID) {
                Num_Record = Num_Record_ID;
                pTemp = pCurr;
            }
            pCurr = pCurr->pNext;
        }
        if(strlen(request.code) == 3) {
            cout << pTemp->data.getpHead()->data.id << endl;
        }
        else {
            cout << "LRV : False" << endl;
        }
    }

    else if (!strcmp(request.code, "LPV")) {
        L1Item<L1List<VRecord>>* pCurr = DoubleLinkedList.getpHead();
        L1Item<L1List<VRecord>>* pTemp = NULL;
        double S_MAX = ZERO;
        //pCurr = pCurr->pNext;
        while(pCurr != NULL) {
            int Num_Record_ID = pCurr->data.getSize();
            if(Num_Record_ID > 1) {
                double S_ID = ZERO;
                L1Item<VRecord>* p = pCurr->data.getpHead();
                L1Item<VRecord>* pp = p->pNext;
                while(pp != NULL) {
                    if(distanceVR(p->data.y, p->data.x, pp->data.y, pp->data.x) != 0) {
                        S_ID = S_ID + distanceVR(p->data.y, p->data.x, pp->data.y, pp->data.x);
                    }
                    p = pp;
                    pp = pp->pNext;
                }
                if(S_MAX < S_ID) {
                    S_MAX = S_ID;
                    pTemp = pCurr;
                }
            }
            pCurr = pCurr->pNext;
        }
        if(strlen(request.code) != 3 || S_MAX == ZERO) {
            cout << "LPV : False" << endl;
        }
        else {
            cout << pTemp->data.getpHead()->data.id <<  "   " << S_MAX << endl;
        }
    }

    else if (!strcmp(request.code, "SPV")) {
        L1Item<L1List<VRecord>>* pCurr = DoubleLinkedList.getpHead();
        L1Item<L1List<VRecord>>* pTemp = NULL;
        double S_MIN = ZERO;
        //pCurr = pCurr->pNext;
        while(pCurr != NULL) {
            int Num_Record_ID = pCurr->data.getSize();
            if(Num_Record_ID > 1) {
                double S_ID = ZERO;
                L1Item<VRecord>* p = pCurr->data.getpHead();
                L1Item<VRecord>* pp = p->pNext;
                while(pp != NULL) {
                    if(distanceVR(p->data.y, p->data.x, pp->data.y, pp->data.x) != 0) {
                        S_ID = S_ID + distanceVR(p->data.y, p->data.x, pp->data.y, pp->data.x);
                    }
                    p = pp;
                    pp = pp->pNext;
                }
                if(S_MIN > S_ID || S_MIN == 0) {
                    S_MIN = S_ID;
                    pTemp = pCurr;
                }
            }
            pCurr = pCurr->pNext;
        }
        if(strlen(request.code) != 3 || S_MIN == ZERO) {
            cout << "SPV : False" << endl;
        }
        else {
            cout << pTemp->data.getpHead()->data.id << "  " << S_MIN << endl;
        }
    }

    else if (!strncmp(request.code, "RVR", 3)) {
        char* ID = getRequestID(request);
        
        L1Item<L1List<VRecord>>* pCurr = DoubleLinkedList.getpHead();
        L1Item<L1List<VRecord>>* pPrev = NULL;
        int flag = 0;
        int Check = 0;
        while(pCurr != NULL) {
            if(strcmp(pCurr->data.getpHead()->data.id, ID) == 0) {
                if(flag == 0) {
                    pCurr = pCurr->pNext;
                    DoubleLinkedList.removeHead();
                }
                else {
                    pPrev->pNext = pCurr->pNext;
                    pCurr->pNext = NULL;
                    DoubleLinkedList.decreaseSize();
                    delete pCurr;
                }
                Check = 1;
                cout << "RVR : Succeed" << endl;
                break;
            }
            else {
                pPrev = pCurr;
                pCurr = pCurr->pNext;
                flag++;
            }
        }
        if(Check == 0) {
            cout << "RVR" << ID << " : False" << endl;
        }
        else {
            L1Item<VRecord>* pDEL = recList.getpHead();
            L1Item<VRecord>* prev = NULL;
            int flag = 0;
            while(pDEL != NULL) {
                if(strcmp(pDEL->data.id, ID) == 0) {
                    if(flag == 0) {
                        pDEL = pDEL->pNext;
                        recList.removeHead();
                    }
                    else {
                        prev->pNext = pDEL->pNext;
                        L1Item<VRecord>* pT = pDEL;
                        pDEL = pDEL->pNext;
                        recList.decreaseSize();
                        delete pDEL;
                    }
                }
                else {
                    prev = pDEL;
                    pDEL = pDEL->pNext;
                    flag++;
                }
            }
        }
    }

    else {
        cout << request.code << ": Failed\n";
    }
    
    return true;
}
