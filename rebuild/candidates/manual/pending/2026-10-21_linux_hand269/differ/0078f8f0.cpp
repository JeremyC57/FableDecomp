// Grade: DIFFER(497v505) -- ebx use-count tie only: retail gives ebx to the constant 1 (CutSceneCancel store, three dead-bit tests, Kill push) and homes caster in the creature's frame word; ours keeps caster in ebx and uses immediates. Frame fixed by the advisor (one function-scope key for both lookups).
// CTCDivineWrath::CancelForCutscene() -- a cutscene cancels the spell: reset every creature it hit, mark the cutscene cancel, fade the environment back at once, finish the caster's current action, reset its interface-0xA7 component, kill the build-up effect, fade the hand effects, stop the looping sound and remove the component
// Retail 0x0078F8F0, 505 bytes (plain `ret`; `and esp,-8` frame for the loop's 8-byte intelligent-pointer copy).
// Callees: CBaseIntelligentPointer ctor 0x00A01B10 / GetPItem 0x00A01B50 / SetPItem 0x00A01B90 / dtor 0x00A01C10
// (CIntelligentPointer<CThingCreatureBase> vtable 0x0123E7D8), CTCDivineWrath::ResetCreatureHitState(CIntelligentPointer
// <CThingCreatureBase>) @ 0x0078E900 (by value), CTCBase::GetWorld @ 0x00686A70, CEnvironment::
// TransitionToDefaultThemeAllChannels(long, float) @ 0x006B3950 (0.001f), CThingCreatureBase::FinishCurrentAction
// @ 0x00662650, LowerBound @ 0x0040F020 (interface 0xA7 and the sound emitter 0x10), CThing::Kill(bool) @ 0x004C9B80,
// FadeOrKillEffect @ 0x0072ABD0 (__fastcall), StopSample @ 0x0066ADD0, CThing::RemoveTC(long) @ 0x004C9840 (0x85).
// Layout: as the landed CTCDivineWrath::FinishMove 0x0078F6E0 (Ego_r.pdb, fields after Timer 4 lower): CutSceneCancel +0x6c,
// LastFrameCreaturesHit +0x78 (STLport map header pointer; node value: key +0x10, PerCreatureInfo +0x14 whose first member is
// the creature pointer), BuildUpEffect +0x9c, HandEffect1 +0xac, HandEffect2 +0xb4, LoopingSound +0xbc.
struct CTCBase;
struct CThingCreatureBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);       // 0x0040F020
};
struct CThing {
    unsigned char _pad00[0x20];
    unsigned long m_presentBits[9];             // +0x20
    CInterfaceMap m_interfaces;                 // +0x44
    unsigned char _pad4c[0x91 - 0x4c];
    unsigned char m_flags91;                    // +0x91 bit 0: dead
    unsigned long HasTC(long interfaceType) const { return m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31)); }
    void FinishCurrentAction();                 // 0x00662650 (CThingCreatureBase)
    void Kill(bool b);                          // 0x004C9B80
    void RemoveTC(long interfaceType);          // 0x004C9840
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
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
    template<class T> __forceinline T* GetTC(long wanted)
    {
        long type = wanted;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&type);
        return (T*)((it == map->last || wanted < it->type) ? map->last : it)->component;
    }
};
struct CBaseIntelligentPointer {
    virtual void v00();                         // +0x00 vptr
    CThing* m_pItem;                            // +0x04
    CBaseIntelligentPointer();                  // 0x00A01B10
    ~CBaseIntelligentPointer();                 // 0x00A01C10
    CThing* GetPItem() const;                   // 0x00A01B50
    void SetPItem(CThing* item);                // 0x00A01B90
};
struct CIntelligentPointerCreature : CBaseIntelligentPointer {   // CIntelligentPointer<CThingCreatureBase>, vtable 0x0123E7D8
    virtual void v00();
    CIntelligentPointerCreature(const CIntelligentPointerCreature& other) : CBaseIntelligentPointer() { SetPItem(other.GetPItem()); }
};
inline bool IS_THING_ALIVE(const CBaseIntelligentPointer& p)
{
    if (p.GetPItem() != 0) {
        if (!(p.GetPItem()->m_flags91 & 1))
            return true;
    }
    return false;
}
void __fastcall FadeOrKillEffect(CBaseIntelligentPointer& effect);   // 0x0072ABD0
struct CEnvironment {
    void TransitionToDefaultThemeAllChannels(long channels, float time);   // 0x006B3950
};
struct CWorld {
    unsigned char _pad00[0x18];
    CEnvironment* PEnvironment;                 // +0x18
};
struct CTCSoundEmitter {
    void StopSample(unsigned long sampleID);    // 0x0066ADD0
};
struct CTCInterfaceA7 {
    unsigned char _pad00[0x0c];
    bool m_active;                              // +0x0c
    long m_count;                               // +0x10
};
struct SPerCreatureInfo {
    CIntelligentPointerCreature Creature;       // +0x00
};
struct SHitNode {                               // STLport _Rb_tree_node<pair<CThingCreatureBase* const, PerCreatureInfo> >
    unsigned char m_color;                      // +0x00
    SHitNode* m_pParent;                        // +0x04
    SHitNode* m_pLeft;                          // +0x08
    SHitNode* m_pRight;                         // +0x0c
    CThingCreatureBase* m_key;                  // +0x10
    SPerCreatureInfo m_info;                    // +0x14
};
struct CHitIterator {
    SHitNode* m_pNode;
    CHitIterator(SHitNode* node) : m_pNode(node) {}
    SPerCreatureInfo& Info() const { return m_pNode->m_info; }
    bool operator!=(const CHitIterator& other) const { return m_pNode != other.m_pNode; }
    static SHitNode* Increment(SHitNode* node)
    {
        if (node->m_pRight != 0) {
            node = node->m_pRight;
            while (node->m_pLeft != 0)
                node = node->m_pLeft;
        } else {
            SHitNode* y = node->m_pParent;
            while (node == y->m_pRight) {
                node = y;
                y = y->m_pParent;
            }
            if (node->m_pRight != y)
                node = y;
        }
        return node;
    }
    void operator++() { m_pNode = Increment(m_pNode); }
};
struct CHitMap {                                // STLport map, 12 bytes
    SHitNode* m_pHeader;
    unsigned long m_nodeCount;
    unsigned char m_compare;
    CHitIterator begin() { return CHitIterator(m_pHeader->m_pLeft); }
    CHitIterator end() { return CHitIterator(m_pHeader); }
};
struct CTCDivineWrath {
    virtual void v00();
    CThing* ParentThing;                        // +0x04 (CTCBase)
    unsigned char _pad08[0x6c - 0x08];
    bool  CutSceneCancel;                       // +0x6c
    unsigned char _pad6d[0x78 - 0x6d];
    CHitMap LastFrameCreaturesHit;              // +0x78
    unsigned char _pad84[0x9c - 0x84];
    CBaseIntelligentPointer BuildUpEffect;      // +0x9c
    unsigned long HeroDefaultChannel;           // +0xa4
    unsigned long SpellExclusionChannel;        // +0xa8
    CBaseIntelligentPointer HandEffect1;        // +0xac
    CBaseIntelligentPointer HandEffect2;        // +0xb4
    unsigned long LoopingSound;                 // +0xbc
    CWorld* GetWorld();                         // 0x00686A70 (CTCBase)
    void ResetCreatureHitState(CIntelligentPointerCreature creature);   // 0x0078E900
    __forceinline void StopSound(unsigned long sound)
    {
        CTCSoundEmitter* emitter;
        if (ParentThing->GetInterface(&emitter, 0x10))
            emitter->StopSample(sound);
    }
    void CancelForCutscene();
};

void CTCDivineWrath::CancelForCutscene()
{
    long key;
    for (CHitIterator it = LastFrameCreaturesHit.begin(), end = LastFrameCreaturesHit.end(); it != end; ++it) {
        CIntelligentPointerCreature creature = it.Info().Creature;
        ResetCreatureHitState(creature);
    }
    CutSceneCancel = true;
    CWorld* world = GetWorld();
    world->PEnvironment->TransitionToDefaultThemeAllChannels(2, 0.001f);
    CThing* caster = ParentThing;
    caster->FinishCurrentAction();
    if (caster->m_presentBits[0xa7 >> 5] & (1UL << (0xa7 & 31))) {
        key = 0xa7;
        CInterfaceMap* map = &caster->m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&key);
        CTCInterfaceA7* tc = (CTCInterfaceA7*)((it == map->last || 0xa7 < it->type) ? map->last : it)->component;
        tc->m_active = false;
        tc->m_count = 0;
    }
    if (IS_THING_ALIVE(BuildUpEffect))
        BuildUpEffect.GetPItem()->Kill(true);
    if (IS_THING_ALIVE(HandEffect1))
        FadeOrKillEffect(HandEffect1);
    if (IS_THING_ALIVE(HandEffect2))
        FadeOrKillEffect(HandEffect2);
    unsigned long sound = LoopingSound;
    CThing* owner = ParentThing;
    if (owner->m_presentBits[0x10 >> 5] & (1UL << (0x10 & 31))) {
        key = 0x10;
        CInterfaceMap* map = &owner->m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&key);
        CTCSoundEmitter* emitter = (CTCSoundEmitter*)((it == map->last || 0x10 < it->type) ? map->last : it)->component;
        emitter->StopSample(sound);
    }
    caster->RemoveTC(0x85);
}
