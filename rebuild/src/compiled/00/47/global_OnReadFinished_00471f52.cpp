#include "rebuild_abi.h"
// Leaf: OnReadFinished

/* 0x00471f52  CActiveFile::OnReadFinished (14 bytes)
 * mov eax,[ecx]; test eax,eax; je end; push eax; call <free>; pop ecx; ret
 * A void __fastcall member that reads the pointer at this+0 and, when non-null,
 * passes it to a cdecl free function (1 arg cleaned by pop ecx).
 */

struct CActiveFile
{
    void* ptr; /* +0x00 */
};

/* cdecl callee; single pointer arg (caller cleans stack via pop ecx). */
extern "C" void __cdecl free(void* p);                 // 0x00BFEA14 MSVCR71 free (import thunk)

void __fastcall CActiveFile_OnReadFinished(CActiveFile* thisptr)
{
    void* p = thisptr->ptr;
    if (p)
    {
        free(p);
    }
}