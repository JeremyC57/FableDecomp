struct CTCRockTrollShield {
    void* vftable;
};

void __fastcall CTCRockTrollShield_dtor(CTCRockTrollShield* shield);
void operator delete(void*);

void* __fastcall CTCRockTrollShield_scalar_deleting_destructor(CTCRockTrollShield* shield, int edx_unused, unsigned int flags)
{
    CTCRockTrollShield_dtor(shield);
    if (flags & 1)
        operator delete(shield);
    return shield;
}