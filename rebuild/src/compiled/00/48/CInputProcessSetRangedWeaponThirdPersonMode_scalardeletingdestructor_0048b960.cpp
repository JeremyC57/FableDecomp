class CInputProcessSetRangedWeaponThirdPersonMode { public:
    void* vftable;
};

void __fastcall CInputProcessSetRangedWeaponThirdPersonMode_dtor(CInputProcessSetRangedWeaponThirdPersonMode* process);
void operator delete(void*);

void* __fastcall CInputProcessSetRangedWeaponThirdPersonMode_scalar_deleting_destructor(CInputProcessSetRangedWeaponThirdPersonMode* process, int edx_unused, unsigned int flags)
{
    CInputProcessSetRangedWeaponThirdPersonMode_dtor(process);
    if (flags & 1)
        operator delete(process);
    return process;
}