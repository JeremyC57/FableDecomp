#include "rebuild_abi.h"
// Leaf: OnReadFinished

// 0x0046324a  CActiveFile::OnReadFinished (14 bytes)
// mov eax,[ecx]; test eax,eax; je end; push eax; call <free>; pop ecx; ret
//
// Pattern: if (this->m_ptr) operator delete(this->m_ptr);
// The callee is cdecl (single arg cleaned by `pop ecx`), address relocation-masked.

extern "C" void __cdecl free(void* p);                 // 0x00BFEA14 MSVCR71 free (import thunk)

struct CActiveFile_46324a {
    void* m_ptr; // +0x00
};

void __fastcall OnReadFinished_46324a(CActiveFile_46324a* thisptr)
{
    void* p = thisptr->m_ptr;
    if (p) {
        free(p);
    }
}