#pragma optimize("s",on)
// Leaf: OnReadFinished
#include "rebuild_abi.h"

struct CActiveFile_0046638b {
    void* field0;   // +0x00
    void* field4;   // +0x04
};

// First callee: fastcall taking ecx=[this+0], edx=[this+4], plus one stack arg (out byte ptr).
extern void __fastcall sub_fastcall_0046638b(void* a, void* b, char* out);
// Second callee: cdecl free function taking one pointer arg (caller cleans via pop ecx).
extern "C" void __cdecl free(void* p);                 // 0x00BFEA14 MSVCR71 free (import thunk)

// 0x0046638b  CActiveFile::OnReadFinished  (__fastcall, void, 1 param)
void __fastcall OnReadFinished_0046638b(CActiveFile_0046638b* thisptr)
{
    char local;
    sub_fastcall_0046638b(thisptr->field0, thisptr->field4, &local);
    void* p = thisptr->field0;
    if (p) {
        free(p);
    }
}