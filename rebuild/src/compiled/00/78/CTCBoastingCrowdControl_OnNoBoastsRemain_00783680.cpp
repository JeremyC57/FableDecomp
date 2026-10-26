// CTCBoastingCrowdControl::OnNoBoastsRemain() -- tell every live crowd recorded in the hero -> crowd map that no boasts remain (each crowd thing's boasting-crowd creature group component, interface 0x74)
// Leaf: ?OnNoBoastsRemain@CTCBoastingCrowdControl@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00783680, 168 bytes (+ int3 padding to 176), __fastcall, plain `ret`.
// Identity: Ego_r.pdb `?OnNoBoastsRemain@CTCBoastingCrowdControl@@QAEXXZ`; its caller 0x0056AB65 (the boast UI) runs it
//   on the control right after CTCBoastingCrowdControl::OnHeroBoast (landed 0x00783C50) when the quest manager has no
//   boasts left; the per-crowd callee is CTCCreatureGroupBoastingCrowd::OnNoBoastsRemain (PDB name set).
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50, CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound @
//   0x0040F020 (the inlined GetInterface), CTCCreatureGroupBoastingCrowd::OnNoBoastsRemain @ 0x00784B20 (catalogued as
//   the propagated CRenderTarget::DetachTarget).
// Layout: PDB CTCBoastingCrowdControl: Active +0x0c, HeroCrowdThings +0x10 (STLport map<unsigned __int64,
//   CIntelligentPointer<CThing>>: header pointer, node count); node value at +0x10 (key) / +0x18 (pointer); retail
//   CThing presence bits +0x20 (0x74 = bit 20 of +0x2c), interface map +0x44, Dead = bit 0 of +0x91.
// Shape: `for (it = begin(); it != end; ++it) { crowdThing = it->second.GetPItem(); if (IS_THING_ALIVE(crowdThing))
//   { crowdThing->GetInterface(&crowd, 0x74); crowd->OnNoBoastsRemain(); } }` with end() in a local (ebp) and `++it`
//   the inlined `_Rb_global<bool>::_M_increment`. The GetInterface result is not checked: on the absent path the
//   never-written `crowd` (esi) keeps its previous value, initially read from the `push ecx` frame word that the
//   lookup key shares.
// Label correction: catalogued as `CTCCreatureGroupBoastingCrowd_DetachAll`; the body is
//   CTCBoastingCrowdControl::OnNoBoastsRemain.
class CTCBase;

struct CInterfaceMap {                                   // CVectorMap<ETCInterfaceType, CTCBase*> at CThing+0x44
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);                // 0x0040F020
};

class CThing {
public:
    unsigned char _pad00[0x20];
    unsigned long m_presentBits[9];                      // +0x20 UsingComponents
    CInterfaceMap m_interfaces;                          // +0x44
    unsigned char _pad4c[0x91 - 0x4c];
    unsigned char Flags91;                               // +0x91 bit 0 Dead
    bool IsDead() const { return (Flags91 & 1) != 0; }
    template <class T> __forceinline bool GetInterface(T** out, long wanted)
    {
        if (m_presentBits[wanted >> 5] & (1UL << (wanted & 31))) {
            long type = wanted;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            if (it == map->last || wanted < it->type)
                it = map->last;
            *out = (T*)it->component;
            return true;
        }
        return false;
    }
};

class CTCCreatureGroupBoastingCrowd {
public:
    void OnNoBoastsRemain();                             // 0x00784B20
};

class CBaseObject;
class CBaseObjectPointer;
class CBaseIntelligentPointer {                       // vtable 0x0129C95C (Ego_r: vptr + PData, 8 bytes)
public:
    CBaseIntelligentPointer();                           // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                  // 0x00A01C10
    CBaseObject* GetPItem() const;                       // 0x00A01B50
    void SetPItem(CBaseObject* item);                    // 0x00A01B90
private:
    CBaseObjectPointer* PData;                           // +0x04
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer { public: 
    T* GetPItem() const { return (T*)CBaseIntelligentPointer::GetPItem(); }
    void SetPItem(T* item) { CBaseIntelligentPointer::SetPItem((CBaseObject*)item); }
};

struct _Rb_tree_node_base {
    long                _M_color;
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

struct CHeroCrowdMap {                                   // STLport map<unsigned __int64, CIntelligentPointer<CThing>>
    typedef pair<const unsigned __int64, CIntelligentPointer<CThing> > value_type;
    struct _Node : _Rb_tree_node_base {
        value_type _M_value_field;                       // +0x10
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

    iterator begin() { return iterator(_M_header->_M_left); }
    iterator end() { return iterator(_M_header); }
};

#define IS_THING_ALIVE(t) ((t) && !(t)->IsDead())

class CTCBase {
public:
    virtual ~CTCBase();
    CThing* ParentThing;                                 // +0x04
    bool    ToBeRemoved;                                 // +0x08
    bool    IsInGlobalUpdateTCs;                         // +0x09
};

class CTCBoastingCrowdControl : public CTCBase {
public:
    bool          Active;                                // +0x0c
    CHeroCrowdMap HeroCrowdThings;                       // +0x10

    void OnNoBoastsRemain();
};

void CTCBoastingCrowdControl::OnNoBoastsRemain()
{
    CHeroCrowdMap::iterator end = HeroCrowdThings.end();
    for (CHeroCrowdMap::iterator it = HeroCrowdThings.begin(); it != end; ++it) {
        CThing* crowdThing = (*it).second.GetPItem();
        if (IS_THING_ALIVE(crowdThing)) {
            CTCCreatureGroupBoastingCrowd* crowd;
            crowdThing->GetInterface(&crowd, 0x74);
            crowd->OnNoBoastsRemain();
        }
    }
}