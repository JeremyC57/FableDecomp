// CTCDivineWrath::FinalCleanup() -- end the spell's effects: kill (cut-scene cancel) or fade every hit effect and every per-creature effect, restore each live hit creature's lighting channel, re-arm its 0x75 component and clear its 0x15d flag, reset its hit state; then restore the hero's lighting channel and re-arm the hero's 0x75 component
// Grade: DIFFER(586v586) -- register swap only in the HitEffects loop: retail keeps the iterator in edi and the strength-reduced &it->Effect in esi, ours the reverse (push esi placement follows); everything else matches
// Leaf: FinalCleanup
//
// Retail 0x0078FAF0, 586 bytes, 8-aligned ebp frame, plain `ret`. CTCUnholyPower's copy at 0x00795090 differs only in
// the HeroDefaultChannel offset.
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50 / ctor @ 0x00A01B10 / SetPItem @ 0x00A01B90 / dtor @ 0x00A01C10
// (CIntelligentPointer<CThingCreatureBase> vtable 0x0123E7D8), CThing::Kill(bool) @ 0x004C9B80, FadeOrKillEffect
// (CIntelligentPointer<CThing>&) @ 0x0072ABD0 (__fastcall), CTCGraphicAppearance::SetLightingChannel @ 0x004BFB40,
// CVectorMap LowerBound @ 0x0040F020 (interface 0x75 on the creatures; inlined lower_bound on the hero),
// CTCDivineWrath::ResetCreatureHitState(CIntelligentPointer<CThingCreatureBase>) @ 0x0078E900.
// Layout: Ego_r.pdb CTCDivineWrath with the retail containers (Timer 4 lower, STLport map/vector 12 bytes, as the landed
// CTCUnholyPower::FinishMove 0x00794C10): CutSceneCancel +0x6c, LastFrameCreaturesHit +0x78, HitEffects +0x84,
// HeroDefaultChannel +0xa4; PerEffectInfo (0x14) Effect +0x0c; PerCreatureInfo ActualCreature +0, Effect +0x0c,
// DefaultChannel +0x14; CThing graphic +0x64, flags byte +0x15d (bit 6 cleared, descriptive).
struct CTCBase;
struct CBaseObject;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);                    // 0x0040F020
};
struct CEnginePrimitiveThemeChannelDesc { unsigned long Channel; };
struct CTCGraphicAppearance {
    void SetLightingChannel(CEnginePrimitiveThemeChannelDesc* channel, bool a, bool b);   // 0x004BFB40
};
struct CTC75 {                                               // interface 0x75 component
    unsigned char _pad00[0x20];
    bool m_flag;                                             // +0x20
};
struct CThing {
    unsigned char _pad00[0x20];
    unsigned long m_presentBits[9];                          // +0x20
    CInterfaceMap m_interfaces;                              // +0x44
    unsigned char _pad4c[0x64 - 0x4c];
    CTCGraphicAppearance* Graphic;                           // +0x64
    unsigned char _pad68[0x91 - 0x68];
    unsigned char m_flags91;                                 // +0x91 bit 0: dead
    unsigned char _pad92[0x15d - 0x92];
    unsigned char m_flags15d;                                // +0x15d
    void ClearFlag15dBit6() { m_flags15d &= ~0x40; }
    void Kill(bool immediately);                             // 0x004C9B80
    __forceinline unsigned long HasTC(long type) const { return m_presentBits[type >> 5] & (1UL << (type & 31)); }
    template <class T> __forceinline T* GetTC(long wanted)
    {
        long type = wanted;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&type);
        return (T*)((it == map->last || wanted < it->type) ? map->last : it)->component;
    }
    template <class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
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
    template <class T> __forceinline T* GetTCInline(long wanted) const
    {
        CInterfaceMap::SEntry* first = m_interfaces.first;
        CInterfaceMap::SEntry* last = m_interfaces.last;
        long len = last - first;
        while (len > 0) {
            long half = len / 2;
            CInterfaceMap::SEntry* middle = first + half;
            if (middle->type < wanted) {
                first = middle + 1;
                len = len - half - 1;
            } else
                len = half;
        }
        if (first == last || wanted < first->type)
            first = last;
        return (T*)first->component;
    }
    template <class T> __forceinline bool GetInterfaceInline(T** ppOut, long wanted) const
    {
        if (m_presentBits[wanted >> 5] & (1UL << (wanted & 31))) {
            CInterfaceMap::SEntry* first = m_interfaces.first;
            CInterfaceMap::SEntry* last = m_interfaces.last;
            long len = last - first;
            while (len > 0) {
                long half = len / 2;
                CInterfaceMap::SEntry* middle = first + half;
                if (middle->type < wanted) {
                    first = middle + 1;
                    len = len - half - 1;
                } else
                    len = half;
            }
            if (first == last || wanted < first->type)
                first = last;
            *ppOut = (T*)first->component;
            return true;
        }
        return false;
    }
};
struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;                                         // +0x04
    CBaseIntelligentPointer();                               // 0x00A01B10
    ~CBaseIntelligentPointer();                              // 0x00A01C10
    CThing* GetPItem() const;                                // 0x00A01B50
    void SetPItem(CBaseObject* item);                        // 0x00A01B90
};
template <class T> struct CIntelligentPointer : CBaseIntelligentPointer {   // vtable 0x0123E7D8 for CThingCreatureBase
    virtual void v00();
    CIntelligentPointer(const CIntelligentPointer& other) : CBaseIntelligentPointer() { SetPItem((CBaseObject*)other.GetPItem()); }
};
struct CThingCreatureBase;
inline bool IS_THING_ALIVE(const CBaseIntelligentPointer& p)
{
    if (p.GetPItem() != 0) {
        if (!(p.GetPItem()->m_flags91 & 1))
            return true;
    }
    return false;
}
void __fastcall FadeOrKillEffect(CBaseIntelligentPointer& effect);   // 0x0072ABD0
struct PerEffectInfo {
    float Position[3];                                       // +0x00 C3DVector
    CBaseIntelligentPointer Effect;                          // +0x0c
};
struct PerCreatureInfo {
    CIntelligentPointer<CThingCreatureBase> ActualCreature;  // +0x00
    long FramesToGetHit;                                     // +0x08
    CBaseIntelligentPointer Effect;                          // +0x0c
    CEnginePrimitiveThemeChannelDesc DefaultChannel;         // +0x14
};
struct SCreatureNode {                                       // STLport _Rb_tree_node<pair<CThingCreatureBase* const, PerCreatureInfo> >
    long m_color;
    SCreatureNode* m_pParent;                                // +0x04
    SCreatureNode* m_pLeft;                                  // +0x08
    SCreatureNode* m_pRight;                                 // +0x0c
    CThingCreatureBase* first;                               // +0x10
    PerCreatureInfo second;                                  // +0x14
    static SCreatureNode* Increment(SCreatureNode* node)     // STLport _Rb_global::_M_increment
    {
        if (node->m_pRight != 0) {
            node = node->m_pRight;
            while (node->m_pLeft != 0)
                node = node->m_pLeft;
        } else {
            SCreatureNode* y = node->m_pParent;
            while (node == y->m_pRight) {
                node = y;
                y = y->m_pParent;
            }
            if (node->m_pRight != y)
                node = y;
        }
        return node;
    }
};
struct CCreatureHitMap {                                     // STLport map (12 bytes)
    SCreatureNode* m_pHeader;
    unsigned long m_count;
    unsigned long m_compare;
};
struct CEffectVector {                                       // STLport vector (12 bytes)
    PerEffectInfo* _M_start;
    PerEffectInfo* _M_finish;
    PerEffectInfo* _M_end_of_storage;
};
struct CTCDivineWrath {
    virtual void v00();
    CThing* ParentThing;                                     // +0x04
    unsigned char _pad08[0x6c - 0x08];
    bool CutSceneCancel;                                     // +0x6c
    unsigned char _pad6d[0x78 - 0x6d];
    CCreatureHitMap LastFrameCreaturesHit;                   // +0x78
    CEffectVector HitEffects;                                // +0x84
    unsigned char _pad90[0xa4 - 0x90];
    CEnginePrimitiveThemeChannelDesc HeroDefaultChannel;     // +0xa4
    void ResetCreatureHitState(CIntelligentPointer<CThingCreatureBase> creature);   // 0x0078E900
    void FinalCleanup();
};

void CTCDivineWrath::FinalCleanup()
{
    PerEffectInfo* end = HitEffects._M_finish;
    for (PerEffectInfo* it = HitEffects._M_start; it != end; ++it) {
        if (CutSceneCancel) {
            if (IS_THING_ALIVE(it->Effect))
                it->Effect.GetPItem()->Kill(true);
        } else
            FadeOrKillEffect(it->Effect);
    }
    SCreatureNode* header = LastFrameCreaturesHit.m_pHeader;
    for (SCreatureNode* node = header->m_pLeft; node != header; node = SCreatureNode::Increment(node)) {
        if (CutSceneCancel) {
            if (IS_THING_ALIVE(node->second.Effect))
                node->second.Effect.GetPItem()->Kill(true);
        } else
            FadeOrKillEffect(node->second.Effect);
        CIntelligentPointer<CThingCreatureBase> creature = node->second.ActualCreature;
        if (IS_THING_ALIVE(creature)) {
            CThing* thing = creature.GetPItem();
            if (thing->Graphic)
                thing->Graphic->SetLightingChannel(&node->second.DefaultChannel, true, true);
            CTC75* tc;
            if (creature.GetPItem()->GetInterface(&tc, 0x75))
                tc->m_flag = true;
            creature.GetPItem()->ClearFlag15dBit6();
        }
        ResetCreatureHitState(creature);
    }
    if (ParentThing->Graphic)
        ParentThing->Graphic->SetLightingChannel(&HeroDefaultChannel, true, true);
    const CThing* parent = ParentThing;
    CTC75* tc;
    if (parent->GetInterfaceInline(&tc, 0x75))
        tc->m_flag = true;
}
