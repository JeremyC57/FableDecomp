// Leaf: ?GetSizeofClass@CCreatureGenerationFamilyDef@@UAEKXZ
#pragma optimize("s",on)
class CCreatureGenerationFamilyDef { public:
    virtual unsigned long GetSizeofClass();
};

unsigned long CCreatureGenerationFamilyDef::GetSizeofClass()
{
    return 0x5c;
}
// Flatten route (flatten_pilot v4, NF324): CCreatureGenerationFamilyDef_GetSizeofClass -> CCreatureGenerationFamilyDef::GetSizeofClass, the RTTI-placed PDB member; class model: the row's local class model.
