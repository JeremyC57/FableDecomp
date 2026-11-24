// Leaf: ?Transfer@CCombatAbilityStrafeDef@@UAEXAAVCPersistContext@@@Z
#pragma optimize("s",on)
struct CFieldA { int a; };
struct CFieldB { int b; };

class CPersistContext { public:
    template <class T> void Transfer(T& value);
    void TransferB(CFieldB* p);
};

class CCombatAbilityStrafeDef { public:
    unsigned char _pad28[0x24];
    CFieldB f28;
    unsigned char _pad40[0x40 - 0x28 - sizeof(CFieldB)];
    CFieldA f40;
    virtual void Transfer(class CPersistContext&);
};

void CCombatAbilityStrafeDef::Transfer(class CPersistContext& ctx)
{
    CCombatAbilityStrafeDef* def = this;
    ctx.Transfer(*(float*)(&def->f40));
    ctx.TransferB(&def->f28);
}
// Flatten route (flatten_pilot v4, NF324): CCombatAbilityStrafeDef_Transfer -> CCombatAbilityStrafeDef::Transfer, the RTTI-placed PDB member; class model: the row's local class model.
