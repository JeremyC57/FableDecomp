// Leaf: ?GetSizeofClass@CSpecialEffectsDef@@UAEKXZ
#pragma optimize("s",on)
class CSpecialEffectsDef { public:
    virtual unsigned long GetSizeofClass();
};

unsigned long CSpecialEffectsDef::GetSizeofClass()
{
    return 0x34;
}
// Flatten route (flatten_pilot v4, NF324): CSpecialEffectsDef_GetSizeofClass -> CSpecialEffectsDef::GetSizeofClass, the RTTI-placed PDB member; class model: the row's local class model.
