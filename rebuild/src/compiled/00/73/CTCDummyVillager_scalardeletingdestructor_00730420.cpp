struct CTCDummyVillager {
    void* vftable;
};

void __fastcall CTCDummyVillager_dtor(CTCDummyVillager* villager);
void operator delete(void*);

void* __fastcall CTCDummyVillager_scalar_deleting_destructor(CTCDummyVillager* villager, int edx_unused, unsigned int flags)
{
    CTCDummyVillager_dtor(villager);
    if (flags & 1)
        operator delete(villager);
    return villager;
}