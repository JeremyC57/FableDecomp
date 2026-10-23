// NScreenEffect::CScreenEffectPriorityStack::ScreenFilterSetFlags(const std::vector<CThingGraphicArgPasser>& args, void* entry) -- re-target a screen-filter entry's colour-filter bypass set: when the entry is on the stack, first restore and forget every listed thing's saved bypass state, then for each listed live thing with a graphic appearance save its current bypass state into the entry's reset map and apply the requested bypass flag
// Grade: DIFFER(591v581) -- register allocation: retail gives ebx to the entry pointer (then &entry->reset in loop 1) and ebp to the loop-1 counter / loop-2 element and graphic, the lane swaps them; that cascades into the loop-1 graphic landing in eax (+`mov ecx,eax`), the erase argument scheduling, loop 2's counter stored as an immediate 0 (retail reuses xor edi) and a loop-alignment pad (+10 bytes). Calls, slots (after block-scoping the ScreenFilterKeys find result), layout and control flow match. Tried: named vs block-scoped find iterator, `end() == find()` order, no filter local, separate loop counters, a hoisted reset reference, the loop-1 restore as an inline helper, a named element reference in loop 2.
// Leaf: ScreenFilterSetFlags
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00881A40, length 592 bytes, `ret 8`.
// Callees: ScreenFilterKeys find 0x008839E0 (set<CStackEntry*>, iterator through the hidden result); the reset map's find
// 0x00883860 and operator[] 0x00884C70; _Rb_tree_rebalance_for_erase 0x0042A34E (__cdecl), free 0x00BFEA14;
// CBaseIntelligentPointer ctor 0x00A01B10 / GetPItem 0x00A01B50 / SetPItem 0x00A01B90 / dtor 0x00A01C10;
// CTCGraphicAppearance::SetByPassColorFilter 0x004BFB00.
// Layout: retail CScreenEffectPriorityStack ScreenFilterKeys +0xa0 (12-byte STLport set); CScreenFilterEntry reset map
// +0x30 (map<CThing*, CThingGraphicByPassFlagState>, node key +0x10, value +0x14); CThingGraphicArgPasser is 9 bytes
// in Ego_r.pdb and retail (the vector divides by 9; `#pragma pack(1)` reproduces it): thing +0 (CIntelligentPointer<CThing>,
// vtable 0x01238C6C), bypassSet +8. CThing (retail = Ego_r.pdb - 0x10 here): PTCGraphicAppearance +0x64, state bits +0x91
// (Dead bit 0); CTCGraphicAppearance ByPassColorFilter +0x5b (signed char).
extern "C" void __cdecl free(void* block);

struct CTCGraphicAppearance {
    unsigned char _pad[0x5b];
    signed char ByPassColorFilter;                     // +0x5B
    void SetByPassColorFilter(bool byPass);            // 0x004BFB00
};
struct CThing {
    unsigned char _pad00[0x64];
    CTCGraphicAppearance* PTCGraphicAppearance;        // +0x64
    unsigned char _pad68[0x91 - 0x68];
    unsigned char StateBits;                           // +0x91 bit 0 Dead
};
struct CBaseIntelligentPointer {
    virtual void v000();
    void* m_pItem;                                     // +0x04
    CBaseIntelligentPointer();                         // 0x00A01B10
    ~CBaseIntelligentPointer();                        // 0x00A01C10
    CThing* GetPItem() const;                          // 0x00A01B50
    void SetPItem(CThing* item);                       // 0x00A01B90
};
inline bool IS_THING_ALIVE(const CBaseIntelligentPointer& pointer)
{
    return pointer.GetPItem() && !(pointer.GetPItem()->StateBits & 1);
}
template <class T> struct CIntelligentPointer : CBaseIntelligentPointer {   // vtable 0x01238C6C
    CIntelligentPointer(T* item = 0) { SetPItem(item); }
    CIntelligentPointer& operator=(const CIntelligentPointer& other) { SetPItem(other.GetPItem()); return *this; }
    virtual void v000();
};

namespace NScreenEffect {

struct CArgVector;
struct CScreenEffectPriorityStack {
    struct CStackEntry {
        virtual void v00();
        long curdurationcount;                         // +0x04
        long durationcount;                            // +0x08
        bool fadingout;                                // +0x0c
    };
#pragma pack(push, 1)
    struct CThingGraphicArgPasser {                    // 9 bytes
        CIntelligentPointer<CThing> thing;             // +0x00
        bool bypassSet;                                // +0x08
    };
#pragma pack(pop)
    struct SKeyNode {
        long _M_color; SKeyNode* _M_parent; SKeyNode* _M_left; SKeyNode* _M_right;
        CStackEntry* _M_value_field;                   // +0x10
    };
    struct CKeySet {                                   // STLport set<CStackEntry*>
        struct iterator {
            SKeyNode* _M_node;
            iterator() {}
            iterator(SKeyNode* node) : _M_node(node) {}
            bool operator==(const iterator& other) const { return _M_node == other._M_node; }
        };
        SKeyNode*     _M_header;
        unsigned long _M_node_count;
        unsigned long _M_key_compare;
        iterator find(CStackEntry* const& key);        // 0x008839E0
        iterator end() { return iterator(_M_header); }
    };
    struct CScreenFilterEntry : CStackEntry {
        struct CThingGraphicByPassFlagState {
            CIntelligentPointer<CThing> thing;         // +0x00
            bool bypass;                               // +0x08
            CThingGraphicByPassFlagState() : bypass(false) {}
        };
        struct SResetNode {
            long _M_color; SResetNode* _M_parent; SResetNode* _M_left; SResetNode* _M_right;
            CThing* first;                             // +0x10
            CThingGraphicByPassFlagState second;       // +0x14
        };
        struct CResetMap {                             // STLport map<CThing*, CThingGraphicByPassFlagState>
            struct iterator {
                SResetNode* _M_node;
                iterator() {}
                iterator(SResetNode* node) : _M_node(node) {}
                bool operator!=(const iterator& other) const { return _M_node != other._M_node; }
            };
            SResetNode*   _M_header;
            unsigned long _M_node_count;
            unsigned long _M_key_compare;
            iterator find(CThing* const& key);         // 0x00883860
            iterator end() { return iterator(_M_header); }
            CThingGraphicByPassFlagState& operator[](CThing* const& key);   // 0x00884C70
            void erase(iterator position);
        };
        unsigned char _pad10[0x30 - 0x10];
        CResetMap reset;                               // +0x30
    };

    virtual void v00();
    unsigned char _pad04[0xa0 - 0x04];
    CKeySet ScreenFilterKeys;                          // +0xa0

    void ScreenFilterSetFlags(const CArgVector& args, void* entry);
};

struct CArgVector {                                    // STLport vector<CThingGraphicArgPasser>
    NScreenEffect::CScreenEffectPriorityStack::CThingGraphicArgPasser* _M_start;
    NScreenEffect::CScreenEffectPriorityStack::CThingGraphicArgPasser* _M_finish;
    NScreenEffect::CScreenEffectPriorityStack::CThingGraphicArgPasser* _M_end_of_storage;
    unsigned int size() const { return _M_finish - _M_start; }
    const NScreenEffect::CScreenEffectPriorityStack::CThingGraphicArgPasser& operator[](unsigned int i) const { return _M_start[i]; }
};

}   // namespace NScreenEffect

void* __cdecl _Rb_tree_rebalance_for_erase(void* z, void*& root, void*& leftmost, void*& rightmost);   // 0x0042A34E

namespace NScreenEffect {

inline void CScreenEffectPriorityStack::CScreenFilterEntry::CResetMap::erase(iterator position)
{
    SResetNode* node = (SResetNode*)_Rb_tree_rebalance_for_erase(position._M_node, (void*&)_M_header->_M_parent,
                                                                (void*&)_M_header->_M_left, (void*&)_M_header->_M_right);
    node->second.~CThingGraphicByPassFlagState();
    if (node)
        free(node);
    --_M_node_count;
}

void CScreenEffectPriorityStack::ScreenFilterSetFlags(const CArgVector& args, void* entry)
{
    {
        CKeySet::iterator found = ScreenFilterKeys.find((CStackEntry*)entry);
        if (found == ScreenFilterKeys.end())
            return;
    }
    for (int i = 0; i < (int)args.size(); ++i) {
        CScreenFilterEntry::CResetMap::iterator it = ((CScreenFilterEntry*)entry)->reset.find(args[i].thing.GetPItem());
        if (it != ((CScreenFilterEntry*)entry)->reset.end()) {
            CScreenFilterEntry::CThingGraphicByPassFlagState& state = it._M_node->second;
            if (IS_THING_ALIVE(state.thing)) {
                CTCGraphicAppearance* graphic = state.thing.GetPItem()->PTCGraphicAppearance;
                if (graphic)
                    graphic->SetByPassColorFilter(state.bypass);
            }
            ((CScreenFilterEntry*)entry)->reset.erase(it);
        }
    }
    for (int j = 0; j < (int)args.size(); ++j) {
        const CThingGraphicArgPasser& arg = args[j];
        if (IS_THING_ALIVE(arg.thing)) {
            CTCGraphicAppearance* graphic = args[j].thing.GetPItem()->PTCGraphicAppearance;
            if (graphic) {
                CScreenFilterEntry::CThingGraphicByPassFlagState state;
                state.thing = args[j].thing;
                state.bypass = graphic->ByPassColorFilter > 0;
                ((CScreenFilterEntry*)entry)->reset[state.thing.GetPItem()] = state;
                graphic->SetByPassColorFilter(args[j].bypassSet);
            }
        }
    }
}

}   // namespace NScreenEffect
