// std::_Rb_tree::_M_create_node(const value_type& __x) -- (map<EDamageAttribute, CHeroStatIncreaseData>) allocate a node and copy-construct the pair into it
// Leaf: ?_M_create_node@?$_Rb_tree@W4EDamageAttribute@@U?$pair@$$CBW4EDamageAttribute@@VCHeroStatIncreaseData@@@std@@U?$_Select1st@U?$pair@$$CBW4EDamageAttribute@@VCHeroStatIncreaseData@@@std@@@3@U?$less@W4EDamageAttribute@@@3@V?$allocator@U?$pair@$$CBW4EDamageAttribute@@VCHeroStatIncreaseData@@@std@@@3@@std@@QAEPAU?$_Rb_tree_node@U?$pair@$$CBW4EDamageAttribute@@VCHeroStatIncreaseData@@@std@@@2@ABU?$pair@$$CBW4EDamageAttribute@@VCHeroStatIncreaseData@@@2@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x004F9934, 44 bytes, thiscall (this unused), `ret 4` (size-optimised module).
// Identity (proven): the value copy is the map<EHeroExperienceStatCategory, float> copy constructor 0x004F9960 (bound), i.e. the implicit copy of Ego_r.pdb CHeroStatIncreaseData { StatIncreases }; node 0x20 = 0x10 + key + 0xC; the tree copy 0x004F98A4 is reached from CHeroExperienceDef::operator= 0x004F914A: Ego_r.pdb StatIncreasePerHitType is map<EDamageAttribute, CHeroStatIncreaseData>.
// Callees: malloc 0x00BFEA0E (node allocator), CHeroStatIncreaseData copy constructor 0x004F9960.
// Shape (family M049, representative 0x00466255): STLport `__tmp = _M_header.allocate(1); _Construct(&__tmp->_M_value_field,
//   __x); return __tmp;` -- allocate/_Construct `__forceinline`; STLport's explicit pair copy constructor copies the key
//   dword, then calls V's copy constructor at +4.
#pragma optimize("s", on)
extern "C" void* __cdecl malloc(unsigned int);
inline void* __cdecl operator new(unsigned int, void* p) { return p; }
namespace std {
template <class _Tp> class allocator { public: allocator() { } __forceinline _Tp* allocate(unsigned int __n) { return (_Tp*)malloc(__n * sizeof(_Tp)); } };
template <class _Tp> struct less { };
template <class _T1, class _T2> struct pair {
    __forceinline pair(const pair<_T1, _T2>& __o) : first(__o.first), second(__o.second) { }
    _T1 first;
    _T2 second;
};
}
enum EDamageAttribute { DAMAGE_ATTRIBUTE_NONE = 0 };
enum EHeroExperienceStatCategory { HERO_EXPERIENCE_STAT_CATEGORY_NONE = 0 };
namespace std {
template <class _Key, class _Tp, class _Compare, class _Alloc> class map {
public:
    map(const map& __x);                                  // 0x004F9960
    void* _M_header;
    unsigned int _M_node_count;
    _Compare _M_key_compare;
};
}
class CHeroStatIncreaseData {                            // Ego_r.pdb: { map<EHeroExperienceStatCategory, float> StatIncreases; }
public:
    std::map<EHeroExperienceStatCategory, float, std::less<EHeroExperienceStatCategory>, std::allocator<std::pair<const EHeroExperienceStatCategory, float> > > StatIncreases;
};
namespace std {
template <class _Pair> struct _Select1st { };
template <class _T1, class _T2> __forceinline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }
struct _Rb_tree_node_base {
    bool _M_color;
    _Rb_tree_node_base* _M_parent;
    _Rb_tree_node_base* _M_left;
    _Rb_tree_node_base* _M_right;
};
template <class _Value> struct _Rb_tree_node : public _Rb_tree_node_base { _Value _M_value_field; };
template <class _Tp, class _Alloc> class _STLP_alloc_proxy : public _Alloc { public: _Tp _M_data; };
template <class _Key, class _Value, class _KeyOfValue, class _Compare, class _Alloc = allocator<_Value> > class _Rb_tree {
public:
    _Rb_tree_node<_Value>* _M_create_node(const _Value& __x)
    {
        _Rb_tree_node<_Value>* __tmp = _M_header.allocate(1);
        _Construct(&__tmp->_M_value_field, __x);
        return __tmp;
    }
    _STLP_alloc_proxy<_Rb_tree_node<_Value>*, allocator<_Rb_tree_node<_Value> > > _M_header;
    unsigned int _M_node_count;
    _Compare _M_key_compare;
};
template class _Rb_tree<EDamageAttribute, pair<const EDamageAttribute, CHeroStatIncreaseData>, _Select1st<pair<const EDamageAttribute, CHeroStatIncreaseData> >, less<EDamageAttribute>, allocator<pair<const EDamageAttribute, CHeroStatIncreaseData> > >;
}