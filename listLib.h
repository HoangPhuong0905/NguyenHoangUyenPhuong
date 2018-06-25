/*
 * =========================================================================================
 * Name        : listLib.h
 * Author      : Duc Dung Nguyen
 * Email       : nddung@hcmut.edu.vn
 * Copyright   : Faculty of Computer Science and Engineering - Bach Khoa University
 * Description : library for Assignment 1 - Data structures and Algorithms - Spring 2018
 * =========================================================================================
 */

#ifndef A01_LISTLIB_H
#define A01_LISTLIB_H

#include <string>
using namespace std;

class DSAException {
    int     _error;
    string  _text;
public:

    DSAException() : _error(0), _text("Success") {}
    DSAException(int err) : _error(err), _text("Unknown Error") {}
    DSAException(int err, const char* text) : _error(err), _text(text) {}

    int getError() { return _error; }
    string& getErrorText() { return _text; }
};

template <class T>
struct L1Item {
    T data;
    L1Item<T> *pNext;
    L1Item() : pNext(NULL) {}
    L1Item(L1Item<T>* p) : pNext(p) {}
    L1Item(T &a) : data(a), pNext(NULL) {}
    L1Item(T &&a) : data(std::move(a)), pNext(NULL) {}
};

template <class T>
class L1List {
    L1Item<T>   *_pHead;// The head pointer of linked list
    size_t      _size;// number of elements in this list
public:
    L1List() : _pHead(NULL), _size(0) {}
    //~L1List();

    void    clean() {
        if(_pHead == NULL) {
            _size = 0;
            return;
        }
        else {
            L1Item<T>* pCurr = _pHead;
            _pHead = NULL;
            while(pCurr) {
                L1Item<T>* pTemp = pCurr;
                pCurr = pCurr->pNext;
                delete pTemp;
            }
            _size = 0;
            return;
        }
    }
    ~L1List() {
        clean();
    }
    bool    isEmpty() {
        return _pHead == NULL;
    }
    size_t  getSize() {
        return _size;
    }
    void decreaseSize() {
        _size--;
    }
    L1Item<T>* getpHead() {
        return _pHead;
    }
    L1Item<T>* getFirst() {
        if(_pHead == NULL) return NULL;
        else {
            L1Item<T>* p = _pHead;
            _pHead = _pHead->pNext;
            p->pNext = NULL;
            _size--;
            return p;
        }
    }
    int traversePrePtr(L1Item<T>* &ptr, int &idx, int flag) {
        if(flag) {
            if(idx == -1 || idx == 0) {
                idx = idx + 1;
                ptr = _pHead;
            }
            else {
                idx = idx + 1;
                ptr = ptr->pNext;
            }
        }
        return idx;
    }

    T&      at(int i);
    T&      operator[](int i);

    bool    find(T& a, int& idx);
    int     insert(int i, T& a);
    int     remove(int i);

    int     push_back(T& a);
    int     insertHead(T& a);
    int     insertHead(L1Item<T>* NewNode);
    int     insertHead() {
        _pHead = new L1Item<T>(_pHead);
        _size++;
        return 0;
    }

    int     removeHead();
    int     removeLast();

    void    reverse();

    void    traverse(void (*op)(T&)) {
        L1Item<T>   *p = _pHead;
        while (p) {
            op(p->data);
            p = p->pNext;
        }
    }
    void    traverse(void (*op)(T&, void*), void* pParam) {
        L1Item<T>   *p = _pHead;
        while (p) {
            op(p->data, pParam);
            p = p->pNext;
        }
    }
};

/// Insert item to the end of the list
/// Return 0 if success
template <class T>
int L1List<T>::push_back(T &a) {
    if (_pHead == NULL) {
        _pHead = new L1Item<T>(a);
    }
    else {
        L1Item<T>   *p = _pHead;
        while (p->pNext) p = p->pNext;
        p->pNext = new L1Item<T>(a);
    }

    _size++;
    return 0;
}

/// Insert item to the front of the list
/// Return 0 if success
template <class T>
int L1List<T>::insertHead(T &a) {
    L1Item<T>   *p = new L1Item<T>(a);
    p->pNext = _pHead;
    _pHead = p;
    _size++;
    return 0;
}

/// Remove the first item of the list
/// Return 0 if success
template <class T>
int L1List<T>::removeHead() {
    if(_pHead) {
        L1Item<T>* p = _pHead;
        _pHead = p->pNext;
        delete p;
        _size--;
        return 0;
    }
    return -1;
}

/// Remove the last item of the list
/// Return 0 if success
template <class T>
int L1List<T>::removeLast() {
    if(_pHead) {
        if(_pHead->pNext) {
            L1Item<T>* prev = _pHead;
            L1Item<T>* pcur = prev->pNext;
            while(pcur->pNext) {
                prev = pcur;
                pcur = pcur->pNext;
            }
            delete pcur;
            prev->pNext = NULL;
        }
        else {
            delete _pHead;
            _pHead = NULL;
        }
        _size--;
        return 0;
    }
    return -1;
}
// insertHead
template <class T>
int L1List<T>::insertHead(L1Item<T>* NewNode) {
    NewNode->pNext = this->_pHead;
    this->_pHead = NewNode;
    this->_size++;
    return 0;
}
// at
template <class T>
T& L1List<T>::at(int i) {
    int idx = 0;
    L1Item<T>* current = _pHead;
    if (i < _size && i >= 0){
        while (idx < i){
            current = (current) ? current->pNext : 0;
            idx++;
        };
    }
    else throw -1;
    return current->data;
}
// operator
template <class T>
T& L1List<T>::operator[](int i) {
    return this->at(i);
}
// reverse
template <class T>
L1Item<T>* Reverse(L1Item<T>* pHead, int N) {
    if(N == 1) return pHead;
    else {
        L1Item<T>* pCurr = pHead;
        L1Item<T>* pPrev = NULL;
        while(pCurr->pNext){
            pPrev = pCurr;
            pCurr = pCurr->pNext;
        }
        pPrev->pNext = NULL;
        pCurr->pNext = pHead;
        pHead = pCurr;
        pHead->pNext = Reverse(pHead->pNext, N - 1);
        return pHead;
    }
}
template <class T>
void L1List<T>::reverse() {
    if(this->_pHead == NULL) return;
    else {
        this->_pHead = Reverse(this->_pHead, this->_size);
    }
}



#endif //A01_LISTLIB_H
