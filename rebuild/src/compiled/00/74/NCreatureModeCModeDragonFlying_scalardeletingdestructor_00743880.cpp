struct CModeDragonFlying {
    void* vftable;
};

void __fastcall CModeDragonFlying_dtor(CModeDragonFlying* mode);
void operator delete(void*);

void* __fastcall CModeDragonFlying_scalar_deleting_destructor(CModeDragonFlying* mode, int edx_unused, unsigned int flags)
{
    CModeDragonFlying_dtor(mode);
    if (flags & 1)
        operator delete(mode);
    return mode;
}