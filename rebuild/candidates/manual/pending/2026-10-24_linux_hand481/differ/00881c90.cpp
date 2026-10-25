// NScreenEffect::CScreenEffectPriorityStack::InsertScreenFilterEntry(float duration, const CScreenFilterParams& param, const vector<CThingGraphicArgPasser>& things, ScreenEffectPriority priority) -- append a timed screen-filter entry to the stack of its priority: every live thing with a graphic appearance gets its colour-filter bypass set as asked while its previous state is saved in the entry's reset map; the entry takes the parameters and that map, is registered in the key set and returned as the effect handle
// Grade: DIFFER(874v874) -- register allocation and store order: the per-thing loop keeps &things[i] in ebp for the first two GetPItem calls and the operator[] result in ebx (re-zeroed after), the default CScreenFilterParams zero stores run Brightness, Contrast, Alpha, and the tail's three back() re-fetches use other registers
// Leaf: ?InsertScreenFilterEntry@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00881C90, length 874 bytes (__thiscall, ret 10h). Ego_r/FableWin PDB
// `?InsertScreenFilterEntry@CScreenEffectPriorityStack@NScreenEffect@@QAEPAXMABUCScreenFilterParams@CEnvironmentSpellEffect
// Interface@@ABV?$vector@UCThingGraphicArgPasser@...@@W4ScreenEffectPriority@2@@Z`; the catalog label
// "CEnvironmentSpellEffectInterface_AddScreenFilter" is descriptive. Siblings: the landed InsertDisplacementEntry 0x00881780
// (same push_back / key-set shape) and CScreenFilterEntry::Reset 0x00883AE0 (same bypass-state model).
// Callees: malloc 0x00BFEA0E (map header, list node), free 0x00BFEA14, CBaseIntelligentPointer ctor 0x00A01B10 / GetPItem
//          0x00A01B50 / SetPItem 0x00A01B90 / dtor 0x00A01C10 (vtable CIntelligentPointer<CThing> 0x01238C6C),
//          map<CThing*, CThingGraphicByPassFlagState>::operator[] 0x00884C70 (landed), CTCGraphicAppearance::
//          SetByPassColorFilter(bool) 0x004BFB00, __ftol2 0x00BFEA70, the map's _M_erase 0x008834F0, copy ctor 0x00884A40,
//          operator= 0x00884770, set<CStackEntry*>::insert 0x00883FD0. Vtables: CStackEntry 0x01277FF0, CScreenFilterEntry
//          0x01278060. Global: g_framesPerSecond 0x01375550.
// Layout (PDB with retail STLport containers, as the landed siblings): ScreenFilterParamStack list<CScreenFilterEntry>[3]
//         +0x94, ScreenFilterKeys +0xA0; CScreenFilterEntry (0x3C) param +0x10, reset +0x30; list node 0x44; CThing
//         graphic appearance +0x64, dead flag +0x91 bit 0; CTCGraphicAppearance ByPassColorFilter (signed char) +0x5B.
// Shape (matched parts): STLport map header allocated in _Rb_tree_base's constructor (before the count store); the
// int loop bound `(int)things.size()` (signed /9); the IS_THING_ALIVE idiom; a 9-byte CThingGraphicByPassFlagState
// local assigned into reset[] (CIntelligentPointer::operator= through SetPItem); push_back of a CScreenFilterEntry
// temporary (placement new null test, implicit copy ctor: base vptr, fields, own vptr, rep movsd, map copy), its
// destruction (own vptr store, map clear + free; the trailing base vptr store is dead); then back() re-fetched for the
// param copy, the map assignment and the key, the key inserted through a __forceinline helper whose by-value parameter
// is homed in the dead `priority` slot.
// Tried (diagnostic compiles): a named thing local, a passer reference, a char counter, SetToDefault-ordered params,
// forced inlining of the entry and tree constructors.
extern "C" void* __cdecl malloc(unsigned int size);     // 0x00BFEA0E
extern "C" void __cdecl free(void* p);                  // 0x00BFEA14
inline void* operator new(unsigned int, void* where) { return where; }
extern long g_framesPerSecond;                          // 0x01375550

class CBaseObject;
namespace NGraphicAppearance {
class CTCGraphicAppearance {
public:
    unsigned char _pad00[0x5b];
    signed char   ByPassColorFilter;                    // +0x5b
    void SetByPassColorFilter(bool byPass);             // 0x004BFB00
};
}
class CThing {
public:
    unsigned char _pad00[0x64];
    NGraphicAppearance::CTCGraphicAppearance* m_pGraphicAppearance;   // +0x64
    unsigned char _pad68[0x29];
    unsigned char m_flags91;                            // +0x91 bit 0: dead
};

#pragma pack(push, 1)
class CBaseIntelligentPointer {
public:
    CBaseIntelligentPointer();                          // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                 // 0x00A01C10
    CThing* GetPItem() const;                           // 0x00A01B50
    void SetPItem(CThing* item);                        // 0x00A01B90
private:
    void* PData;                                        // +4
};
template <class T>
class CIntelligentPointer : public CBaseIntelligentPointer {   // CIntelligentPointer<CThing>: vtable 0x01238C6C
public:
    CIntelligentPointer() { SetPItem(0); }
    CIntelligentPointer(const CIntelligentPointer& other) { SetPItem(other.GetPItem()); }
    CIntelligentPointer& operator=(T* item) { SetPItem(item); return *this; }
    CIntelligentPointer& operator=(const CIntelligentPointer& other) { SetPItem(other.GetPItem()); return *this; }
};
#pragma pack(pop)

namespace std {
template <class _Tp> class allocator {
public:
    allocator() { }
    _Tp* allocate(unsigned int __n) { return __n != 0 ? (_Tp*)malloc(__n * sizeof(_Tp)) : 0; }
    void deallocate(_Tp* __p, unsigned int) { if (__p != 0) free(__p); }
};
template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
    _STLP_alloc_proxy(const _MaybeReboundAlloc& __a, _Value __p) : _MaybeReboundAlloc(__a), _M_data(__p) { }
};
template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
};
template <class _Tp> struct less {
    bool operator()(const _Tp& __x, const _Tp& __y) const { return __x < __y; }
};

struct _Rb_tree_node_base {
    bool _M_color;                                      // +0
    _Rb_tree_node_base* _M_parent;                      // +4
    _Rb_tree_node_base* _M_left;                        // +8
    _Rb_tree_node_base* _M_right;                       // +0xC
};
template <class _Value> struct _Rb_tree_node : public _Rb_tree_node_base {
    _Value _M_value_field;                              // +0x10
};
template <class _Value> struct _Rb_tree_iterator {
    _Rb_tree_node_base* _M_node;
};
template <class _Value>
class _Rb_tree_base {
public:
    typedef _Rb_tree_node<_Value> _Node;
    __forceinline _Rb_tree_base() : _M_header(allocator<_Node>(), 0) { _M_header._M_data = _M_header.allocate(1); }
    _STLP_alloc_proxy<_Rb_tree_node_base*, _Node, allocator<_Node> > _M_header;   // +0
};
template <class _Key, class _Value, class _Compare>
class _Rb_tree : public _Rb_tree_base<_Value> {
public:
    typedef _Rb_tree_node<_Value> _Node;
    typedef _Rb_tree_node_base* _Base_ptr;
    unsigned int _M_node_count;                         // +4
    _Compare _M_key_compare;                            // +8
    __forceinline _Rb_tree() : _M_node_count(0)
    {
        this->_M_header._M_data->_M_color = false;
        this->_M_header._M_data->_M_parent = 0;
        this->_M_header._M_data->_M_left = this->_M_header._M_data;
        this->_M_header._M_data->_M_right = this->_M_header._M_data;
    }
    _Rb_tree(const _Rb_tree& __x);                      // 0x00884A40
    ~_Rb_tree()
    {
        clear();
        this->_M_header.deallocate((_Node*)this->_M_header._M_data, 1);
    }
    void clear()
    {
        if (_M_node_count != 0) {
            _M_erase((_Node*)this->_M_header._M_data->_M_parent);
            this->_M_header._M_data->_M_left = this->_M_header._M_data;
            this->_M_header._M_data->_M_parent = 0;
            this->_M_header._M_data->_M_right = this->_M_header._M_data;
            _M_node_count = 0;
        }
    }
    void _M_erase(_Node* __x);                          // 0x008834F0
    _Rb_tree& operator=(const _Rb_tree& __x);           // 0x00884770
};
template <class _Key, class _Tp, class _Compare = less<_Key> >
class map {
public:
    typedef pair<const _Key, _Tp> value_type;
    _Rb_tree<_Key, value_type, _Compare> _M_t;
    _Tp& operator[](const _Key& __k);                   // 0x00884C70
};
template <class _Key, class _Compare = less<_Key> >
class set {
public:
    typedef _Rb_tree_iterator<_Key> iterator;
    unsigned char _M_t[0x0C];
    pair<iterator, bool> insert(const _Key& __x);       // 0x00883FD0
};

struct _List_node_base {
    _List_node_base* _M_next;
    _List_node_base* _M_prev;
};
template <class _Tp> struct _List_node : public _List_node_base {
    _Tp _M_data;
};
template <class _Tp, class _Alloc = allocator<_Tp> >
class list {
public:
    typedef _List_node<_Tp> _Node;
    _STLP_alloc_proxy<_List_node_base*, _Node, allocator<_Node> > _M_node;
    _Node* _M_create_node(const _Tp& __x)
    {
        _Node* __p = (_Node*)malloc(sizeof(_Node));
        new (&__p->_M_data) _Tp(__x);
        return __p;
    }
    void push_back(const _Tp& __x)
    {
        _List_node_base* __n = _M_node._M_data;
        _Node* __tmp = _M_create_node(__x);
        _List_node_base* __prev = __n->_M_prev;
        __tmp->_M_next = __n;
        __tmp->_M_prev = __prev;
        __prev->_M_next = __tmp;
        __n->_M_prev = __tmp;
    }
    _Tp& back() { return ((_Node*)_M_node._M_data->_M_prev)->_M_data; }
};

template <class _Tp, class _Alloc = allocator<_Tp> >
class vector {
public:
    typedef unsigned int size_type;
    _Tp* _M_start;
    _Tp* _M_finish;
    _Tp* _M_end_of_storage;
    size_type size() const { return size_type(_M_finish - _M_start); }
    const _Tp& operator[](size_type __n) const { return *(_M_start + __n); }
};
} // namespace std

struct CRGBFloatColour {
    float R, G, B, A;
    CRGBFloatColour(float r, float g, float b, float a) : R(r), G(g), B(b), A(a) { }
};
class CEnvironmentSpellEffectInterface {
public:
    struct CScreenFilterParams {                        // PDB, 0x20 bytes
        float           Alpha;                          // +0x00
        float           Saturation;                     // +0x04
        float           Contrast;                       // +0x08
        float           Brightness;                     // +0x0C
        CRGBFloatColour Tint;                           // +0x10
        CScreenFilterParams() : Alpha(0.0f), Saturation(1.0f), Contrast(0.0f), Brightness(0.0f), Tint(1.0f, 1.0f, 1.0f, 1.0f) { }
    };
};

namespace NScreenEffect {
enum ScreenEffectPriority { SEP_LOW, SEP_MEDIUM, SEP_HIGH };
class CScreenEffectPriorityStack {
public:
#pragma pack(push, 1)
    struct CThingGraphicArgPasser {                     // Ego_r.pdb, 9 bytes
        CIntelligentPointer<CThing> thing;              // +0
        bool bypassSet;                                 // +8
    };
#pragma pack(pop)
    struct CStackEntry {                                // vtable 0x01277FF0
        CStackEntry(long frames) : curdurationcount(0), durationcount(frames), fadingout(false) { }
        virtual ~CStackEntry() { }
        long curdurationcount;                          // +0x04
        long durationcount;                             // +0x08
        bool fadingout;                                 // +0x0C
    };
    struct CScreenFilterEntry : public CStackEntry {    // vtable 0x01278060
#pragma pack(push, 1)
        class CThingGraphicByPassFlagState {            // Ego_r.pdb, 9 bytes
        public:
            CIntelligentPointer<CThing> thing;          // +0
            bool bypassReset;                           // +8
            CThingGraphicByPassFlagState() : bypassReset(false) { }
        };
#pragma pack(pop)
        typedef std::map<CThing*, CThingGraphicByPassFlagState> CResetMap;
        __forceinline CScreenFilterEntry(long frames) : CStackEntry(frames) { }
        virtual ~CScreenFilterEntry() { }
        CEnvironmentSpellEffectInterface::CScreenFilterParams param;    // +0x10
        CResetMap reset;                                // +0x30
    };

    virtual ~CScreenEffectPriorityStack();
    unsigned char                         _pad04[0x94 - 0x04];
    std::list<CScreenFilterEntry>         ScreenFilterParamStack[3];    // +0x94
    std::set<CStackEntry*>                ScreenFilterKeys;             // +0xA0

    __forceinline void AddScreenFilterKey(CStackEntry* key) { ScreenFilterKeys.insert(key); }
    void* InsertScreenFilterEntry(float duration, const CEnvironmentSpellEffectInterface::CScreenFilterParams& param,
                                  const std::vector<CThingGraphicArgPasser>& things, ScreenEffectPriority priority);
};

void* CScreenEffectPriorityStack::InsertScreenFilterEntry(float duration, const CEnvironmentSpellEffectInterface::CScreenFilterParams& param,
                                                          const std::vector<CThingGraphicArgPasser>& things, ScreenEffectPriority priority)
{
    CScreenFilterEntry::CResetMap reset;
    for (int i = 0; i < (int)things.size(); ++i) {
        if (things[i].thing.GetPItem() != 0) {
            if (!(things[i].thing.GetPItem()->m_flags91 & 1)) {
                NGraphicAppearance::CTCGraphicAppearance* appearance = things[i].thing.GetPItem()->m_pGraphicAppearance;
                if (appearance) {
                    CScreenFilterEntry::CThingGraphicByPassFlagState state;
                    state.thing = things[i].thing.GetPItem();
                    state.bypassReset = appearance->ByPassColorFilter > 0;
                    reset[state.thing.GetPItem()] = state;
                    appearance->SetByPassColorFilter(things[i].bypassSet);
                }
            }
        }
    }
    ScreenFilterParamStack[priority].push_back(CScreenFilterEntry((long)(duration * g_framesPerSecond)));
    ScreenFilterParamStack[priority].back().param = param;
    ScreenFilterParamStack[priority].back().reset = reset;
    CStackEntry* key = &ScreenFilterParamStack[priority].back();
    AddScreenFilterKey(key);
    return key;
}
}
