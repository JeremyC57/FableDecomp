// CTCHeroOpinionDeedLog::~CTCHeroOpinionDeedLog() -- delete every COpinionDeed held in OpinionDeedMap, then the members die in reverse order (DeedReactions' animation strings, VillageForCrimes, the witness-search list, the deed map) and ~CTCBase runs
// Leaf: ??1CTCHeroOpinionDeedLog@@UAE@XZ
// Plan phase 5 (2026-11-05), CIntelligentPointer wave: the local CBaseIntelligentPointer / CIntelligentPointer stand-ins replaced by the shared engine/CIntelligentPointer.h, retail (byte-packed) branch FABLE_RETAIL_CINTELLIGENTPOINTER (Ego_r.pdb: vptr +0x0, PData +0x4; ctor 0x00A01B10, dtor 0x00A01C10, GetPItem 0x00A01B50, SetPItem 0x00A01B90).
// Plan phase 3 (2026-11-04), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
// Retail: Fable.exe (TLC, VC7.1) @ 0x006C5D70, 284 bytes, thiscall, no frame; ends `jmp ~CTCBase` (0x00686830).
// Identity (proven): the first store is vtable 0x0125EA44 (RTTI .?AVCTCHeroOpinionDeedLog@@ : CTCBase : CBaseClassNonCopyable :
//   CBaseClass), the sole caller is that vtable's slot-0 ??_G 0x006C9C30 (call, `test [esp+8],1`, operator delete), and Ego_r.pdb
//   has ??1CTCHeroOpinionDeedLog@@UAE@XZ (user-declared: the store stays). The member walk is Ego_r's CTCHeroOpinionDeedLog at
//   STLport sizes (Ego_r is Dinkumware: map 16 -> 12, list 12 -> 4, so DeedReactions 0xF4 -> 0xE8, 92 x 0x90 ending at 0x34A8 =
//   Ego_r sizeof 0x34B4 - 0xC): COpinionDeed::PTarget +0x18, CWitnessesSearchingDeed::Recipient at node +0x10 (data +8),
//   COpinionDeedReaction::Animations = CCharString[18] at +0. The catalog label LTextTreeWalkThrough_Destructor is propagated.
// Callees: ~CBaseIntelligentPointer 0x00A01C10 (virtual, called directly by the implicit ~CIntelligentPointer<T>), operator
//   delete 0x00BFE9BC, `vector destructor iterator' 0x00401400 (??_I, the implicit ~COpinionDeedReaction per element, with
//   ~CCharString 0x0099EAE0), the map's _Rb_tree::_M_erase 0x006C4F80 (out of line, stl/_tree.c), free 0x00BFEA14 (STLport
//   node allocator), ~CTCBase 0x00686830.
// Shape: the body is only `delete it->second` over the map; end() is held in a local before the loop (retail keeps the header
//   in ebp across the deletes; `it != OpinionDeedMap.end()` reloads it each pass, 300v284). _Rb_global::_M_increment expands
//   inline from the real stl/_tree.c text; STLport ~_List_base (clear inline, free per node) and ~_Rb_tree (clear: _M_erase +
//   header reset, then header free) are the real 4.6 text, as chunk 664's ~CNavigatorAStar.
// Layout: engine/CTCBase.h is flattened (vptr word, pack 1), so CTCBase is restated as the polymorphic retail base over
//   engine/CBaseClassNonCopyable.h; COpinionDeed / CWitnessesSearchingDeed / COpinionDeedReaction have no engine headers
//   (Ego_r.pdb members restated). STLport 4.6 text (stl/_tree.h, _tree.c, _list.h, _list.c) reduced to what the dtor uses.
extern "C" void __cdecl free(void* block);                                       // 0x00BFEA14
void __cdecl operator delete(void* block);                                       // 0x00BFE9BC
#include "engine/CBaseClassNonCopyable.h"

namespace std {
template <class _Tp> class allocator {
public:
    void deallocate(_Tp* __p, unsigned int) { if (__p != 0) free(__p); }
};
template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
};
template <class _T1, class _T2> struct pair {
    typedef _T1 first_type;
    typedef _T2 second_type;
    _T1 first;
    _T2 second;
};
template <class _Tp> struct less { bool operator()(const _Tp& __x, const _Tp& __y) const { return __x < __y; } };
template <class _Pair> struct _Select1st { const typename _Pair::first_type& operator()(const _Pair& __x) const { return __x.first; } };
template <class _Tp> inline void _Destroy(_Tp* __pointer) { __pointer->~_Tp(); }
// ---- list (stl/_list.h, stl/_list.c)
struct _List_node_base {
    _List_node_base* _M_next;
    _List_node_base* _M_prev;
};
template <class _Tp> struct _List_node : public _List_node_base {
    _Tp _M_data;
};
template <class _Tp, class _Alloc> class _List_base {
public:
    typedef _List_node<_Tp> _Node;
    _STLP_alloc_proxy<_List_node_base*, _Node, allocator<_Node> > _M_node;
    ~_List_base()
    {
        clear();
        _M_node.deallocate((_Node*)_M_node._M_data, 1);
    }
    void clear();
};
template <class _Tp, class _Alloc>
void _List_base<_Tp, _Alloc>::clear()
{
    _List_node<_Tp>* __cur = (_List_node<_Tp>*)this->_M_node._M_data->_M_next;
    while (__cur != this->_M_node._M_data) {
        _List_node<_Tp>* __tmp = __cur;
        __cur = (_List_node<_Tp>*)__cur->_M_next;
        _Destroy(&__tmp->_M_data);
        this->_M_node.deallocate(__tmp, 1);
    }
    this->_M_node._M_data->_M_next = this->_M_node._M_data;
    this->_M_node._M_data->_M_prev = this->_M_node._M_data;
}
template <class _Tp, class _Alloc = allocator<_Tp> > class list : public _List_base<_Tp, _Alloc> {
public:
    ~list() { }
};
// ---- tree (stl/_tree.h, stl/_tree.c)
struct _Rb_tree_node_base {
    typedef _Rb_tree_node_base* _Base_ptr;
    bool      _M_color;
    _Base_ptr _M_parent;
    _Base_ptr _M_left;
    _Base_ptr _M_right;
};
template <class _Value> struct _Rb_tree_node : public _Rb_tree_node_base {
    _Value _M_value_field;
};
template <class _Dummy> class _Rb_global {
public:
    typedef _Rb_tree_node_base* _Base_ptr;
    static _Rb_tree_node_base* __cdecl _M_increment(_Rb_tree_node_base* __x);
};
typedef _Rb_global<bool> _Rb_global_inst;
template <class _Dummy>
_Rb_tree_node_base* __cdecl _Rb_global<_Dummy>::_M_increment(_Rb_tree_node_base* _M_node)
{
    if (_M_node->_M_right != 0) {
        _M_node = _M_node->_M_right;
        while (_M_node->_M_left != 0)
            _M_node = _M_node->_M_left;
    }
    else {
        _Base_ptr __y = _M_node->_M_parent;
        while (_M_node == __y->_M_right) {
            _M_node = __y;
            __y = __y->_M_parent;
        }
        if (_M_node->_M_right != __y)
            _M_node = __y;
    }
    return _M_node;
}
struct _Rb_tree_base_iterator {
    typedef _Rb_tree_node_base* _Base_ptr;
    _Base_ptr _M_node;
    void _M_increment() { _M_node = _Rb_global_inst::_M_increment(_M_node); }
};
inline bool operator==(const _Rb_tree_base_iterator& __x, const _Rb_tree_base_iterator& __y) { return __x._M_node == __y._M_node; }
inline bool operator!=(const _Rb_tree_base_iterator& __x, const _Rb_tree_base_iterator& __y) { return __x._M_node != __y._M_node; }
template <class _Tp> struct _Nonconst_traits {
    typedef _Tp value_type;
    typedef _Tp* pointer;
    typedef _Tp& reference;
};
template <class _Value, class _Traits> struct _Rb_tree_iterator : public _Rb_tree_base_iterator {
    typedef _Rb_tree_iterator<_Value, _Traits> _Self;
    typedef typename _Traits::reference reference;
    typedef typename _Traits::pointer pointer;
    typedef _Rb_tree_node<_Value>* _Link_type;
    _Rb_tree_iterator() { _M_node = 0; }
    _Rb_tree_iterator(_Link_type __x) { _M_node = __x; }
    reference operator*() const { return _Link_type(_M_node)->_M_value_field; }
    pointer operator->() const { return &(operator*()); }
    _Self& operator++() { _M_increment(); return *this; }
};
template <class _Tp, class _Alloc> struct _Rb_tree_base {
    typedef _Rb_tree_node<_Tp> _Node;
    _STLP_alloc_proxy<_Rb_tree_node_base*, _Node, allocator<_Node> > _M_header;
    ~_Rb_tree_base() { _M_header.deallocate((_Node*)_M_header._M_data, 1); }
};
template <class _Key, class _Value, class _KeyOfValue, class _Compare, class _Alloc = allocator<_Value> >
class _Rb_tree : public _Rb_tree_base<_Value, _Alloc> {
public:
    typedef _Rb_tree_node_base* _Base_ptr;
    typedef _Rb_tree_node<_Value>* _Link_type;
    typedef _Rb_tree_iterator<_Value, _Nonconst_traits<_Value> > iterator;
    unsigned int _M_node_count;
    _Compare _M_key_compare;
    _Base_ptr& _M_root() const { return (_Base_ptr&)this->_M_header._M_data->_M_parent; }
    _Base_ptr& _M_leftmost() const { return (_Base_ptr&)this->_M_header._M_data->_M_left; }
    _Base_ptr& _M_rightmost() const { return (_Base_ptr&)this->_M_header._M_data->_M_right; }
    iterator begin() { return iterator((_Link_type)_M_leftmost()); }
    iterator end() { return iterator((_Link_type)this->_M_header._M_data); }
private:
    void _M_erase(_Link_type __x);                                       // stl/_tree.c (out of line: 0x006C4F80)
public:
    ~_Rb_tree() { clear(); }
    void clear()
    {
        if (_M_node_count != 0) {
            _M_erase((_Link_type)_M_root());
            _M_leftmost() = this->_M_header._M_data;
            _M_root() = 0;
            _M_rightmost() = this->_M_header._M_data;
            _M_node_count = 0;
        }
    }
};
template <class _Key, class _Tp, class _Compare = less<_Key>, class _Alloc = allocator<pair<const _Key, _Tp> > >
class map {
public:
    typedef pair<const _Key, _Tp> value_type;
    typedef _Rb_tree<_Key, value_type, _Select1st<value_type>, _Compare, _Alloc> _Rep_type;
    typedef typename _Rep_type::iterator iterator;
    _Rep_type _M_t;
    iterator begin() { return _M_t.begin(); }
    iterator end() { return _M_t.end(); }
};
} // namespace std

class CBaseObject;
class CBaseObjectPointer;
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#include "engine/CIntelligentPointer.h"
class CThing;
class CThingAICreature;
class CFaction;
class CCharString {
public:
    void* PStringData;
    ~CCharString();                                      // 0x0099EAE0
};
#include "engine/C3DVector.h"
enum EOpinionDeedType { };
enum EOpinionPostDeedType { };

class COpinionDeed {                                     // Ego_r.pdb, 0x28 bytes
public:
    EOpinionDeedType DeedType;                           // +0x00
    long DeedHandle;                                     // +0x04
    C3DVector Position;                                  // +0x08
    long FrameHappened;                                  // +0x14
    CIntelligentPointer<const CThing> PTarget;           // +0x18
    bool WasSearchingForWitnesses;                       // +0x20
    bool IsStillHappening;                               // +0x21
    bool IsCrimeReportedToVillage;                       // +0x22
    bool IsCrime;                                        // +0x23
    long ExtraData;                                      // +0x24
};
class CWitnessesSearchingDeed {                          // Ego_r.pdb, 0x1C bytes
public:
    long DeedHandle;                                     // +0x00
    EOpinionDeedType DeedType;                           // +0x04
    CIntelligentPointer<CThingAICreature> Recipient;     // +0x08
    EOpinionPostDeedType PostType;                       // +0x10
    CFaction* PFaction;                                  // +0x14
    bool ReverseFactionSearch;                           // +0x18
};
class COpinionDeedReaction {                             // Ego_r.pdb, 0x90 bytes
public:
    CCharString Animations[18];                          // +0x00
    unsigned long DialogueTags[18];                      // +0x48
};
class CTCBase : public CBaseClassNonCopyable {
public:
    virtual ~CTCBase();                                  // 0x00686830
    CThing* ParentThing;                                 // +0x04
    bool ToBeRemoved;                                    // +0x08
    bool IsInGlobalUpdateTCs;                            // +0x09
};
class CTCHeroOpinionDeedLog : public CTCBase {           // vtable 0x0125EA44
public:
    virtual ~CTCHeroOpinionDeedLog();
    std::map<long, COpinionDeed*> OpinionDeedMap;                            // +0x0C
    std::list<CWitnessesSearchingDeed> OpinionDeedsSearchingForWitnesses;    // +0x18
    long NextHandle;                                                         // +0x1C
    CIntelligentPointer<CThing> VillageForCrimes;                            // +0x20
    long WeaponOutDeedHandle;                                                // +0x28
    bool WeaponOutCrimeEnabled;                                              // +0x2C
    bool GuardsIgnoreCrimes;                                                 // +0x2D
    unsigned short DeedEffectsDefIndices[92];                                // +0x2E
    COpinionDeedReaction DeedReactions[92];                                  // +0xE8
};

CTCHeroOpinionDeedLog::~CTCHeroOpinionDeedLog()
{
    std::map<long, COpinionDeed*>::iterator end = OpinionDeedMap.end();
    for (std::map<long, COpinionDeed*>::iterator it = OpinionDeedMap.begin(); it != end; ++it)
        delete it->second;
}