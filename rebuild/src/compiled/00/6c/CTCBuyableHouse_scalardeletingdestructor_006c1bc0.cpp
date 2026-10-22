struct CTCBuyableHouse {
    void* vftable;
};

void __fastcall CTCBuyableHouse_dtor(CTCBuyableHouse* house);
void operator delete(void*);

void* __fastcall CTCBuyableHouse_scalar_deleting_destructor(CTCBuyableHouse* house, int edx_unused, unsigned int flags)
{
    CTCBuyableHouse_dtor(house);
    if (flags & 1)
        operator delete(house);
    return house;
}