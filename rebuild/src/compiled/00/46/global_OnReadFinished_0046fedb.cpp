#include "rebuild_abi.h"
// Leaf: OnReadFinished

struct CActiveFile {
    void* field0;
};

extern "C" void __cdecl free(void* p);                 // 0x00BFEA14 MSVCR71 free (import thunk)

void __fastcall OnReadFinished_0046fedb(CActiveFile* thisptr)
{
    void* p = thisptr->field0;
    if (p) {
        free(p);
    }
}