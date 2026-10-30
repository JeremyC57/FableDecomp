// CActiveFile::OnReadFinished  @ 0x008832a0
// Leaf: OnReadFinished
// push esi; mov esi,ecx; call helper (thiscall, no args); mov esi,[esi];
// test esi,esi; je skip; push esi; call free (cdecl); add esp,4; skip: pop esi; ret

void __fastcall CActiveFile_Helper(void* self);   // 0x882b80 (thiscall, no args)
extern "C" void __cdecl free(void* p);                 // 0x00BFEA14 MSVCR71 free (import thunk)

struct CActiveFile {
    void* m_buffer;   // +0x00
};

void __fastcall CActiveFile_OnReadFinished(CActiveFile* self)
{
    CActiveFile_Helper(self);
    void* p = self->m_buffer;
    if (p)
        free(p);
}