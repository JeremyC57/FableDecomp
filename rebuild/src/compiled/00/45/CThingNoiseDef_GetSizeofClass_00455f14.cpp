// Leaf: ?GetSizeofClass@CThingNoiseDef@@UAEKXZ
class CThingNoiseDef { public:
    virtual unsigned long GetSizeofClass();
};

unsigned long CThingNoiseDef::GetSizeofClass()
{
    return 0xa4;
}
// Flatten route (flatten_pilot v4, NF324): CThingNoiseDef_GetSizeofClass -> CThingNoiseDef::GetSizeofClass, the RTTI-placed PDB member; class model: the row's local class model.
