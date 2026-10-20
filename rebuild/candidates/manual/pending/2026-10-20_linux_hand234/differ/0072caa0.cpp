// CTCSpecialAbilities::SetupAssassinRush() -- start the assassin-rush cast action on the hero (not while interfaces 0xA8/0xA9 are present): aimed at a live creature target when the targeting component has one (with the player's flag at +0x20A for a player-controlled hero), otherwise untargeted; returns whether the action was set
// Grade: DIFFER(291v252) -- VC7.1 merges the 0xA9/0xA8 presence tests into one test ch,3 and keeps 8 in ebx where retail re-reads the word, shares a 0x100 mask register with the interface-8 test and zeroes ebx; the two action blocks' set+destructor tails do not cross-jump in the nested form (separate early returns do share the frame)
// Leaf: SetupAssassinRush
// Retail: Fable.exe (TLC, VC7.1) @ 0x0072CAA0, 252 bytes (plain `ret`).
// Callees: the const interface LowerBound @ 0x004365B0 (interface 8), the targeting component's slot 33 (+0x84, target thing),
//   CThing::GetPlayer() @ 0x004C7A10, CCreatureAction_CastAssassinRush(CThingCreatureBase*, CThing*, bool, long,
//   CCountedPointer<CCreatureActionBase>) @ 0x008CE040 and (CThingCreatureBase*, long, CCountedPointer<CCreatureActionBase>)
//   @ 0x008CE100, CThingCreatureBase::SetCurrentAction(CCreatureActionBase*) @ 0x006644F0, ~CCreatureAction_CastAssassinRush
//   @ 0x0072D990 (manifest "_Dest_val<...>" is propagated).
// Layout: CTCSpecialAbilities Creature +0x18 (retail 4-byte list); retail CThing interface bitset +0x20 (8 = word +0x20 bit 8,
//   0xA8/0xA9 = word +0x34 bits 8/9), map +0x44, ThingTypeFlags +0x6C (8 creature, 0x20 player-controlled), dead bit +0x91;
//   CPlayer flag +0x20A (named descriptively).
// Shape: the targeted and untargeted actions are locals of sibling blocks at one frame address, each followed by `return
//   Creature->SetCurrentAction(&action);` (VC tail-merges the set + destructor); the empty counted-pointer argument is built
//   in its argument slot (user copy constructor); the flag shares the lookup key's frame word.
class CTCBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    const SEntry* LowerBound(const long* type) const;   // 0x004365B0
};
struct CPlayer {
    unsigned char _pad00[0x20a];
    bool          RushFlag;                             // +0x20a (named descriptively)
};
class CThing;
struct CTCTargeter {                                    // interface 8 (named descriptively)
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17();
    virtual void v18(); virtual void v19(); virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23();
    virtual void v24(); virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28(); virtual void v29();
    virtual void v30(); virtual void v31(); virtual void v32();
    virtual CThing* GetPTarget();                       // slot 33 (+0x84)
};
class CThing {
public:
    unsigned char _pad00[0x20];
    unsigned long m_presentBits[9];                     // +0x20
    CInterfaceMap m_interfaces;                         // +0x44
    unsigned char _pad4c[0x6c - 0x4c];
    unsigned long ThingTypeFlags;                       // +0x6c
    unsigned char _pad70[0x91 - 0x70];
    unsigned char m_flags91;                            // +0x91 bit 0: dead
    CPlayer* GetPlayer();                               // 0x004C7A10
    __forceinline unsigned long HasTC(long type) const { return m_presentBits[type >> 5] & (1UL << (type & 31)); }
    template <class T> __forceinline T* GetTC(long type) const
    {
        long key = type;
        const CInterfaceMap* map = &m_interfaces;
        const CInterfaceMap::SEntry* it = map->LowerBound(&key);
        return (T*)((it == map->last || type < it->type) ? map->last : it)->component;
    }
};
struct CCreatureActionBase;
struct SCountBlock;
struct CCountedActionPointer {                          // CCountedPointer<CCreatureActionBase>
    CCreatureActionBase* m_pObject;
    SCountBlock*         m_pCount;
    CCountedActionPointer() : m_pObject(0), m_pCount(0) {}
    CCountedActionPointer(const CCountedActionPointer& other);
};
class CThingCreatureBase : public CThing {
public:
    bool SetCurrentAction(CCreatureActionBase* action); // 0x006644F0
};
struct CCreatureAction_CastAssassinRush {               // vtable 0x01284254
    unsigned char _body[0x138];
    CCreatureAction_CastAssassinRush(CThingCreatureBase* creature, CThing* target, bool flag, long priority,
                                     CCountedActionPointer next);   // 0x008CE040
    CCreatureAction_CastAssassinRush(CThingCreatureBase* creature, long priority, CCountedActionPointer next);   // 0x008CE100
    ~CCreatureAction_CastAssassinRush();                // 0x0072D990
    operator CCreatureActionBase*() { return (CCreatureActionBase*)this; }
};
struct CTCSpecialAbilities {
    virtual void v00();
    CThing* ParentThing;                                // +0x04 (CTCBase)
    unsigned char _base08[4];
    bool AbilityMutexLock;                              // +0x0c
    bool UsingBed;                                      // +0x0d
    long CurrentAbility;                                // +0x10
    void* UndeadFollowers;                              // +0x14 list (retail 4 bytes)
    CThingCreatureBase* Creature;                       // +0x18
    bool SetupAssassinRush();
};

bool CTCSpecialAbilities::SetupAssassinRush()
{
    if (!Creature->HasTC(0xa9) && !Creature->HasTC(0xa8)) {
        if (Creature->HasTC(8)) {
            CThing* target = Creature->GetTC<CTCTargeter>(8)->GetPTarget();
            if (target && !(target->m_flags91 & 1) && (target->ThingTypeFlags & 8)) {
                bool flag = false;
                if (Creature->ThingTypeFlags & 0x20)
                    flag = Creature->GetPlayer()->RushFlag;
                CCreatureAction_CastAssassinRush action(Creature, target, flag, 0, CCountedActionPointer());
                return Creature->SetCurrentAction(action);
            }
        }
        {
            CCreatureAction_CastAssassinRush action(Creature, 0, CCountedActionPointer());
            return Creature->SetCurrentAction(action);
        }
    }
    return false;
}
