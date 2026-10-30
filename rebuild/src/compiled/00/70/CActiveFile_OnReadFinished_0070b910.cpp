// CActiveFile::OnReadFinished @ 0x0070b910
// Leaf: OnReadFinished
// shape: base thiscall call, then reload this->f0, if nonzero pass to cdecl free fn.

struct CActiveFile {
    void *f0;
};

// base function at 0x70b8a0 (thiscall, this only)
void __fastcall CActiveFile_base(CActiveFile *self);

// cdecl free function at 0x4f3104, one pointer arg
extern "C" void __cdecl free(void* p);                 // 0x00BFEA14 MSVCR71 free (import thunk)

void __fastcall CActiveFile__OnReadFinished(CActiveFile *self)
{
    CActiveFile_base(self);
    void *p = self->f0;
    if (p)
        free(p);
}