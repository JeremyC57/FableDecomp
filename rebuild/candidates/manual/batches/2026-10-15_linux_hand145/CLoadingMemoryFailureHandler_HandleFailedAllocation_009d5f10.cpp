// CLoadingMemoryFailureHandler::HandleFailedAllocation(unsigned long size) -- under the handler's critical section, ask each registered bank to free memory for a failed allocation; true as soon as one bank handled it, false when none did
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009D5F10, length 112 bytes.
// Leaf: ?HandleFailedAllocation@CLoadingMemoryFailureHandler@@UAE_NK@Z
// Callees: CCriticalSectionLock::CCriticalSectionLock(RTL_CRITICAL_SECTION*, bool) @ 0x00A62920,
//          CBankFileAsync::HandleFailedAllocation() @ 0x009D5BF0, CCriticalSectionLock::~CCriticalSectionLock @ 0x00A629A0.
// Shape: the layout of the landed destructor 0x009D5EE0 (Banks array +0x04, CriticalSection +0x10); the 8-byte
// lock local is destroyed on both exits (no EH frame: the TU is built without /EHsc); the bank count
// `(last - first) >> 2` is re-evaluated each iteration and compared unsigned; the size argument is unused.
struct SCriticalSection { unsigned char _body[0x18]; };   // RTL_CRITICAL_SECTION
struct CCriticalSectionLock {                      // 8 bytes on the stack
    SCriticalSection* m_pSection;
    bool              m_locked;
    CCriticalSectionLock(SCriticalSection* section, bool tryOnly);   // 0x00A62920
    ~CCriticalSectionLock();                       // 0x00A629A0
};
struct CBankFileAsync {
    bool HandleFailedAllocation();                 // 0x009D5BF0
};
template <class T> struct CRetailArray {           // retail CArray: {first, last, end}
    T* first;
    T* last;
    T* end_of_storage;
    unsigned long Size() const { return last - first; }
};
struct CFailedAllocationHandler {                  // vtable 0x0129B874
    virtual bool HandleFailedAllocation(unsigned long size);
};
struct CLoadingMemoryFailureHandler : CFailedAllocationHandler {   // vtable 0x0129B8C4
    CRetailArray<CBankFileAsync*> Banks;           // +0x04
    SCriticalSection              CriticalSection; // +0x10
    virtual bool HandleFailedAllocation(unsigned long size);
};

bool CLoadingMemoryFailureHandler::HandleFailedAllocation(unsigned long size)
{
    CCriticalSectionLock lock(&CriticalSection, false);
    for (unsigned long i = 0; i < Banks.Size(); ++i)
        if (Banks.first[i]->HandleFailedAllocation())
            return true;
    return false;
}
