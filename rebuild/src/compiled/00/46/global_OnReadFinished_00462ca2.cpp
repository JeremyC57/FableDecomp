#include "rebuild_abi.h"
// Leaf: OnReadFinished

struct CActiveFile
{
    void* field_0;
};

extern "C" void __cdecl free(void* block);   // 0x00BFEA14 (MSVCR71)

void __fastcall CActiveFile_OnReadFinished(CActiveFile* thisptr)
{
    void* p = thisptr->field_0;
    if (p != 0)
        free(p);
}