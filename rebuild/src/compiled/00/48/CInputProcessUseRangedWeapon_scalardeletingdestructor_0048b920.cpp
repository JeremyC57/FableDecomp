class CInputProcessUseRangedWeapon { public:
    void* vftable;
};

void __fastcall CInputProcessUseRangedWeapon_dtor(CInputProcessUseRangedWeapon* process);
void operator delete(void*);

void* __fastcall CInputProcessUseRangedWeapon_scalar_deleting_destructor(CInputProcessUseRangedWeapon* process, int edx_unused, unsigned int flags)
{
    CInputProcessUseRangedWeapon_dtor(process);
    if (flags & 1)
        operator delete(process);
    return process;
}