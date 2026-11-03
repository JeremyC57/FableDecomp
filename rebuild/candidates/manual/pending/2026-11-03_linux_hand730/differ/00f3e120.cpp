// CTriangleStripifier2::FindBestStartVertex(const CArray<unsigned short>& poly_buffer, const CArray<CArray<long> >& vertex_to_poly_map, const CArray<CArray<unsigned short> >& vertex_to_vertex_map) -- pick the strip start vertex: prefer open-fan (mesh edge) vertices, score each candidate's ring section, fall back to any vertex
// Leaf: ?FindBestStartVertex@CTriangleStripifier2@@AAEJABV?$CArray@G@@ABV?$CArray@V?$CArray@J@@@@ABV?$CArray@V?$CArray@G@@@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00F3E120, 1411 bytes, `ret 0Ch`.
// Grade: DIFFER(1411v1411) -- same size and instructions, register allocation only: retail keeps this in ebx (home slot 0x24) and vertex_to_poly_map in edi
// Callees: CTriangleStripifier2::BuildRings 0x00F3D980, BuildCacheOptimizedVertexList 0x00F3BA70, CalcSectionScore
//   0x00F3D830, free 0x00BFEA14 (the inlined ~CArray of the four locals); floats 0.0f 0x0122DEDC, 1e30f 0x01301CF4.
#define FABLE_RETAIL_CTRIANGLESTRIPIFIER2
#include "engine/CTriangleStripifier2.h"

long CTriangleStripifier2::FindBestStartVertex(const CArray<unsigned short>& poly_buffer,
    const CArray<CArray<long> >& vertex_to_poly_map, const CArray<CArray<unsigned short> >& vertex_to_vertex_map)
{
    long num_verts = vertex_to_poly_map.size();
    float best_score = 0.0f;
    long best_vert = -1;
    for (long i = 0; i < num_verts; ++i) {
        if (vertex_to_poly_map[i].size()) {
            const CArray<unsigned short>& ring = vertex_to_vertex_map[i];
            unsigned long ring_size = ring.size();
            long first = ring[0];
            long vert = first;
            bool closed = false;
            for (unsigned long j = 0; j < ring_size; ++j) {
                for (unsigned long k = 0; k < vertex_to_poly_map[vert].size(); ++k) {
                    long poly = vertex_to_poly_map[vert][k];
                    long m;
                    for (m = 0; m < 3; ++m)
                        if (poly_buffer[poly * 3 + m] == vert)
                            break;
                    long next = (m + 1) % 3;
                    long third = (next + 1) % 3;
                    if (poly_buffer[poly * 3 + next] == i) {
                        vert = poly_buffer[poly * 3 + third];
                        if (vert == first)
                            closed = true;
                        break;
                    }
                }
                if (closed)
                    break;
            }
            if (!closed) {
                CArray<unsigned short> ring_a;
                CArray<unsigned short> ring_b;
                CArray<CArray<unsigned short> > sections;
                BuildRings(i, poly_buffer, vertex_to_poly_map, vertex_to_vertex_map, ring_a, ring_b, sections);
                CArray<unsigned short> section;
                BuildCacheOptimizedVertexList(poly_buffer, vertex_to_poly_map, vertex_to_vertex_map, ring_a, sections, section);
                float score = CalcSectionScore(section, poly_buffer, vertex_to_poly_map);
                if (score > best_score) {
                    best_score = score;
                    best_vert = i;
                }
                if (best_score == 1e30f)
                    break;
            }
        }
    }
    if (best_score == 0.0f) {
        for (long i = 0; i < num_verts; ++i) {
            if (vertex_to_poly_map[i].size()) {
                CArray<unsigned short> ring_a;
                CArray<unsigned short> ring_b;
                CArray<CArray<unsigned short> > sections;
                BuildRings(i, poly_buffer, vertex_to_poly_map, vertex_to_vertex_map, ring_a, ring_b, sections);
                CArray<unsigned short> section;
                BuildCacheOptimizedVertexList(poly_buffer, vertex_to_poly_map, vertex_to_vertex_map, ring_a, sections, section);
                float score = CalcSectionScore(section, poly_buffer, vertex_to_poly_map);
                if (score > best_score) {
                    best_score = score;
                    best_vert = i;
                }
                if (best_score == 1e30f)
                    break;
            }
        }
        if (best_score == 0.0f) {
            for (long i = 0; i < num_verts; ++i)
                if (vertex_to_poly_map[i].size())
                    return i;
        }
    }
    return best_vert;
}
