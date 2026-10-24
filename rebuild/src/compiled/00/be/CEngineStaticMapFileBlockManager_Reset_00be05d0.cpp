// CEngineStaticMapFileBlockManager::Reset() -- erase every file block from the Blocks map (each erase rebalances, releases the block's LoadingData counted pointer and frees the node), then reset the AsyncBankEntry counted pointer
// Leaf: ?Reset@CEngineStaticMapFileBlockManager@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00BE05D0, 218 bytes (+ int3 padding). Ego_r.pdb name set:
// ?Reset@CEngineStaticMapFileBlockManager@@QAEXXZ (the manifest label "BankFileEntry_Destroy" is descriptive). Caller: the owner's
// destructor 0x00B42A90 (manifest "~CBankFileEntryUpdateData", propagated) on its member at +0x44, right before the manager's own
// destructor 0x00B443B0.
// Callees: STLport _Rb_global<bool>::_Rebalance_for_erase @ 0x0042A34E (__cdecl, node + the header's root/leftmost/rightmost
// references), the count block's destroy function (slot +4, the object at +8 in ecx), operator delete @ 0x00BFE9BC (count blocks),
// free @ 0x00BFEA14 (tree nodes: STLport's malloc node allocator, `if (p) free(p)`).
// Layout (Ego_r.pdb CEngineStaticMapFileBlockManager; retail STLport map is 12 bytes): AsyncBankEntry
// CCountedPointer<CBankFileAsyncEntry> +0 (Data, Info), Blocks map<long, CFileBlock> +8 (_M_header +8, _M_node_count +0xc).
// Node: STLport _Rb_tree_node_base {color, parent, left, right} + pair<const long, CFileBlock> at +0x10 (CFileBlock: RefCount +4,
// LowPriority +8, LoadingData CCountedPointer<CBankFileAsyncData> +0xc/+0x10 of the pair) -- the node the landed _M_create_node
// 0x00BE0AC0 allocates (0x24 bytes).
// Shape: `it = begin(); while (it != end()) { next = it; ++next; Blocks.erase(it); it = next; }` -- the current node stays in
// ecx and the copy is advanced in esi (`mov ecx,esi` closes the loop; `erase(it++)` advances `it` in place, 2 bytes short);
// the inline _M_increment (right subtree's leftmost, else climb) runs before the inlined _Rb_tree::erase (rebalance, ~pair -> ~CCountedPointer: release then
// Data = 0, Info = 0, node free, --_M_node_count); then AsyncBankEntry.Reset(): release, then Info = 0 before Data = 0 (the
// destructor clears in the other order). Grades: 2.
extern "C" void __cdecl free(void* p);                     // 0x00BFEA14
void __cdecl operator delete(void* p);                     // 0x00BFE9BC
inline void* operator new(unsigned int, void* p) { return p; }

struct CCPPointerInfo {                                    // the counted pointer's shared block
    long  RefCount;                                        // +0
    void (__fastcall* DeleteFunction)(void* data);         // +4
    void* Data;                                            // +8
};

template <class T> class CCountedPointer {
public:
    T*              Data;                                  // +0
    CCPPointerInfo* Info;                                  // +4
    CCountedPointer(const CCountedPointer& other) : Data(other.Data), Info(other.Info) { if (Info) ++Info->RefCount; }
    void DeleteData()
    {
        if (Info) {
            --Info->RefCount;
            if (Info->RefCount == 0) {
                Info->DeleteFunction(Info->Data);
                operator delete(Info);
            }
        }
    }
    void Reset() { DeleteData(); Info = 0; Data = 0; }
    ~CCountedPointer() { DeleteData(); Data = 0; Info = 0; }
};

class CBankFileAsyncEntry;
class CBankFileAsyncData;

namespace _STL {
template <class T> struct less { bool operator()(const T& a, const T& b) const { return a < b; } };
template <class T> class allocator { };
template <class T1, class T2> struct pair {
    T1 first;
    T2 second;
};
template <class P> struct _Select1st { };

struct _Rb_tree_node_base {
    bool                _M_color;                          // +0x00
    _Rb_tree_node_base* _M_parent;                         // +0x04
    _Rb_tree_node_base* _M_left;                           // +0x08
    _Rb_tree_node_base* _M_right;                          // +0x0c
};
template <class V> struct _Rb_tree_node : _Rb_tree_node_base {
    V _M_value_field;                                      // +0x10
};
template <class Dummy> class _Rb_global {
public:
    static _Rb_tree_node_base* __cdecl _Rebalance_for_erase(_Rb_tree_node_base* z, _Rb_tree_node_base*& root,
                                                            _Rb_tree_node_base*& leftmost, _Rb_tree_node_base*& rightmost);   // 0x0042A34E
};
typedef _Rb_global<bool> _Rb_global_inst;

struct _Rb_tree_base_iterator {
    _Rb_tree_node_base* _M_node;
    void _M_increment()
    {
        if (_M_node->_M_right != 0) {
            _M_node = _M_node->_M_right;
            while (_M_node->_M_left != 0)
                _M_node = _M_node->_M_left;
        }
        else {
            _Rb_tree_node_base* y = _M_node->_M_parent;
            while (_M_node == y->_M_right) {
                _M_node = y;
                y = y->_M_parent;
            }
            if (_M_node->_M_right != y)
                _M_node = y;
        }
    }
};
template <class V> struct _Rb_tree_iterator : _Rb_tree_base_iterator {
    _Rb_tree_iterator(_Rb_tree_node_base* x) { _M_node = x; }
    _Rb_tree_iterator(const _Rb_tree_iterator& it) { _M_node = it._M_node; }
    _Rb_tree_iterator& operator++() { _M_increment(); return *this; }
    _Rb_tree_iterator operator++(int) { _Rb_tree_iterator tmp = *this; _M_increment(); return tmp; }
    bool operator!=(const _Rb_tree_iterator& y) const { return _M_node != y._M_node; }
};

template <class T> inline void _Destroy(T* p) { p->~T(); }

template <class K, class V, class KoV, class C, class A> class _Rb_tree {
public:
    typedef _Rb_tree_node<V>*     _Link_type;
    typedef _Rb_tree_iterator<V>  iterator;
    _Rb_tree_node_base* _M_header;                         // +0 (the alloc proxy's data)
    unsigned int        _M_node_count;                     // +4
    C                   _M_key_compare;                    // +8
    iterator begin() { return iterator(_M_header->_M_left); }
    iterator end() { return iterator(_M_header); }
    void erase(iterator position)
    {
        _Link_type y = (_Link_type)_Rb_global_inst::_Rebalance_for_erase(position._M_node, _M_header->_M_parent,
                                                                         _M_header->_M_left, _M_header->_M_right);
        _Destroy(&y->_M_value_field);
        if (y != 0)
            free(y);
        --_M_node_count;
    }
};

template <class K, class T, class C, class A> class map {
public:
    typedef pair<const K, T> value_type;
    typedef _Rb_tree<K, value_type, _Select1st<value_type>, C, A> _Rep_type;
    typedef typename _Rep_type::iterator iterator;
    _Rep_type _M_t;
    iterator begin() { return _M_t.begin(); }
    iterator end() { return _M_t.end(); }
    void erase(iterator position) { _M_t.erase(position); }
};
}

class CEngineStaticMapFileBlockManager {
public:
    struct CFileBlock {
        long                                RefCount;      // +0x0
        bool                                LowPriority;   // +0x4
        CCountedPointer<CBankFileAsyncData> LoadingData;   // +0x8
    };
    CCountedPointer<CBankFileAsyncEntry> AsyncBankEntry;   // +0x00
    _STL::map<long, CFileBlock, _STL::less<long>, _STL::allocator<_STL::pair<const long, CFileBlock> > > Blocks;   // +0x08
    void Reset();
};

void CEngineStaticMapFileBlockManager::Reset()
{
    _STL::map<long, CFileBlock, _STL::less<long>, _STL::allocator<_STL::pair<const long, CFileBlock> > >::iterator it = Blocks.begin();
    while (it != Blocks.end()) {
        _STL::map<long, CFileBlock, _STL::less<long>, _STL::allocator<_STL::pair<const long, CFileBlock> > >::iterator next = it;
        ++next;
        Blocks.erase(it);
        it = next;
    }
    AsyncBankEntry.Reset();
}