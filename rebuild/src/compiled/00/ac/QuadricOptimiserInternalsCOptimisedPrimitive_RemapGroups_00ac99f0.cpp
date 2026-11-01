// QuadricOptimiserInternals::COptimisedPrimitive::RemapGroups(const std::map<unsigned long, unsigned long>& remap) -- remap the smoothing-group ids of every vertex through the given table
// Leaf: ?RemapGroups@COptimisedPrimitive@QuadricOptimiserInternals@@QAEXABV?$map@KKU?$less@K@std@@V?$allocator@U?$pair@$$CBKK@std@@@2@@std@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00AC99F0, length 104 bytes (padded to 112).
// Name (naming wave 20, chunk 686): Ego_r.pdb ?RemapGroups@COptimisedPrimitive@QuadricOptimiserInternals@@QAEXABV?$map@KKU?$less@K@std@@V?$allocator@U?$pair@$$CBKK@std@@@2@@std@@@Z,
//   the spelling the landed caller COptimisedMesh::RemoveBones 0x00ACA0C0 binds; the callee is the landed (chunk 686)
//   ?RemapGroups@COptimisedVertex@QuadricOptimiserInternals@@QAEXABV?$map@KK...@Z 0x00AC97D0 (`this` is the vertex, one stack argument).
//   Was the un-namespaced COptimisedPrimitive::RemapGroups(const CGroupRemap&) over a CRetailVector stand-in.
// Layout: retail COptimisedPrimitive::Vertices is a 12-byte STLport vector at +0x10 of 0xb8-byte COptimisedVertex records
//   (the `imul 0xb21642c9 ; sar 7` division by 0xb8).
// Shape: `for (unsigned long i = 0; i < Vertices.size(); ++i)` -- size() re-evaluated every pass (signed pointer
//   difference / sizeof, unsigned `jb` compare), the element address strength-reduced to a running byte offset.
namespace std {
template <class _Tp> class allocator { };
template <class _Tp> struct less;
template <class _T1, class _T2> struct pair;
template <class _Key, class _Tp, class _Compare = less<_Key>, class _Alloc = allocator<pair<const _Key, _Tp> > > class map;
template <class _Tp, class _Alloc = allocator<_Tp> > class vector { public:   // STLport 4.6 vector (12 bytes)
    _Tp* _M_start;
    _Tp* _M_finish;
    _Tp* _M_end_of_storage;
    unsigned long size() const { return (unsigned long)(_M_finish - _M_start); }
    _Tp& operator[](unsigned long __n) { return *(_M_start + __n); }
};
}
namespace QuadricOptimiserInternals {
class COptimisedVertex { public:                // retail 0xb8 bytes
    unsigned char _body[0xb8];
    void RemapGroups(const std::map<unsigned long, unsigned long>& remap);     // 0x00AC97D0
};
class COptimisedPrimitive { public:
    long                              MaterialIndex;      // +0x00
    unsigned char                     UniqueVertices[0xc];// +0x04
    std::vector<COptimisedVertex>     Vertices;           // +0x10
    void RemapGroups(const std::map<unsigned long, unsigned long>& remap);
};

void COptimisedPrimitive::RemapGroups(const std::map<unsigned long, unsigned long>& remap)
{
    for (unsigned long i = 0; i < Vertices.size(); ++i)
        Vertices[i].RemapGroups(remap);
}
}
