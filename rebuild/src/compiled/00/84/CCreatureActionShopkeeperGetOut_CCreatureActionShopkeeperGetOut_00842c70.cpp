// CCreatureAction_ShopkeeperGetOut::CCreatureAction_ShopkeeperGetOut(CThingCreatureBase*, CTCShop*, const C3DVector&, const CRightHandedSet&, long) -- base CCreatureActionBase(owner, -1.0, 1.0, the owner's GET_OUT animation, zeroed CSoundPair, 0, 0), then TCShop, Goods, Pos and RHSet
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00842C70, length 206 bytes (+ int3 padding), `ret 14h`. Vtable 0x01273FCC.
// Callees: CCharString::CCharString(const char*, long) @ 0x0099EBF0, the animation lookup by name @ 0x006924B0
// (`__fastcall`: ecx owner, edx the name), CCreatureActionBase::CCreatureActionBase(CThingCreatureBase*, float, float,
// const CAnimationEntry&, const CSoundPair&, long, long) @ 0x00693B30, CCharString::~CCharString @ 0x0099EAE0.
// Shape: as CCreatureAction_SendMessage (0x00982210): the literal converts implicitly to a CCharString temporary built
// first; the init-list members after the vtable store in declaration order, the vector and the right-handed set copied
// dword by dword through their memberwise copy constructors.
// Layout: Ego_r.pdb TCShop +0xb4, Goods +0xb8, Pos +0xbc, RHSet +0xc8 (0x18 bytes); retail CCreatureActionBase is 0xa8,
// so they sit 12 bytes lower (+0xa8 / +0xac / +0xb0 / +0xbc).
class CThingCreatureBase;
class CTCShop;
class CAnimationEntry;

class CCharString { public:
    void* m_pData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                    // 0x0099EAE0
};

class CSoundPair { public:
    long FirstSound;
    long LastSound;
    CSoundPair() : FirstSound(0), LastSound(0) { }
};

const CAnimationEntry& __fastcall GetAnimationFromName(CThingCreatureBase* owner, const CCharString& name);   // 0x006924B0

class CCreatureActionBase { public:                // retail sizeof 0xa8
    virtual void v00();
    unsigned char _body[0xa8 - 4];
    CCreatureActionBase(CThingCreatureBase* owner, float a, float b, const CAnimationEntry& animation, const CSoundPair& sound, long c, long d);   // 0x00693B30
};

class C3DVector { public:
    float X, Y, Z;
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) { }
};

class CRightHandedSet { public:                    // 0x18 bytes: two vectors
    C3DVector Up;
    C3DVector Forward;
};

class CCreatureAction_ShopkeeperGetOut : public CCreatureActionBase { public:   // vtable 0x01273FCC
    CTCShop*        TCShop;                 // +0xa8 (PDB +0xb4)
    long            Goods;                  // +0xac (PDB +0xb8)
    C3DVector       Pos;                    // +0xb0 (PDB +0xbc)
    CRightHandedSet RHSet;                  // +0xbc (PDB +0xc8)
    virtual void v00();
    CCreatureAction_ShopkeeperGetOut(CThingCreatureBase* owner, CTCShop* shop, const C3DVector& pos, const CRightHandedSet& rhSet, long goods);
};

CCreatureAction_ShopkeeperGetOut::CCreatureAction_ShopkeeperGetOut(CThingCreatureBase* owner, CTCShop* shop, const C3DVector& pos, const CRightHandedSet& rhSet, long goods)
    : CCreatureActionBase(owner, -1.0f, 1.0f, GetAnimationFromName(owner, "GET_OUT"), CSoundPair(), 0, 0)
    , TCShop(shop)
    , Goods(goods)
    , Pos(pos)
    , RHSet(rhSet)
{
}