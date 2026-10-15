// CCreatureAction_HeroLoadRangedWeapon::CCreatureAction_HeroLoadRangedWeapon(const CCreatureAction_HeroLoadRangedWeapon&) -- copy constructor: base CCreatureActionBase(other), own vtable (0x0126B9D4), the two CIntelligentPointer members at +0xa8 / +0xb0 copied through GetPItem/SetPItem
// Label: the manifest row says CCreatureAction_BattleChargeImpact (propagated); the stored vtable 0x0126B9D4 is CCreatureAction_HeroLoadRangedWeapon (rtti-vtables.tsv).
// Leaf: ??0CCreatureAction_HeroLoadRangedWeapon
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0078BD60, length 105 bytes.
// Callees: CCreatureActionBase::CCreatureActionBase(const CCreatureActionBase&) @ 0x0062EBC0, CBaseIntelligentPointer::CBaseIntelligentPointer
// @ 0x00A01B10 (twice), CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (twice), CBaseIntelligentPointer::SetPItem @ 0x00A01B90 (twice).
// Shape: `other` is loaded into ebx before esi/edi are pushed and pushed for the out-of-line base copy constructor; the vtable
// is stored before the first member's base call; each member is the inline CIntelligentPointer copy constructor (base ctor,
// its own vtable, SetPItem(other.GetPItem())) -- the first instantiation (vtable 0x01238C6C, CIntelligentPointer<CThing>) then
// the second (0x0125A858, CIntelligentPointer<CThingObject>). `ret 4`.
// Layout: retail CCreatureActionBase is 0xa8 bytes; no engine header exists for this class, so `this` is modelled locally
// (the member names describe the weapon being loaded and its ammunition).
struct CBaseObject;

struct CBaseIntelligentPointer {            // 8 bytes: vptr + the pointed-to item
    virtual void v00();
    CBaseObject* m_pItem;                   // +4
    CBaseIntelligentPointer();              // 0x00A01B10
    CBaseObject* GetPItem() const;          // 0x00A01B50
    void SetPItem(CBaseObject* item);       // 0x00A01B90
};

struct CIntelligentPointer_CThing : CBaseIntelligentPointer {   // vtable 0x01238C6C
    virtual void v00();
    CIntelligentPointer_CThing(const CIntelligentPointer_CThing& other) : CBaseIntelligentPointer() { SetPItem(other.GetPItem()); }
};

struct CIntelligentPointer_CThingObject : CBaseIntelligentPointer {   // vtable 0x0125A858
    virtual void v00();
    CIntelligentPointer_CThingObject(const CIntelligentPointer_CThingObject& other) : CBaseIntelligentPointer() { SetPItem(other.GetPItem()); }
};

struct CCreatureActionBase {                // retail sizeof 0xa8
    virtual void v00();
    unsigned char _body[0xa8 - 4];
    CCreatureActionBase(const CCreatureActionBase& other);   // 0x0062EBC0
};

struct CCreatureAction_HeroLoadRangedWeapon : CCreatureActionBase {   // vtable 0x0126B9D4
    CIntelligentPointer_CThing       PWeapon;   // +0xa8
    CIntelligentPointer_CThingObject PAmmo;     // +0xb0
    virtual void v00();
    CCreatureAction_HeroLoadRangedWeapon(const CCreatureAction_HeroLoadRangedWeapon& other);
};

CCreatureAction_HeroLoadRangedWeapon::CCreatureAction_HeroLoadRangedWeapon(const CCreatureAction_HeroLoadRangedWeapon& other)
    : CCreatureActionBase(other)
    , PWeapon(other.PWeapon)
    , PAmmo(other.PAmmo)
{
}