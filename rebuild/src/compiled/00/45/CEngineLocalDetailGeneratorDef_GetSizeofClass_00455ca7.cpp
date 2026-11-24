// Leaf: ?GetSizeofClass@CEngineLocalDetailGeneratorDef@@UAEKXZ
#pragma optimize("s",on)
class CEngineLocalDetailGeneratorDef { public:
    virtual unsigned long GetSizeofClass();
};

unsigned long CEngineLocalDetailGeneratorDef::GetSizeofClass()
{
    return 0x48;
}
// Flatten route (flatten_pilot v4, NF324): CEngineLocalDetailGeneratorDef_GetSizeofClass -> CEngineLocalDetailGeneratorDef::GetSizeofClass, the RTTI-placed PDB member; class model: the row's local class model.
