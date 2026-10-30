#include "rebuild_abi.h"
// Leaf: OnReadFinished

struct CActiveFile
{
    void* m_ptr;   // +0x00
};

/* single-arg __cdecl callee (relocation-masked target; likely operator delete / free) */
extern "C" void __cdecl free(void* p);                 // 0x00BFEA14 MSVCR71 free (import thunk)

/* __fastcall accessor modeled as free __fastcall(this) — byte-identical for the lander */
void __fastcall CActiveFile_OnReadFinished(CActiveFile* thisptr)
{
    void* p = thisptr->m_ptr;      /* mov eax,[ecx] ; test eax,eax */
    if (p != 0)                    /* je end */
        free(p);        /* push eax ; call ; pop ecx (cdecl 1 arg) */
}