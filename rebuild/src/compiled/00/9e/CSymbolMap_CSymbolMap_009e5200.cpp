// CSymbolMap::CSymbolMap() -- CBase part, this class's vtable, an empty clean symbol vector-map, an empty used-names set (fresh STLport tree header) and duplicate checking off
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009E5200, length 76 bytes (80 with int3 padding).
// Leaf: ??0CSymbolMap
// Callees: CBase::CBase() @ 0x0099A2F0, malloc @ 0x00BFEA0E (the 0x14-byte tree header). Vtable 0x0129C158 =
// CSymbolMap (rtti-vtables.tsv).
// Label correction: the manifest's "CParticleUpdateParameters<unsigned long,unsigned long>" constructor is
// propagated. Layout: Ego_r.pdb CSymbolMap (vptr; SLONGMap CVectorMap<CCharString, CSymbolInfo, CCompareString>;
// NamesUsed set<CCharString>; bool CheckForDuplicateSymbols) with the retail 16-byte CVectorMap and 12-byte
// STLport set: SLONGMap +0x04 (Dirty +0x11), NamesUsed +0x14 (header +0x14, node count +0x18, comparator +0x1c),
// CheckForDuplicateSymbols +0x20.
// Shape: one ebx zero; the STLport _Rb_tree_base nulls the header and allocates it through the malloc
// allocator, _Rb_tree sets the node count and `_M_empty_initialize` colours the header red and links
// parent/left/right, re-reading the header after every store through it (the model of the landed
// CLookDrawRequest constructor 0x007554C0); the bool last.
extern "C" void* __cdecl malloc(unsigned int size);         // 0x00BFEA0E

struct CBase {
    CBase();                                                // 0x0099A2F0
    virtual ~CBase();
};

struct CCharString { void* PStringData; };

namespace _STL {
template <class T> struct less { };
template <class T> struct allocator { };

struct _Rb_tree_node_base {
    unsigned char       _M_color;                           // +0x00 (0 = red)
    _Rb_tree_node_base* _M_parent;                          // +0x04
    _Rb_tree_node_base* _M_left;                            // +0x08
    _Rb_tree_node_base* _M_right;                           // +0x0c
};
template <class V> struct _Rb_tree_node : _Rb_tree_node_base {
    V _M_value_field;                                       // +0x10
};

template <class V> struct _Rb_tree_base {
    _Rb_tree_node<V>* _M_header;                            // +0x00 (alloc proxy data)
    _Rb_tree_base() : _M_header(0)
    {
        _M_header = (_Rb_tree_node<V>*)malloc(sizeof(_Rb_tree_node<V>));
    }
};

template <class K, class V, class Compare> struct _Rb_tree : _Rb_tree_base<V> {
    unsigned int _M_node_count;                             // +0x04
    Compare      _M_key_compare;                            // +0x08
    _Rb_tree() : _M_node_count(0)
    {
        this->_M_header->_M_color = 0;
        this->_M_header->_M_parent = 0;
        this->_M_header->_M_left = this->_M_header;
        this->_M_header->_M_right = this->_M_header;
    }
};

template <class K, class Compare = less<K>, class Alloc = allocator<K> > struct set {
    _Rb_tree<K, K, Compare> _M_t;                           // 12 bytes
};
}

template <class T> struct CAllocProxy {
    T* _M_data;
    CAllocProxy(T* p) : _M_data(p) {}
};
template <class T> struct CVector {                         // STLport vector, 12 bytes
    T* _M_start; T* _M_finish; CAllocProxy<T> _M_end_of_storage;
    CVector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) {}
};
template <class K, class V> struct CKeyValuePair { K first; V second; };
template <class K, class V, class C> struct CVectorMap {    // retail 16 bytes
    CVector<CKeyValuePair<K, V> > Data;                     // +0x00
    C                             Compare;                  // +0x0c empty comparator
    bool                          Dirty;                    // +0x0d
    CVectorMap() : Dirty(false) {}
};
template <class T> struct CCompareString { };

struct CSymbolMap : CBase {                                 // vtable 0x0129C158
    struct CSymbolInfo { CCharString GroupName; CCharString EntryName; long Val; };   // PDB, 12 bytes
    CVectorMap<CCharString, CSymbolInfo, CCompareString<CSymbolInfo> > SLONGMap;   // +0x04
    _STL::set<CCharString>                                             NamesUsed;  // +0x14
    bool                                                               CheckForDuplicateSymbols;   // +0x20
    CSymbolMap();
    virtual ~CSymbolMap();
};

CSymbolMap::CSymbolMap()
    : CheckForDuplicateSymbols(false)
{
}