class CEnginePrimitiveManagerWeaponTrails { public:
    void* vftable;
};

void __fastcall CEnginePrimitiveManagerWeaponTrails_dtor(CEnginePrimitiveManagerWeaponTrails* trails);
void operator delete(void*);

void* __fastcall CEnginePrimitiveManagerWeaponTrails_scalar_deleting_destructor(CEnginePrimitiveManagerWeaponTrails* trails, int edx_unused, unsigned int flags)
{
    CEnginePrimitiveManagerWeaponTrails_dtor(trails);
    if (flags & 1)
        operator delete(trails);
    return trails;
}