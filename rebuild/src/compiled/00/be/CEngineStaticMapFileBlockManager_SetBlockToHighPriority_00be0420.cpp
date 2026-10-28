// CEngineStaticMapFileBlockManager::SetBlockToHighPriority(long index) -- if the block's asynchronous load is still at low priority, raise it to the high loading priority
// Leaf: ?SetBlockToHighPriority@CEngineStaticMapFileBlockManager@@QAEXJ@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00BE0420, 54 bytes, thiscall, `ret 4`.
// Identity (proven): Ego_r.pdb ?SetBlockToHighPriority@CEngineStaticMapFileBlockManager@@QAEXJ@Z (the bound
//   "PrioritiseBlock" is an invented spelling); Blocks +0x08 is map<long, CEngineStaticMapFileBlockManager::CFileBlock>
//   (node value +0x10: key, CFileBlock +0x14 {RefCount, LowPriority +4 = node +0x18, LoadingData
//   CCountedPointer<CBankFileAsyncData> +8 = node +0x1C}); callee ?ChangeLoadingPriority@CBankFileAsyncData@@QAEXW4EThreadedFilePriority@@@Z
//   0x009D5630 with the priority read from the global word at 0x0139A728.
// Callees: STLport _Rb_tree<long, ...>::find(const long&) 0x00BE08E0 (the non-const find; retail bytes are the STLport
//   _M_find + iterator, the const twin is 0x00BE0930; the bound "CRetailBlockSet::FindInsertionPoint" is invented).
// Shape: the found iterator's block used without an end() test; the flag cleared after the priority change.
class CBankFileAsyncData;
class CCPPointerInfo;
template <class T> class CCountedPointer {
public:
    T*              Data;                               // +0x00
    CCPPointerInfo* Info;                               // +0x04
    T* operator->() const { return Data; }
};
enum EThreadedFilePriority { THREADED_FILE_PRIORITY_FORCE_DWORD = 0x7fffffff };
class CBankFileAsyncData {
public:
    void ChangeLoadingPriority(EThreadedFilePriority priority);   // 0x009D5630
};
extern EThreadedFilePriority g_HighLoadingPriority;     // 0x0139A728

namespace std {
template <class _Tp> struct less { };
template <class _Tp> class allocator { };
template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
};
template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
};
template <class _Tp> struct _Nonconst_traits { typedef _Tp& reference; typedef _Tp* pointer; };

struct _Rb_tree_node_base {
    bool                _M_color;                       // +0x00
    _Rb_tree_node_base* _M_parent;                      // +0x04
    _Rb_tree_node_base* _M_left;                        // +0x08
    _Rb_tree_node_base* _M_right;                       // +0x0c
};
template <class _Value> struct _Rb_tree_node : public _Rb_tree_node_base {
    _Value _M_value_field;                              // +0x10
};
struct _Rb_tree_base_iterator {
    _Rb_tree_node_base* _M_node;
};
template <class _Value, class _Traits>
struct _Rb_tree_iterator : public _Rb_tree_base_iterator {
    typedef typename _Traits::pointer pointer;
    typedef _Rb_tree_node<_Value>* _Link_type;
    _Rb_tree_iterator() { }
    _Rb_tree_iterator(_Link_type __x) { _M_node = __x; }
    pointer operator->() const { return &((_Link_type)_M_node)->_M_value_field; }
};
template <class _Key, class _Value, class _KeyOfValue, class _Compare, class _Alloc>
class _Rb_tree {
public:
    typedef _Rb_tree_node<_Value> _Node;
    typedef _Rb_tree_iterator<_Value, _Nonconst_traits<_Value> > iterator;
    _STLP_alloc_proxy<_Node*, _Node, allocator<_Node> > _M_header;   // +0x00
    unsigned int _M_node_count;                                       // +0x04
    _Compare _M_key_compare;                                          // +0x08
    iterator find(const _Key& __k);                                   // 0x00BE08E0 (out of line)
};
template <class _Pair> struct _Select1st { };

template <class _Key, class _Tp, class _Compare = less<_Key>, class _Alloc = allocator<pair<const _Key, _Tp> > >
class map {
public:
    typedef _Key key_type;
    typedef pair<const _Key, _Tp> value_type;
    typedef _Rb_tree<_Key, value_type, _Select1st<value_type>, _Compare, _Alloc> _Rep_type;
    typedef typename _Rep_type::iterator iterator;
    _Rep_type _M_t;
    iterator find(const key_type& __x) { return _M_t.find(__x); }
};
}

class CBankFileAsyncEntry;
class CEngineStaticMapFileBlockManager {
public:
    struct CFileBlock {                                  // 16 bytes (Ego_r.pdb)
    public:
        long                                RefCount;      // +0x00
        bool                                LowPriority;   // +0x04
        CCountedPointer<CBankFileAsyncData> LoadingData;   // +0x08
    };
    CCountedPointer<CBankFileAsyncEntry>  AsyncBankEntry;   // +0x00
    std::map<long, CFileBlock>            Blocks;           // +0x08
    void SetBlockToHighPriority(long index);
};

void CEngineStaticMapFileBlockManager::SetBlockToHighPriority(long index)
{
    CFileBlock& block = Blocks.find(index)->second;
    if (block.LowPriority) {
        block.LoadingData->ChangeLoadingPriority(g_HighLoadingPriority);
        block.LowPriority = false;
    }
}