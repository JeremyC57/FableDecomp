struct CEnginePrimitiveManagerSprites {
    void* vftable;
};

void __fastcall CEnginePrimitiveManagerSprites_dtor(CEnginePrimitiveManagerSprites* sprites);
void operator delete(void*);

void* __fastcall CEnginePrimitiveManagerSprites_scalar_deleting_destructor(CEnginePrimitiveManagerSprites* sprites, int edx_unused, unsigned int flags)
{
    CEnginePrimitiveManagerSprites_dtor(sprites);
    if (flags & 1)
        operator delete(sprites);
    return sprites;
}