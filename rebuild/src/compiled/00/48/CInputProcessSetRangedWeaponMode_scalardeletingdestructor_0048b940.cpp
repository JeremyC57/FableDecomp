struct CInputProcessSetRangedWeaponMode {
    void* vftable;
};

void __fastcall CInputProcessSetRangedWeaponMode_dtor(CInputProcessSetRangedWeaponMode* process);
void operator delete(void*);

void* __fastcall CInputProcessSetRangedWeaponMode_scalar_deleting_destructor(CInputProcessSetRangedWeaponMode* process, int edx_unused, unsigned int flags)
{
    CInputProcessSetRangedWeaponMode_dtor(process);
    if (flags & 1)
        operator delete(process);
    return process;
}