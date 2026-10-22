#pragma optimize("s",on)
extern "C" void __cdecl free_block(void* p);

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
        free_block(self->field10);
    if (self->hRoot)
        free_block(self->hRoot);
}