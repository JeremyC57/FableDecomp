// CTriangleStripifier2::CalcSectionScore(const CArray<unsigned short>& section, const CArray<unsigned short>& triangles, const CArray<CArray<long> >& vertexPolys) -- score a strip section: mark its polygons, strip the unused vertices from a copy of the section, count the vertices that still touch an unmarked polygon and return polygons per such vertex (0 when no polygon was marked, 1e30 when none touches an unmarked one)
// Leaf: CalcSectionScore
// Retail: Fable.exe (TLC, VC7.1) @ 0x00F3D830, 336 bytes, `ret 0Ch`.
// Callees: CTriangleStripifier2::CalcSectionPolyMask @ 0x00F3CF50 (returns the marked polygon count), the CArray<unsigned
//          short> copy ctor @ 0x00A87090, CTriangleStripifier2::StripUnusedVerts @ 0x00F3B970, free @ 0x00BFEA14 (the two
//          local arrays, at every return); float pool 0x0122DEDC = 0.0f, 0x01301CF4 = 1e30f.
// Layout: CArray = the retail STLport vector {first, last, end_of_storage} (12 bytes), as the landed
//          CTriangleStripifier2::ReorderTriangleListInPlace (0x00F3F5A0). Signatures from Ego_r.pdb mangled names.
// Shape: the two callees retail compiled in this TU get same-TU `auto_inline(off)` stand-in bodies (CalcSectionPolyMask
//          allocates and fills the mask through its out parameter, StripUnusedVerts compacts its array): only then does
//          VC7.1 keep the mask pointer in ebx across the free() calls. The inner size is spelled through
//          `vertexPolys[verts[i]]` in the loop condition so it is recomputed each step as in retail.
// Shape: the polygon count lives in the dead `triangles` slot and the miss counter in the dead `section` slot; the inner
//          loop leaves on the first unmarked
//          polygon; the result is `fild count ; fidiv misses`.
extern "C" void __cdecl free(void* block);                     // 0x00BFEA14

template<class T> struct CArray {                              // retail STLport vector: first, last, end_of_storage
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    CArray() : _M_start(0), _M_finish(0), _M_end_of_storage(0) { }
    CArray(const CArray& other);                               // 0x00A87090 for CArray<unsigned short>
    ~CArray() { if (_M_start) free(_M_start); }
    T* begin() const { return _M_start; }
    unsigned long size() const { return (unsigned long)(_M_finish - _M_start); }
    T& operator[](unsigned long n) const { return *(begin() + n); }
};

class CTriangleStripifier2 {
public:
    void* __vftable;                                           // +0x00 (4 bytes, Ego_r.pdb)
    long CalcSectionPolyMask(const CArray<unsigned short>& triangles, const CArray<CArray<long> >& vertexPolys,
                             const CArray<unsigned short>& section, CArray<bool>& polyMask);            // 0x00F3CF50
    void StripUnusedVerts(CArray<unsigned short>& section, const CArray<CArray<long> >& vertexPolys,
                          const CArray<bool>& polyMask);                                                // 0x00F3B970
    float CalcSectionScore(const CArray<unsigned short>& section, const CArray<unsigned short>& triangles,
                           const CArray<CArray<long> >& vertexPolys);
};

void* AllocPolyMask(unsigned long count);                    // allocation for the stand-in
#pragma auto_inline(off)
// Same-TU stand-ins for the two callees retail compiled in this TU (0x00F3CF50 and 0x00F3B970 sit just above the scorer):
// with their bodies visible VC7.1 knows the mask array's address does not escape, so its first pointer stays in ebx
// across the later free() calls as in retail. They write only through their out parameters.
__declspec(noinline) long CTriangleStripifier2::CalcSectionPolyMask(const CArray<unsigned short>& triangles,
    const CArray<CArray<long> >& vertexPolys, const CArray<unsigned short>& section, CArray<bool>& polyMask)
{
    unsigned long numPolys = triangles.size() / 3;
    bool* mask = (bool*)AllocPolyMask(numPolys);
    for (unsigned long k = 0; k < numPolys; ++k)
        mask[k] = false;
    polyMask._M_start = mask;
    polyMask._M_finish = mask + numPolys;
    polyMask._M_end_of_storage = mask + numPolys;
    long count = 0;
    for (unsigned long i = 0; i < section.size(); ++i) {
        const CArray<long>& polys = vertexPolys[section[i]];
        for (unsigned long j = 0; j < polys.size(); ++j) {
            unsigned long poly = (unsigned long)polys[j];
            if (poly < numPolys && !polyMask[poly]) {
                polyMask[poly] = true;
                ++count;
            }
        }
    }
    return count;
}

__declspec(noinline) void CTriangleStripifier2::StripUnusedVerts(CArray<unsigned short>& section,
    const CArray<CArray<long> >& vertexPolys, const CArray<bool>& polyMask)
{
    unsigned short* out = section._M_start;
    for (unsigned long i = 0; i < section.size(); ++i) {
        const CArray<long>& polys = vertexPolys[section[i]];
        for (unsigned long j = 0; j < polys.size(); ++j) {
            if (polyMask[polys[j]]) {
                *out++ = section[i];
                break;
            }
        }
    }
    section._M_finish = out;
}
#pragma auto_inline(on)

float CTriangleStripifier2::CalcSectionScore(const CArray<unsigned short>& section, const CArray<unsigned short>& triangles,
                                             const CArray<CArray<long> >& vertexPolys)
{
    CArray<bool> polyMask;
    long count = CalcSectionPolyMask(triangles, vertexPolys, section, polyMask);
    if (count == 0)
        return 0.0f;
    CArray<unsigned short> verts(section);
    StripUnusedVerts(verts, vertexPolys, polyMask);
    long misses = 0;
    for (unsigned long i = 0; i < verts.size(); ++i) {
        for (unsigned long j = 0; j < vertexPolys[verts[i]].size(); ++j) {
            if (!polyMask[vertexPolys[verts[i]][j]]) {
                ++misses;
                break;
            }
        }
    }
    if (misses == 0)
        return 1e30f;
    return (float)count / misses;
}
