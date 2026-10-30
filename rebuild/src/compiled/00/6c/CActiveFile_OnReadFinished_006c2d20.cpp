// CActiveFile::OnReadFinished @ 006c2d20
// Leaf: OnReadFinished
struct CActiveFile {
    void* field0;
};

// base method at 0x6c2c00 (__fastcall on this)
void __fastcall CActiveFile_base_006c2c00(CActiveFile* self);
// free/cdecl function at 0xbfea14 taking one pointer arg
extern "C" void __cdecl free(void* p);                 // 0x00BFEA14 MSVCR71 free (import thunk)

void __fastcall CActiveFile__OnReadFinished(CActiveFile* self)
{
    CActiveFile_base_006c2c00(self);
    void* p = self->field0;
    if (p)
        free(p);
}