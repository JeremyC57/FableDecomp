// CActiveFile::OnReadFinished  0x009ac2b0  __fastcall void(CActiveFile*)

extern "C" void freefn(void* p);            // cdecl callee 0xbfea14

struct CActiveFile {
    void* buffer;      // +0x00
    void Helper_009aae50();     // external thiscall member 0x9aae50 (no args)
    void OnReadFinished();
};

void CActiveFile::OnReadFinished()
{
    this->Helper_009aae50();               // call 0x9aae50 (thiscall, ecx=self)
    void* p = this->buffer;       // mov esi,[esi]
    if (p != 0)                   // test/je
        freefn(p);                // push esi; call; add esp,4
}