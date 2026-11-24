// Leaf: ?GetSizeofClass@CSpecialAbilitiesDrunkennessDef@@UAEKXZ
#pragma optimize("s",on)
class CSpecialAbilitiesDrunkennessDef { public:
    virtual unsigned long GetSizeofClass();
};

unsigned long CSpecialAbilitiesDrunkennessDef::GetSizeofClass()
{
    return 0x50;
}
// Flatten route (flatten_pilot v4, NF324): CSpecialAbilitiesDrunkennessDef_GetSizeofClass -> CSpecialAbilitiesDrunkennessDef::GetSizeofClass, the RTTI-placed PDB member; class model: the row's local class model.
