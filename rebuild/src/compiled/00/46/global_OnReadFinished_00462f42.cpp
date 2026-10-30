#include "rebuild_abi.h"
// Leaf: OnReadFinished

// 0x00462f42  CActiveFile::OnReadFinished (14 bytes)
// mov eax,[ecx]; test eax,eax; je end; push eax; call cdecl_free; pop ecx; ret

struct CActiveFile {
    void* ptr0; // this+0x00
};

// cdecl callee taking the single pointer argument (address relocation-masked)
extern "C" void __cdecl free(void* p);                 // 0x00BFEA14 MSVCR71 free (import thunk)

void __fastcall OnReadFinished_00462f42(CActiveFile* thisptr)
{
    void* p = thisptr->ptr0;
    if (p != 0) {
        free(p);
    }
}