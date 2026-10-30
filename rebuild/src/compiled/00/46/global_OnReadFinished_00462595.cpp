#include "rebuild_abi.h"
// Leaf: OnReadFinished

struct CActiveFile
{
    void* m_ptr; /* +0x00 */
};

/* cdecl callee taking the pointer (relocation-masked target) */
extern "C" void __cdecl free(void* p);                 // 0x00BFEA14 MSVCR71 free (import thunk)

/* __fastcall accessor modeled as free __fastcall(thisptr) */
void __fastcall CActiveFile_OnReadFinished(CActiveFile* thisptr)
{
    void* p = thisptr->m_ptr;   /* mov eax,[ecx] */
    if (p != 0)                 /* test eax,eax / je */
    {
        free(p); /* push eax / call / pop ecx */
    }
}