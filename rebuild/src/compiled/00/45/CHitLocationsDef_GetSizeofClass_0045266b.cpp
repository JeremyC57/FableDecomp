// Leaf: ?GetSizeofClass@CHitLocationsDef@@UAEKXZ
#pragma optimize("s",on)
class CHitLocationsDef { public:
    virtual unsigned long GetSizeofClass();
};
unsigned long CHitLocationsDef::GetSizeofClass()
{
    return 0x34;
}
// Flatten route (flatten_pilot v4, NF324): CHitLocationsDef_GetSizeofClass -> CHitLocationsDef::GetSizeofClass, the RTTI-placed PDB member; class model: the row's local class model.
