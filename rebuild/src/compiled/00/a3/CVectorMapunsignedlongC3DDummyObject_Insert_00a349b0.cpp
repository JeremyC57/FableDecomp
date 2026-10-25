// CVectorMap<unsigned long,C3DDummyObject>::Insert(const pair<unsigned long, C3DDummyObject>& x) -- append a (key, dummy object) entry to the mesh-stats dummy-object map and mark it dirty (unsorted)
// Leaf: ?Insert@?$CVectorMap@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A349B0, 82 bytes (+ int3 padding), thiscall `ret 4`.
// Identity: Ego_r.pdb `?Insert@?$CVectorMap@KVC3DDummyObject@@...`; an unreferenced COMDAT copy (its callers inline it). The catalog's
//   "Vector_PushBack_56bytes" is descriptive. Callee: vector::_M_insert_overflow(pos, x, const __false_type&, fill_len,
//   atend) @ 0x00A33E80.
// Layout: CVectorMap<K, V, C> derives from vector<pair<K, V> > (PDB: base +0..+0xf in the Dinkumware build, Compare +0x10,
//   Dirty +0x11); with retail's 12-byte STLport vector the comparator sits at +0xc and Dirty at +0xd.
// Shape: STLport `push_back(x)` -- `if (_M_finish != _M_end_of_storage) { _Construct(_M_finish, x); ++_M_finish; } else
//   _M_insert_overflow(_M_finish, x, _IsPODType(), 1UL, true);` (placement new's null test, STLport's pair copy constructor: the key, then the 52-byte C3DDummyObject as `rep movsd` x13,
//   the __false_type temporary homed in the dead x slot) -- then `Dirty = true` on both paths.
inline void* operator new(unsigned int, void* where) { return where; }

struct CMatrix3x4 { float M[12]; };
struct C3DDummyObject {                                // 0x34 bytes (Ego_r.pdb)
    CMatrix3x4 Transform;                              // +0x00
    long       BoneIndex;                              // +0x30
};

namespace std {

struct __false_type { __false_type() { } };

template <class T1, class T2> struct pair {
    T1 first;
    T2 second;
    pair(const pair& p) : first(p.first), second(p.second) { }
};

template <class T1, class T2> inline void _Construct(T1* p, const T2& value) { new (p) T1(value); }

template <class T> class vector {
public:
    T* _M_start;                                       // +0x00
    T* _M_finish;                                      // +0x04
    T* _M_end_of_storage;                              // +0x08
    void _M_insert_overflow(T* position, const T& x, const __false_type&, unsigned int fill_len, bool atend);   // 0x00A33E80
    void push_back(const T& x)
    {
        if (_M_finish != _M_end_of_storage) {
            _Construct(_M_finish, x);
            ++_M_finish;
        }
        else
            _M_insert_overflow(_M_finish, x, __false_type(), 1UL, true);
    }
};

}

template <class K, class V> struct CKeyPairCompareLess {
    bool operator()(const std::pair<K, V>& a, const std::pair<K, V>& b) const { return a.first < b.first; }
};

template <class K, class V, class C> class CVectorMap : public std::vector<std::pair<K, V> > {
public:
    C    Compare;                                      // +0x0c
    bool Dirty;                                        // +0x0d
    void Insert(const std::pair<K, V>& x)
    {
        push_back(x);
        Dirty = true;
    }
};

template void CVectorMap<unsigned long, C3DDummyObject, CKeyPairCompareLess<unsigned long, C3DDummyObject> >::Insert(const std::pair<unsigned long, C3DDummyObject>& x);