// CThreadedFileManager::CPendingObjectMap::CPendingObjectMap() -- one device queue: an empty STLport map<CPendingObjectSortKey, CFileObject*> (malloc'd red header linked to itself) and a null CurrentPosition iterator
// Leaf: ??0CPendingObjectMap@CThreadedFileManager@@QAE@XZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00991240, length 64 bytes (61 + int3 padding), plain `ret`.
// PDB (FableWin/Ego_r): ??0CPendingObjectMap@CThreadedFileManager@@QAE@XZ; layout Objects (map) +0x00, CurrentPosition
// (map iterator) +0x0C -- 16 bytes in retail (the landed CThreadedFileManager ctor @ 0x0098F3A0 builds its 2x7
// DeviceQueue array with this body inlined).
// Callees: malloc @ 0x00BFEA0E (the 0x18-byte header node: 0x10 links + the 8-byte pair).
// References: none in the image (an unreferenced out-of-line copy of the constructor).
// Label: the manifest's "LTextBinTree_Initialize" is descriptive.
// Shape: STLport 4.6 _Rb_tree default ctor (alloc-proxy base nulls then allocates the header, node count 0,
// _M_empty_initialize re-reading the header after each store), then the STLport iterator default ctor (_M_node = 0).
#include <stddef.h>

extern "C" void* __cdecl malloc(size_t size);   // 0x00BFEA0E

class CThreadedFileManager {
public:
    class CFileObject;
    class CPendingObjectSortKey {
    public:
        CFileObject* Object;                    // +0x00
    };
    class CPendingObjectMap;
};

namespace std {

template <class T1, class T2> struct pair { T1 first; T2 second; };
template <class T> struct less { };

template <class T> class allocator {
public:
    allocator() { }
    T* allocate(size_t count) { return count != 0 ? static_cast<T*>(malloc(count * sizeof(T))) : 0; }
};

template <class Value, class T, class Alloc> class _STLP_alloc_proxy : public Alloc {
public:
    Value _M_data;
    _STLP_alloc_proxy(const Alloc& alloc, Value p) : Alloc(alloc), _M_data(p) { }
};

struct _Rb_tree_node_base {
    bool _M_color;
    _Rb_tree_node_base* _M_parent;
    _Rb_tree_node_base* _M_left;
    _Rb_tree_node_base* _M_right;
};

template <class Value> struct _Rb_tree_node : _Rb_tree_node_base {
    Value _M_value_field;
};

template <class Value> struct _Rb_tree_iterator {
    _Rb_tree_node_base* _M_node;
    _Rb_tree_iterator() { _M_node = 0; }
};

template <class Value, class Alloc> class _Rb_tree_base {
public:
    typedef _Rb_tree_node<Value> _Node;
    _STLP_alloc_proxy<_Rb_tree_node_base*, _Node, allocator<_Node> > _M_header;
    _Rb_tree_base(const Alloc&) : _M_header(allocator<_Node>(), 0)
    {
        _M_header._M_data = _M_header.allocate(1);
    }
};

template <class Key, class Value, class Compare, class Alloc> class _Rb_tree : public _Rb_tree_base<Value, Alloc> {
public:
    size_t _M_node_count;
    Compare _M_key_compare;
    _Rb_tree(const Compare& comp = Compare(), const Alloc& alloc = Alloc())
        : _Rb_tree_base<Value, Alloc>(alloc), _M_node_count(0), _M_key_compare(comp)
    {
        _M_empty_initialize();
    }
    void _M_empty_initialize()
    {
        this->_M_header._M_data->_M_color = false;          // _S_rb_tree_red
        this->_M_header._M_data->_M_parent = 0;             // root
        this->_M_header._M_data->_M_left = this->_M_header._M_data;
        this->_M_header._M_data->_M_right = this->_M_header._M_data;
    }
};

template <class Key, class T, class Compare = less<Key>, class Alloc = allocator<pair<const Key, T> > > class map {
public:
    typedef pair<const Key, T> value_type;
    typedef _Rb_tree_iterator<value_type> iterator;
    _Rb_tree<Key, value_type, Compare, Alloc> _M_t;
    map() : _M_t(Compare(), Alloc()) { }
};

}   // namespace std

class CThreadedFileManager::CPendingObjectMap {
public:
    std::map<CPendingObjectSortKey, CFileObject*> Objects;              // +0x00
    std::map<CPendingObjectSortKey, CFileObject*>::iterator CurrentPosition;   // +0x0c
    CPendingObjectMap();
};

CThreadedFileManager::CPendingObjectMap::CPendingObjectMap()
{
}