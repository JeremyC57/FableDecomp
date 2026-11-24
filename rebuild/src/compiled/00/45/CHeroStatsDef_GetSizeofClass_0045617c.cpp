// Leaf: ?GetSizeofClass@CHeroStatsDef@@UAEKXZ
class CHeroStatsDef { public:
    virtual unsigned long GetSizeofClass();
};

unsigned long CHeroStatsDef::GetSizeofClass()
{
    return 0x138;
}
// Flatten route (flatten_pilot v4, NF324): CHeroStatsDef_GetSizeofClass -> CHeroStatsDef::GetSizeofClass, the RTTI-placed PDB member; class model: the row's local class model.
