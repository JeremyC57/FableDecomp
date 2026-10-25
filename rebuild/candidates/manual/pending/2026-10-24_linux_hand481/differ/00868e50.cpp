// GetArmourWeightsFromThing(const CThing& thing, multimap<CDefPointer<const CArmourDef>, float>& armour_weights, float& total_weight) -- for each appearance modifier the thing wears (component 0x5E, list type 2) that names an armour def, add (armour def, protection weighting) to the multimap and the weighting to the total
// Grade: DIFFER(456v456) -- one x87 commutation: retail `total_weight += weighting` loads the total first (`fld [eax] ; fadd [edi+60h]`), ours loads the weighting first (`fld [edi+60h] ; fadd [eax]`); everything else is byte-identical.
// Leaf: GetArmourWeightsFromThing
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00868E50, 464 bytes incl. int3 padding, /Gr `__fastcall` (thing in ecx,
//   the multimap in edx, the total on the stack), `ret 4`.
// Identity: catalog "GetArmourWeightsFromThing" (_global) is Ego_r.pdb `?GetArmourWeightsFromThing@?A0x23f4dc30@@YIXABVCThing
//   @@AAV?$multimap@V?$CDefPointer@$$CBVCArmourDef@@@@MV?$CCompareDefPointer@$$CBVCArmourDef@@M@@...@std@@AAM@Z` (an
//   anonymous-namespace function of the hit-parameters TU; its one caller is the landed CHitParameters::
//   SetAsModifiedByArmour 0x00869020, which calls it "GetArmourDefsForThing").
// Callees: CThing::GetDefinitionManager() const 0x004C7990 (propagated label "CViewBase::GetDisplayEngine"), the const
//   component LowerBound 0x004365B0, CTCHeroAttachableAppearanceModifiers::GetAppearanceModifierList(list<long>&, long)
//   const 0x00706800, CDefinitionManager::GetDef<CThingDef> 0x0042B040 and GetDef<CArmourDef> 0x00817B40 (labels
//   propagated), CDefClassBase::GetSubDef<CAppearanceModifierDef> 0x0057FF92 (label "GetSubDef<CAbilityDef>" propagated),
//   the multimap's _Rb_tree::_M_insert 0x00869CA0, malloc 0x00BFEA0E / free 0x00BFEA14 (the list sentinel and nodes),
//   the defs' slot 1 on the last release.
// Layout: CThing UsingComponents +0x20 (0x5E = bit 30 of +0x28), Components +0x44; retail CAppearanceModifierDef
//   ArmourDefIndex +0x5C, OveralProtectionWeighting +0x60 (the landed 0x005B4A95 model); STLport list node {next, prev,
//   data} 12 bytes; multimap 12 bytes {header, count, compare}, node value at +0x10.
// Shape: same-TU stand-ins for GetDef<T>, GetSubDef<T> and _M_insert (all only read or write their out pointers), so
//   VC7.1 keeps the modifier in edi with no reload or null test at its release, reuses the armour pointer's slot for the
//   insert's iterator result and releases the converted pair via esi; STLport's `_List_base` ctor stores the sentinel's
//   links before `_M_node._M_data`; the CDefPointer dtor zeroes its object after the release (the converted pair's
//   `mov [esp+24h],ebx`); make_pair + the template pair conversion give the two increments.
// Tried for the last fadd: `total += w`, `total = total + w`, `w + total`, a float& / float* alias, a const float& or a
//   float copy of the weighting, a raw def pointer variable, inline AddWeight/Sum helpers, a by-value make_pair second,
//   an explicit pair temporary, the QFE toolset -- the order never flips (single-term x87 commutation, GOTCHAS lane gap).
extern "C" void* __cdecl malloc(unsigned int size);         // 0x00BFEA0E
extern "C" void __cdecl free(void* p);                      // 0x00BFEA14
inline void* operator new(unsigned int, void* where) { return where; }

class CCharString {
public:
    void* PStringData;                                      // +0x00
    CCharString(const char* text, long length = -1);       // 0x0099EBF0
    ~CCharString();                                         // 0x0099EAE0
};

namespace std {

template <class _Tp> class allocator {
public:
    allocator() { }
    template <class _Tp1> allocator(const allocator<_Tp1>&) { }
    _Tp* allocate(unsigned int __n) { return __n != 0 ? (_Tp*)malloc(__n * sizeof(_Tp)) : 0; }
    void deallocate(_Tp* __p, unsigned int) { free(__p); }
};

template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
    _STLP_alloc_proxy(const _MaybeReboundAlloc& __a, _Value __p) : _MaybeReboundAlloc(__a), _M_data(__p) { }
};

template <class _T1, class _T2>
struct pair {
    typedef _T1 first_type;
    typedef _T2 second_type;
    _T1 first;
    _T2 second;
    pair(const _T1& __a, const _T2& __b) : first(__a), second(__b) { }
    template <class _U1, class _U2>
    pair(const pair<_U1, _U2>& __p) : first(__p.first), second(__p.second) { }
};
template <class _T1, class _T2>
inline pair<_T1, _T2> make_pair(const _T1& __x, const _T2& __y) { return pair<_T1, _T2>(__x, __y); }

template <class _Pair>
struct _Select1st {
    const typename _Pair::first_type& operator()(const _Pair& __x) const { return __x.first; }
};

struct _List_node_base {
    _List_node_base* _M_next;
    _List_node_base* _M_prev;
};
template <class _Tp>
struct _List_node : public _List_node_base {
    _Tp _M_data;
};

template <class _Tp, class _Alloc>
class _List_base {
public:
    typedef _List_node<_Tp> _Node;
    _List_base(const _Alloc& __a) : _M_node(__a, (_Node*)0)
    {
        _Node* __n = _M_node.allocate(1);
        __n->_M_next = __n;
        __n->_M_prev = __n;
        _M_node._M_data = __n;
    }
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
            _M_node.deallocate(__tmp, 1);
        }
        _M_node._M_data->_M_next = _M_node._M_data;
        _M_node._M_data->_M_prev = _M_node._M_data;
    }
    _STLP_alloc_proxy<_Node*, _Node, allocator<_Node> > _M_node;
};

template <class _Tp, class _Alloc = allocator<_Tp> >
class list : public _List_base<_Tp, _Alloc> {
public:
    typedef _List_node<_Tp> _Node;
    struct iterator {
        _List_node_base* _M_node;
        iterator(_List_node_base* __x) : _M_node(__x) { }
        _Tp& operator*() const { return ((_Node*)_M_node)->_M_data; }
        iterator& operator++() { _M_node = _M_node->_M_next; return *this; }
        bool operator!=(const iterator& __y) const { return _M_node != __y._M_node; }
    };
    explicit list(const _Alloc& __a = _Alloc()) : _List_base<_Tp, _Alloc>(__a) { }
    iterator begin() { return iterator(this->_M_node._M_data->_M_next); }
    iterator end() { return iterator(this->_M_node._M_data); }
};

struct _Rb_tree_node_base {
    bool _M_color;                                          // +0x00
    _Rb_tree_node_base* _M_parent;                          // +0x04
    _Rb_tree_node_base* _M_left;                            // +0x08
    _Rb_tree_node_base* _M_right;                           // +0x0c
};
template <class _Value>
struct _Rb_tree_node : public _Rb_tree_node_base {
    _Value _M_value_field;                                  // +0x10
};
template <class _Value>
struct _Rb_tree_iterator {
    _Rb_tree_node_base* _M_node;
    _Rb_tree_iterator(_Rb_tree_node_base* __x) : _M_node(__x) { }
};

template <class _Tp, class _Alloc>
class _Rb_tree_base {
public:
    _STLP_alloc_proxy<_Rb_tree_node_base*, _Rb_tree_node<_Tp>, allocator<_Rb_tree_node<_Tp> > > _M_header;
};

template <class _Key, class _Value, class _KeyOfValue, class _Compare, class _Alloc = allocator<_Value> >
class _Rb_tree : public _Rb_tree_base<_Value, _Alloc> {
public:
    typedef _Rb_tree_node_base* _Base_ptr;
    typedef _Rb_tree_node<_Value>* _Link_type;
    typedef _Rb_tree_iterator<_Value> iterator;
    unsigned int _M_node_count;                             // +0x04
    _Compare _M_key_compare;                                // +0x08

    _Link_type& _M_root() const { return (_Link_type&)this->_M_header._M_data->_M_parent; }
    static _Link_type& _S_left(_Base_ptr __x) { return (_Link_type&)(__x->_M_left); }
    static _Link_type& _S_right(_Base_ptr __x) { return (_Link_type&)(__x->_M_right); }
    static const _Key& _S_key(_Base_ptr __x) { return _KeyOfValue()(((_Link_type)__x)->_M_value_field); }

    iterator _M_insert(_Base_ptr __x, _Base_ptr __y, const _Value& __v, _Base_ptr __w = 0)   // 0x00869CA0
    {
        _Link_type __z = (_Link_type)malloc(sizeof(_Rb_tree_node<_Value>));
        new (&__z->_M_value_field) _Value(__v);
        if (__y == this->_M_header._M_data || __x != 0 || _M_key_compare(_KeyOfValue()(__v), _S_key(__y))) {
            __y->_M_left = __z;
            if (__y == this->_M_header._M_data) {
                this->_M_header._M_data->_M_parent = __z;
                this->_M_header._M_data->_M_right = __z;
            } else if (__y == this->_M_header._M_data->_M_left)
                this->_M_header._M_data->_M_left = __z;
        } else {
            __y->_M_right = __z;
            if (__y == this->_M_header._M_data->_M_right)
                this->_M_header._M_data->_M_right = __z;
        }
        __z->_M_parent = __y;
        __z->_M_left = 0;
        __z->_M_right = 0;
        ++_M_node_count;
        return iterator(__z);
    }
    iterator insert_equal(const _Value& __v)
    {
        _Base_ptr __y = this->_M_header._M_data;
        _Base_ptr __x = _M_root();
        while (__x != 0) {
            __y = __x;
            __x = _M_key_compare(_KeyOfValue()(__v), _S_key(__x)) ? _S_left(__x) : _S_right(__x);
        }
        return _M_insert(__x, __y, __v);
    }
};

template <class _Key, class _Tp, class _Compare, class _Alloc = allocator<pair<const _Key, _Tp> > >
class multimap {
public:
    typedef pair<const _Key, _Tp> value_type;
    typedef _Rb_tree<_Key, value_type, _Select1st<value_type>, _Compare, _Alloc> _Rep_type;
    typedef typename _Rep_type::iterator iterator;
    _Rep_type _M_t;
    iterator insert(const value_type& __x) { return _M_t.insert_equal(__x); }
};

} // namespace std

class CDefClassBase;
static __forceinline bool DecRefToZero(long& count) { return --count == 0; }

template <class T> class CDefPointer {
public:
    T* Object;
    CDefPointer() : Object(0) { }
    CDefPointer(const CDefPointer& other) : Object(other.Object)
    {
        if (Object)
            ++Object->RefCount;
    }
    ~CDefPointer()
    {
        if (Object) {
            if (DecRefToZero(Object->RefCount))
                Object->Destroy();
            Object = 0;
        }
    }
    T* operator->() const { return Object; }
    CDefPointer& operator=(const CDefPointer& other)
    {
        if (Object != other.Object) {
            if (Object) {
                if (--Object->RefCount == 0)
                    Object->Destroy();
                Object = 0;
            }
            Object = other.Object;
            if (Object)
                ++Object->RefCount;
        }
        return *this;
    }
};

template <class T, class V> struct CCompareDefPointer {
    bool operator()(const CDefPointer<T>& a, const CDefPointer<T>& b) const { return a.Object < b.Object; }
};

class CDefinitionManager {
public:
    template <class T> CDefPointer<const T> GetPDefFromGlobalIndex(long index) const;   // 0x009AD9E0
    template <class T> bool GetDef(long index, CDefPointer<const T>* out) const;
};

class CDefClassBase {
public:
    virtual void v00();
    virtual void Destroy() const;                           // slot 1
    virtual void v08(); virtual void v0c(); virtual void v10();
    virtual void v14(); virtual void v18(); virtual void v1c(); virtual void v20(); virtual void v24();
    virtual void v28(); virtual void v2c(); virtual void v30(); virtual void v34();
    virtual const long* GetSubDefIndex(const CCharString& name) const;   // slot 14 (+0x38)
    mutable long RefCount;                                  // +0x04
    unsigned char _pad08[0x1c - 0x08];
    CDefinitionManager* m_pManager;                         // +0x1c
    template <class T> bool GetSubDef(CDefPointer<const T>* out) const;
};
class CThingDef : public CDefClassBase { };
class CArmourDef : public CDefClassBase { };
class CAppearanceModifierDef : public CDefClassBase {
public:
    unsigned char _pad20[0x5c - 0x20];
    long  ArmourDefIndex;                                   // +0x5c
    float OveralProtectionWeighting;                        // +0x60
};

#pragma auto_inline(off)
// Same-TU stand-ins (the GetDef<T> / GetSubDef<T> template bodies): they only write their out pointer.
template <class T> bool CDefinitionManager::GetDef(long index, CDefPointer<const T>* out) const
{
    if (index > 0) {
        CDefPointer<const T> def = GetPDefFromGlobalIndex<T>(index);
        if (def.Object) {
            *out = def;
            return true;
        }
    }
    return false;
}
template <> bool CDefClassBase::GetSubDef<CAppearanceModifierDef>(CDefPointer<const CAppearanceModifierDef>* out) const
{
    const long* index;
    {
        CCharString name("CAppearanceModifierDef");
        index = GetSubDefIndex(name);
    }
    if (index) {
        CDefPointer<const CAppearanceModifierDef> result = m_pManager->GetPDefFromGlobalIndex<CAppearanceModifierDef>(*index);
        *out = result;
        return true;
    }
    return false;
}
#pragma auto_inline(on)

class CTCHeroAttachableAppearanceModifiers {               // interface 0x5E
public:
    void GetAppearanceModifierList(std::list<long>& out, long type) const;   // 0x00706800
};

class CTCBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    const SEntry* LowerBound(const long* type) const;       // 0x004365B0
};

class CThing {
public:
    unsigned char _pad00[0x20];
    unsigned long m_presentBits[9];                         // +0x20
    CInterfaceMap m_interfaces;                             // +0x44
    CDefinitionManager* GetDefinitionManager() const;       // 0x004C7990
    __forceinline bool HasTC(long type) const
    {
        if (m_presentBits[type >> 5] & (1UL << (type & 31)))
            return true;
        return false;
    }
    __forceinline const CTCBase* PeekTC(long type) const
    {
        long key = type;
        const CInterfaceMap* map = &m_interfaces;
        const CInterfaceMap::SEntry* it = map->LowerBound(&key);
        const CInterfaceMap::SEntry* end = map->last;
        return (it == end || type < it->type ? end : it)->component;
    }
};

typedef std::multimap<CDefPointer<const CArmourDef>, float, CCompareDefPointer<const CArmourDef, float> > TArmourWeights;

namespace {

void __fastcall GetArmourWeightsFromThing(const CThing& thing, TArmourWeights& armour_weights, float& total_weight)
{
    const CDefinitionManager* defs = thing.GetDefinitionManager();
    if (thing.HasTC(0x5e)) {
        const CTCHeroAttachableAppearanceModifiers* modifiers =
            (const CTCHeroAttachableAppearanceModifiers*)thing.PeekTC(0x5e);
        std::list<long> worn;
        modifiers->GetAppearanceModifierList(worn, 2);
        for (std::list<long>::iterator it = worn.begin(); it != worn.end(); ++it) {
            CDefPointer<const CThingDef> def;
            defs->GetDef(*it, &def);
            CDefPointer<const CAppearanceModifierDef> modifier;
            def->GetSubDef(&modifier);
            if (modifier->ArmourDefIndex >= 0) {
                CDefPointer<const CArmourDef> armour;
                defs->GetDef(modifier->ArmourDefIndex, &armour);
                armour_weights.insert(std::make_pair(armour, modifier->OveralProtectionWeighting));
                total_weight += modifier->OveralProtectionWeighting;
            }
        }
    }
}

}

void __fastcall Use(const CThing& thing, TArmourWeights& armour_weights, float& total_weight)
{
    GetArmourWeightsFromThing(thing, armour_weights, total_weight);
}
