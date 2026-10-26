// CActiveFile::OnReadFinished @ 0x007196b0
// Leaf: OnReadFinished
// push esi; mov esi,ecx; call base(0x719280,this); mov esi,[esi];
// test esi,esi; jz end; push esi; call free(0xbfea14); add esp,4; pop esi; ret

struct CActiveFile {
    void* field0;
};

// base function at 0x719280 (thiscall, void)
extern void __fastcall CActiveFile_base_00719280(CActiveFile* self);

// cdecl free function at 0xbfea14
extern "C" void __cdecl free(void* block);   // 0x00BFEA14 (MSVCR71)

void __fastcall CActiveFile_OnReadFinished(CActiveFile* self)
{
    CActiveFile_base_00719280(self);
    void* p = self->field0;
    if (p)
        free(p);
}