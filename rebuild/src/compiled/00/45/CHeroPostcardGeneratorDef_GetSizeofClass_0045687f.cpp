// Leaf: ?GetSizeofClass@CHeroPostcardGeneratorDef@@UAEKXZ
#pragma optimize("s",on)
class CHeroPostcardGeneratorDef { public:
    virtual unsigned long GetSizeofClass();
};

unsigned long CHeroPostcardGeneratorDef::GetSizeofClass()
{
    return 0x5c;
}
// Flatten route (flatten_pilot v4, NF324): CHeroPostcardGeneratorDef_GetSizeofClass -> CHeroPostcardGeneratorDef::GetSizeofClass, the RTTI-placed PDB member; class model: the row's local class model.
