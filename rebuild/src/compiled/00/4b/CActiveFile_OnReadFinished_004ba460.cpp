// CActiveFile::OnReadFinished @ 0x004ba460
// Leaf: OnReadFinished
struct CActiveFile;

extern "C" void __cdecl free(void* block);   // 0x00BFEA14 (MSVCR71)
void __fastcall CActiveFileBase_OnReadFinished(CActiveFile* self);

struct CActiveFile {
    void* field0;
};

void __fastcall OnReadFinished(CActiveFile* self)
{
    CActiveFileBase_OnReadFinished(self);
    void* p = self->field0;
    if (p != 0)
        free(p);
}