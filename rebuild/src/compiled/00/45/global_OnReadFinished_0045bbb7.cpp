#include "rebuild_abi.h"
// Leaf: OnReadFinished

// 0x0045bbb7 CActiveFile::OnReadFinished (__fastcall, modeled as __fastcall)
// Disasm:
//   mov eax,[ecx]     ; load first member
//   test eax,eax
//   je  end
//   push eax
//   call <freefn>     ; some free/cdecl callee taking the pointer
//   pop  ecx
//   ret

struct CActiveFile
{
    void* field_0;
};

// Relocation-masked callee; takes the member pointer.
extern "C" void __cdecl free(void* p);                 // 0x00BFEA14 MSVCR71 free (import thunk)

void __fastcall CActiveFile_OnReadFinished(CActiveFile* thisptr)
{
    void* p = thisptr->field_0;
    if (p)
        free(p);
}