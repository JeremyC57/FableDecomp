// CTCFlammable::InitialiseFirePoints() -- rebuild the fire-point list: one NFire::CFirePoint per helper point of the thing's graphic appearance that the flammable def names as a fire helper (numbered per helper name), else the def's first fire helper on the appearance's first helper point
// Grade: DIFFER(1027v923) -- retail inlines the 5-arg CFirePoint ctor at the loop site but calls 0x00781200 at the fallback site (VC here inlines both, +100 bytes; ctor placement/out-of-class/temporary forms did not change it, probes 9-11); the first loop matches through GetString/copy/dtor except the T3/T4 temporary slot order (retail T3@0x20/0x24, T4@0x28/0x2c, result@0x30); zero register ebp (retail) vs ebx and the fireHelpers iterator ebx vs ebp; the found node is spilled here. Next step: a same-TU noinline stand-in for the 5-arg ctor is NOT it (the loop site is inlined in retail): try the fallback site through a different ctor overload / push_back(CFirePoint) by value, and the map insert through pair<const CCharString,long> (the retail T3/T4 order).
// Leaf: ?InitialiseFirePoints@CTCFlammable@@QAEXXZ
// Identity (proven): Ego_r.pdb ?InitialiseFirePoints@CTCFlammable@@QAEXXZ; the landed CTCFlammable::OnInitialActivate 0x00780D20 and
// ExtinguishFire 0x00780C80 call this address as InitialiseFirePoints(); `this` is CTCFlammable (FirePoints +0x10, HasFirePoint +0x31,
// PDef +0x0c, ParentThing +0x04 as the landed siblings). The manifest label "CTCAvoidRegion_Initialize" is a stand-in.
// Callees: STLport __copy 0x00781720 / __destroy_aux 0x007817B0 (FirePoints.clear()), CFlammableDef::GetAllFireHelpers 0x00781B90
// (PDB ?GetAllFireHelpers@CFlammableDef@@QBEXAAV?$vector@VCDefString@@...@std@@@Z; landed as the invented TreeOwner::BuildVectorFromTree),
// malloc 0x00BFEA0E, CDefStringTable::GetString(CDefString) const 0x009D49B0 on the table singleton 0x013CA828, CCharString copy ctor
// 0x0099EC30 / dtor 0x0099EAE0, map<CCharString, long>::insert_unique 0x00431606, CTCGraphicAppearance::GetAllHelperPointNames
// 0x004BDA30, _Rb_tree::_M_find 0x0042B467, CFlammableDef::GetEffectAtHelperPoint 0x007817F0 (PDB ?GetEffectAtHelperPoint@CFlammableDef@@
// QBE_NABVCCharString@@AAJ@Z; landed as CNamedValueTreeOwner::FindAttributeByName), CBaseIntelligentPointer ctor/dtor 0x00A01B10/0x00A01C10,
// CFirePoint copy ctor 0x007813D0, vector<CFirePoint>::_M_insert_overflow 0x00781A80, the def's map<CDefString, long>::find 0x00781670,
// CFirePoint ctor 0x00781200, _Rb_tree::_M_erase 0x0043020A, free 0x00BFEA14.
// Models: CFirePoint / CIntelligentPointer as the landed CFirePoint ctors; STLport 4.6 vector/_Rb_tree text (speed TU: the tree's
// _M_find / insert_unique / _M_erase live in the 0x0042xxxx size TU and are called).
extern "C" void* __cdecl malloc(unsigned int size);                      // 0x00BFEA0E
extern "C" void __cdecl free(void* block);                               // 0x00BFEA14
inline void* __cdecl operator new(unsigned int, void* where) { return where; }
inline void __cdecl operator delete(void*, void*) { }
class CThing;
class CBaseObject;
class CCharString { public:
    void* PStringData;
    CCharString(const CCharString& other);                               // 0x0099EC30
    ~CCharString();                                                      // 0x0099EAE0
};
class CDefStringTable;
extern CDefStringTable g_DefStringTable;                                 // 0x013CA828 (the def-string table singleton)
class CDefString { public:
    long TablePos;                                                       // Ego_r.pdb: 4 bytes
    operator CCharString() const;
};
class CDefStringTable { public:
    CCharString GetString(long tablePos) const;                          // 0x009D49B0 (PDB ?GetString@CDefStringTable@@QBE?AVCCharString@@J@Z)
};
inline CDefString::operator CCharString() const { return g_DefStringTable.GetString(TablePos); }
class CBaseObjectPointer;
class CBaseIntelligentPointer {                                          // vtable 0x0129C95C
public:
    CBaseIntelligentPointer();                                           // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                                  // 0x00A01C10
    CBaseObject* GetPItem() const;                                       // 0x00A01B50
    void SetPItem(CBaseObject* item);                                    // 0x00A01B90
private:
    CBaseObjectPointer* PData;                                           // +0x04
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {   // vtable 0x01238C6C
public:
    CIntelligentPointer(T* item = 0) : CBaseIntelligentPointer() { if (item) SetPItem(item); }
    CIntelligentPointer(const CIntelligentPointer& other) : CBaseIntelligentPointer() { SetPItem(other.GetPItem()); }
    T* GetPItem() const { return (T*)CBaseIntelligentPointer::GetPItem(); }
    void SetPItem(T* item) { CBaseIntelligentPointer::SetPItem((CBaseObject*)item); }
};
namespace NFire {
class CFirePoint { public:                                               // Ego_r.pdb, 44 bytes
    CCharString                 HelperName;                              // +0x00
    long                        HelperIndex;                             // +0x04
    long                        EffectIndex;                             // +0x08
    long                        Resistance;                              // +0x0c
    bool                        Burning;                                 // +0x10
    long                        FireDamageAfterFrame;                    // +0x14
    CThing*                     PParentThing;                            // +0x18
    CIntelligentPointer<CThing> PEffect;                                 // +0x1c
    CIntelligentPointer<CThing> PSingeEffect;                            // +0x24
    CFirePoint(CThing* parentThing, const CCharString& helperName, long helperIndex, long effectIndex, long resistance)   // 0x00781200 (out-of-line copy)
        : HelperName(helperName), HelperIndex(helperIndex), EffectIndex(effectIndex), Resistance(resistance), Burning(false), FireDamageAfterFrame(0), PParentThing(parentThing) { }
    CFirePoint(const CFirePoint& other);                                 // 0x007813D0
    ~CFirePoint() { }
};
}
namespace std {
struct __true_type { __true_type() { } };
struct __false_type { __false_type() { } };
struct random_access_iterator_tag { random_access_iterator_tag() { } };
template <class _Tp> class allocator { public:
    allocator() { }
    _Tp* allocate(unsigned int __n) { return __n != 0 ? (_Tp*)malloc(__n * sizeof(_Tp)) : 0; }
    void deallocate(_Tp* __p, unsigned int) { if (__p != 0) free(__p); }
};
template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc { public:
    _STLP_alloc_proxy(const _MaybeReboundAlloc& __a, _Value __p) : _MaybeReboundAlloc(__a), _M_data(__p) { }
    _Value _M_data;
};
template <class _T1, class _T2> struct pair {
    typedef _T1 first_type;
    typedef _T2 second_type;
    _T1 first;
    _T2 second;
    pair() : first(_T1()), second(_T2()) { }
    pair(const _T1& __a, const _T2& __b) : first(__a), second(__b) { }
    template <class _U1, class _U2> pair(const pair<_U1, _U2>& __p) : first(__p.first), second(__p.second) { }
};
template <class _T1, class _T2> inline pair<_T1, _T2> make_pair(const _T1& __x, const _T2& __y) { return pair<_T1, _T2>(__x, __y); }
template <class _Tp> struct less { less() { } bool operator()(const _Tp& __x, const _Tp& __y) const; };
template <class _Pair> struct _Select1st { const typename _Pair::first_type& operator()(const _Pair& __x) const { return __x.first; } };
template <class _T1, class _T2> inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }
template <class _Tp> inline void _Destroy(_Tp* __pointer) { __pointer->~_Tp(); }
template <class _ForwardIterator>
void __fastcall __destroy_aux(_ForwardIterator __first, _ForwardIterator __last, const __false_type&);   // 0x007817B0
template <class _ForwardIterator> inline void _Destroy(_ForwardIterator __first, _ForwardIterator __last) { __destroy_aux(__first, __last, __false_type()); }
template <class _ForwardIterator> inline void _Destroy_trivial(_ForwardIterator, _ForwardIterator) { }
template <class _RandomAccessIter, class _OutputIter, class _Distance>
_OutputIter __fastcall __copy(_RandomAccessIter __first, _RandomAccessIter __last, _OutputIter __result, const random_access_iterator_tag&, _Distance*);   // 0x00781720
template <class _Tp, class _Alloc> class _Vector_base { public:
    _Vector_base(const _Alloc& __a) : _M_start(0), _M_finish(0), _M_end_of_storage(__a, 0) { }
    ~_Vector_base() { if (_M_start != 0) _M_end_of_storage.deallocate(_M_start, _M_end_of_storage._M_data - _M_start); }
    _Tp* _M_start;
    _Tp* _M_finish;
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;
};
template <class _Tp, class _Alloc = allocator<_Tp> > class vector : public _Vector_base<_Tp, _Alloc> { public:
    typedef _Tp* iterator;
    typedef const _Tp* const_iterator;
    vector() : _Vector_base<_Tp, _Alloc>(_Alloc()) { }
    iterator begin() { return this->_M_start; }
    iterator end() { return this->_M_finish; }
    bool empty() const { return this->_M_start == this->_M_finish; }
    void _M_insert_overflow(_Tp* __position, const _Tp& __x, const __false_type&, unsigned int __fill_len, bool __atend);   // 0x00781A80
    void push_back(const _Tp& __x)
    {
        if (this->_M_finish != this->_M_end_of_storage._M_data) {
            _Construct(this->_M_finish, __x);
            ++this->_M_finish;
        } else
            _M_insert_overflow(this->_M_finish, __x, __false_type(), 1UL, true);
    }
    iterator erase(iterator __first, iterator __last)
    {
        iterator __i = __copy(__last, this->_M_finish, __first, random_access_iterator_tag(), (int*)0);
        _Destroy(__i, this->_M_finish);
        this->_M_finish = __i;
        return __first;
    }
    void clear() { erase(begin(), end()); }
};
template <class _Tp, class _Alloc> class trivial_vector : public _Vector_base<_Tp, _Alloc> { public:   // vector<T> of a trivially destructible T
    typedef _Tp* iterator;
    trivial_vector() : _Vector_base<_Tp, _Alloc>(_Alloc()) { }
    ~trivial_vector() { }
    iterator begin() { return this->_M_start; }
    iterator end() { return this->_M_finish; }
    bool empty() const { return this->_M_start == this->_M_finish; }
};
template <class _Tp, class _Alloc> class string_vector : public _Vector_base<_Tp, _Alloc> { public:   // vector<CCharString>
    typedef _Tp* iterator;
    string_vector() : _Vector_base<_Tp, _Alloc>(_Alloc()) { }
    ~string_vector() { for (_Tp* __p = this->_M_start; __p != this->_M_finish; ++__p) _Destroy(__p); }
    iterator begin() { return this->_M_start; }
    iterator end() { return this->_M_finish; }
    bool empty() const { return this->_M_start == this->_M_finish; }
};
typedef bool _Rb_tree_Color_type;
const _Rb_tree_Color_type _S_rb_tree_red = false;
struct _Rb_tree_node_base {
    _Rb_tree_Color_type _M_color;
    _Rb_tree_node_base* _M_parent;
    _Rb_tree_node_base* _M_left;
    _Rb_tree_node_base* _M_right;
};
template <class _Value> struct _Rb_tree_node : public _Rb_tree_node_base { _Value _M_value_field; };
struct _Rb_tree_base_iterator { _Rb_tree_node_base* _M_node; };
template <class _Value, class _Traits> struct _Rb_tree_iterator : public _Rb_tree_base_iterator {
    _Rb_tree_iterator() { _M_node = 0; }
    _Rb_tree_iterator(_Rb_tree_node<_Value>* __x) { _M_node = __x; }
    _Value& operator*() const { return ((_Rb_tree_node<_Value>*)_M_node)->_M_value_field; }
    _Value* operator->() const { return &(operator*()); }
    bool operator==(const _Rb_tree_iterator& __y) const { return _M_node == __y._M_node; }
    bool operator!=(const _Rb_tree_iterator& __y) const { return _M_node != __y._M_node; }
};
template <class _Tp, class _Alloc> struct _Rb_tree_base {
    typedef _Rb_tree_node<_Tp> _Node;
    _Rb_tree_base(const _Alloc&) : _M_header(allocator<_Node>(), 0) { _M_header._M_data = _M_header.allocate(1); }
    ~_Rb_tree_base() { _M_header.deallocate(_M_header._M_data, 1); }
    _STLP_alloc_proxy<_Node*, _Node, allocator<_Node> > _M_header;
};
template <class _Key, class _Value, class _KeyOfValue, class _Compare, class _Alloc = allocator<_Value> >
class _Rb_tree : public _Rb_tree_base<_Value, _Alloc> {
public:
    typedef _Rb_tree_node<_Value>* _Link_type;
    typedef _Rb_tree_iterator<_Value, int> iterator;
    unsigned int _M_node_count;
    _Compare _M_key_compare;
    _Link_type& _M_root() const { return (_Link_type&)this->_M_header._M_data->_M_parent; }
    _Link_type& _M_leftmost() const { return (_Link_type&)this->_M_header._M_data->_M_left; }
    _Link_type& _M_rightmost() const { return (_Link_type&)this->_M_header._M_data->_M_right; }
    static _Rb_tree_Color_type& _S_color(_Link_type __x) { return (_Rb_tree_Color_type&)(__x->_M_color); }
    void _M_empty_initialize()
    {
        _S_color(this->_M_header._M_data) = _S_rb_tree_red;
        _M_root() = 0;
        _M_leftmost() = this->_M_header._M_data;
        _M_rightmost() = this->_M_header._M_data;
    }
    _Rb_tree() : _Rb_tree_base<_Value, _Alloc>(_Alloc()), _M_node_count(0), _M_key_compare() { _M_empty_initialize(); }
    ~_Rb_tree() { clear(); }
    _Rb_tree_node<_Value>* _M_find(const _Key& __k) const;                           // 0x0042B467 (map<CCharString, long>: the size TU)
    pair<iterator, bool> insert_unique(const _Value& __x);                            // 0x00431606
    void _M_erase(_Link_type __x);                                                    // 0x0043020A
    iterator begin() { return iterator(_M_leftmost()); }
    iterator end() { return iterator(this->_M_header._M_data); }
    iterator find(const _Key& __k);                                                   // 0x00781670 (map<CDefString, long>: this TU, out of line)
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
};
template <class _Key, class _Tp, class _Compare = less<_Key>, class _Alloc = allocator<pair<const _Key, _Tp> > >
class map { public:
    typedef pair<const _Key, _Tp> value_type;
    typedef _Rb_tree<_Key, value_type, _Select1st<value_type>, _Compare, _Alloc> _Rep_type;
    typedef typename _Rep_type::iterator iterator;
    _Rep_type _M_t;
    map() : _M_t() { }
    iterator begin() { return _M_t.begin(); }
    iterator end() { return _M_t.end(); }
    iterator find(const _Key& __x) { return _M_t.find(__x); }
    pair<iterator, bool> insert(const value_type& __x) { return _M_t.insert_unique(__x); }
};
}
class CFlammableDef { public:
    unsigned char _pad00[0x28];
    long FireResistance;                                                 // +0x28
    unsigned char _pad2c[0x14];
    std::_Rb_tree<CDefString, std::pair<const CDefString, long>, std::_Select1st<std::pair<const CDefString, long> >, std::less<CDefString> > FireHelperEffects;   // +0x40 map<CDefString, long>
    void GetAllFireHelpers(std::trivial_vector<CDefString, std::allocator<CDefString> >& helpers) const;   // 0x00781B90
    bool GetEffectAtHelperPoint(const CCharString& helperName, long& effectIndex) const;                   // 0x007817F0
};
namespace NGraphicAppearance {
class CTCGraphicAppearance { public:
    void GetAllHelperPointNames(std::string_vector<CCharString, std::allocator<CCharString> >& names) const;   // 0x004BDA30
};
}
class CThing { public:
    unsigned char _pad00[0x64];
    NGraphicAppearance::CTCGraphicAppearance* PTCGraphicAppearance;      // +0x64
};
class CTCBase { public:
    virtual void v00();
    CThing* ParentThing;                                                 // +0x04
};
class CTCFlammable : public CTCBase { public:
    bool           ToBeRemoved;                                          // +0x08
    bool           IsInGlobalUpdateTCs;                                  // +0x09
    CFlammableDef* PDef;                                                 // +0x0c CDefPointer<const CFlammableDef>
    std::vector<NFire::CFirePoint> FirePoints;                           // +0x10
    bool           Burning;                                              // +0x1c
    long           FireDamageAfterFrame;                                 // +0x20
    long           FireResistance;                                       // +0x24
    long           FireStrength;                                         // +0x28
    float          DamageCaused;                                         // +0x2c
    bool           DamagePerPoint;                                       // +0x30
    bool           HasFirePoint;                                         // +0x31
    void InitialiseFirePoints();
};

void CTCFlammable::InitialiseFirePoints()
{
    FirePoints.clear();
    std::trivial_vector<CDefString, std::allocator<CDefString> > fireHelpers;
    PDef->GetAllFireHelpers(fireHelpers);
    std::map<CCharString, long> helperCounts;
    for (CDefString* helper = fireHelpers.begin(); helper != fireHelpers.end(); ++helper) {
        CCharString name = *helper;
        helperCounts.insert(std::pair<CCharString, long>(name, 0L));
    }
    std::string_vector<CCharString, std::allocator<CCharString> > helperPointNames;
    ParentThing->PTCGraphicAppearance->GetAllHelperPointNames(helperPointNames);
    HasFirePoint = false;
    for (CCharString* helperPoint = helperPointNames.begin(); helperPoint != helperPointNames.end(); ++helperPoint) {
        std::map<CCharString, long>::iterator found(helperCounts._M_t._M_find(*helperPoint));
        if (found != helperCounts.end()) {
            long effectIndex;
            if (PDef->GetEffectAtHelperPoint(*helperPoint, effectIndex)) {
                long helperIndex = found->second++;
                long resistance = PDef->FireResistance;
                NFire::CFirePoint firePoint(ParentThing, *helperPoint, helperIndex, effectIndex, resistance);
                FirePoints.push_back(firePoint);
                HasFirePoint = true;
            }
        }
    }
    if (!HasFirePoint && !helperPointNames.empty() && !fireHelpers.empty()) {
        CDefString firstHelper = fireHelpers.begin()[0];
        std::_Rb_tree<CDefString, std::pair<const CDefString, long>, std::_Select1st<std::pair<const CDefString, long> >, std::less<CDefString> >::iterator found = PDef->FireHelperEffects.find(firstHelper);
        if (found != PDef->FireHelperEffects.end()) {
            NFire::CFirePoint firePoint(ParentThing, helperPointNames.begin()[0], 0, found->second, PDef->FireResistance);
            FirePoints.push_back(firePoint);
            HasFirePoint = true;
        }
    }
}
