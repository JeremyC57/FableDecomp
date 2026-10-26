// CTCCarriedActionUseFish::vfunc_32(CThingCreatureBase* user) -- using a carried fish: when it targets a wall mount, a live user declares the holder a criminal (wall-mount theft); otherwise the holder sheathes the fish back into the inventory (kind 100)
//
// Retail 0x007EDCD0, 139 bytes (`ret 4`). Vtable 0x0123FD5C (CTCCarriedActionUseFish) slot 32.
// Callees: CTCCarriedActionUseFish::IsWallMountTargeted(CThing*) @ 0x007EDB80,
// CCreatureAction_DeclarePlayerACriminal::CCreatureAction_DeclarePlayerACriminal(CThingCreatureBase*,
// CThingPlayerCreature*) @ 0x00843610, CThingCreatureBase::SetCurrentAction @ 0x006644F0,
// ~CBaseIntelligentPointer @ 0x00A01C10 (the criminal pointer at action +0xa8),
// ~CCreatureActionBase @ 0x00693EF0, CCreatureAction_SheatheItemToInventory::
// CCreatureAction_SheatheItemToInventory(CThingCreatureBase*, CCreatureActionBase*, long) @ 0x0069FFD0
// and its destructor @ 0x0048C950 (an out-of-line COMDAT copy; manifest label "_Dest_val<..>" is
// propagated across the identical body).
// Shape: two 0xb0-byte action temporaries passed straight from their constructors; the live test
// is the inline IsThingAlive on the raw pointer (null, then dead bit +0x91 & 1); the criminal is
// the parent thing (this +4) and the sheathe action is given to the parent thing too; the parent is a
// named local in each arm (edi, overwriting the dead `this`).
class CCreatureActionBase;
class CThingPlayerCreature;

class CThingCreatureBase { public:
    unsigned char _pad00[0x91];
    unsigned char m_stateFlags;                     // +0x91 (bit 0 = dead)
    bool SetCurrentAction(CCreatureActionBase* action);   // 0x006644F0
};

inline bool IsThingAlive(const CThingCreatureBase* thing)
{
    return thing && !(thing->m_stateFlags & 1);
}

struct CBaseIntelligentPointer {
    virtual void v00();
    void* m_pItem;                                  // +0x04
    ~CBaseIntelligentPointer();                     // 0x00A01C10
};
template <class T>
class CIntelligentPointer : public CBaseIntelligentPointer { public: };

class CCreatureActionBase { public:                        // retail sizeof 0xa8
    virtual void v00();
    unsigned char _pad04[0xa8 - 0x04];
    ~CCreatureActionBase();                         // 0x00693EF0
};
class CCreatureAction_DeclarePlayerACriminal : public CCreatureActionBase { public:
    CIntelligentPointer<CThingPlayerCreature> PCriminal;   // +0xa8
    CCreatureAction_DeclarePlayerACriminal(CThingCreatureBase* owner, CThingPlayerCreature* criminal);   // 0x00843610
};
class CCreatureAction_SheatheItemToInventory : public CCreatureActionBase { public:   // retail sizeof 0xb0
    unsigned char PNextAction[8];                   // +0xa8 CCountedPointer<CCreatureActionBase>
    CCreatureAction_SheatheItemToInventory(CThingCreatureBase* owner, CCreatureActionBase* next, long kind);   // 0x0069FFD0
    ~CCreatureAction_SheatheItemToInventory();      // 0x0048C950 (out-of-line copy)
};

struct CTCCarriedActionUseFish {
    virtual void v00();
    CThingCreatureBase* ParentThing;                // +0x04 (CTCBase)
    bool IsWallMountTargeted(CThingCreatureBase* user);   // 0x007EDB80
    void vfunc_32(CThingCreatureBase* user);
};

void CTCCarriedActionUseFish::vfunc_32(CThingCreatureBase* user)
{
    if (IsWallMountTargeted(user))
    {
        if (IsThingAlive(user))
        {
            CThingCreatureBase* holder = ParentThing;
            user->SetCurrentAction(&CCreatureAction_DeclarePlayerACriminal(user, (CThingPlayerCreature*)holder));
        }
    }
    else
    {
        CThingCreatureBase* holder = ParentThing;
        holder->SetCurrentAction(&CCreatureAction_SheatheItemToInventory(holder, 0, 100));
    }
}