// PKCWorkerThread::ProcessNextRequest() -- worker-thread step: wait on the queue's semaphore, pop the front request under the queue's lock, hand a real request to the handler; returns 1 when the popped request was the null (stop) request, else 0
// Flags: /EHsc
// Leaf: ?ProcessNextRequest@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C26B20, 153 bytes (`ret`, EH-framed for the scoped lock). Descriptive names (no
//   PDB or RTTI covers the libpk audio library): the manifest's "Container_TryRemoveItem" is propagated. The function is
//   slot 1 of vtable 0x012B4010 (slot 0 the deleting destructor, which chains to the thread base destructor
//   0x00C2A2D0); the libpk worker queue (chunk 433: PKCWorkerQueue, critical section +0, request vector +0x18,
//   semaphore +0x28) pushes a null request to stop its thread, which is the `return 1` here.
// Callees: PKCSemaphore::Wait @ 0x00C2A090 (WaitForSingleObject(h, INFINITE) with a PKCSemaphore.cpp warning),
//   PKCProtectionInstance @ 0x00C0FFC0 / ~PKCProtectionInstance @ 0x00C0FFE0, std::vector<PKCRequest*>::erase @ 0x00C26BC0 (the iterator is a
//   class: hidden return slot, begin() passed by value), the handler's virtual slot 0 (+0x10 member).
// Shape: the lock lives in an inner block so it is released before the handler runs; `stop` (ebx) and `request`
//   (edi) are zeroed between the lock argument push and the constructor call, and edi doubles as the EH state-0
//   store; `!empty()` is Dinkumware's `size() == 0` test (`_Myfirst == 0 ? 0 : _Mylast - _Myfirst`). The front
//   request is read through ONE named begin() iterator that is then erased: `front()` plus a second `begin()`
//   reloads _Myfirst (`mov eax,ecx ; mov edx,[eax+4]`) instead of reusing the size() test's edx.
class PKCCriticalSection;
class PKCProtectionInstance { public:                            // 8-byte scoped lock (chunk 431 model)
    void*         m_pSection;
    unsigned long m_tick;
    PKCProtectionInstance(PKCCriticalSection* section);                 // 0x00C0FFC0
    ~PKCProtectionInstance();                             // 0x00C0FFE0
};

struct PKCSemaphore {                           // a Win32 semaphore handle
    void* m_hSemaphore;
    void Wait();                                // 0x00C2A090
};

struct PKCRequest;

struct PKCRequestHandler {                       // polymorphic request handler
    virtual void Process(PKCRequest* request);  // slot 0
};

namespace std {
template<class _Ty> class allocator { };

template<class _Ty> class _Ptrit {              // VC7.1 Dinkumware vector iterator: a class over one pointer
public:
    _Ptrit() { }
    explicit _Ptrit(_Ty* _Ptr) : current(_Ptr) { }
    _Ty& operator*() const { return (*current); }
    _Ty* current;
};

template<class _Ty, class _Ax = allocator<_Ty> > class vector {
public:
    typedef _Ptrit<_Ty> iterator;
    unsigned int size() const { return (_Myfirst == 0 ? 0 : _Mylast - _Myfirst); }
    bool empty() const { return (size() == 0); }
    iterator begin() { return (iterator(_Myfirst)); }
    iterator erase(iterator _Where);            // 0x00C26BC0
    _Ax  _Alval;                                // +0x00 (an empty allocator padded to a word)
    _Ty* _Myfirst;                              // +0x04
    _Ty* _Mylast;                               // +0x08
    _Ty* _Myend;                                // +0x0c
};
}

struct PKCWorkerThread {                         // vtable 0x012B4010
    virtual ~PKCWorkerThread();
    virtual long ProcessNextRequest();          // slot 1
    unsigned char             _pad04[0xc];
    PKCRequestHandler*         m_pHandler;          // +0x10
    PKCSemaphore*             m_pSemaphore;        // +0x14 the queue's semaphore
    void*                     m_pSection;          // +0x18 the queue's critical section
    std::vector<PKCRequest*>* m_pRequests;         // +0x1c the queue's request vector
};

long PKCWorkerThread::ProcessNextRequest()
{
    m_pSemaphore->Wait();
    long stop = 0;
    PKCRequest* request = 0;
    {
        PKCProtectionInstance lock((PKCCriticalSection*)m_pSection);
        if (!m_pRequests->empty()) {
            std::vector<PKCRequest*>::iterator first = m_pRequests->begin();
            request = *first;
            m_pRequests->erase(first);
            if (!request)
                stop = 1;
        }
    }
    if (request)
        m_pHandler->Process(request);
    return stop;
}