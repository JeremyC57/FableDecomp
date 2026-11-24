// Leaf: ?GetSizeofClass@CRegionDef@@UAEKXZ
class CRegionDef { public:
    virtual unsigned long GetSizeofClass();
};

unsigned long CRegionDef::GetSizeofClass()
{
    return 0xc4;
}
// Flatten route (flatten_pilot v4, NF324): CRegionDef_GetSizeofClass -> CRegionDef::GetSizeofClass, the RTTI-placed PDB member; class model: the row's local class model.
