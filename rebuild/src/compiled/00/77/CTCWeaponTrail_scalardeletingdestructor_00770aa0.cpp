class CTCWeaponTrail { public:
    void* vftable;
};

void __fastcall CTCWeaponTrail_dtor(CTCWeaponTrail* trail);
void operator delete(void*);

void* __fastcall CTCWeaponTrail_scalar_deleting_destructor(CTCWeaponTrail* trail, int edx_unused, unsigned int flags)
{
    CTCWeaponTrail_dtor(trail);
    if (flags & 1)
        operator delete(trail);
    return trail;
}