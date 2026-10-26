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
class CBaseObject;

class CThing { public:
    unsigned char _pad00[0x10];
    unsigned char m_type;                      // +0x10 thing type (5 = object)
    unsigned char _pad11[0x80];
    unsigned char Dead : 1;                    // +0x91 bit 0
};

class CThingObject : public CThing { public: };

class CBaseObjectPointer;
class CBaseIntelligentPointer {                       // vtable 0x0129C95C (Ego_r: vptr + PData, 8 bytes)
public:
    CBaseIntelligentPointer();                           // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                  // 0x00A01C10
    CBaseObject* GetPItem() const;                       // 0x00A01B50
    void SetPItem(CBaseObject* item);                    // 0x00A01B90
private:
    CBaseObjectPointer* PData;                           // +0x04
};

template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {   // vtable 0x01238C6C
public:

public:
    T* GetPItem() const { return reinterpret_cast<T*>(CBaseIntelligentPointer::GetPItem()); }
public:
    void SetPItem(T* item) { CBaseIntelligentPointer::SetPItem(reinterpret_cast<CBaseObject*>(item)); }
};

class CCreatureActionBase { public:
    virtual void v00();
    unsigned char _body[0xa4];
    virtual ~CCreatureActionBase();                    // 0x00693EF0
};

class CCreatureAction_AddRealObjectToInventory : public CCreatureActionBase { public:
    CIntelligentPointer<CThing> PObject;               // +0xa8
    CCreatureAction_AddRealObjectToInventory(class CThingCreatureBase* creature, CThingObject* object);   // 0x007EB2D0
};

class CThingCreatureBase : public CThing { public:
    bool SetCurrentAction(CCreatureActionBase* action);   // 0x006644F0
};

class CTCBase { public:                               // engine/CTCBase.h layout, modelled polymorphic
    virtual void v00();
    CThing* ParentThing;                       // +0x04
    bool    ToBeRemoved;                       // +0x08
    bool    IsInGlobalUpdateTCs;               // +0x09
};

class CTCActionUseBase : public CTCBase { public:
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