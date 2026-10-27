// CSkeletalMorphResourceManager::UnloadBoneConfigs() -- empty the member tree (+0x4)
// Leaf: ?UnloadBoneConfigs@CSkeletalMorphResourceManager@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x006C35E0, 52 bytes, thiscall, plain `ret`.
// Identity (proven): its only caller is CWorld::Uninitialise 0x004A2E90 (tail jump with the singleton 0x013BAB10, built by the constructor 0x004AE2A0 that stores vtable 0x01238F84, RTTI .?AVCSkeletalMorphResourceManager@@, and allocates the 0x1C-byte map header); Ego_r.pdb CSkeletalMorphResourceManager has one member, CachedMorphs map<CCharString, CCountedPointer<C3DSkeletalMorphTarget> > (+4), and ?UnloadBoneConfigs@CSkeletalMorphResourceManager@@QAEXXZ is its emptying method (PreloadBoneConfigs / GetMorphTarget fill it).
// Callees: the member tree's _Rb_tree::_M_erase(_Rb_tree_node*) 0x004AD690.
// Shape (family M119, representative 0x0052D9A0): STLport 4.6 map/set `clear()` inline -- `if (_M_node_count != 0) {
//   _M_erase(_M_root()); _M_leftmost() = _M_header; _M_root() = 0; _M_rightmost() = _M_header; _M_node_count = 0; }`.
// Header debt: the owning class is modelled locally (no shared header declares its methods / the container member).
namespace std {
template <class _Tp> class allocator { public: allocator() { } };
template <class _Tp> struct less { };
template <class _T1, class _T2> struct pair { _T1 first; _T2 second; };
template <class _Pair> struct _Select1st { };
template <class _Tp> struct _Identity { };
struct _Rb_tree_node_base {
    typedef _Rb_tree_node_base* _Base_ptr;
    bool _M_color;
    _Base_ptr _M_parent;
    _Base_ptr _M_left;
    _Base_ptr _M_right;
};
template <class _Value> struct _Rb_tree_node : public _Rb_tree_node_base { _Value _M_value_field; };
template <class _Tp, class _Alloc> class _STLP_alloc_proxy : public _Alloc { public: _Tp _M_data; };
template <class _Key, class _Value, class _KeyOfValue, class _Compare, class _Alloc = allocator<_Value> > class _Rb_tree {
public:
    typedef _Rb_tree_node_base* _Base_ptr;
    typedef _Rb_tree_node<_Value>* _Link_type;
    _Base_ptr& _M_root() const { return _M_header._M_data->_M_parent; }
    _Base_ptr& _M_leftmost() const { return _M_header._M_data->_M_left; }
    _Base_ptr& _M_rightmost() const { return _M_header._M_data->_M_right; }
    void _M_erase(_Link_type __x);
    void clear()
    {
        if (_M_node_count != 0) {
            _M_erase((_Link_type)_M_root());
            _M_leftmost() = _M_header._M_data;
            _M_root() = 0;
            _M_rightmost() = _M_header._M_data;
            _M_node_count = 0;
        }
    }
    _STLP_alloc_proxy<_Base_ptr, allocator<_Rb_tree_node<_Value> > > _M_header;
    unsigned int _M_node_count;
    _Compare _M_key_compare;
};
template <class _Key, class _Tp, class _Compare = less<_Key>, class _Alloc = allocator<pair<const _Key, _Tp> > > class map {
public:
    void clear() { _M_t.clear(); }
    _Rb_tree<_Key, pair<const _Key, _Tp>, _Select1st<pair<const _Key, _Tp> >, _Compare, _Alloc> _M_t;
};
template <class _Key, class _Compare = less<_Key>, class _Alloc = allocator<_Key> > class set {
public:
    void clear() { _M_t.clear(); }
    _Rb_tree<_Key, _Key, _Identity<_Key>, _Compare, _Alloc> _M_t;
};
}
class CCharString { public: void* PStringData; };
class C3DSkeletalMorphTarget;
template <class T> class CCountedPointer { public: T* Data; void* Info; };
class CSkeletalMorphResourceManager {                     // Ego_r.pdb; vtable 0x01238F84 (the CTBaseSingleton base)
public:
    virtual ~CSkeletalMorphResourceManager();
    void UnloadBoneConfigs();
    std::map<CCharString, CCountedPointer<C3DSkeletalMorphTarget> > CachedMorphs;   // +0x04
};
void CSkeletalMorphResourceManager::UnloadBoneConfigs()
{
    CachedMorphs.clear();
}