// CEngineInternalPrimitiveWeaponTrail::`scalar deleting destructor'(unsigned int flags) -- ~CEngineInternalPrimitiveWeaponTrail, then operator delete when bit 0 of flags is set; returns this
// Leaf: ?CEngineInternalPrimitiveWeaponTrail_scalar_deleting_destructor@@YIPAXPAVCEngineInternalPrimitiveWeaponTrail@@HI@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00BCDE20, `ret 4`; vftable 0x012A6B24 slot 0; calls 0x00BCDE40 (the class destructor) and operator delete 0x00BFE9BC.
// Naming (NF456, r454b unit with 0x00BCDE40): the destructor call is the real one, ??1CEngineInternalPrimitiveWeaponTrail@@UAE@XZ (Ego_r + FableWin name
//   sets; virtual), written as the qualified call `trail->CEngineInternalPrimitiveWeaponTrail::~CEngineInternalPrimitiveWeaponTrail()` (direct, not
//   through the vftable); was the free stand-in CEngineInternalPrimitiveWeaponTrail_dtor. The self stays the free stand-in: retail's ??_G is
//   compiler-generated and only emitted beside the vftable (the ctor-owned thin-row route), which is not this row's object.
class CEngineInternalPrimitiveWeaponTrail { public:
    virtual ~CEngineInternalPrimitiveWeaponTrail();  // 0x00BCDE40
};

void operator delete(void*);

void* __fastcall CEngineInternalPrimitiveWeaponTrail_scalar_deleting_destructor(CEngineInternalPrimitiveWeaponTrail* trail, int edx_unused, unsigned int flags)
{
    trail->CEngineInternalPrimitiveWeaponTrail::~CEngineInternalPrimitiveWeaponTrail();
    if (flags & 1)
        operator delete(trail);
    return trail;
}
