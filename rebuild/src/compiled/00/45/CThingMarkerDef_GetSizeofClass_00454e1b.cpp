// Leaf: ?GetSizeofClass@CThingMarkerDef@@UAEKXZ
class CThingMarkerDef { public:
    virtual unsigned long GetSizeofClass();
};

unsigned long CThingMarkerDef::GetSizeofClass()
{
    return 0x88;
}
// Flatten route (flatten_pilot v4, NF324): CThingMarkerDef_GetSizeofClass -> CThingMarkerDef::GetSizeofClass, the RTTI-placed PDB member; class model: the row's local class model.
