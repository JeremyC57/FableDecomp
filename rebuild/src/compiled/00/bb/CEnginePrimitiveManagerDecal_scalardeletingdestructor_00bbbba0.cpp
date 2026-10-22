struct CEnginePrimitiveManagerDecal {
    void* vftable;
};

void __fastcall CEnginePrimitiveManagerDecal_dtor(CEnginePrimitiveManagerDecal* decal);
void operator delete(void*);

void* __fastcall CEnginePrimitiveManagerDecal_scalar_deleting_destructor(CEnginePrimitiveManagerDecal* decal, int edx_unused, unsigned int flags)
{
    CEnginePrimitiveManagerDecal_dtor(decal);
    if (flags & 1)
        operator delete(decal);
    return decal;
}