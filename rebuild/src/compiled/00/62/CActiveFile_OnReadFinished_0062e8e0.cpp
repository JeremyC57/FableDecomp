// CActiveFile::OnReadFinished @ 0062e8e0
// Leaf: OnReadFinished
struct CActiveFile;

void __fastcall base_0x62e2b0(CActiveFile* self);   // 0x62e2b0
extern "C" void __cdecl free(void* block);   // 0x00BFEA14 (MSVCR71)

struct CActiveFile {
    void* field0;
};

void __fastcall CActiveFile_OnReadFinished(CActiveFile* self);

void __fastcall CActiveFile_OnReadFinished(CActiveFile* self)
{
    base_0x62e2b0(self);
    void* p = *(void**)self;   // esi = [esi] ; reload from this
    if (p) {
        free(p);
    }
}