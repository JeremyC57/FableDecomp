// Leaf: ?Transfer@CDragonActionNapalmDef@@UAEXAAVCPersistContext@@@Z
#pragma optimize("s",on)
struct Field { unsigned char b[4]; };

class CPersistContext { public:
    template <class T> void Transfer(T& value);
};

class CDragonActionNapalmDef { public:
    unsigned char _pad[0x24];
    Field f0; // +0x28
    Field f1; // +0x2c
    Field f2; // +0x30
    Field f3; // +0x34
    virtual void Transfer(class CPersistContext&);
};

void CDragonActionNapalmDef::Transfer(class CPersistContext& ctx)
{
    CDragonActionNapalmDef* def = this;
    ctx.Transfer(*(float*)(&def->f0));
    ctx.Transfer(*(float*)(&def->f1));
    ctx.Transfer(*(float*)(&def->f2));
    ctx.Transfer(*(float*)(&def->f3));
}
// Flatten route (flatten_pilot v4, NF324): CDragonActionNapalmDef_Transfer -> CDragonActionNapalmDef::Transfer, the RTTI-placed PDB member; class model: the row's local class model.
