struct CPSCSingleSprite {
    void* vftable;
};

void __fastcall CPSCSingleSprite_dtor(CPSCSingleSprite* sprite);
void operator delete(void*);

void* __fastcall CPSCSingleSprite_scalar_deleting_destructor(CPSCSingleSprite* sprite, int edx_unused, unsigned int flags)
{
    CPSCSingleSprite_dtor(sprite);
    if (flags & 1)
        operator delete(sprite);
    return sprite;
}