// Leaf: ?Transfer@CVillagerInteractionsDef@@UAEXAAVCPersistContext@@@Z
#pragma optimize("s",on)
class CDefString;
class CPersistContext { public:
    template <class T> void Transfer(T& value);
};

class CVillagerInteractionsDef { public:
    char pad[0x38];
    int fieldA;   // +0x3c
    int fieldB;   // +0x40
    int fieldC;   // +0x44
    virtual void Transfer(class CPersistContext&);
};

void CVillagerInteractionsDef::Transfer(class CPersistContext& ctx)
{
    CVillagerInteractionsDef* def = this;
    ctx.Transfer(*(CDefString*)&(def->fieldA));
    ctx.Transfer(*(CDefString*)&(def->fieldB));
    ctx.Transfer(*(float*)&(def->fieldC));
}
// Flatten route (flatten_pilot v4, NF324): CVillagerInteractionsDef_Transfer -> CVillagerInteractionsDef::Transfer, the RTTI-placed PDB member; class model: the row's local class model.
