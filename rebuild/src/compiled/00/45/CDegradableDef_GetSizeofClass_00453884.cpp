// Leaf: ?GetSizeofClass@CDegradableDef@@UAEKXZ
#pragma optimize("s",on)
typedef unsigned long ulong;
class CDegradableDef { public:
    virtual unsigned long GetSizeofClass();
};
unsigned long CDegradableDef::GetSizeofClass()
{
    return 0x3c;
}
// Flatten route (flatten_pilot v4, NF324): CDegradableDef_GetSizeofClass -> CDegradableDef::GetSizeofClass, the RTTI-placed PDB member; class model: the row's local class model.
