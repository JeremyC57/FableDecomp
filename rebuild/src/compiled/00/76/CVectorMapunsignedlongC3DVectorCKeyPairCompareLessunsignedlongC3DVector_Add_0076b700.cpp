// CVectorMap<unsigned long, C3DVector, CKeyPairCompareLess<unsigned long, C3DVector> >::Add(const pair<unsigned long, C3DVector>& entry) -- append a {key, vector} pair to the map's vector and mark the map dirty
// Leaf: ?Add@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0076B700, 83 bytes (+int3 pad to 96), `ret 4`. No direct caller or reference (an
//   unreferenced out-of-line copy of an inline CVectorMap member).
// Callee: vector<pair<unsigned long, C3DVector> >::_M_insert_overflow(pos, x, tag, 1, true) 0x0076ADA0 (24 call
//   sites across the augmentation, creature-mode and AI TUs).
// Identity: the 16-byte (unsigned long key, 12-byte payload) pair whose sort helpers are landed beside it
//   (0x00769470 __push_heap, 0x00769D50 __adjust_heap, 0x0076C000 sort; this chunk's 0x00769420 .. 0x0076B530); the
//   payload is modelled as the landed siblings' C3DVector. CVectorMap = vector (retail 12 bytes) + empty Compare
//   +0xc + Dirty +0xd (the landed CVectorMap::Add family, 0x006FCF40). The catalog's "Vector_PushBack_16bytes" is a
//   descriptive auto label.
// Shape: the STLport push_back is inlined (placement copy of the pair -- key, then the vector's three words -- with
//   its null test, `add [esi+4],10h`; else _M_insert_overflow with the empty tag homed in the dead `entry` slot),
//   then Dirty = true in both tails.
inline void* operator new(unsigned int, void* where) { return where; }

struct C3DVector {
    float X, Y, Z;
    C3DVector(const C3DVector& other) : X(other.X), Y(other.Y), Z(other.Z) { }   // memberwise user copy (no user operator=)
};

struct __false_type { __false_type() { } };

namespace std {

template <class _T1, class _T2>
class pair {
public:
    _T1 first;
    _T2 second;
    pair(const pair& o) : first(o.first), second(o.second) { }
};

}

template <class _T1, class _T2>
inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }

template <class _Tp> struct SAllocProxy {                   // STLport _STLP_alloc_proxy: the end-of-storage pointer
    _Tp* _M_data;
};

template <class _Tp> class CRetailVector {                  // STLport vector (12 bytes)
public:
    _Tp* _M_start;                                          // +0x00
    _Tp* _M_finish;                                         // +0x04
    SAllocProxy<_Tp> _M_end_of_storage;                     // +0x08
    void _M_insert_overflow(_Tp* __position, const _Tp& __x, const __false_type&, unsigned int __fill_len, bool __atend);   // 0x0076ADA0
    void push_back(const _Tp& __x)
    {
        if (this->_M_finish != this->_M_end_of_storage._M_data) {
            _Construct(this->_M_finish, __x);
            ++this->_M_finish;
        }
        else
            _M_insert_overflow(this->_M_finish, __x, __false_type(), 1UL, true);
    }
};

template <class K, class V> struct CKeyPairCompareLess {
    bool operator()(const std::pair<K, V>& a, const std::pair<K, V>& b) const { return a.first < b.first; }
};

template <class K, class V, class C> class CVectorMap {
public:
    typedef std::pair<K, V> value_type;
    CRetailVector<value_type> m_vector;                     // +0x00
    C    Compare;                                           // +0x0c (empty)
    bool Dirty;                                             // +0x0d
    void Add(const value_type& entry);
};

template <class K, class V, class C> void CVectorMap<K, V, C>::Add(const value_type& entry)
{
    m_vector.push_back(entry);
    Dirty = true;
}

template class CVectorMap<unsigned long, C3DVector, CKeyPairCompareLess<unsigned long, C3DVector> >;