#include "rebuild_abi.h"
// Leaf: OnReadFinished

// 0x00461009  CActiveFile::OnReadFinished (14 bytes)
// Loads this+0x00, and if non-null, passes it to a global free-like callee.
// __fastcall accessor modeled as free __fastcall(objptr).

struct CActiveFile
{
    void* ptr; // +0x00
};

// relocation-masked engine callee (operator delete / free-like)
extern "C" void __cdecl free(void* p);                 // 0x00BFEA14 MSVCR71 free (import thunk)

void __fastcall CActiveFile_OnReadFinished(CActiveFile* thisptr)
{
    void* p = thisptr->ptr;   // mov eax,[ecx]
    if (p != 0)               // test eax,eax / je
    {
        free(p); // push eax / call ; pop ecx
    }
}