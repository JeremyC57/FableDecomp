// Leaf: ?GetSizeofClass@CAbilityDef@@UAEKXZ
#pragma optimize("s",on)
typedef unsigned long ulong;

class CAbilityDef { public:
    virtual unsigned long GetSizeofClass();
};

unsigned long CAbilityDef::GetSizeofClass()
{
    return 0x2c;
}
// Flatten route (flatten_pilot v4, NF324): CAbilityDef_GetSizeofClass -> CAbilityDef::GetSizeofClass, the RTTI-placed PDB member; class model: the row's local class model.
