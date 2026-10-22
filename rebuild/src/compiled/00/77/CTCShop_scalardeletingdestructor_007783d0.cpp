struct CTCShop {
    void* vftable;
};

void __fastcall CTCShop_dtor(CTCShop* shop);
void operator delete(void*);

void* __fastcall CTCShop_scalar_deleting_destructor(CTCShop* shop, int edx_unused, unsigned int flags)
{
    CTCShop_dtor(shop);
    if (flags & 1)
        operator delete(shop);
    return shop;
}