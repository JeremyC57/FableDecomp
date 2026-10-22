struct CTCShopKeeper {
    void* vftable;
};

void __fastcall CTCShopKeeper_dtor(CTCShopKeeper* keeper);
void operator delete(void*);

void* __fastcall CTCShopKeeper_scalar_deleting_destructor(CTCShopKeeper* keeper, int edx_unused, unsigned int flags)
{
    CTCShopKeeper_dtor(keeper);
    if (flags & 1)
        operator delete(keeper);
    return keeper;
}