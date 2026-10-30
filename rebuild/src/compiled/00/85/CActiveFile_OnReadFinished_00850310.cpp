// CActiveFile::OnReadFinished  @ 0x00850310  (__fastcall, void)
// Leaf: OnReadFinished
// push esi; mov esi,ecx; call 0x850280; mov esi,[esi]; test/je;
// push esi; call 0xbfea14 (free); add esp,4; pop esi; ret

void __fastcall base_helper_850280(void* self);   // __fastcall void(this)
extern "C" void __cdecl free(void* p);                 // 0x00BFEA14 MSVCR71 free (import thunk)

struct CActiveFile
{
    void* m_buf;   // +0x00
};

void __fastcall CActiveFile_OnReadFinished(CActiveFile* self, int /*edx*/)
{
    base_helper_850280(self);
    void* p = self->m_buf;
    if (p)
        free(p);
}