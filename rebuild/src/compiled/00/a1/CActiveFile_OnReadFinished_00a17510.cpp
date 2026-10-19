// CActiveFile::OnReadFinished @ 0x00a17510
// push esi; mov esi,ecx; call Helper(this); esi=[this]; if(esi) delete esi; ret
extern "C" void __cdecl free(void* block);   // MSVCR71 import thunk 0x00BFEA14

struct CActiveFile {
    void* m_data;         // +0x00
    void Finish();        // helper @ 0xa17390, __fastcall void(this) -- decl only, forces a call
    void OnReadFinished();
};

void CActiveFile::OnReadFinished()
{
    Finish();
    void* p = m_data;
    if (p)
        free(p);
}