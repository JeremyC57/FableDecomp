// std::_Rb_tree::erase(iterator position) -- (the static map tree of pair<const long, CEngineStaticMapFileBlockManager::CFileBlock>) unlink one file-block node from the static map's tree (rebalance), release its LoadingData counted pointer, free the node and drop the count
// Leaf: ?erase@?$_Rb_tree@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00BE0A50, 103 bytes (+ int3 padding), `__fastcall`, the iterator by value (`ret 4`).
// The manifest label "CMemoryAllocatorVariableSize_Erase_Destructor" is propagated. Caller: the map's out-of-line erase(iterator)
// forwarder 0x00BE0B00 (the by-value iterator copy built in its argument slot). The same body is inlined into
// CEngineStaticMapFileBlockManager::Reset 0x00BE05D0.
// Callees: STLport _Rb_global<bool>::_Rebalance_for_erase @ 0x0042A34E (__cdecl: node, then the header's root/leftmost/rightmost
// references pushed right to left), the count block's destroy function (slot +4, object at +8 in ecx), operator delete
// @ 0x00BFE9BC, free @ 0x00BFEA14 (the malloc node allocator's `if (p) free(p)`).
// Shape: STLport `erase(iterator)`: rebalance, `_Destroy(&y->_M_value_field)` (~pair -> ~CFileBlock -> ~CCountedPointer: release,
// then Data = 0 and Info = 0, the null test of the node scheduled between them), deallocate, `--_M_node_count` (`dec [edi+4]`).
// Layout: as the landed _M_create_node 0x00BE0AC0 / insert_unique 0x00BE0BF0: STLport node {color, parent, left, right} + the
// pair at +0x10 (key, CFileBlock RefCount / LowPriority / LoadingData Data +0x1c, Info +0x20); tree {_M_header, _M_node_count}.
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

namespace std {
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
};

typedef std::pair<const long, CEngineStaticMapFileBlockManager::CFileBlock> TBlockValue;
template void std::_Rb_tree<long, TBlockValue, std::_Select1st<TBlockValue>, std::less<long>, std::allocator<TBlockValue> >::erase(
    std::_Rb_tree<long, TBlockValue, std::_Select1st<TBlockValue>, std::less<long>, std::allocator<TBlockValue> >::iterator);