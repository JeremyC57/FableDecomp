#pragma optimize("s",on)
// Leaf: ??1CSystemRegistry@
extern "C" void __cdecl free(void* block);   // 0x00BFEA14 (MSVCR71)

struct CSystemRegistry {
    void* hRoot;
    void* hKey;
    void* field8;
    void* fieldC;
    void* field10;
};

void __fastcall CSystemRegistry_dtor(CSystemRegistry* self)
{
    if (self->field10)
        free(self->field10);
    if (self->hRoot)
        free(self->hRoot);
}