// Leaf: ?Transfer@CPhysicsDef@@UAEXAAVCPersistContext@@@Z
#pragma optimize("s",on)
class CPersistContext { public:
    template <class T> void Transfer(T& value);
};

class CPhysicsDef { public:
    unsigned char _pad[0x24];
    float Diameter;
    float Friction;
    float Elasticity;
    int   pad34;
    int   pad38;
    int   pad3c;
    unsigned char sub[4];
    virtual void Transfer(class CPersistContext&);
};

void CPhysicsDef::Transfer(class CPersistContext& ctx)
{
    CPhysicsDef* def = this;
    ctx.Transfer(*(float*)(&def->Diameter));
    ctx.Transfer(*(float*)(&def->Friction));
    ctx.Transfer(*(float*)(&def->Elasticity));
    ctx.Transfer(*(long*)(&def->pad3c));
    ctx.Transfer(*(float*)((float*)&def->pad34));
    ctx.Transfer(*(float*)((float*)&def->pad38));
    ctx.Transfer(*(bool*)(&def->sub[0]));
}
// Flatten route (flatten_pilot v4, NF324): CPhysicsDef_Transfer -> CPhysicsDef::Transfer, the RTTI-placed PDB member; class model: the row's local class model.
