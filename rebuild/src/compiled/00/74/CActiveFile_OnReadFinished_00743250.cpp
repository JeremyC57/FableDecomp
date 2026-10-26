struct CActiveFile {
// Leaf: OnReadFinished
    void* field0;
};

// member at 0x742290, this-in-ecx, no other args
void __fastcall CActiveFile_Base_00742290(CActiveFile* self);
// cdecl free function at 0xbfea14
extern "C" void __cdecl free(void* block);   // 0x00BFEA14 (MSVCR71)

void __fastcall CActiveFile_OnReadFinished(CActiveFile* self)
{
    CActiveFile_Base_00742290(self);
    void* p = self->field0;
    if (p)
        free(p);
}