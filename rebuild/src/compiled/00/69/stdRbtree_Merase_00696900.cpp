// std::_Rb_tree::_M_erase(_Rb_tree_node<value_type>* x) -- (std::map<CCharString, CCountedPointer<NCreatureAction::CActionEventBase> >) STLport 4.6 recursive subtree release: erase the right subtree, release the value (counted pointer, then key), free the node, continue left
// Leaf: ?_M_erase@?$_Rb_tree@VCCharString@@U?$pair@$$CBVCCharString@@V?$CCountedPointer@VCActionEventBase@NCreatureAction@@@@@std@@U?$_Select1st@U?$pair@$$CBVCCharString@@V?$CCountedPointer@VCActionEventBase@NCreatureAction@@@@@std@@@3@U?$less@VCCharString@@@3@V?$allocator@U?$pair@$$CBVCCharString@@V?$CCountedPointer@VCActionEventBase@NCreatureAction@@@@@std@@@3@@std@@QAEXPAU?$_Rb_tree_node@U?$pair@$$CBVCCharString@@V?$CCountedPointer@VCActionEventBase@NCreatureAction@@@@@std@@@2@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00696900, 108 bytes, thiscall `ret 4`; recursive; speed-optimised TU.
// Identity (proven): the atexit destructor 0x01229280 of the static tree at 0x013BA8BC calls it; that static is read by RegisterActionEventTypes 0x006932B0 and CCreatureActionBase::InitialiseActionEvents 0x006946C0 -- Ego_r.pdb CCreatureActionBase::RegisteredActionEvents (static, dynamic initializer + atexit destructor), map<CCharString, CCountedPointer<NCreatureAction::CActionEventBase> >.
// Callees: itself (right subtree), the count block's deleter (`call [info+4]`) and operator delete 0x00BFE9BC (the inline
//   ~CCountedPointer), CCharString::~CCharString 0x0099EAE0 (the key), free 0x00BFEA14 (allocator<_Node>::deallocate).
// Shape (family M078, representative 0x004AD690): the landed STLport 4.6 `_tree.c` _M_erase model (0x00AD5240) with value
//   pair<const CCharString, CCountedPointer<T> > -- the pair's implicit destructor releases +0x14/+0x18 then the key at +0x10.
extern "C" void __cdecl free(void* block);             // 0x00BFEA14
void operator delete(void* block);                      // 0x00BFE9BC
class CCharString {                                     // 4 bytes (PStringData)
public:
    ~CCharString();                                     // 0x0099EAE0
    bool operator<(const CCharString& other) const;
    void* PStringData;
};
class CCPPointerInfo { public:                          // the shared count block
    long RefCount;                                      // +0x0
    void (__fastcall *DeleteFunc)(void*);               // +0x4
    void* Data;                                         // +0x8
};
template <class T> class CCountedPointer {             // 8 bytes: object, shared count block
public:
    T* Data;                                            // +0x0
    CCPPointerInfo* Info;                               // +0x4
    ~CCountedPointer()
    {
        if (Info) {
            --Info->RefCount;
            if (Info->RefCount == 0) {
                Info->DeleteFunc(Info->Data);
                operator delete(Info);
            }
        }
        Data = 0;
        Info = 0;
    }
};
namespace NCreatureAction { class CActionEventBase; }
namespace std {

template <class _Tp> class allocator {
public:
    void deallocate(_Tp* __p, unsigned int) { free(__p); }
};

template <class _T1, class _T2> struct pair {
    typedef _T1 first_type;
    typedef _T2 second_type;
    _T1 first;
    _T2 second;
};

template <class _Tp> struct less {
    bool operator()(const _Tp& __x, const _Tp& __y) const { return __x < __y; }
};
template <class _Tp> struct _Identity {
    const _Tp& operator()(const _Tp& __x) const { return __x; }
};
template <class _Pair> struct _Select1st {
    const typename _Pair::first_type& operator()(const _Pair& __x) const { return __x.first; }
};

template <class _Tp> inline void _Destroy(_Tp* __pointer) { __pointer->~_Tp(); }

template <class _Tp, class _Alloc> class _STLP_alloc_proxy : public _Alloc {
public:
    _Tp _M_data;
};

struct _Rb_tree_node_base {
    typedef bool _Color_type;
    typedef _Rb_tree_node_base* _Base_ptr;
    _Color_type _M_color;                               // +0x00
    _Base_ptr   _M_parent;                              // +0x04
    _Base_ptr   _M_left;                                // +0x08
    _Base_ptr   _M_right;                               // +0x0C
};

template <class _Value> struct _Rb_tree_node : public _Rb_tree_node_base {
    _Value _M_value_field;                              // +0x10
};

template <class _Key, class _Value, class _KeyOfValue, class _Compare, class _Alloc>
class _Rb_tree {
public:
    typedef _Rb_tree_node_base* _Base_ptr;
    typedef _Rb_tree_node<_Value> _Node;
    typedef _Node* _Link_type;
    typedef _Value value_type;

    _STLP_alloc_proxy<_Link_type, allocator<_Node> > _M_header;   // +0x00
    unsigned int _M_node_count;                                    // +0x04
    _Compare     _M_key_compare;                                   // +0x08

    static _Link_type& _S_left(_Base_ptr __x) { return (_Link_type&)(__x->_M_left); }
    static _Link_type& _S_right(_Base_ptr __x) { return (_Link_type&)(__x->_M_right); }
    void _M_erase(_Link_type __x);
};

template <class _Key, class _Value, class _KeyOfValue, class _Compare, class _Alloc>
void _Rb_tree<_Key, _Value, _KeyOfValue, _Compare, _Alloc>::_M_erase(_Link_type __x)
{
    // erase without rebalancing
    while (__x != 0) {
        _M_erase(_S_right(__x));
        _Link_type __y = _S_left(__x);
        _Destroy(&__x->_M_value_field);
        this->_M_header.deallocate(__x, 1);
        __x = __y;
    }
}
typedef pair<const CCharString, CCountedPointer<NCreatureAction::CActionEventBase> > _V;
template class _Rb_tree<CCharString, _V, _Select1st<_V>, less<CCharString>, allocator<_V> >;
}   // namespace std