// CActiveFile::OnReadFinished @ 0062e820
// Leaf: OnReadFinished
struct CActiveFile;

void __fastcall base_helper(CActiveFile* self);       // 0x62e2b0, ecx=this
extern "C" void __cdecl free(void* block);   // 0x00BFEA14 (MSVCR71)

struct CActiveFile {
    void* m0;   // [this+0]
};

void __fastcall OnReadFinished(CActiveFile* self)
{
    base_helper(self);
    void* p = self->m0;
    if (p)
        free(p);
}