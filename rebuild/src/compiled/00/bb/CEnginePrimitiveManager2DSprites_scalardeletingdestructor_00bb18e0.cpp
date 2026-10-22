struct CEnginePrimitiveManager2DSprites {
    void* vftable;
};

void __fastcall CEnginePrimitiveManager2DSprites_dtor(CEnginePrimitiveManager2DSprites* sprites);
void operator delete(void*);

void* __fastcall CEnginePrimitiveManager2DSprites_scalar_deleting_destructor(CEnginePrimitiveManager2DSprites* sprites, int edx_unused, unsigned int flags)
{
    CEnginePrimitiveManager2DSprites_dtor(sprites);
    if (flags & 1)
        operator delete(sprites);
    return sprites;
}