struct CInputProcessUseRangedWeaponZLock {
    void* vftable;
};

void __fastcall CInputProcessUseRangedWeaponZLock_dtor(CInputProcessUseRangedWeaponZLock* process);
void operator delete(void*);

void* __fastcall CInputProcessUseRangedWeaponZLock_scalar_deleting_destructor(CInputProcessUseRangedWeaponZLock* process, int edx_unused, unsigned int flags)
{
    CInputProcessUseRangedWeaponZLock_dtor(process);
    if (flags & 1)
        operator delete(process);
    return process;
}