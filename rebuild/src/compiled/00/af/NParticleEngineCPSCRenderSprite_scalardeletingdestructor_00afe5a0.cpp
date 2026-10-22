struct CPSCRenderSprite {
    void* vftable;
};

void __fastcall CPSCRenderSprite_dtor(CPSCRenderSprite* sprite);
void operator delete(void*);

void* __fastcall CPSCRenderSprite_scalar_deleting_destructor(CPSCRenderSprite* sprite, int edx_unused, unsigned int flags)
{
    CPSCRenderSprite_dtor(sprite);
    if (flags & 1)
        operator delete(sprite);
    return sprite;
}