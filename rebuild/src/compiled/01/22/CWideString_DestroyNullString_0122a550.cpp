// CWideString::DestroyNullString() -- the dynamic atexit destructor for `CWideString::NullString`: the inline ~CWideString on the static empty wide string (drop the data block's owner count, free text and block on the last owner, clear the pointer) and the instance-count decrement
// Leaf: _$E2
// Grade: RELOCATION_MATCH -- the owner decrement is a forceinline helper returning `--*owners` through a pointer
//   parameter (gives retail's formed `add eax,0Ch` address and the `mov ecx,edx` return copy) and the free is a
//   forceinline helper taking `storage_` (gives the re-load of the static before `push esi`); helper names descriptive
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0122A550, 78 bytes (+2 int3), in `.text$yd`; registered with atexit by
//   its dynamic initializer @ 0x01223DB0. VC7.1 names the pair `$E1` / `$E2` in the TU.
// Identity: the static's data pointer is 0x013BCA24 and the counter it decrements is 0x013BCA20, the CWideString instance
//   count (bumped by the CWideString constructor 0x0099B440, dropped by ~CWideString 0x0099B510); the block's owner count
//   sits at +0x0C (CWideStringData, CCharString's is the packed +0x0D). FableWin.pdb lists the thunks
//   "`dynamic atexit destructor for 'CCharString::NullString''" and "... 'CWideString::NullString''" side by side; this
//   one is the wide one, so the catalog's "CCharString::NullString" label is the neighbour's (propagated).
// Callees: free @ 0x00BFEA14 (the text), operator delete @ 0x00BFE9BC (the block) -- both import thunks.
// Shape: retail compiles the thunk in the CWideString TU, where the destructor's body is visible and auto-inlined: the
//   landed ~CWideString 0x0099B510 body verbatim (`--owners; if (owners <= 0)` keeps the register decrement, the copy
//   tested before the store, then re-reads the data pointer for the release), then `--InstanceCount`.
//   Tried: this body (RTM and QFE-4035 compilers, same 75 bytes), and ~CWideString() { UnassignString(); --count; }
//   with UnassignString in the TU. The leftover is register allocation only: the same `add eax,N` address formation
//   appears in the PGuiDef thunk 0x01228B70 (parked), so it is likely one inline-expansion artefact of these thunks.
#include <stddef.h>                                        // wchar_t

extern "C" void __cdecl free(void* block);                 // 0x00BFEA14 (import thunk)
void __cdecl operator delete(void* block);                 // 0x00BFE9BC (import thunk)

class CWideStringData { public:                                   // 0x10 bytes
    wchar_t*      text;                                    // +0x00
    unsigned long unknown04;                               // +0x04
    unsigned long unknown08;                               // +0x08
    long          owners;                                  // +0x0C
};

extern long g_CWideStringInstanceCount;                    // 0x013BCA20

class CWideString {
public:
    CWideString();                                         // 0x0099AED0
    ~CWideString();
    static CWideString NullString;                         // 0x013BCA24
private:
    CWideStringData* storage_;
};

static __forceinline long DecrementOwners(long* owners)
{
    long count = --*owners;
    return count;
}

static __forceinline void FreeData(CWideStringData* data)
{
    if (data != 0)
    {
        if (data->text != 0)
            free(data->text);
        delete data;
    }
}

CWideString::~CWideString()
{
    if (storage_ != 0)
    {
        if (DecrementOwners(&storage_->owners) <= 0)
            FreeData(storage_);
        storage_ = 0;
    }

    --g_CWideStringInstanceCount;
}

CWideString CWideString::NullString;