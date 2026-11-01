// COpinionReactionManagerDef::~COpinionReactionManagerDef() -- delete every CReactionMatch the def owns and clear the Matches list; the compiler then destroys the members in reverse order (the 79x2 frequency-traits pointers, the list, the eight reaction/attitude maps, the AttitudeBlur vector of polymorphic CBlurredAttitudesDef) and the inline parent-def bases before `jmp ~CResource`
// Leaf: ??1COpinionReactionManagerDef@@UAE@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00678460, 758 bytes, thiscall, `jmp ~CResource 0x009FC550` tail. Stores its own vtable
//   0x0125B7DC first (RTTI .?AVCOpinionReactionManagerDef@@, rebuild/manifest/rtti-vtables.tsv: slot 0 = the scalar deleting
//   destructor 0x0067C570, its only caller; slots 18-20 Transfer 0x00679740 / Copy 0x0067CE40 / GetSizeofClass 0x0067C3B0),
//   ends with the CDefPointeeBase vtable 0x01230BA0 store. The catalog's "CMemoryBlock_Destructor_Extended" is a propagated
//   placeholder (same for the callers' spellings of 0x009FC550, NAME_FIXES.tsv).
// Callees: free 0x00BFEA14 (STLport deallocate: list nodes and header, map headers, vector storage, the CParentDefClassBase
//   SubDefInfoMap storage at +0x29), operator delete 0x00BFE9BC (the traits pointers), the five map _M_erase instantiations
//   0x004507FB (map<EOpinionReactionType,bool>, x4), 0x004DA360 (map<EOpinionAttitudeType,float>), 0x0067A840
//   (map<EOpinionReactionType,float>), 0x0067A800 (map<EOpinionReactionType,EOpinionTargetingConditionType>), 0x0067A7C0
//   (map<EOpinionReactionType,EOpinionAttitudeType>) -- all catalogued as BuildTreeArray/FreeTree/ReleaseThing stand-ins
//   (NAME_FIXES.tsv) -- and the CReactionMatch / CBlurredAttitudesDef virtual destructors through vtable slot 0.
// Layout: Ego_r.pdb COpinionReactionManagerDef (860 bytes with 16-byte maps) at the retail 12-byte STLport sizes: 0x32C
//   bytes, AttitudeBlur +0x3C, AttitudeCondition +0x48, TargetingCondition +0x54, PreReactionDelay +0x60, ToleranceToBeingHit
//   +0x6C, BlockFurtherReactions +0x78, AllowSpeechOnNonPureAISpeaker +0x84, AllowWhileCarrying +0x90,
//   AllowWhileFollowingPlayer +0x9C, Matches +0xA8, FrequencyTraitsArray +0xAC (LowHigh[2][79]), FrequencyTraits +0x324,
//   FrequencyTraitsHigh +0x328. CReactionMatch is polymorphic (deleted through slot 0 with flag 1); CBlurredAttitudesDef
//   (0x48 bytes, vtable 0x0125B8AC) is destroyed in place through its virtual destructor (flag 0). The STLport bodies are
//   the real 4.6 list / _Rb_tree / vector text (the landed ~CGameCameraManager 0x00699BF0 model); the def bases are the
//   landed ~CBlurredAttitudesDef 0x0067C370 model (engine/CDefClassBase.h opt-in, local packed CParentDefClassBase --
//   header debt as there).
// Shape: the user loop evaluates `Matches.end()` once (the header stays in ebx across the deletes: probe 1 with `it !=
//   Matches.end()` reloads it, 759v758); `Matches.clear()` is the inline _List_base::clear (header re-read after each free).
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/CDefClassBase.h"                        // : CDefPointeeBase (vtable 0x01230BA0) : CResource (~CResource 0x009FC550)
extern "C" void __cdecl free(void* __p);                 // 0x00BFEA14
void __cdecl operator delete(void* p);                   // 0x00BFE9BC
class CDefString {                                       // Ego_r.pdb, 4 bytes
public:
    long TablePos;
};
namespace std {
template <class _Tp> class allocator {
public:
    allocator() {}
    void deallocate(_Tp* __p, unsigned int) { if (__p != 0) free(__p); }
};
template <class _T1, class _T2> struct pair { _T1 first; _T2 second; };
template <class _Tp> struct less { };
struct __false_type { __false_type() { } };
template <class _Tp> inline void _Destroy(_Tp* __pointer) { __pointer->~_Tp(); }
template <class _ForwardIterator>
inline void __destroy_aux(_ForwardIterator __first, _ForwardIterator __last, const __false_type&)
{
    for (; __first != __last; ++__first)
        _Destroy(&*__first);
}
template <class _ForwardIterator>
inline void _Destroy(_ForwardIterator __first, _ForwardIterator __last)
{
    __destroy_aux(__first, __last, __false_type());
}
template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
};
template <class _Tp, class _Alloc> class _Vector_base {
public:
    ~_Vector_base()
    {
        if (_M_start != 0)
            _M_end_of_storage.deallocate(_M_start, _M_end_of_storage._M_data - _M_start);
    }
    _Tp* _M_start;
    _Tp* _M_finish;
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;
};
template <class _Tp, class _Alloc = allocator<_Tp> >
class vector : public _Vector_base<_Tp, _Alloc> {
public:
    typedef _Tp* iterator;
    ~vector() { _Destroy(this->_M_start, this->_M_finish); }
};

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
    ~_List_base()
    {
        clear();
        _M_node.deallocate(_M_node._M_data, 1);
    }
    void clear()
    {
        _Node* __cur = (_Node*)_M_node._M_data->_M_next;
        while (__cur != _M_node._M_data) {
            _Node* __tmp = __cur;
            __cur = (_Node*)__cur->_M_next;
            _Destroy(&__tmp->_M_data);
            _M_node.deallocate(__tmp, 1);
        }
        _M_node._M_data->_M_next = _M_node._M_data;
        _M_node._M_data->_M_prev = _M_node._M_data;
    }
    _STLP_alloc_proxy<_Node*, _Node, allocator<_Node> > _M_node;
};
template <class _Tp, class _Alloc = allocator<_Tp> > class list : public _List_base<_Tp, _Alloc> {
public:
    typedef _List_node<_Tp> _Node;
    struct iterator {
        _List_node_base* _M_node;
        iterator(_List_node_base* __x) : _M_node(__x) { }
        _Tp& operator*() const { return ((_Node*)_M_node)->_M_data; }
        iterator& operator++() { _M_node = _M_node->_M_next; return *this; }
        bool operator!=(const iterator& __y) const { return _M_node != __y._M_node; }
    };
    iterator begin() { return iterator(this->_M_node._M_data->_M_next); }
    iterator end() { return iterator(this->_M_node._M_data); }
    void clear() { _List_base<_Tp, _Alloc>::clear(); }
};

struct _Rb_tree_node_base {
    bool                _M_color;
    _Rb_tree_node_base* _M_parent;
    _Rb_tree_node_base* _M_left;
    _Rb_tree_node_base* _M_right;
};
template <class _Value> struct _Rb_tree_node : public _Rb_tree_node_base {
    _Value _M_value_field;
};
template <class _Tp, class _Alloc> struct _Rb_tree_base {
    typedef _Rb_tree_node<_Tp> _Node;
    ~_Rb_tree_base() { _M_header.deallocate(_M_header._M_data, 1); }
    _STLP_alloc_proxy<_Node*, _Node, allocator<_Node> > _M_header;
};
template <class _Key, class _Value, class _KeyOfValue, class _Compare, class _Alloc>
class _Rb_tree : public _Rb_tree_base<_Value, _Alloc> {
public:
    typedef _Rb_tree_node<_Value> _Node;
    typedef _Node* _Link_type;
    unsigned int _M_node_count;
    _Compare _M_key_compare;
protected:
    _Link_type& _M_root() const { return (_Link_type&)this->_M_header._M_data->_M_parent; }
    _Link_type& _M_leftmost() const { return (_Link_type&)this->_M_header._M_data->_M_left; }
    _Link_type& _M_rightmost() const { return (_Link_type&)this->_M_header._M_data->_M_right; }
private:
    void _M_erase(_Link_type __x);                     // map<EOpinionReactionType,bool> 0x004507FB, <EOpinionAttitudeType,float> 0x004DA360,
                                                       // <EOpinionReactionType,float> 0x0067A840, <..,EOpinionTargetingConditionType> 0x0067A800, <..,EOpinionAttitudeType> 0x0067A7C0
public:
    void clear()
    {
        if (_M_node_count != 0) {
            _M_erase(_M_root());
            _M_leftmost() = this->_M_header._M_data;
            _M_root() = 0;
            _M_rightmost() = this->_M_header._M_data;
            _M_node_count = 0;
        }
    }
    ~_Rb_tree() { clear(); }
};
template <class _Pair> struct _Select1st { };
template <class _Key, class _Tp, class _Compare = less<_Key>, class _Alloc = allocator<pair<const _Key, _Tp> > >
class map {
public:
    typedef pair<const _Key, _Tp> value_type;
    _Rb_tree<_Key, value_type, _Select1st<value_type>, _Compare, _Alloc> _M_t;
};
}
template <class K, class V> class CKeyPairCompareLess { public: };
template <class K, class V, class C>
class CVectorMap : public std::vector<std::pair<K, V> > {   // retail 0x10 bytes
public:
    C    Compare;                                        // +0x0c
    bool Dirty;                                          // +0x0d
};
enum EOpinionAttitudeType { };
enum EOpinionReactionType { };
enum EOpinionTargetingConditionType { };

#pragma pack(push, 1)                                    // as engine/CParentDefClassBase.h (retail 0x39 bytes)
class CParentDefClassBase : public CDefClassBase {       // vtable 0x01230CBC
public:
    virtual CDefString GetInstantiationName() const;                     // slot 10 0x004303B0
    virtual void SetInstantiationName(CDefString);                       // slot 11 0x004303C0
    virtual void AddSubDef(const CCharString&, long, long);              // slot 12 0x009B00D0
    virtual void RemoveSubDef(const CCharString&);                       // slot 13 0x009B0170
    virtual CSubDefInfo* GetPSubDefInfo(const CCharString&);             // slot 15 0x009AD510
    virtual const CSubDefInfo* GetPSubDefInfo(const CCharString&) const; // slot 14 0x009AD4A0
    virtual bool IsParentDef() const;                                    // slot 16 0x004303D0
    virtual void InternalSaveBinary(CDataOutputStream&);                 // slot 21 0x009B0200
    virtual void InternalLoadBinary(CDataInputStream&);                  // slot 22 0x009AED70
    virtual CVectorMap<unsigned long, CSubDefInfo, CKeyPairCompareLess<unsigned long, CSubDefInfo> >*
        GetPSubDefInfoMap();                                             // slot 24 0x004303E0
    CDefString InstantiationName;                                        // +0x25
    CVectorMap<unsigned long, CSubDefInfo, CKeyPairCompareLess<unsigned long, CSubDefInfo> > SubDefInfoMap;   // +0x29
};
#pragma pack(pop)

class CBlurredAttitudesDef : public CParentDefClassBase {   // vtable 0x0125B8AC, 0x48 bytes
public:
    std::vector<EOpinionAttitudeType> Attitudes;         // +0x3c
    virtual ~CBlurredAttitudesDef();                     // 0x0067C370 (deleting 0x0067C520)
    virtual void Transfer(CPersistContext&);             // slot 18 0x0067CDB0
    virtual void Copy(const CDefClassBase*);             // slot 19 0x0067C980
    virtual unsigned long GetSizeofClass();              // slot 20 0x0067C510
};

class CReactionMatch {                                   // polymorphic (deleted through its vtable slot 0)
public:
    virtual ~CReactionMatch();
    EOpinionReactionType ReactionType;                   // +0x04
};
class CReactionFrequencyTraits { public:
    void* __vftable;
};
class CReactionFrequencyTraitsArray { public:
    CReactionFrequencyTraits* LowHigh[2][79];           // +0x000 low row, +0x13C high row
    ~CReactionFrequencyTraitsArray()
    {
        for (unsigned long i = 0; i < 79; ++i) {
            delete LowHigh[0][i];
            delete LowHigh[1][i];
        }
    }
};

class COpinionReactionManagerDef : public CParentDefClassBase {   // vtable 0x0125B7DC, 0x32C bytes
public:
    std::vector<CBlurredAttitudesDef> AttitudeBlur;                                    // +0x3c
    std::map<EOpinionReactionType, EOpinionAttitudeType> AttitudeCondition;            // +0x48
    std::map<EOpinionReactionType, EOpinionTargetingConditionType> TargetingCondition; // +0x54
    std::map<EOpinionReactionType, float> PreReactionDelay;                            // +0x60
    std::map<EOpinionAttitudeType, float> ToleranceToBeingHit;                         // +0x6c
    std::map<EOpinionReactionType, bool> BlockFurtherReactions;                        // +0x78
    std::map<EOpinionReactionType, bool> AllowSpeechOnNonPureAISpeaker;                // +0x84
    std::map<EOpinionReactionType, bool> AllowWhileCarrying;                           // +0x90
    std::map<EOpinionReactionType, bool> AllowWhileFollowingPlayer;                    // +0x9c
    std::list<CReactionMatch*> Matches;                                                // +0xa8
    CReactionFrequencyTraitsArray FrequencyTraitsArray;                                // +0xac
    CReactionFrequencyTraits** FrequencyTraits;                                        // +0x324
    CReactionFrequencyTraits** FrequencyTraitsHigh;                                    // +0x328
    virtual ~COpinionReactionManagerDef();
    virtual void Transfer(CPersistContext&);             // slot 18 0x00679740
    virtual void Copy(const CDefClassBase*);             // slot 19 0x0067CE40
    virtual unsigned long GetSizeofClass();              // slot 20 0x0067C3B0
};

COpinionReactionManagerDef::~COpinionReactionManagerDef()
{
    std::list<CReactionMatch*>::iterator end = Matches.end();
    for (std::list<CReactionMatch*>::iterator it = Matches.begin(); it != end; ++it)
        delete *it;
    Matches.clear();
}