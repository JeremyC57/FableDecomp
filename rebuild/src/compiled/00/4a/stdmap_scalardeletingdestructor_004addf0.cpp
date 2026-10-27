// std::map::`scalar deleting destructor'(unsigned int flags) -- (std::map<void*, CBulletTimeManager::CBulletTimeInfo>) destroy the tree (clear, free the header) and free the object when bit 0 is set
// Leaf: ??_G?$map@PAXVCBulletTimeInfo@CBulletTimeManager@@U?$less@PAX@std@@V?$allocator@U?$pair@QAXVCBulletTimeInfo@CBulletTimeManager@@@std@@@4@@std@@QAEPAXI@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x004ADDF0, 86 bytes, thiscall, `ret 4`.
// Identity (descriptive): unreferenced; it calls 0x004AA650, the _M_erase of map<void*, CBulletTimeManager::CBulletTimeInfo> (the landed boost::checked_delete<CBulletTimeManager> 0x004AE080 and CBulletTimeManager::PrepareForLoad 0x004C6080 clear BulletTimeInfoMap with it); whether the deleted class is that map or CBulletTimeManager itself (map at +0, the rest trivial) is not decidable from the bytes.
// Callees: _Rb_tree::_M_erase 0x004AA650 (out of line), free 0x00BFEA14 (the header node), operator delete 0x00BFE9BC.
// Shape (family M080, representative 0x004ADDF0): the compiler-generated non-virtual `??_G` -- STLport 4.6 ~_Rb_tree =
//   clear() (`if (_M_node_count != 0) { _M_erase(_M_root()); leftmost = header; root = 0; rightmost = header; count = 0; }`)
//   then ~_Rb_tree_base's `_M_header.deallocate(header, 1)` (`if (p) free(p)`); the delete-expression in DeleteOne forces
//   it into the object (not part of the retail body).
extern "C" void __cdecl free(void* block);                 // 0x00BFEA14
void __cdecl operator delete(void* p);                      // 0x00BFE9BC
class CBulletTimeManager {
public:
    class CBulletTimeInfo {                               // Ego_r.pdb, 8 bytes
    public:
        float Multiplier;
        float SoundPitch;
    };
};
namespace std {
template <class _Tp> struct less { bool operator()(const _Tp& __x, const _Tp& __y) const { return __x < __y; } };
template <class _Tp> struct _Identity { const _Tp& operator()(const _Tp& __x) const { return __x; } };
template <class _T1, class _T2> struct pair { typedef _T1 first_type; _T1 first; _T2 second; };
template <class _Pair> struct _Select1st { const typename _Pair::first_type& operator()(const _Pair& __x) const { return __x.first; } };
template <class _Tp> class allocator {
public:
    allocator() { }
    void deallocate(_Tp* __p, unsigned int) { if (__p != 0) free(__p); }
};
template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc { public: _Value _M_data; };
struct _Rb_tree_node_base {
    bool _M_color;                                        // +0x00
    _Rb_tree_node_base* _M_parent;                        // +0x04
    _Rb_tree_node_base* _M_left;                          // +0x08
    _Rb_tree_node_base* _M_right;                         // +0x0C
};
template <class _Value> struct _Rb_tree_node : public _Rb_tree_node_base { _Value _M_value_field; };
template <class _Tp, class _Alloc> struct _Rb_tree_base {
    typedef _Rb_tree_node<_Tp> _Node;
    _STLP_alloc_proxy<_Node*, _Node, allocator<_Node> > _M_header;   // +0x00
    ~_Rb_tree_base() { _M_header.deallocate(_M_header._M_data, 1); }
};
template <class _Key, class _Value, class _KeyOfValue, class _Compare, class _Alloc>
class _Rb_tree : public _Rb_tree_base<_Value, _Alloc> {
public:
    typedef _Rb_tree_node<_Value>* _Link_type;
    unsigned int _M_node_count;                           // +0x04
    _Compare _M_key_compare;                              // +0x08
    _Rb_tree_node_base*& _M_root() const { return this->_M_header._M_data->_M_parent; }
    _Rb_tree_node_base*& _M_leftmost() const { return this->_M_header._M_data->_M_left; }
    _Rb_tree_node_base*& _M_rightmost() const { return this->_M_header._M_data->_M_right; }
    void _M_erase(_Link_type __x);                        // 0x004AA650
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
    ~_Rb_tree() { clear(); }
};
template <class _Key, class _Tp, class _Compare = less<_Key>, class _Alloc = allocator<pair<const _Key, _Tp> > >
class map {
public:
    typedef pair<const _Key, _Tp> value_type;
    _Rb_tree<_Key, value_type, _Select1st<value_type>, _Compare, _Alloc> _M_t;
};
}
void DeleteOne(std::map<void*, CBulletTimeManager::CBulletTimeInfo>* object)   // forces the scalar deleting destructor into the object; not part of the retail body
{
    delete object;
}