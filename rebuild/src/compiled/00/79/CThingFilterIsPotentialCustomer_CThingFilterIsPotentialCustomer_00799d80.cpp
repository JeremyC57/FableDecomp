// CThingFilter_IsPotentialCustomer::CThingFilter_IsPotentialCustomer(CTCShopKeeper* shopKeeper, bool allowExistingShopper) -- remember the shop keeper, the shop's position and, for a building shop whose physics mesh has a BUILDING_BOUNDRY containment volume, the shop thing as the shopper container
// Leaf: ??0CThingFilter_IsPotentialCustomer
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00799D80, length 173 bytes, `ret 8`, returns `this`.
// Callees: CTCShopKeeper::GetShopTC @ 0x00798F50, CThing::GetPos @ 0x004C73D0, CTCPhysicsBase::GetPhysicsMesh (slot 93,
//   +0x174), CTCPhysicsBase::GetPhysicsMeshObject @ 0x006B0850 (CPhysicsMeshObject by hidden pointer),
//   CPhysicsMeshObject::HasContainmentVolume(const CCharString&) @ 0x00A128A0, ~CPhysicsMeshObject @ 0x00A127D0,
//   CCharString(const char*, long) @ 0x0099EBF0 / ~CCharString @ 0x0099EAE0.
// Literal: 0x0124432C = "BUILDING_BOUNDRY".
// Layout: CThingFilter_IsPotentialCustomer (Ego_r.pdb, 24 bytes): ShopperContainer +0, Pos +4, TCShopKeeper +0x10,
//   AllowExistingShopper +0x14; CTCBase ParentThing +4; retail CTCShop StallLike +0x50 (PDB +0x5c); retail CThing
//   PhysicsTC +0x60; CPhysicsMeshObject 0x40 bytes (the frame).
// Shape: the two arguments are initialiser-list members (stored before the call); ShopperContainer and Pos are body
//   assignments after the shop and its physics component are read into locals (Pos copied through the implicit
//   C3DVector assignment, `lea edx,[esi+4]`); the mesh object is a block-scoped local with the string an implicit
//   conversion temporary (re-addressed with lea), and the found flag survives its destructor in bl.
class C3DVector { public: float X, Y, Z; };
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                               // 0x0099EAE0
};
class CPhysicsMeshObject { public:
    virtual void v00();
    void*         Mesh;                            // +0x04 CIVCountedPointer<CPhysicsMesh const>
    float         PositionMatrix[12];              // +0x08 CMatrix3x4
    float         Scale;                           // +0x38
    float         OO_Scale;                        // +0x3c
    virtual ~CPhysicsMeshObject();                         // 0x00A127D0
    bool HasContainmentVolume(const CCharString& name);   // 0x00A128A0
};
#define FILL10(n) virtual void v##n##0(); virtual void v##n##1(); virtual void v##n##2(); virtual void v##n##3(); \
    virtual void v##n##4(); virtual void v##n##5(); virtual void v##n##6(); virtual void v##n##7(); \
    virtual void v##n##8(); virtual void v##n##9();
class CTCPhysicsBase { public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09();
    FILL10(1) FILL10(2) FILL10(3) FILL10(4) FILL10(5) FILL10(6) FILL10(7) FILL10(8)
    virtual void v90(); virtual void v91(); virtual void v92();
    virtual void* GetPhysicsMesh() const;          // slot 93 (+0x174)
public:
    CPhysicsMeshObject GetPhysicsMeshObject() const;   // 0x006B0850
};
class CThing {
public:
    unsigned char _pad00[0x60];
    CTCPhysicsBase* PhysicsTC;                     // +0x60
    const C3DVector& GetPos() const;          // 0x004C73D0
};
class CTCShop { public:
    void* __vftable;
    CThing* ParentThing;                           // +0x04 (CTCBase)
    unsigned char _pad08[0x50 - 0x08];
    bool StallLike;                                // +0x50
};
class CTCShopKeeper { public:
public:
    CTCShop* GetShopTC() const;   // 0x00798F50
};

class CThingFilter_IsPotentialCustomer { public:
    CThing* ShopperContainer;                      // +0x00
    C3DVector Pos;                                 // +0x04
    CTCShopKeeper* TCShopKeeper;                   // +0x10
    bool AllowExistingShopper;                     // +0x14
    CThingFilter_IsPotentialCustomer(CTCShopKeeper* shopKeeper, bool allowExistingShopper);
};

CThingFilter_IsPotentialCustomer::CThingFilter_IsPotentialCustomer(CTCShopKeeper* shopKeeper, bool allowExistingShopper)
    : TCShopKeeper(shopKeeper), AllowExistingShopper(allowExistingShopper)
{
    CTCShop* shop = TCShopKeeper->GetShopTC();
    CTCPhysicsBase* physics = shop->ParentThing->PhysicsTC;
    ShopperContainer = 0;
    Pos = shop->ParentThing->GetPos();
    if (!shop->StallLike && physics && physics->GetPhysicsMesh()) {
        bool insideBuilding;
        {
            CPhysicsMeshObject mesh = physics->GetPhysicsMeshObject();
            insideBuilding = mesh.HasContainmentVolume("BUILDING_BOUNDRY");
            if (insideBuilding)
                ShopperContainer = shop->ParentThing;
        }
    }
}