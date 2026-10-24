// CTCHeroOpinionDeedLog::FindCriminalDeed(EOpinionDeedType type, const CThing* target) -- the handle of the first logged crime of this type against this target, or -1
// Leaf: ?FindCriminalDeed@CTCHeroOpinionDeedLog@@QAEJW4EOpinionDeedType@@PBVCThing@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x006C5800, 142 bytes (+ int3 padding), thiscall `ret 8`.
// Callers: 0x0065CD07, 0x0065CD25, 0x0065CDE4 (CTCVillage crime code) and 0x00832CB5.
// Callees: CBaseIntelligentPointer::GetPItem() @ 0x00A01B50 (the deed's target pointer at +0x18, landed).
// Layout: as FindWitnessDeed 0x006C5710 (this chunk): OpinionDeedMap at +0x0C; COpinionDeed DeedType +0, PTarget +0x18,
//   IsCrime +0x23.
// Shape: FindDeed 0x006C5780 (end() in a named local, the STLport 4.5 `_Rb_global` increment inline) plus
//   `&& it->second->IsCrime` -- every deed access goes through `it->second`, so it is re-read after the GetPItem call.
// Label correction: the catalog `CBaseObject_FindInListByID` is descriptive; Ego_r.pdb names CTCHeroOpinionDeedLog::FindCriminalDeed.
class CThing;
class CBaseObject;
class CBaseObjectPointer;

class CBaseIntelligentPointer {
public:
    virtual ~CBaseIntelligentPointer();                 // 0x00A01C10
    CBaseObject* GetPItem() const;                      // 0x00A01B50
    CBaseObjectPointer* PData;                          // +4
};

template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {
public:
    T* GetPItem() const { return (T*)CBaseIntelligentPointer::GetPItem(); }
};

class C3DVector {
public:
    float X, Y, Z;
};

enum EOpinionDeedType { OPINION_DEED_TYPE_WITNESS_0x35 = 0x35 };

class COpinionDeed {                                    // 40 bytes (Ego_r.pdb)
public:
    EOpinionDeedType DeedType;                          // +0x00
    long DeedHandle;                                    // +0x04
    C3DVector Position;                                 // +0x08
    long FrameHappened;                                 // +0x14
    CIntelligentPointer<const CThing> PTarget;          // +0x18
    bool WasSearchingForWitnesses;                      // +0x20
    bool IsStillHappening;                              // +0x21
    bool IsCrimeReportedToVillage;                      // +0x22
    bool IsCrime;                                       // +0x23
    long ExtraData;                                     // +0x24
};

namespace std {

template <class _T1, class _T2> class pair {
public:
    _T1 first;
    _T2 second;
};

class _Rb_tree_node_base {
public:
    typedef _Rb_tree_node_base* _Base_ptr;
    long _M_color;                                      // +0x00
    _Base_ptr _M_parent;                                // +0x04
    _Base_ptr _M_left;                                  // +0x08
    _Base_ptr _M_right;                                 // +0x0C
};

template <class _Value> class _Rb_tree_node : public _Rb_tree_node_base {
public:
    _Value _M_value_field;                              // +0x10
};

template <class _Dummy> class _Rb_global {
public:
    static _Rb_tree_node_base* _M_increment(_Rb_tree_node_base* _M_node);
};

template <class _Dummy>
inline _Rb_tree_node_base* _Rb_global<_Dummy>::_M_increment(_Rb_tree_node_base* _M_node)
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

class _Rb_tree_base_iterator {
public:
    typedef _Rb_tree_node_base::_Base_ptr _Base_ptr;
    _Base_ptr _M_node;
    void _M_increment() { _M_node = _Rb_global<bool>::_M_increment(_M_node); }
};

template <class _Value> class _Rb_tree_iterator : public _Rb_tree_base_iterator {
public:
    _Rb_tree_iterator(_Rb_tree_node<_Value>* __x) { _M_node = __x; }
    _Value& operator*() const { return ((_Rb_tree_node<_Value>*)_M_node)->_M_value_field; }
    _Value* operator->() const { return &(operator*()); }
    _Rb_tree_iterator& operator++() { _M_increment(); return *this; }
    bool operator==(const _Rb_tree_iterator& __y) const { return _M_node == __y._M_node; }
    bool operator!=(const _Rb_tree_iterator& __y) const { return _M_node != __y._M_node; }
};

template <class _Tp> class less { };
template <class _Tp> class allocator { };

template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
};

template <class _Key, class _Tp, class _Compare = less<_Key>, class _Alloc = allocator<pair<const _Key, _Tp> > >
class map {
public:
    typedef pair<const _Key, _Tp> value_type;
    typedef _Rb_tree_node<value_type> _Node;
    typedef _Rb_tree_iterator<value_type> iterator;
    _STLP_alloc_proxy<_Node*, _Node, allocator<_Node> > _M_header;   // +0x00
    unsigned int _M_node_count;                                       // +0x04
    _Compare _M_key_compare;                                          // +0x08
    iterator begin() { return (_Node*)_M_header._M_data->_M_left; }
    iterator end() { return _M_header._M_data; }
};

}

class CTCBase {
public:
    virtual ~CTCBase();
    CThing* ParentThing;                                // +0x04
    bool ToBeRemoved;                                   // +0x08
    bool IsInGlobalUpdateTCs;                           // +0x09
};

class CTCHeroOpinionDeedLog : public CTCBase {
public:
    std::map<long, COpinionDeed*> OpinionDeedMap;       // +0x0C (STLport map: 12 bytes)
    long FindWitnessDeed(long deedHandle);
    long FindDeed(EOpinionDeedType type, const CThing* target);
    long FindCriminalDeed(EOpinionDeedType type, const CThing* target);
};

long CTCHeroOpinionDeedLog::FindCriminalDeed(EOpinionDeedType type, const CThing* target)
{
    std::map<long, COpinionDeed*>::iterator end = OpinionDeedMap.end();
    for (std::map<long, COpinionDeed*>::iterator it = OpinionDeedMap.begin(); it != end; ++it) {
        if (it->second && it->second->DeedType == type && it->second->PTarget.GetPItem() == target && it->second->IsCrime)
            return it->first;
    }
    return -1;
}