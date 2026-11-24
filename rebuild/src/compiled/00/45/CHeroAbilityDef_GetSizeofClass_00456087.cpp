// Leaf: ?GetSizeofClass@CHeroAbilityDef@@UAEKXZ
class CHeroAbilityDef { public:
    virtual unsigned long GetSizeofClass();
};

unsigned long CHeroAbilityDef::GetSizeofClass()
{
    return 0x9c;
}
// Flatten route (flatten_pilot v4, NF324): CHeroAbilityDef_GetSizeofClass -> CHeroAbilityDef::GetSizeofClass, the RTTI-placed PDB member; class model: the row's local class model.
