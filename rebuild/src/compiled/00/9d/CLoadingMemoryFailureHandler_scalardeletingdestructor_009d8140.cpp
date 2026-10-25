// CLoadingMemoryFailureHandler::`scalar deleting destructor'(unsigned int) -- delete the critical section, free the bank list, the inlined CFailedAllocationHandler destructor stores its vtable, flag-tested delete
// Leaf: ??_GCLoadingMemoryFailureHandler@@UAEPAXI@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x009D8140, 63 bytes (+1 int3 padding), thiscall, `ret 4`.
// Identity: slot 0 of the CLoadingMemoryFailureHandler vtable 0x0129B8C4 (rtti-vtables.tsv); the second vptr store
//   0x0129B874 is CFailedAllocationHandler's. The catalog label CWaterPatchMesh::`scalar_deleting_destructor' is propagated.
// Callees: DeleteCriticalSection (kernel32 import through 0x0143FDE4), free 0x00BFEA14 (Banks is a CArray over the malloc
//   allocator), operator delete 0x00BFE9BC.
// Layout: Ego_r.pdb CLoadingMemoryFailureHandler (vptr, Banks +0x04, CriticalSection) with the 12-byte retail CArray
//   (PDB 16), so CriticalSection sits at +0x10 (retail size 0x28). No engine/ headers.
// Shape: the destructor body is the DeleteCriticalSection call (its vtable store scheduled after the argument push); the
//   Banks member destructor follows; CFailedAllocationHandler's inline `virtual ~CFailedAllocationHandler() {}` stores its
//   vtable (scheduled between the flag test and its branch). The out-of-line destructor defined in the TU emits the vtable,
//   and VC7.1 inlines it into the deleting destructor.
#include <stdlib.h>
struct _RTL_CRITICAL_SECTION {                           // 24 bytes (winnt.h)
    void*         DebugInfo;
    long          LockCount;
    long          RecursionCount;
    void*         OwningThread;
    void*         LockSemaphore;
    unsigned long SpinCount;
};
extern "C" __declspec(dllimport) void __stdcall DeleteCriticalSection(_RTL_CRITICAL_SECTION* section);   // [0x0143FDE4]

class CBankFileAsync;
template <class T> class CArray {                        // retail CArray: STLport vector<T> over the malloc allocator
public:
    T* m_pData;
    T* m_pLast;
    T* m_pEnd;
    ~CArray() { if (m_pData) free(m_pData); }
};
class CFailedAllocationHandler {                         // vtable 0x0129B874
public:
    virtual ~CFailedAllocationHandler() {}
};
class CLoadingMemoryFailureHandler : public CFailedAllocationHandler {   // vtable 0x0129B8C4
public:
    virtual ~CLoadingMemoryFailureHandler();
    CArray<CBankFileAsync*> Banks;                       // +0x04
    _RTL_CRITICAL_SECTION   CriticalSection;             // +0x10
};
CLoadingMemoryFailureHandler::~CLoadingMemoryFailureHandler()
{
    DeleteCriticalSection(&CriticalSection);
}