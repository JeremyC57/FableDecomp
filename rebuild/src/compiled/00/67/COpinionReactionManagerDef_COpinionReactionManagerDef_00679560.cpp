// COpinionReactionManagerDef::COpinionReactionManagerDef() -- default constructor: the inlined parent-def bases over CResource, own vtable, empty blur vector, eight empty reaction maps, empty match list, zeroed frequency-traits table and its two row cursors
// Leaf: ??0COpinionReactionManagerDef@@QAE@XZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00679560, length 472 bytes, plain `ret`.
// Identity: the vtable stored is 0x0125B7DC = COpinionReactionManagerDef (rtti-vtables.tsv: slot 0 ??_G 0x0067C570,
//   slot 18 Transfer 0x00679740, slot 19 Copy 0x0067CE40, slot 20 GetSizeofClass 0x0067C3B0 -- all landed); the PDB
//   names ??0COpinionReactionManagerDef@@QAE@XZ (Ego_r donor 0x006AB9C0, FableWin 0x0183DA38). The catalog label
//   "CResource_Constructor" names the first callee, not this function.
// Callees: CResource::CResource @ 0x009FBEC0, malloc @ 0x00BFEA0E (eight 0x18-byte tree header nodes, one 0xC list node).
// Layout (Ego_r.pdb COpinionReactionManagerDef, retail STLport sizes): CParentDefClassBase (byte packed, 0x39)
//   {CDefClassBase: PDefManager +0x1c, GlobalIndex +0x20, Setup/Template/DefaultValsApplied bits +0x24;
//   InstantiationName +0x25; SubDefInfoMap +0x29 (vector 0xc, compare, Dirty +0x36)}; AttitudeBlur +0x3c,
//   AttitudeCondition +0x48, TargetingCondition +0x54, PreReactionDelay +0x60, ToleranceToBeingHit +0x6c,
//   BlockFurtherReactions +0x78, AllowSpeechOnNonPureAISpeaker +0x84, AllowWhileCarrying +0x90,
//   AllowWhileFollowingPlayer +0x9c, Matches +0xa8, FrequencyTraitsArray +0xac (LowHigh[2][79]),
//   FrequencyTraits +0x324, FrequencyTraitsHigh +0x328. Same model as the landed CUIStateDef ctor 0x00630350.
// Shape: STLport 4.6 container constructors inline (vector zeros; _Rb_tree_base header from malloc then
//   _M_empty_initialize; _List_base self-linked node); CReactionFrequencyTraitsArray's user ctor (PDB ??0, inlined)
//   is the unsigned 79-step loop clearing both rows.
extern "C" void* __cdecl malloc(unsigned int __size);   // 0x00BFEA0E
extern "C" void __cdecl free(void* __p);                 // 0x00BFEA14

namespace std {
template <class _Tp> class allocator {
public:
    typedef _Tp value_type;
    allocator() { }
    allocator(const allocator<_Tp>&) { }
    _Tp* allocate(unsigned int __n, const void* = 0) { return __n != 0 ? (_Tp*)malloc(__n * sizeof(_Tp)) : 0; }
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
template <class _Arg1, class _Arg2, class _Result> struct binary_function { };
template <class _Tp> struct less : public binary_function<_Tp, _Tp, bool> {
    bool operator()(const _Tp& __x, const _Tp& __y) const { return __x < __y; }
};
template <class _Pair> struct _Select1st { };

// vector
template <class _Tp, class _Alloc> class _Vector_base {
public:
    typedef _Alloc allocator_type;
    _Vector_base(const _Alloc& __a) : _M_start(0), _M_finish(0), _M_end_of_storage(__a, (_Tp*)0) { }
protected:
    _Tp* _M_start;
    _Tp* _M_finish;
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;
};
template <class _Tp, class _Alloc = allocator<_Tp> > class vector : protected _Vector_base<_Tp, _Alloc> {
public:
    typedef _Alloc allocator_type;
    explicit vector(const allocator_type& __a = allocator_type()) : _Vector_base<_Tp, _Alloc>(__a) { }
};

// tree / map
struct _Rb_tree_node_base {
    typedef bool _Color_type;
    _Color_type _M_color;
    _Rb_tree_node_base* _M_parent;
    _Rb_tree_node_base* _M_left;
    _Rb_tree_node_base* _M_right;
};
template <class _Value> struct _Rb_tree_node : public _Rb_tree_node_base {
    _Value _M_value_field;
};
template <class _Tp, class _Alloc> struct _Rb_tree_base {
    typedef _Rb_tree_node<_Tp> _Node;
    typedef _Alloc allocator_type;
    _Rb_tree_base(const allocator_type& __a) : _M_header(allocator<_Node>(), (_Node*)0) {
        _M_header._M_data = _M_header.allocate(1);
    }
protected:
    _STLP_alloc_proxy<_Node*, _Node, allocator<_Node> > _M_header;
};
template <class _Key, class _Value, class _KeyOfValue, class _Compare, class _Alloc = allocator<_Value> >
class _Rb_tree : public _Rb_tree_base<_Value, _Alloc> {
public:
    typedef _Rb_tree_node<_Value>* _Link_type;
    typedef _Alloc allocator_type;
protected:
    unsigned int _M_node_count;
    _Compare _M_key_compare;
    _Link_type& _M_root() const { return (_Link_type&) this->_M_header._M_data->_M_parent; }
    _Link_type& _M_leftmost() const { return (_Link_type&) this->_M_header._M_data->_M_left; }
    _Link_type& _M_rightmost() const { return (_Link_type&) this->_M_header._M_data->_M_right; }
private:
    void _M_empty_initialize() {
        this->_M_header._M_data->_M_color = false;      // _S_rb_tree_red
        _M_root() = 0;
        _M_leftmost() = this->_M_header._M_data;
        _M_rightmost() = this->_M_header._M_data;
    }
public:
    _Rb_tree(const _Compare& __comp, const allocator_type& __a)
        : _Rb_tree_base<_Value, _Alloc>(__a), _M_node_count(0), _M_key_compare(__comp)
    { _M_empty_initialize(); }
};
template <class _Key, class _Tp, class _Compare = less<_Key>, class _Alloc = allocator<pair<const _Key, _Tp> > >
class map {
public:
    typedef pair<const _Key, _Tp> value_type;
    typedef _Alloc allocator_type;
    typedef _Rb_tree<_Key, value_type, _Select1st<value_type>, _Compare, _Alloc> _Rep_type;
    map() : _M_t(_Compare(), allocator_type()) { }
private:
    _Rep_type _M_t;
};

// list
struct _List_node_base {
    _List_node_base* _M_next;
    _List_node_base* _M_prev;
};
template <class _Tp> struct _List_node : public _List_node_base {
    _Tp _M_data;
};
template <class _Tp, class _Alloc> class _List_base {
protected:
    typedef _List_node<_Tp> _Node;
public:
    typedef _Alloc allocator_type;
    _List_base(const allocator_type& __a) : _M_node(allocator<_Node>(), (_Node*)0) {
        _Node* __n = _M_node.allocate(1);
        __n->_M_next = __n;
        __n->_M_prev = __n;
        _M_node._M_data = __n;
    }
public:
    _STLP_alloc_proxy<_Node*, _Node, allocator<_Node> > _M_node;
};
template <class _Tp, class _Alloc = allocator<_Tp> > class list : protected _List_base<_Tp, _Alloc> {
public:
    typedef _Alloc allocator_type;
    explicit list(const allocator_type& __a = allocator_type()) : _List_base<_Tp, _Alloc>(__a) { }
};
}

enum EOpinionReactionType { };
enum EOpinionAttitudeType { };
enum EOpinionTargetingConditionType { };

class CResource { public:
    virtual ~CResource();
    unsigned char _body[0x18];                      // +0x04 ResourceList, PrevResource, NextResource, ResourceSize, LastUsedFrame
    CResource();                                    // 0x009FBEC0
};
class CDefinitionManager;
class CDefPointeeBase : public CResource { public:
    CDefPointeeBase() { }
};
#pragma pack(push, 1)
class __declspec(novtable) CDefClassBase : public CDefPointeeBase { public:
    CDefinitionManager* PDefManager;                // +0x1c
    unsigned long       GlobalIndex;                // +0x20 (not initialised here)
    unsigned char       Setup : 1;                  // +0x24
    unsigned char       Template : 1;
    unsigned char       DefaultValsApplied : 1;
    CDefClassBase() : PDefManager(0), Setup(0), Template(0), DefaultValsApplied(0) { }
    class CSubDefInfo { public: long DefIndex; long OriginalParentDefIndex; };
};
#pragma pack(pop)
class CDefString { public:
    long TablePos;
    CDefString() : TablePos(-1) { }
};
template <class K, class V> class CKeyPairCompareLess { public: };
template <class K, class V, class C> class CVectorMap { public:
    std::vector<std::pair<K, V> > Vector;           // +0x00
    C                             Compare;          // +0x0c
    bool                          Dirty;            // +0x0d
    CVectorMap() : Dirty(false) { }
};
#pragma pack(push, 1)
class __declspec(novtable) CParentDefClassBase : public CDefClassBase { public:
    CDefString InstantiationName;                   // +0x25
    CVectorMap<unsigned long, CDefClassBase::CSubDefInfo, CKeyPairCompareLess<unsigned long, CDefClassBase::CSubDefInfo> > SubDefInfoMap;   // +0x29
    CParentDefClassBase() { }
};
#pragma pack(pop)

class CBlurredAttitudesDef;
class CReactionMatch;
class CReactionFrequencyTraits;
class CReactionFrequencyTraitsArray { public:
    CReactionFrequencyTraits* LowHigh[2][79];       // +0x000 low row, +0x13c high row
    __forceinline CReactionFrequencyTraitsArray()
    {
        for (unsigned long i = 0; i < 79; ++i) {
            LowHigh[0][i] = 0;
            LowHigh[1][i] = 0;
        }
    }
};

class COpinionReactionManagerDef : public CParentDefClassBase { public:   // vtable 0x0125B7DC
    std::vector<CBlurredAttitudesDef>                         AttitudeBlur;                    // +0x3c
    std::map<EOpinionReactionType, EOpinionAttitudeType>      AttitudeCondition;               // +0x48
    std::map<EOpinionReactionType, EOpinionTargetingConditionType> TargetingCondition;         // +0x54
    std::map<EOpinionReactionType, float>                     PreReactionDelay;                // +0x60
    std::map<EOpinionAttitudeType, float>                     ToleranceToBeingHit;             // +0x6c
    std::map<EOpinionReactionType, bool>                      BlockFurtherReactions;           // +0x78
    std::map<EOpinionReactionType, bool>                      AllowSpeechOnNonPureAISpeaker;   // +0x84
    std::map<EOpinionReactionType, bool>                      AllowWhileCarrying;              // +0x90
    std::map<EOpinionReactionType, bool>                      AllowWhileFollowingPlayer;       // +0x9c
    std::list<CReactionMatch*>                                Matches;                         // +0xa8
    CReactionFrequencyTraitsArray                             FrequencyTraitsArray;            // +0xac
    CReactionFrequencyTraits**                                FrequencyTraits;                 // +0x324
    CReactionFrequencyTraits**                                FrequencyTraitsHigh;             // +0x328
    COpinionReactionManagerDef();
    virtual ~COpinionReactionManagerDef();
};

COpinionReactionManagerDef::COpinionReactionManagerDef()
{
    FrequencyTraits = FrequencyTraitsArray.LowHigh[0];
    FrequencyTraitsHigh = FrequencyTraitsArray.LowHigh[1];
}