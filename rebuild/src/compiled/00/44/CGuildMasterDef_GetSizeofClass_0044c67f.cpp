// Leaf: ?GetSizeofClass@CGuildMasterDef@@UAEKXZ
#pragma optimize("s",on)
class CGuildMasterDef { public:
    virtual unsigned long GetSizeofClass();
};

unsigned long CGuildMasterDef::GetSizeofClass()
{
    return 0x30;
}
// Flatten route (flatten_pilot v4, NF324): CGuildMasterDef_GetSizeofClass -> CGuildMasterDef::GetSizeofClass, the RTTI-placed PDB member; class model: the row's local class model.
