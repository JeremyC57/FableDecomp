struct CTCChest {
    void* vftable;
};

void __fastcall CTCChest_dtor(CTCChest* chest);
void operator delete(void*);

void* __fastcall CTCChest_scalar_deleting_destructor(CTCChest* chest, int edx_unused, unsigned int flags)
{
    CTCChest_dtor(chest);
    if (flags & 1)
        operator delete(chest);
    return chest;
}