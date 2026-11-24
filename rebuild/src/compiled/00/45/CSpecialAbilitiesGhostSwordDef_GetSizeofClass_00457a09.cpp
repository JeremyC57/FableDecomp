// Leaf: ?GetSizeofClass@CSpecialAbilitiesGhostSwordDef@@UAEKXZ
#pragma optimize("s",on)
typedef unsigned long ulong;
class CSpecialAbilitiesGhostSwordDef { public:
    virtual unsigned long GetSizeofClass();
};
unsigned long CSpecialAbilitiesGhostSwordDef::GetSizeofClass()
{
    return 0x58;
}
// Flatten route (flatten_pilot v4, NF324): CSpecialAbilitiesGhostSwordDef_GetSizeofClass -> CSpecialAbilitiesGhostSwordDef::GetSizeofClass, the RTTI-placed PDB member; class model: the row's local class model.
