// std::map::operator[](const CCharString& k) -- STLport subscript of map<CCharString, CScriptGameResourceObjectScriptedThingBase> (size TU): lower_bound, insert (k, T()) when absent, return the scripted-thing handle
// Leaf: ??A?$map@VCCharString@@VCScriptGameResourceObjectScriptedThingBase@@U?$less@VCCharString@@@std@@V?$allocator@U?$pair@$$CBVCCharString@@VCScriptGameResourceObjectScriptedThingBase@@@std@@@4@@std@@QAEAAVCScriptGameResourceObjectScriptedThingBase@@ABVCCharString@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00CD3D2E, 118 bytes, __fastcall `ret 4`, frame-pointer size TU.
// Identity: the mapped value is the 16-byte CScriptGameResourceObjectScriptedThingBase (vtable 0x0127094C; ctor 0x007E72A0
//   stores it and zeroes the counted pointer at +8/+0xC, dtor 0x007E74D0 releases it), as landed CWillWhisper::Main
//   0x00D68810 models it; 587 call sites in RunCutsceneMacro_Func (0x00CBFB7D). The manifest's
//   CCountedPointer<NUISystem::CComponent> and the callee labels (CCarriedReadableDef, ~CScriptGameResourceObjectMovieBase,
//   pair<CCharString, CTCBase::CRegisteredTCInfo>) are propagated across byte-identical bodies.
// Callees: _M_lower_bound @ 0x00CD28BC, less<CCharString>::operator() @ 0x0042AAB3 (landed), T() @ 0x007E72A0, pair(const
//   CCharString&, const T&) @ 0x00CD27C7 (CCharString copy ctor + T copy ctor 0x00905330), insert(iterator, const
//   value_type&) @ 0x00CD3CAE, ~T @ 0x007E74D0, ~CCharString @ 0x0099EAE0.
// Shape: the landed 0x005812DF model with this TU's helpers, but the default value is STLport's
//   `__default_constructed((_Tp*)0)` (_STLP_DEFAULT_CONSTRUCTED): the inline by-value return re-addresses the temporary
//   with `lea` after its out-of-line ctor and the insert result gets its own frame word (ebp-4). `T def;` puts the result
//   in the dead `k` slot (118v118, frame 0x24); a bare `T()` pushes the ctor's eax (115v118). Destruction order:
//   pair.second, pair.first, then the default temporary.
// NF350 (2026-11-24, NAME_FIXES 534 / 571 / nf343): the local model is respelled onto the STLport 4.6 template shapes, so the self is the
//   four-argument map<K, T, less<K>, allocator<pair<const K, T> > > spelling the repo's stlport/fable_map.h emits (= table line target
//   0x00CD3D2E, the UND of the landed 0x00D58490), the hinted insert binds the four-argument map::insert of row 0x00CD3CAE, and
//   _M_lower_bound binds the private _Rb_tree member that row 0x00CD28BC defines (was a made-up map member). The iterator takes
//   STLport's _Nonconst_traits parameter. Bytes unchanged: only names move. The shared-header route (fable_map.h with the existing
//   opt-ins) keeps the leaf but also emits the pair ctor, map::insert, the STB copy ctor and two vftables (NF350 probe Q1).
#include "fable_compiler.h"
#pragma optimize("y", off)
#pragma optimize("s", on)

template <class C> class CBasicString {
public:
    bool operator<(const CBasicString& other) const;     // 0x00429950
};

class CCharString
{
public:
    CBasicString<char>* PData;                           // +0x00
    CCharString(const CCharString& other);               // 0x0099EC30
    ~CCharString();                                      // 0x0099EAE0
    FABLE_FORCEINLINE bool operator<(const CCharString& other) const
    {
        if (other.PData == PData)
            return false;
        if (PData == 0)
            return true;
        if (other.PData == 0)
            return false;
        return *PData < *other.PData;
    }
};

#define FABLE_RETAIL_BASE_OBJECT
#include "engine/CBaseObject.h"                       // : CBaseClass, vtable 0x0129A7D8 (~ slot 0, Validate slot 1)

class CScriptGameResourceObjectScriptedThingBase : public CBaseObject {   // vtable 0x0127094C, 0x10 bytes
public:
    void* PImpData;                                      // +0x08 (CCountedPointer Data)
    long* PImpInfo;                                      // +0x0c (CCountedPointer Info)
    CScriptGameResourceObjectScriptedThingBase();        // 0x007E72A0
    CScriptGameResourceObjectScriptedThingBase(const CScriptGameResourceObjectScriptedThingBase& other);   // 0x00905330
    ~CScriptGameResourceObjectScriptedThingBase();       // 0x007E74D0
};

namespace std {
template <class _Tp> inline _Tp __default_constructed(_Tp*) { return _Tp(); }   // _STLP_DEFAULT_CONSTRUCTED

template <class T> struct less {
    __declspec(noinline) bool operator()(const T& x, const T& y) const { return x < y; }   // 0x0042AAB3 for CCharString
};
template <class T> class allocator { };
template <class P> struct _Select1st { };
template <class V> struct _Nonconst_traits { };
template <class T1, class T2> struct pair {
    T1 first;
    T2 second;
    pair(const T1& a, const T2& b);                      // 0x00CD27C7
};
struct _Rb_tree_node_base {
    bool                _M_color;
    _Rb_tree_node_base* _M_parent;
    _Rb_tree_node_base* _M_left;
    _Rb_tree_node_base* _M_right;
};
template <class V> struct _Rb_tree_node : _Rb_tree_node_base {
    V _M_value_field;                                    // +0x10
};
struct _Rb_tree_base_iterator {
    _Rb_tree_node_base* _M_node;
    bool operator==(const _Rb_tree_base_iterator& y) const { return _M_node == y._M_node; }
};
template <class V, class Traits> struct _Rb_tree_iterator : _Rb_tree_base_iterator {
    _Rb_tree_iterator(_Rb_tree_node<V>* x) { _M_node = x; }
    _Rb_tree_iterator(const _Rb_tree_iterator& it) { _M_node = it._M_node; }
    V& operator*() const { return static_cast<_Rb_tree_node<V>*>(_M_node)->_M_value_field; }
};
template <class K, class T, class C, class A> class map;
template <class K, class V, class KoV, class C, class A> class _Rb_tree {
public:
    _Rb_tree_node_base* _M_header;                       // +0x00
    unsigned int        _M_node_count;                   // +0x04
    C                   _M_key_compare;                  // +0x08
private:
    _Rb_tree_node_base* _M_lower_bound(const K& k) const;              // 0x00CD28BC (STLport 4.6 _tree.h: private)
    template <class K2, class T2, class C2, class A2> friend class map;
};
template <class K, class T, class C = less<K>, class A = allocator<pair<const K, T> > > class map {
public:
    typedef pair<const K, T> value_type;
    typedef _Rb_tree<K, value_type, _Select1st<value_type>, C, A> _Rep_type;
    typedef _Rb_tree_iterator<value_type, _Nonconst_traits<value_type> > iterator;
    _Rep_type           _M_t;                            // +0x00
    iterator insert(iterator position, const value_type& v);          // 0x00CD3CAE
    iterator lower_bound(const K& k) { return iterator(static_cast<_Rb_tree_node<value_type>*>(_M_t._M_lower_bound(k))); }
    iterator end() { return iterator(static_cast<_Rb_tree_node<value_type>*>(_M_t._M_header)); }
    C key_comp() const { return _M_t._M_key_compare; }
    T& operator[](const K& k)
    {
        iterator i = lower_bound(k);
        if (i == end() || key_comp()(k, (*i).first))
            i = insert(i, value_type(k, __default_constructed((T*)0)));
        return (*i).second;
    }
};
template CScriptGameResourceObjectScriptedThingBase& map<CCharString, CScriptGameResourceObjectScriptedThingBase>::operator[](const CCharString&);
}