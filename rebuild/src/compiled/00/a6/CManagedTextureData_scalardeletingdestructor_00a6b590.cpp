struct CManagedTextureData {
    void* vftable;
};

void __fastcall CManagedTextureData_dtor(CManagedTextureData* texture);
void operator delete(void*);

void* __fastcall CManagedTextureData_scalar_deleting_destructor(CManagedTextureData* texture, int edx_unused, unsigned int flags)
{
    CManagedTextureData_dtor(texture);
    if (flags & 1)
        operator delete(texture);
    return texture;
}