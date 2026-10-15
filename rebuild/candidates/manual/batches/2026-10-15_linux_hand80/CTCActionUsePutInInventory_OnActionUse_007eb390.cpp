// CTCActionUsePutInInventory::OnActionUse(CThing& user) -- base OnActionUse, then when the user is a live creature and this component's thing is an object (type 5) the user gets a CCreatureAction_AddRealObjectToInventory(user, thing) temporary as its current action
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007EB390, length 96 bytes.
// Leaf: OnActionUse
// Callees: CTCActionUseBase::OnActionUse(CThing&) @ 0x007EAB90 (an empty `ret 4`, called non-virtually; the manifest's "$E2"
// is propagated), CCreatureAction_AddRealObjectToInventory::CCreatureAction_AddRealObjectToInventory(CThingCreatureBase*,
// CThingObject*) @ 0x007EB2D0, CThingCreatureBase::SetCurrentAction @ 0x006644F0, CBaseIntelligentPointer::
// ~CBaseIntelligentPointer @ 0x00A01C10, CCreatureActionBase::~CCreatureActionBase @ 0x00693EF0 (the manifest's
// "CPhysicsMeshInfo::~CPhysicsMeshInfo" is propagated). Slot 28 = OnActionUse (Ego_r.pdb).
// Retail CThing: the type byte at +0x10 (5 = object, as the landed script-interface bodies read it) and the `Dead` bit 0 of
// the flag byte at +0x91 (Ego_r.pdb +0xa1, the retail layout is 0x10 bytes narrower before it).
// Shape: `&user` is null-tested (VC7.1 does not assume a reference is non-null); the 0xb0-byte action temporary has an
// IMPLICIT destructor -- its CIntelligentPointer member at +0xa8 is destroyed (base destructor directly) then the base
// action destructor, with no vtable store.
struct CBaseObject;

struct CThing {
    unsigned char _pad00[0x10];
    unsigned char m_type;                      // +0x10 thing type (5 = object)
    unsigned char _pad11[0x80];
    unsigned char Dead : 1;                    // +0x91 bit 0
};

struct CThingObject : CThing { };

struct CBaseIntelligentPointer {
    virtual void v00();
    CBaseObject* m_pItem;                      // +0x04
    CBaseIntelligentPointer();                 // 0x00A01B10
    ~CBaseIntelligentPointer();                // 0x00A01C10
};

struct CIntelligentPointer : CBaseIntelligentPointer {   // vtable 0x01238C6C
    virtual void v00();
};

struct CCreatureActionBase {
    virtual void v00();
    unsigned char _body[0xa4];
    ~CCreatureActionBase();                    // 0x00693EF0
};

struct CCreatureAction_AddRealObjectToInventory : CCreatureActionBase {
    CIntelligentPointer PObject;               // +0xa8
    CCreatureAction_AddRealObjectToInventory(struct CThingCreatureBase* creature, CThingObject* object);   // 0x007EB2D0
};

struct CThingCreatureBase : CThing {
    bool SetCurrentAction(CCreatureActionBase* action);   // 0x006644F0
};

struct CTCBase {                               // engine/CTCBase.h layout, modelled polymorphic
    virtual void v00();
    CThing* ParentThing;                       // +0x04
    bool    ToBeRemoved;                       // +0x08
    bool    IsInGlobalUpdateTCs;               // +0x09
};

struct CTCActionUseBase : CTCBase {
    unsigned char _pad0c[8];                   // +0x0c PActionUseDef, +0x10 Usable
    virtual void OnActionUse(CThing& user);    // slot 28: 0x007EAB90
};

struct CTCActionUsePutInInventory : CTCActionUseBase {
    virtual void OnActionUse(CThing& user);
};

void CTCActionUsePutInInventory::OnActionUse(CThing& user)
{
    CTCActionUseBase::OnActionUse(user);
    CThingCreatureBase* creature = static_cast<CThingCreatureBase*>(&user);
    if (creature && !creature->Dead && ParentThing->m_type == 5)
        creature->SetCurrentAction(&CCreatureAction_AddRealObjectToInventory(creature, static_cast<CThingObject*>(ParentThing)));
}
