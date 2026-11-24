// Leaf: ?Transfer@CCarrySlotDef@@UAEXAAVCPersistContext@@@Z
#pragma optimize("s",on)
struct A { int x; };
struct B { int x; };
struct C { int x; };

class CDefString;
class CPersistContext { public:
    template <class T> void Transfer(T& value);
};

class CCarrySlotDef { public:
    unsigned char _pad[0x24];
    A a;   // +0x28
    B b;   // +0x2c
    C c;   // +0x30
    A a2;  // +0x34
    virtual void Transfer(class CPersistContext&);
};

void CCarrySlotDef::Transfer(class CPersistContext& ctx)
{
    CCarrySlotDef* def = this;
    ctx.Transfer(*(CDefString*)(&def->a));
    ctx.Transfer(*(long*)(&def->b));
    ctx.Transfer(*(bool*)(&def->c));
    ctx.Transfer(*(CDefString*)(&def->a2));
}
// Flatten route (flatten_pilot v4, NF324): CCarrySlotDef_Transfer -> CCarrySlotDef::Transfer, the RTTI-placed PDB member; class model: the row's local class model.
