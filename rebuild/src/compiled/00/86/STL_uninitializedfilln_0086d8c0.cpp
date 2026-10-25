// std::__uninitialized_fill_n(CRandomAppearanceMorph::CBodyPartMesh* first, unsigned int n, const CRandomAppearanceMorph::CBodyPartMesh& x, const __false_type&) -- copy-construct n body-part mesh records (mesh index, priority, texture-morph map) into raw storage at first and return first + n
// Leaf: ??$__uninitialized_fill_n@
// Retail: Fable.exe (TLC, VC7.1) @ 0x0086D8C0, 63 bytes, __fastcall (first ecx, n edx; x and the empty tag on the stack, `ret 8`).
// Callers: 0x0086DA1A, 0x0086E461 (the vector's insert-grow 0x0086E3B0, landed) and 0x0086E68F (its _M_fill_insert
//   0x0086E5C0, landed, whose 24-byte element it left unnamed).
// Element: Ego_r.pdb CRandomAppearanceMorph::CBodyPartMesh -- long MeshIndex +0x0, long Priority +0x4, CVectorMap<unsigned long,
//   CRandomAppearanceMorph::CTextureMorph, CKeyPairCompareLess<...> > TextureMorphs +0x8; retail STLport vectors are 12
//   bytes, so the map is vector +0x8, the empty comparator +0x14 and bool Dirty +0x15 (24 bytes in all; the PDB's 0x1C is
//   the Ego_r 16-byte vector). Implicit copy constructors throughout: the two longs, the map's vector through STLport's
//   out-of-line vector copy constructor 0x00673E10 (landed), nothing for the empty comparator, then Dirty.
// Label: catalog "vector<unsigned __int64>::_Ufill" is propagated across the byte-identical-shaped STLport helpers.
// Shape: STLport 4.6 __uninitialized_fill_n verbatim (`for (; __n > 0; --__n, ++__cur) _Construct(&*__cur, __x);`, unsigned
//   n so `jbe`), the placement new keeping its null test (`test esi,esi ; je`).
inline void* operator new(unsigned int, void* p) { return p; }

namespace std {
struct __false_type { __false_type() { } };

template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
};

template <class _Tp> class allocator {
public:
    allocator() { }
};

template <class _Tp, class _Alloc = allocator<_Tp> > class vector {
public:
    _Tp* _M_start;                              // +0x0
    _Tp* _M_finish;                             // +0x4
    _Tp* _M_end_of_storage;                     // +0x8
    vector(const vector<_Tp, _Alloc>& __x);     // 0x00673E10
};
}

template <class K, class V> class CKeyPairCompareLess {
public:
    bool operator()(const std::pair<K, V>& a, const std::pair<K, V>& b) const { return a.first < b.first; }
};

template <class K, class V, class C> class CVectorMap {
public:
    std::vector<std::pair<K, V> > Vec;        // +0x0
    C    Compare;                               // +0xC (empty)
    bool Dirty;                                 // +0xD
};

class CRandomAppearanceMorph {
public:
    struct CTextureMorph {                      // Ego_r.pdb, 8 bytes
        unsigned long NewTexture;               // +0x0
        unsigned long GroupID;                  // +0x4
    };
    class CBodyPartMesh {                       // Ego_r.pdb CRandomAppearanceMorph::CBodyPartMesh, 24 bytes retail
    public:
        long MeshIndex;                         // +0x00
        long Priority;                          // +0x04
        CVectorMap<unsigned long, CTextureMorph,
                   CKeyPairCompareLess<unsigned long, CTextureMorph> > TextureMorphs;   // +0x08
    };
};

namespace std {
template <class _T1, class _T2> inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }

template <class _ForwardIter, class _Size, class _Tp>
inline _ForwardIter __fastcall __uninitialized_fill_n(_ForwardIter __first, _Size __n, const _Tp& __x, const __false_type&)
{
    _ForwardIter __cur = __first;
    for (; __n > 0; --__n, ++__cur)
        _Construct(&*__cur, __x);
    return __cur;
}

template CRandomAppearanceMorph::CBodyPartMesh* __fastcall __uninitialized_fill_n(CRandomAppearanceMorph::CBodyPartMesh*, unsigned int,
                                                                                const CRandomAppearanceMorph::CBodyPartMesh&, const __false_type&);
}