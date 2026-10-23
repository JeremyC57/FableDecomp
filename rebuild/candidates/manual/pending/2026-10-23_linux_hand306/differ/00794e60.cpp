// Grade: DIFFER(552v552) -- env Transition args pushed before GetWorld call (retail after); StopSound thing in eax vs retail ebp reload; any env/world local flips ebx=1 to ebx=parent and shrinks frame
// CTCUnholyPower::CancelForCutscene() -- a cutscene interrupts the spell: reset every hit creature's hit state, flag the cancel, fade the environment back at once, stop the caster's action and its interface-0xA7 state, kill or fade the effects, stop the looping sound and drop the caster's interface 0x86
// Leaf: ?CancelForCutscene@CTCUnholyPower
// Retail: Fable.exe (TLC, VC7.1) @ 0x00794E60, 552 bytes (+ int3 padding), plain `ret`, 8-aligned frame.
// Callees: as the landed sibling FinishMove 0x00794C10, plus CTCUnholyPower::ResetCreatureHitState 0x00793E00 (by-value
//   CIntelligentPointer<CThingCreatureBase>, vtable 0x0123E7D8, CBaseIntelligentPointer() 0x00A01B10 / SetPItem 0x00A01B90 /
//   ~ 0x00A01C10), CThingCreatureBase::FinishCurrentAction 0x00662650 and the thing's by-type interface call 0x004C9840.
// Layout: retail CTCUnholyPower (Ego_r.pdb less 4 after Timer): CutSceneCancel +0x6c, LastFrameCreaturesHit +0x78
//   (STLport map header pointer; PerCreatureInfo's creature pointer at node +0x14), effects as FinishMove.
extern long g_framesPerSecond;                  // 0x01375550
struct CTCBase;
struct CInterfaceMap {                          // CVectorMap<ETCInterfaceType, CTCBase*> at CThing+0x44: {first, last}
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);       // 0x0040F020
};
struct CThing {
    unsigned char _pad00[0x20];
    unsigned long m_presentBits[9];             // +0x20 std::bitset of interface types
    CInterfaceMap m_interfaces;                 // +0x44
    unsigned char _pad4c[0x91 - 0x4c];
    unsigned char m_flags91;                    // +0x91 bit 0: dead
    void Kill(bool immediately);                // 0x004C9B80
    void FinishCurrentAction();                 // 0x00662650 (CThingCreatureBase)
    void RemoveTC(long interfaceType);          // 0x004C9840 (descriptive: looks the interface up by type)
    unsigned long HasTC(long interfaceType) const { return m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31)); }
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
struct CBaseObject;
struct CBaseIntelligentPointer {                // CIntelligentPointer<T>: vptr + item
    virtual void v00();
    CThing* m_pItem;                            // +0x04
    CBaseIntelligentPointer();                  // 0x00A01B10
    ~CBaseIntelligentPointer();                 // 0x00A01C10
    CThing* GetPItem() const;                   // 0x00A01B50
    void SetPItem(CBaseObject* item);           // 0x00A01B90
};
template<class T> struct CIntelligentPointer : CBaseIntelligentPointer {   // CIntelligentPointer<CThingCreatureBase>, vtable 0x0123E7D8
    virtual void v00();
    CIntelligentPointer(const CIntelligentPointer& other) : CBaseIntelligentPointer() { SetPItem((CBaseObject*)other.GetPItem()); }
};
struct CThingCreatureBase;
struct SHitNode {                               // STLport _Rb_tree_node<pair<CThingCreatureBase* const, PerCreatureInfo> >
    long Color;                                 // +0x00
    SHitNode* Parent;                           // +0x04
    SHitNode* Left;                             // +0x08
    SHitNode* Right;                            // +0x0c
    CThingCreatureBase* Creature;               // +0x10 key
    CIntelligentPointer<CThingCreatureBase> PCreature;   // +0x14 PerCreatureInfo
};
struct CHitIterator {
    SHitNode* m_pNode;
    CHitIterator(SHitNode* node) : m_pNode(node) { }
    SHitNode* operator->() const { return m_pNode; }
    bool operator!=(const CHitIterator& other) const { return m_pNode != other.m_pNode; }
    static SHitNode* Increment(SHitNode* node)  // STLport _Rb_global::_M_increment
    {
        if (node->Right != 0) {
            node = node->Right;
            while (node->Left != 0)
                node = node->Left;
        } else {
            SHitNode* y = node->Parent;
            while (node == y->Right) {
                node = y;
                y = y->Parent;
            }
            if (node->Right != y)
                node = y;
        }
        return node;
    }
    void operator++() { m_pNode = Increment(m_pNode); }
};
struct CHitMap {                                // map<CThingCreatureBase*, PerCreatureInfo> (STLport, 12 bytes)
    SHitNode* m_pHeader;
    unsigned long m_nodeCount;
    unsigned char m_compare;
    CHitIterator begin() { return CHitIterator(m_pHeader->Left); }
    CHitIterator end() { return CHitIterator(m_pHeader); }
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
struct CTCSoundEmitter {                        // interface 0x10
    void StopSample(unsigned long sampleID);    // 0x0066ADD0
};
struct CTCInterfaceA7 {                         // interface 0xA7 component
    unsigned char _pad00[0x0c];
    bool m_active;                              // +0x0c
    long m_count;                               // +0x10
};
struct CSpecialAbilitiesUnholyPowerDef {
    unsigned char _pad00[0xc4];
    float* FadeOutTimes;                        // +0xc4 vector<float> first pointer (per spell level; descriptive)
};
struct CTCUnholyPower {
    virtual void v00();
    CThing* ParentThing;                        // +0x04 (CTCBase)
    unsigned char _pad08[0x6c - 0x08];
    bool  CutSceneCancel;                       // +0x6c
    unsigned char _pad6d[0x78 - 0x6d];          // PTarget .. PerformedReleaseAction
    CHitMap LastFrameCreaturesHit;              // +0x78
    unsigned char _pad84[0x9c - 0x84];
    CBaseIntelligentPointer BuildUpEffect;      // +0x9c
    CBaseIntelligentPointer DarkFacesEffect;    // +0xa4
    unsigned long HeroDefaultChannel;           // +0xac
    unsigned long SpellExclusionChannel;        // +0xb0
    CBaseIntelligentPointer HandEffect1;        // +0xb4
    CBaseIntelligentPointer HandEffect2;        // +0xbc
    unsigned long LoopingSound;                 // +0xc4
    CWorld* GetWorld();                         // 0x00686A70 (CTCBase)
    void StopSound(unsigned long sound)
    {
        CTCSoundEmitter* emitter;
        if (ParentThing->GetInterface(&emitter, 0x10))
            emitter->StopSample(sound);
    }
    void ResetCreatureHitState(CIntelligentPointer<CThingCreatureBase> creature);   // 0x00793E00
    void CancelForCutscene();
};

void CTCUnholyPower::CancelForCutscene()
{
    CHitIterator end = LastFrameCreaturesHit.end();
    for (CHitIterator it = LastFrameCreaturesHit.begin(); it != end; ++it) {
        CIntelligentPointer<CThingCreatureBase> creature = it->PCreature;
        ResetCreatureHitState(creature);
    }
    CutSceneCancel = true;
    GetWorld()->PEnvironment->TransitionToDefaultThemeAllChannels(2, 0.001f);
    CThing* parent = ParentThing;
    parent->FinishCurrentAction();
    if (parent->HasTC(0xa7)) {
        CTCInterfaceA7* tc = parent->GetTC<CTCInterfaceA7>(0xa7);
        tc->m_active = false;
        tc->m_count = 0;
    }
    if (IS_THING_ALIVE(BuildUpEffect))
        BuildUpEffect.GetPItem()->Kill(true);
    if (IS_THING_ALIVE(HandEffect1))
        FadeOrKillEffect(HandEffect1);
    if (IS_THING_ALIVE(HandEffect2))
        FadeOrKillEffect(HandEffect2);
    if (IS_THING_ALIVE(DarkFacesEffect))
        DarkFacesEffect.GetPItem()->Kill(true);
    StopSound(LoopingSound);
    parent->RemoveTC(0x86);
}
