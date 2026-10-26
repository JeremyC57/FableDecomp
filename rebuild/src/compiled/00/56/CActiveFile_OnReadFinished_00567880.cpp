struct CActiveFile;
// Leaf: OnReadFinished

// __fastcall helper on self, returns nothing we use
void __fastcall CActiveFile_base_00566e10(CActiveFile* self);
// cdecl free helper taking the field0 pointer
extern "C" void __cdecl free(void* block);   // 0x00BFEA14 (MSVCR71)

struct CActiveFile {
    void* field0;
};

void __fastcall OnReadFinished(CActiveFile* self)
{
    CActiveFile_base_00566e10(self);
    void* p = self->field0;
    if (p)
        free(p);
}