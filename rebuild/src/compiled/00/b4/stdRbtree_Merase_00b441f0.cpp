// std::_Rb_tree::_M_erase(_Rb_tree_node<value_type>* x) -- (std::map<long, CEngineStaticMapFileBlockManager::CFileBlock >) STLport 4.6 recursive subtree release without rebalancing: for each node erase its right subtree, release the value's counted pointer (LoadingData, node +0x1C), free the node and continue with its left child
// Leaf: ?_M_erase@?$_Rb_tree@JU?$pair@$$CBJUCFileBlock@CEngineStaticMapFileBlockManager@@@std@@U?$_Select1st@U?$pair@$$CBJUCFileBlock@CEngineStaticMapFileBlockManager@@@std@@@2@U?$less@J@2@V?$allocator@U?$pair@$$CBJUCFileBlock@CEngineStaticMapFileBlockManager@@@std@@@2@@std@@AAEXPAU?$_Rb_tree_node@U?$pair@$$CBJUCFileBlock@CEngineStaticMapFileBlockManager@@@std@@@2@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B441F0, 92 bytes (+ int3 padding to 96), thiscall `ret 4`; recursive; speed-optimised TU.
// Identity (proven): std::map<long, CEngineStaticMapFileBlockManager::CFileBlock >; evidence: CEngineStaticMapFileBlockManager::~CEngineStaticMapFileBlockManager 0x00B443B0 (landed) calls it (0x00B443C9) on this+8 = Blocks, Ego_r.pdb map<long, CEngineStaticMapFileBlockManager::CFileBlock> (CFileBlock {long RefCount, bool LowPriority, CCountedPointer<CBankFileAsyncData> LoadingData +8}); the landed _M_create_node 0x00BE0AC0 builds the same 0x24-byte node, and this body releases node +0x1C/+0x20.
// Label: the manifest's "Utility_DestroyRefcountedTreeNode" is propagated (family J005: six _M_erase bodies whose only non-trivial value member is a
//   CCountedPointer; retail has no identical-code folding, so each address is one instantiation); this is STLport 4.6
//   `_Rb_tree::_M_erase` (`_tree.c`).
// Callees: the count block's deleter (`call [info+4]`, the object in ecx) and operator delete 0x00BFE9BC (the count block)
//   from the inline ~CCountedPointer, free 0x00BFEA14 (allocator<_Node>::deallocate).
// Model: the landed 0x0069D3D0 (agent952 H079, map<NCameraModes::EMode, CCountedPointer<NCameraModes::CBaseInit> >).
extern "C" void __cdecl free(void* block);             // 0x00BFEA14
void operator delete(void* block);                      // 0x00BFE9BC

class CCPPointerInfo { public:                                 // the shared count block
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
class CBankFileAsyncData;
class CEngineStaticMapFileBlockManager {
public:
    struct CFileBlock {                                 // Ego_r.pdb, 16 bytes (class key U from the PDB name sets)
        long RefCount;                                  // +0x0
        bool LowPriority;                               // +0x4
        CCountedPointer<CBankFileAsyncData> LoadingData;   // +0x8
    };
};

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
private:
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
}   // namespace std

template class std::_Rb_tree<long, std::pair<const long, CEngineStaticMapFileBlockManager::CFileBlock >, std::_Select1st<std::pair<const long, CEngineStaticMapFileBlockManager::CFileBlock > >, std::less<long >, std::allocator<std::pair<const long, CEngineStaticMapFileBlockManager::CFileBlock > > >;