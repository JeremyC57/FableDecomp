class CInputProcessWatchForRangedWeaponThirdPersonModeTermination { public:
    void* vftable;
};

void __fastcall CInputProcessWatchForRangedWeaponThirdPersonModeTermination_dtor(CInputProcessWatchForRangedWeaponThirdPersonModeTermination* process);
void operator delete(void*);

void* __fastcall CInputProcessWatchForRangedWeaponThirdPersonModeTermination_scalar_deleting_destructor(CInputProcessWatchForRangedWeaponThirdPersonModeTermination* process, int edx_unused, unsigned int flags)
{
    CInputProcessWatchForRangedWeaponThirdPersonModeTermination_dtor(process);
    if (flags & 1)
        operator delete(process);
    return process;
}