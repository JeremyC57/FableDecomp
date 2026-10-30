struct CActiveFile {
// Leaf: OnReadFinished
    void* field0;
};

// __fastcall base method at 0x70b8a0 (same module)
void __fastcall CActiveFile_base(CActiveFile* self);

// cdecl free function at 0xbfea14
extern "C" void __cdecl free(void* p);                 // 0x00BFEA14 MSVCR71 free (import thunk)

void __fastcall OnReadFinished(CActiveFile* self)
{
    CActiveFile_base(self);
    void* p = self->field0;
    if (p) {
        free(p);
    }
}