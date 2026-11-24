// Leaf: ?GetSizeofClass@CCoopSpiritDef@@UAEKXZ
#pragma optimize("s",on)
class CCoopSpiritDef { public:
    virtual unsigned long GetSizeofClass();
};

unsigned long CCoopSpiritDef::GetSizeofClass()
{
    return 0x3c;
}
// Flatten route (flatten_pilot v4, NF324): CCoopSpiritDef_GetSizeofClass -> CCoopSpiritDef::GetSizeofClass, the RTTI-placed PDB member; class model: the row's local class model.
