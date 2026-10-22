struct CEnginePrimitiveManagerRepeatedZSprites {
    void* vftable;
};

void __fastcall CEnginePrimitiveManagerRepeatedZSprites_dtor(CEnginePrimitiveManagerRepeatedZSprites* sprites);
void operator delete(void*);

void* __fastcall CEnginePrimitiveManagerRepeatedZSprites_scalar_deleting_destructor(CEnginePrimitiveManagerRepeatedZSprites* sprites, int edx_unused, unsigned int flags)
{
    CEnginePrimitiveManagerRepeatedZSprites_dtor(sprites);
    if (flags & 1)
        operator delete(sprites);
    return sprites;
}