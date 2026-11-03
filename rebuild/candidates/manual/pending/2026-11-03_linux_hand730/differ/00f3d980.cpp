// CTriangleStripifier2::BuildRings(long start_vertex, const CArray<unsigned short>& poly_buffer, const CArray<CArray<long> >& vertex_to_poly_map, const CArray<CArray<unsigned short> >& vertex_to_vertex_map, CArray<unsigned short>& vertex_ring, CArray<unsigned short>& vertex_ring_position, CArray<CArray<unsigned short> >& rings) -- grow breadth-first vertex rings around the start vertex and order each ring by walking the fans around the previous ring
// Leaf: ?BuildRings@CTriangleStripifier2@@AAEXJABV?$CArray@G@@ABV?$CArray@V?$CArray@J@@@@ABV?$CArray@V?$CArray@G@@@@AAV2@3AAV4@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00F3D980, 1947 bytes, `ret 1Ch`.
// Grade: DIFFER(2301v1947) -- per-site inlining: retail calls push_back 0x009B4A20 out of line for the section list and the CArray<unsigned short> copy ctor 0x00A87090 inside rings.push_back, the build expands both; retail also strength-reduces level*12 for rings[level-1] (compared with 0xBFFF4), the build recomputes it per level
#define FABLE_RETAIL_CTRIANGLESTRIPIFIER2
#define FABLE_RETAIL_STLP_VECTOR
#include "engine/CTriangleStripifier2.h"

void CTriangleStripifier2::BuildRings(long start_vertex, const CArray<unsigned short>& poly_buffer,
    const CArray<CArray<long> >& vertex_to_poly_map, const CArray<CArray<unsigned short> >& vertex_to_vertex_map,
    CArray<unsigned short>& vertex_ring, CArray<unsigned short>& vertex_ring_position,
    CArray<CArray<unsigned short> >& rings)
{
    long num_verts = vertex_to_poly_map.size();
    CArray<long> vertex_parent(num_verts, -1);
    vertex_ring.clear();
    vertex_ring.resize(num_verts, 0xffff);
    vertex_ring_position.clear();
    vertex_ring_position.resize(num_verts, 0xffff);
    rings.clear();
    CArray<unsigned short> ring;
    ring.push_back(start_vertex);
    vertex_ring[start_vertex] = 0;
    vertex_ring_position[start_vertex] = 0;
    rings.push_back(ring);
    long level = 1;
    unsigned long ring_size = ring.size();
    while (ring_size > 0 && level != 0xffff) {
        CArray<unsigned short> next_ring;
        for (unsigned long j = 0; j < ring_size; ++j) {
            long vert = ring[j];
            for (unsigned long k = 0; k < vertex_to_vertex_map[vert].size(); ++k) {
                long neighbour = vertex_to_vertex_map[vert][k];
                if (vertex_ring[neighbour] == 0xffff) {
                    vertex_ring[neighbour] = (unsigned short)level;
                    vertex_parent[neighbour] = vert;
                    next_ring.push_back(neighbour);
                }
            }
        }
        unsigned long next_size = next_ring.size();
        if (next_size == 0)
            break;
        CArray<unsigned short> section;
        CArray<bool> used(num_verts, false);
        long vert = next_ring[0];
        unsigned long prev_index = 0;
        unsigned long section_size = 0;
        for (;;) {
            long prev = rings[level - 1][prev_index];
            unsigned long steps = 0;
            for (;;) {
                unsigned long k;
                long next_vert = -1;
                for (k = 0; k < vertex_to_poly_map[vert].size(); ++k) {
                    long poly = vertex_to_poly_map[vert][k];
                    long m;
                    for (m = 0; m < 3; ++m)
                        if (poly_buffer[poly * 3 + m] == vert)
                            break;
                    long n1 = (m + 1) % 3;
                    long n2 = (n1 + 1) % 3;
                    if (poly_buffer[poly * 3 + n1] == prev) {
                        next_vert = poly_buffer[poly * 3 + n2];
                        break;
                    }
                }
                if (next_vert == -1 || vertex_ring[next_vert] != level || used[next_vert])
                    break;
                vert = next_vert;
                if (++steps > next_size)
                    break;
            }
            vertex_ring_position[vert] = (unsigned short)section_size;
            section.push_back(vert);
            used[vert] = true;
            section_size = section.size();
            if (section_size == next_size)
                break;
            for (;;) {
                long found = -1;
                for (unsigned long k = 0; k < vertex_to_poly_map[vert].size(); ++k) {
                    long poly = vertex_to_poly_map[vert][k];
                    long m;
                    for (m = 0; m < 3; ++m)
                        if (poly_buffer[poly * 3 + m] == vert)
                            break;
                    long p1 = (m + 2) % 3;
                    if (poly_buffer[poly * 3 + p1] == prev) {
                        long v = poly_buffer[poly * 3 + (p1 + 2) % 3];
                        if (vertex_ring[v] == level && !used[v]) {
                            found = v;
                            break;
                        }
                    }
                }
                if (found == -1) {
                    for (unsigned long k = 0; k < vertex_to_vertex_map[prev].size(); ++k) {
                        long v = vertex_to_vertex_map[prev][k];
                        if (vertex_ring[v] == level && !used[v]) {
                            found = v;
                            break;
                        }
                    }
                }
                if (found != -1) {
                    vert = found;
                    break;
                }
                ++prev_index;
                prev = rings[level - 1][prev_index];
            }
        }
        rings.push_back(section);
        ring = section;
        ++level;
        ring_size = ring.size();
    }
}
