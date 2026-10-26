// CLoadingMemoryFailureHandler::~CLoadingMemoryFailureHandler() -- deletes the critical section, frees the Banks array storage, then the inlined empty CFailedAllocationHandler destructor
// Leaf: ??1CLoadingMemoryFailureHandler
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009D5EE0, length 43 bytes.
// Callees: DeleteCriticalSection (import [0x0143FDE4]), free @ 0x00BFEA14.
// Identity: vtables 0x0129B8C4 = CLoadingMemoryFailureHandler (stored first) and 0x0129B874 =
// CFailedAllocationHandler (stored last, the inlined base destructor) per rtti-vtables.tsv; the manifest's
// "CTCCarryable::OnKill" label is propagated and rejected.
// Layout: Ego_r.pdb CLoadingMemoryFailureHandler: Banks (CArray<CBankFileAsync*>) +4, CriticalSection +0x14;
// the retail CArray is 12 bytes, so the critical section sits at +0x10.
// Shape: the body deletes the section; the member CArray's inline destructor frees its block (pointer
// elements need no destroy loop); the base destructor is inline and empty, leaving only its vtable store.
extern "C" void __cdecl free(void* block);                                  // 0x00BFEA14

struct SCriticalSection { unsigned char _body[0x18]; };                      // RTL_CRITICAL_SECTION
extern "C" __declspec(dllimport) void __stdcall DeleteCriticalSection(SCriticalSection* section);   // [0x0143FDE4]

class CBankFileAsync;

template <class T> struct CRetailArray {                                     // retail CArray: {first, last, end}
    T* first;
    T* last;
    T* end_of_storage;
    ~CRetailArray()
    {
        if (first != 0)
            free(first);
    }
};

class CFailedAllocationHandler { public:                                            // vtable 0x0129B874
    virtual void v00();
    ~CFailedAllocationHandler() { }
};

class CLoadingMemoryFailureHandler : public CFailedAllocationHandler { public:             // vtable 0x0129B8C4
    CRetailArray<CBankFileAsync*> Banks;                                    // +0x04
    SCriticalSection              CriticalSection;                          // +0x10
    virtual void v00();
    ~CLoadingMemoryFailureHandler();
};

CLoadingMemoryFailureHandler::~CLoadingMemoryFailureHandler()
{
    DeleteCriticalSection(&CriticalSection);
}