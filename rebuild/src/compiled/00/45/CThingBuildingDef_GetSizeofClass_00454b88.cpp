// Leaf: ?GetSizeofClass@CThingBuildingDef@@UAEKXZ
class CThingBuildingDef { public:
    virtual unsigned long GetSizeofClass();
};

unsigned long CThingBuildingDef::GetSizeofClass()
{
    return 0xa4;
}
// Flatten route (flatten_pilot v4, NF324): CThingBuildingDef_GetSizeofClass -> CThingBuildingDef::GetSizeofClass, the RTTI-placed PDB member; class model: the row's local class model.
