#pragma optimize("s",on)
// Leaf: OnReadFinished
struct CActiveFile;
extern "C" void __fastcall CActiveFile_helper_004511a6(CActiveFile* self);
extern "C" void __cdecl free(void* block);   // 0x00BFEA14 (MSVCR71)

struct CActiveFile {
    void* field0;
};

void __fastcall CActiveFile_OnReadFinished(CActiveFile* self)
{
    CActiveFile_helper_004511a6(self);
    void* p = self->field0;
    if (p) {
        free(p);
    }
}