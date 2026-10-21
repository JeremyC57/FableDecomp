// Grade: DIFFER(346v346) -- register allocation only: retail loads ParentThing (PostDeed target) into eax after the deed-log pointer, pushes the second key address through ecx and computes the morality sum in edx/eax; ours uses ecx/edx/eax+ecx
// CTCActionUseChest::OnActionUse(CThingCreatureBase* user) -- a creature uses a chest: start the open-chest action when the chest is shut and not busy, and when it is theft post a theft deed and apply the theft morality to the user
// Leaf: OnActionUse
// Retail 0x007EE8F0, 352 bytes, `ret 4` (vtable slot 10, +0x28; catalogued as vfunc_28). The landed callee
// CTCChest::GetOpenAnimationForCreature 0x00750240 names this caller as CTCActionUseChest::OnActionUse.
// Callees: the base use hook 0x007EAB90, CVectorMap<ETCInterfaceType,CTCBase*>::LowerBound 0x004365B0 (const, the chest
// lookup) and 0x0040F020, CTCChest::GetOpenAnimationForCreature 0x00750240 (CCharString result),
// CCreatureAction_OpenChest ctor 0x007EE7E0, CThingCreatureBase::SetCurrentAction 0x006644F0, ~CBaseIntelligentPointer
// 0x00A01C10, ~CCreatureActionBase 0x00693EF0, ~CCharString 0x0099EAE0, CTCActionUseBase::IsThisTheft 0x007EACB0,
// CThing::GetPosition 0x004C73D0, CTCHeroOpinionDeedLog::PostDeed 0x006C6990, CTCBase::GetDefinitionManager 0x006869D0,
// CTCHeroStats::SetMorality 0x0057C2E6.
// Layout: CTCBase ParentThing +4; CThing interface bitset +0x20, map +0x44; CTCChest Opening +0x1C / Closing +0x1E
// (descriptive); retail CCreatureAction_OpenChest 0xB0 bytes (base 0xA8 + PChest); the definition manager's hero
// def +0xDC with the theft morality delta +0xE0; CTCHeroStats morality +0x28.
// Shape: the chest comes from the out-parameter GetInterface (left unset when absent: retail then reads the shared key
// slot); the action is a temporary inside the SetCurrentAction call; the deed log (0x51) and hero stats (4) are HasTC +
// GetTC lookups sharing the map pointer and the key slot.
struct CTCBase;
struct CFaction;
struct CThingAICreature;
struct CThingCreatureBase;
struct C3DVector { float X, Y, Z; };
struct CCharString {
    void* PStringData;
    ~CCharString();                                        // 0x0099EAE0
};
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);                  // 0x0040F020
    SEntry* LowerBound(const long* type) const;            // 0x004365B0
};
struct CThing {
    unsigned char _pad00[0x20];
    unsigned long m_presentBits[9];                        // +0x20
    CInterfaceMap m_interfaces;                            // +0x44
    const C3DVector& GetPosition() const;                  // 0x004C73D0
    __forceinline unsigned long HasTC(long type) const { return m_presentBits[type >> 5] & (1UL << (type & 31)); }
    template <class T> __forceinline T* GetTC(long type)
    {
        long key = type;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&key);
        return (T*)((it == map->last || type < it->type) ? map->last : it)->component;
    }
    template <class T> __forceinline bool GetInterface(T** ppOut, long interfaceType) const
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            const CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            *ppOut = (T*)((it == map->last || interfaceType < it->type) ? map->last : it)->component;
            return true;
        }
        return false;
    }
};
struct CCreatureActionBase {
    virtual void v00();
    unsigned char _body[0xa8 - 4];
    ~CCreatureActionBase();                                // 0x00693EF0
};
struct CBaseIntelligentPointer {
    void*   __vftable;
    void*   PData;
    ~CBaseIntelligentPointer();                            // 0x00A01C10
};
struct CCreatureAction_OpenChest : CCreatureActionBase {
    CBaseIntelligentPointer PChest;                        // +0xa8
    CCreatureAction_OpenChest(CThingCreatureBase* owner, CThing* chest, const CCharString& animation);   // 0x007EE7E0
};
struct CThingCreatureBase : CThing {
    bool SetCurrentAction(const CCreatureActionBase& action);   // 0x006644F0
};
struct CTCChest {
    unsigned char _pad00[0x1c];
    bool          Opening;                                 // +0x1c
    bool          Open;                                    // +0x1d
    bool          Closing;                                 // +0x1e
    CCharString GetOpenAnimationForCreature() const;       // 0x00750240
};
struct CTCHeroOpinionDeedLog {
    long PostDeed(long deedType, const C3DVector& position, const CThing* target, CThingAICreature* victim,
                  long postType, bool b1, bool b2, const CFaction* faction, bool b3, long extra);   // 0x006C6990
};
struct CTCHeroStats {
    unsigned char _pad00[0x28];
    long          Morality;                                // +0x28
    void SetMorality(long morality);                       // 0x0057C2E6
};
struct CHeroDef {
    unsigned char _pad00[0xe0];
    long          TheftMoralityChange;                     // +0xe0
};
struct CDefinitionManager {
    unsigned char _pad00[0xdc];
    CHeroDef*     HeroDef;                                 // +0xdc
};
struct CTCActionUseBase {
    virtual void v00();
    CThing* ParentThing;                                   // +0x04
    void OnUseBase(CThingCreatureBase* user);              // 0x007EAB90
    bool IsThisTheft();                                    // 0x007EACB0
    CDefinitionManager* GetDefinitionManager();            // 0x006869D0
};
struct CTCActionUseChest : CTCActionUseBase {
    void OnActionUse(CThingCreatureBase* user);
};

void CTCActionUseChest::OnActionUse(CThingCreatureBase* user)
{
    OnUseBase(user);
    CThing* thing = ParentThing;
    CTCChest* chest;
    thing->GetInterface(&chest, 0x3c);
    if (!chest->Opening && !chest->Closing)
        user->SetCurrentAction(CCreatureAction_OpenChest(user, thing, chest->GetOpenAnimationForCreature()));
    if (IsThisTheft() && user->HasTC(0x51)) {
        CTCHeroOpinionDeedLog* log = user->GetTC<CTCHeroOpinionDeedLog>(0x51);
        log->PostDeed(6, user->GetPosition(), ParentThing, 0, 2, true, false, 0, false, 0);
        if (user->HasTC(4)) {
            CTCHeroStats* stats = user->GetTC<CTCHeroStats>(4);
            stats->SetMorality(GetDefinitionManager()->HeroDef->TheftMoralityChange + stats->Morality);
        }
    }
}
