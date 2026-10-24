// CVectorMap<unsigned long,C3DHelperPoint>::Insert(const pair<unsigned long, C3DHelperPoint>& x) -- append a (key, helper point) entry to the mesh-stats helper-point map and mark it dirty (unsorted)
// Leaf: ?Insert@?$CVectorMap@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A34950, 89 bytes (+ int3 padding), thiscall `ret 4`.
// Identity: Ego_r.pdb `?Insert@?$CVectorMap@KVC3DHelperPoint@@V?$CKeyPairCompareLess@KVC3DHelperPoint@@@@@@QAEXABU?$pair@
//   KVC3DHelperPoint@@@std@@@Z`; an unreferenced COMDAT copy (its callers inline it). The catalog's
//   "Vector_PushBack_20bytes" is descriptive. Callee: vector::_M_insert_overflow(pos, x, const __false_type&, fill_len,
//   atend) @ 0x00A33D90.
// Layout: CVectorMap<K, V, C> derives from vector<pair<K, V> > (PDB: base +0..+0xf in the Dinkumware build, Compare +0x10,
//   Dirty +0x11); with retail's 12-byte STLport vector the comparator sits at +0xc and Dirty at +0xd.
// Shape: STLport `push_back(x)` -- `if (_M_finish != _M_end_of_storage) { _Construct(_M_finish, x); ++_M_finish; } else
//   _M_insert_overflow(_M_finish, x, _IsPODType(), 1UL, true);` (placement new's null test, the pair's memberwise copy,
//   the __false_type temporary homed in the dead x slot) -- then `Dirty = true` on both paths.
inline void* operator new(unsigned int, void* where) { return where; }

struct C3DVector {
    float X, Y, Z;
    C3DVector(const C3DVector& v) : X(v.X), Y(v.Y), Z(v.Z) { }
};

struct C3DHelperPoint {                                // 16 bytes (Ego_r.pdb)
    C3DVector Position;                                // +0x00
    long      BoneIndex;                               // +0x0c
};

namespace _STL {

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
    void _M_insert_overflow(T* position, const T& x, const __false_type&, unsigned int fill_len, bool atend);   // 0x00A33D90
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
    bool operator()(const _STL::pair<K, V>& a, const _STL::pair<K, V>& b) const { return a.first < b.first; }
};

template <class K, class V, class C> class CVectorMap : public _STL::vector<_STL::pair<K, V> > {
public:
    C    Compare;                                      // +0x0c
    bool Dirty;                                        // +0x0d
    void Insert(const _STL::pair<K, V>& x)
    {
        push_back(x);
        Dirty = true;
    }
};

template void CVectorMap<unsigned long, C3DHelperPoint, CKeyPairCompareLess<unsigned long, C3DHelperPoint> >::Insert(const _STL::pair<unsigned long, C3DHelperPoint>& x);