#include "rebuild_abi.h"
// Leaf: OnReadFinished

struct CActiveFile
{
    void* m_ptr; /* +0x00 */
};

extern "C" void __cdecl free(void* p);                 // 0x00BFEA14 MSVCR71 free (import thunk)

void __fastcall CActiveFile_OnReadFinished(CActiveFile* thisptr)
{
    void* p = thisptr->m_ptr;
    if (p != 0)
    {
        free(p);
    }
}