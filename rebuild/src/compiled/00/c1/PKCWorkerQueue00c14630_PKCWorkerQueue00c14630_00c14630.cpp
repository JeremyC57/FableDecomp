// PKCWorkerQueue_00c14630::~PKCWorkerQueue_00c14630() -- worker-queue destructor: under its own critical section it appends a null (quit) request to the request vector and releases the semaphore once, then joins the worker thread and deletes it; the compiler then destroys the semaphore, the vector and the critical section
// Leaf: ??1PKCWorkerQueue_00c14630@
// Flags: /EHsc
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C14630, 204 bytes (+ int3 padding), __fastcall `ret`, EH-framed.
// Identity: a libpk worker queue (no vtable): the critical section +0 (InitializeCriticalSection @ 0x00C1C810 /
//   DeleteCriticalSection @ 0x00C1C820), a VC7.1 Dinkumware vector of request pointers +0x18 (16 bytes: allocator word,
//   _Myfirst +0x1c, _Mylast +0x20, _Myend +0x24; `_Insert_n` @ 0x00C14270), the semaphore +0x28 (Release @ 0x00C2A150
//   = ReleaseSemaphore(h, 1, 0) with a PKCSemaphore.cpp warning; destructor @ 0x00C29FB0 = CloseHandle) and the worker
//   thread +0x2c (polymorphic, deleted through its virtual deleting destructor; Join @ 0x00C2A2A0 waits on its handle,
//   sleeps 20 ms and closes it). The sound-event manager's destructor 0x00C13470 deletes the part that holds this
//   queue at +0xc (part pointer +0x7B8); the sibling queue type is @ 0x00C14560.
// Callees: the scoped lock @ 0x00C0FFC0 / 0x00C0FFE0 over the section, operator delete @ 0x00BFE9BC (the vector's
//   _Tidy, inline).
// Shape: EH states: 2 = all three members alive (`mov ebx,2`, `this` homed for the unwind funclets), 3 = the lock;
//   `insert(end(), 0)` reduces to the `_Insert_n(_Mylast, 1, &zero)` call (the unused offset/return folds away, the
//   zero temporary stored after the pushes); the destructor's members die semaphore, vector (inline _Tidy, the zero
//   register reused for the three stores), critical section. operator delete must be declared `throw()` (as VC7.1's
//   <new> does): a plain declaration adds an EH state-0 store before the vector's inline _Tidy and moves the zero
//   register ahead of the lock (the state 2 then stored as an immediate).
// Label correction: the catalog's `DetailCache_Cleanup_Variant` is propagated; descriptive names.
void __cdecl operator delete(void* block) throw();   // 0x00BFE9BC (nothrow, as VC7.1 <new> declares it)

struct PKCCriticalSection_00c14630 {               // 0x18 bytes: a CRITICAL_SECTION
    unsigned char m_section[0x18];
    PKCCriticalSection_00c14630();                  // 0x00C1C810
    ~PKCCriticalSection_00c14630();                 // 0x00C1C820
};

struct CScopedLock_00c14630 {                      // 8 bytes: section + entry tick
    void* m_pSection;
    unsigned long m_tick;
    CScopedLock_00c14630(void* section);            // 0x00C0FFC0
    ~CScopedLock_00c14630();                        // 0x00C0FFE0
};

struct PKCSemaphore_00c14630 {                     // a Win32 semaphore handle
    void* m_hSemaphore;
    void Release();                            // 0x00C2A150
    ~PKCSemaphore_00c14630();                       // 0x00C29FB0
};

struct PKCThread_00c14630 {                        // polymorphic worker thread
    virtual ~PKCThread_00c14630();                  // slot 0: the deleting destructor
    void Join();                               // 0x00C2A2A0
};

struct PKCRequest_00c14630;

namespace std {
template<class _Ty> class allocator {
public:
    void deallocate(_Ty* _Ptr, unsigned int) { operator delete(_Ptr); }
};

template<class _Ty, class _Ax = allocator<_Ty> > class vector {
public:
    typedef _Ty* iterator;
    ~vector() { _Tidy(); }
    unsigned int size() const { return (_Myfirst == 0 ? 0 : _Mylast - _Myfirst); }
    iterator begin() { return (_Myfirst); }
    iterator end() { return (_Mylast); }
    iterator insert(iterator _Where, const _Ty& _Val)
    {
        unsigned int _Off = size() == 0 ? 0 : _Where - begin();
        _Insert_n(_Where, (unsigned int)1, _Val);
        return (begin() + _Off);
    }
    void _Insert_n(iterator _Where, unsigned int _Count, const _Ty& _Val);   // 0x00C14270
    void _Tidy()
    {
        if (_Myfirst != 0)
            this->_Alval.deallocate(_Myfirst, _Myend - _Myfirst);
        _Myfirst = 0, _Mylast = 0, _Myend = 0;
    }
    _Ax _Alval;                                // +0x00 (an empty allocator padded to a word)
    _Ty* _Myfirst;                             // +0x04
    _Ty* _Mylast;                              // +0x08
    _Ty* _Myend;                               // +0x0c
};
}

class PKCWorkerQueue_00c14630 {
public:
    ~PKCWorkerQueue_00c14630();
    PKCCriticalSection_00c14630              m_section;     // +0x00
    std::vector<PKCRequest_00c14630*>        m_requests;    // +0x18
    PKCSemaphore_00c14630                    m_semaphore;   // +0x28
    PKCThread_00c14630*                      m_pThread;     // +0x2c
};

PKCWorkerQueue_00c14630::~PKCWorkerQueue_00c14630()
{
    {
        CScopedLock_00c14630 lock(&m_section);
        m_requests.insert(m_requests.end(), 0);
        m_semaphore.Release();
    }
    m_pThread->Join();
    delete m_pThread;
}