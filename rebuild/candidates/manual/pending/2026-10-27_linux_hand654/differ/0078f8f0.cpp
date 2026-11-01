// CTCDivineWrath::CancelForCutscene() -- a cutscene cancels the spell: reset every creature it hit, mark the cutscene cancel, fade the environment back at once, finish the caster's current action, reset its interface-0xA7 component, kill the build-up effect, fade the hand effects, stop the looping sound and remove the component
// Grade: DIFFER(506v505) -- one residue left: retail loads the caster (ParentThing) into esi and spills it to [esp+18h] BEFORE the FinishCurrentAction call (ecx = esi), keeping the constant 1 in ebx; ours calls FinishCurrentAction through a fresh ParentThing read and loads the caster after it. Declaring the caster first (any form: plain, const, CThingCreatureBase*, declared early, copied, through an inline GetTC/GetInterface) makes VC7.1 give the caster ebx and drop the constant register (499v505). next: find the variable shape that splits the caster web (retail keeps it in esi across the call and in the frame for RemoveTC), else lane tie.
// Leaf: ?CancelForCutscene@CTCDivineWrath@@QAEXXZ
// Retail 0x0078F8F0, 505 bytes (plain `ret`; `and esp,-8` frame for the loop's 8-byte intelligent-pointer copy).
// Callees: CBaseIntelligentPointer ctor 0x00A01B10 / GetPItem 0x00A01B50 / SetPItem 0x00A01B90 / dtor 0x00A01C10
// (CIntelligentPointer<CThingCreatureBase> vtable 0x0123E7D8), CTCDivineWrath::ResetCreatureHitState(CIntelligentPointer
// <CThingCreatureBase>) @ 0x0078E900 (by value), CTCBase::GetWorld @ 0x00686A70, CEnvironment::
// TransitionToDefaultThemeAllChannels(long, float) @ 0x006B3950 (0.001f), CThingCreatureBase::FinishCurrentAction
// @ 0x00662650, LowerBound @ 0x0040F020 (interface 0xA7 and the sound emitter 0x10), CThing::Kill(bool) @ 0x004C9B80,
// FadeOrKillEffect @ 0x0072ABD0 (__fastcall), StopSample @ 0x0066ADD0, CThing::RemoveTC(long) @ 0x004C9840 (0x85).
// Shape: the hit-map loop is the landed CancelMove 0x0078F820 STLport model (_Rb_tree_node_base, _Rb_global::_M_increment) with
//   begin/end in the for-init (a named end() local spills here); StopSound(LoopingSound) is the same-TU private member
//   0x0078EC80 (landed) defined before this function, inlined by VC7.1 (its own key shares the A7 key slot).
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
struct _Rb_tree_node_base {
    unsigned char       _M_color;
    _Rb_tree_node_base* _M_parent;
    _Rb_tree_node_base* _M_left;
    _Rb_tree_node_base* _M_right;
};
struct _Rb_global {
    static _Rb_tree_node_base* _M_increment(_Rb_tree_node_base* _M_node)
    {
        if (_M_node->_M_right != 0) {
            _M_node = _M_node->_M_right;
            while (_M_node->_M_left != 0)
                _M_node = _M_node->_M_left;
        }
        else {
            _Rb_tree_node_base* __y = _M_node->_M_parent;
            while (_M_node == __y->_M_right) {
                _M_node = __y;
                __y = __y->_M_parent;
            }
            if (_M_node->_M_right != __y)
                _M_node = __y;
        }
        return _M_node;
    }
};
template <class K, class V> struct pair {
    K first;
    V second;
};
struct PerCreatureInfo {
    CIntelligentPointerCreature ActualCreature;   // +0x00
};
struct CCreatureInfoMap {                                // STLport map<CThingCreatureBase*, PerCreatureInfo> (12 bytes)
    typedef pair<CThingCreatureBase* const, PerCreatureInfo> value_type;
    struct _Node : _Rb_tree_node_base {
        value_type _M_value_field;                       // +0x10 (key) / +0x14 (info)
    };
    struct iterator {
        _Rb_tree_node_base* _M_node;
        iterator(_Rb_tree_node_base* n) : _M_node(n) { }
        value_type& operator*() const { return static_cast<_Node*>(_M_node)->_M_value_field; }
        bool operator!=(const iterator& o) const { return _M_node != o._M_node; }
        iterator& operator++() { _M_node = _Rb_global::_M_increment(_M_node); return *this; }
    };
    _Rb_tree_node_base* _M_header;
    unsigned long       _M_node_count;
    unsigned long       _M_key_compare;
    iterator begin() { return iterator(_M_header->_M_left); }
    iterator end() { return iterator(_M_header); }
};
struct CTCDivineWrath {
    virtual void v00();
    CThing* ParentThing;                        // +0x04 (CTCBase)
    unsigned char _pad08[0x6c - 0x08];
    bool  CutSceneCancel;                       // +0x6c
    unsigned char _pad6d[0x78 - 0x6d];
    CCreatureInfoMap LastFrameCreaturesHit;     // +0x78
    unsigned char _pad84[0x9c - 0x84];
    CBaseIntelligentPointer BuildUpEffect;      // +0x9c
    unsigned long HeroDefaultChannel;           // +0xa4
    unsigned long SpellExclusionChannel;        // +0xa8
    CBaseIntelligentPointer HandEffect1;        // +0xac
    CBaseIntelligentPointer HandEffect2;        // +0xb4
    unsigned long LoopingSound;                 // +0xbc
    CWorld* GetWorld();                         // 0x00686A70 (CTCBase)
    void ResetCreatureHitState(CIntelligentPointerCreature creature);   // 0x0078E900
    void StopSound(unsigned long sound);        // 0x0078EC80 (same TU)
    void CancelForCutscene();
};

void CTCDivineWrath::StopSound(unsigned long sound)
{
    CTCSoundEmitter* emitter;
    if (ParentThing->GetInterface(&emitter, 0x10))
        emitter->StopSample(sound);
}

void CTCDivineWrath::CancelForCutscene()
{
    for (CCreatureInfoMap::iterator it = LastFrameCreaturesHit.begin(), end = LastFrameCreaturesHit.end(); it != end; ++it) {
        CIntelligentPointerCreature creature = (*it).second.ActualCreature;
        ResetCreatureHitState(creature);
    }
    CutSceneCancel = true;
    CWorld* world = GetWorld();
    world->PEnvironment->TransitionToDefaultThemeAllChannels(2, 0.001f);
    ParentThing->FinishCurrentAction();
    CThing* caster = ParentThing;
    CTCInterfaceA7* abilities;
    if (caster->GetInterface(&abilities, 0xa7)) {
        abilities->m_active = false;
        abilities->m_count = 0;
    }
    if (IS_THING_ALIVE(BuildUpEffect))
        BuildUpEffect.GetPItem()->Kill(true);
    if (IS_THING_ALIVE(HandEffect1))
        FadeOrKillEffect(HandEffect1);
    if (IS_THING_ALIVE(HandEffect2))
        FadeOrKillEffect(HandEffect2);
    StopSound(LoopingSound);
    caster->RemoveTC(0x85);
}
