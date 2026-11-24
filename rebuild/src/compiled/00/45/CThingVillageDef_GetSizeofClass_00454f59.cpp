// Leaf: ?GetSizeofClass@CThingVillageDef@@UAEKXZ
class CThingVillageDef { public:
    virtual unsigned long GetSizeofClass();
};
unsigned long CThingVillageDef::GetSizeofClass()
{
    return 0x80;
}
// Flatten route (flatten_pilot v4, NF324): CThingVillageDef_GetSizeofClass -> CThingVillageDef::GetSizeofClass, the RTTI-placed PDB member; class model: the row's local class model.
