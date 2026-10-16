// CTCCarriedActionUseDrop::vfunc_28(CThingAICreature* user) -- run the base use hook; if the dropped thing is an alarm item (interface 0x48) and the user is flagged to react, make the user alert the player, else take the thing out of the user's carrying component
//
// Retail 0x007ED0A0, 151 bytes (`ret 4`). Vtable slot 28 of CTCCarriedActionUseDrop. Callees: the
// base hook @ 0x007EAB90 (empty `ret 4` in another TU), CVectorMap<ETCInterfaceType, CTCBase*>::
// LowerBound @ 0x0040F020 (this module's copy), CCreatureAction_AlertToPlayer::
// CCreatureAction_AlertToPlayer(CThingAICreature*) @ 0x007ECFD0, CThingCreatureBase::SetCurrentAction
// @ 0x006644F0, CCreatureActionBase::~CCreatureActionBase @ 0x00693EF0, CTCCarrying::RemoveThing
// (CThing*) @ 0x006A19B0.
// Shape: the user's CTCCarrying (interface 0x46: `test byte [edi+28h],40h`) comes from the inline
// out-parameter GetInterface whose out pointer is NOT initialised (a clear bit reads the stale frame
// word `mov eax,[esp+8]`); the parent thing's interface-0x48 test reads the whole bitset word
// (`mov ecx,[edx+28h] ; test ch,1`) and the parent stays in edx for the RemoveThing push. The
// 0xa8-byte alert action is a temporary passed straight from its constructor.
struct CTCBase;
struct CCreatureActionBase;

struct CInterfaceMap {                                      // CVectorMap<ETCInterfaceType, CTCBase*>
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);                   // 0x0040F020
};

struct CThing {
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];                         // +0x20 interface-present bitset
    CInterfaceMap m_interfaces;                             // +0x44
    unsigned char _pad4c[0x6c - 0x4c];
    unsigned char m_flags6c;                                // +0x6c
    unsigned long HasInterface(long type) const { return m_presentBits[type >> 5] & (1UL << (type & 31)); }
    template<class T> bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            if (it == map->last || interfaceType < it->type)
                it = map->last;
            *ppOut = (T*)it->component;
            return true;
        }
        return false;
    }
};

struct CThingAICreature : CThing {
    bool SetCurrentAction(CCreatureActionBase* action);     // 0x006644F0 (CThingCreatureBase)
};

struct CTCCarrying {
    void RemoveThing(CThing* thing);                        // 0x006A19B0
};

struct CCreatureActionBase {                                // retail sizeof 0xa8
    virtual void v00();
    unsigned char _pad04[0xa8 - 0x04];
    ~CCreatureActionBase();                                 // 0x00693EF0
};
struct CCreatureAction_AlertToPlayer : CCreatureActionBase {
    CCreatureAction_AlertToPlayer(CThingAICreature* creature);   // 0x007ECFD0
};

struct CTCCarriedActionUseBase {
    virtual void v00();
    CThing* ParentThing;                                    // +0x04 (CTCBase)
    void OnUseBase(CThingAICreature* user);                 // 0x007EAB90 (empty base implementation)
};

struct CTCCarriedActionUseDrop : CTCCarriedActionUseBase {
    void vfunc_28(CThingAICreature* user);
};

void CTCCarriedActionUseDrop::vfunc_28(CThingAICreature* user)
{
    CTCCarriedActionUseBase::OnUseBase(user);
    CTCCarrying* carrying;
    user->GetInterface(&carrying, 0x46);                    // TC_CARRYING
    if (ParentThing->HasInterface(0x48) && (user->m_flags6c & 8))
        user->SetCurrentAction(&CCreatureAction_AlertToPlayer(user));
    else
        carrying->RemoveThing(ParentThing);
}
