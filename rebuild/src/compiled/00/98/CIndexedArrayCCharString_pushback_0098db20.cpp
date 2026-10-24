// CIndexedArray<CCharString>::push_back(const CCharString& item) -- record the item's index (the current array size) in the name -> index map, then append the item to the array
// Leaf: ?push_back@?$CIndexedArray@VCCharString@@@@QAEXABVCCharString@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0098DB20, length 128 bytes (124 + int3 padding), `ret 4`.
// PDB (Ego_r, donor 0x00AE86A0): ?push_back@?$CIndexedArray@VCCharString@@@@QAEXABVCCharString@@@Z.
// Callees: CCharString copy ctor @ 0x0099EC30 (the pair key, then the appended element), _STL::map<CCharString,long>::
// insert(const value_type&) @ 0x00431606 (pair<iterator,bool> returned through the hidden pointer, dropped),
// ~CCharString @ 0x0099EAE0 (the pair temporary), _STL::vector<CCharString>::_M_insert_overflow @ 0x00433530
// (finish, item, __false_type, 1, true).
// Callers: ten sites in CShaderRenderManager (0x0098AFB6.. 0x0098B3D5), e.g. SetPixelShaderConstant @ 0x0098B390.
// Label: the manifest's "CVectorMap_Insert_Morphs" is propagated; the PDB names it CIndexedArray<CCharString>::push_back.
// Layout: PDB Array (CArray<CCharString>) +0x00, IndexMap +0x10 in the debug build; retail STLport vector is 12 bytes, so
// IndexMap is at +0x0C.
// Shape: `IndexMap.insert(pair<const T, long>(item, size()))` -- size() (`sar edi,2`) evaluated before the key copy;
// then STLport vector::push_back inlined (placement-new copy under a null test, `_IsPODType()` __false_type
// temporary homed on the item argument slot in the overflow arm).
#include <new>
#include <stddef.h>

class CCharString {
public:
    void* PStringData;
    CCharString(const CCharString& other);      // 0x0099EC30
    ~CCharString();                             // 0x0099EAE0
};

namespace _STL {

struct __false_type { __false_type() { } };

template <class T1, class T2> struct pair {
    T1 first;
    T2 second;
    pair(const T1& a, const T2& b) : first(a), second(b) { }
};

template <class T> struct less { };
template <class T> class allocator { public: allocator() { } };

template <class Value, class T, class Alloc> class _STLP_alloc_proxy : public Alloc {
public:
    Value _M_data;
};

template <class T, class Alloc> class _Vector_base {
public:
    T* _M_start;
    T* _M_finish;
    _STLP_alloc_proxy<T*, T, Alloc> _M_end_of_storage;
};

template <class T, class Alloc = allocator<T> > class vector : public _Vector_base<T, Alloc> {
public:
    size_t size() const { return size_t(this->_M_finish - this->_M_start); }
    void _M_insert_overflow(T* position, const T& x, const __false_type&, size_t fillLen, bool atEnd = false);   // 0x00433530
    void push_back(const T& x)
    {
        if (this->_M_finish != this->_M_end_of_storage._M_data) {
            new (this->_M_finish) T(x);
            ++this->_M_finish;
        }
        else
            _M_insert_overflow(this->_M_finish, x, __false_type(), 1UL, true);
    }
};

struct _Rb_tree_node_base;
template <class V> struct _Rb_tree_iterator { _Rb_tree_node_base* _M_node; };

template <class K, class V, class C = less<K>, class A = allocator<pair<const K, V> > > class map {
public:
    typedef pair<const K, V> value_type;
    _Rb_tree_node_base* _M_header;              // +0x00
    size_t _M_node_count;                       // +0x04
    C _M_key_compare;                           // +0x08
    pair<_Rb_tree_iterator<value_type>, bool> insert(const value_type& value);   // 0x00431606
};

}   // namespace _STL

template <class T> class CArray : public _STL::vector<T> { };

template <class T> class CIndexedArray {
public:
    CArray<T> Array;                            // +0x00
    _STL::map<T, long> IndexMap;                // +0x0c
    void push_back(const T& item)
    {
        IndexMap.insert(_STL::pair<const T, long>(item, Array.size()));
        Array.push_back(item);
    }
};

template void CIndexedArray<CCharString>::push_back(const CCharString&);