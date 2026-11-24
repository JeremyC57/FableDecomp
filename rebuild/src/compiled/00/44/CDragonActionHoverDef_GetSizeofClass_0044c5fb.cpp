// Leaf: ?GetSizeofClass@CDragonActionHoverDef@@UAEKXZ
#pragma optimize("s",on)
class CDragonActionHoverDef { public:
    virtual unsigned long GetSizeofClass();
};

unsigned long CDragonActionHoverDef::GetSizeofClass()
{
    return 0x2c;
}
// Flatten route (flatten_pilot v4, NF324): CDragonActionHoverDef_GetSizeofClass -> CDragonActionHoverDef::GetSizeofClass, the RTTI-placed PDB member; class model: the row's local class model.
