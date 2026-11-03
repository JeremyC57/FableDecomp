// CTriangleStripifier2::GenerateTriangleListAndStrip(CArray<unsigned short>& poly_buffer, CArray<unsigned short>& poly_list, CArray<unsigned short>& strip) -- the index-list overload: split the mesh into connected islands, order each island's polygons and vertices for the post-transform cache and append the island's polygons to the list and its triangle strip (joined by degenerate links) to the strip
// Leaf: ?GenerateTriangleListAndStrip@CTriangleStripifier2@@QAEXAAV?$CArray@G@@00@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00F3EA50, 2379 bytes, `ret 0Ch`. Catalogued as "GenerateStrips" (descriptive): the PDB
// Grade: DIFFER(2603v2379) -- per-site inlining: retail calls _M_insert_overflow 0x005135D0 at the two strip joins and the strip append loop and expands it only at the three poly_list pushes, the build expands it in the append loop; retail keeps 0 in ebx and last_vertex in its frame slot, the build keeps last_vertex in ebx
//   name sets (Ego_r 0x00BDA9B0 / FableWin 0x03325190) carry ?GenerateTriangleListAndStrip@CTriangleStripifier2@@QAEXAAV?$CArray@G@@00@Z,
//   and its sole caller, the virtual overload 0x00F3F3A0, passes three CArray<unsigned short> (NAME_FIXES.tsv).
// Callees: vector<bool>(n, const bool&, const allocator&) 0x00451583, BuildVertexAndPolyMaps 0x00F3E6B0, GetMeshIslandSubsection
//   0x00F3D320, FindBestStartVertex 0x00F3E120, BuildRings 0x00F3D980, BuildCacheOptimizedVertexList 0x00F3BA70,
//   CalcSectionPolyMask 0x00F3CF50, StripUnusedVerts 0x00F3B970, vector<bool>::_M_fill_insert 0x004676EF, FillInPolygons
//   0x00F3C990, FillInTriStrip 0x00F3BDD0, vector<unsigned short>::_M_insert_overflow 0x005135D0, malloc 0x00BFEA0E, memmove
//   0x00BFEAE6, free 0x00BFEA14.
#define FABLE_RETAIL_CTRIANGLESTRIPIFIER2
#define FABLE_RETAIL_STLP_VECTOR
#include "engine/CTriangleStripifier2.h"

void CTriangleStripifier2::GenerateTriangleListAndStrip(CArray<unsigned short>& poly_buffer, CArray<unsigned short>& poly_list,
    CArray<unsigned short>& strip)
{
    long num_polys = poly_buffer.size() / 3;
    if (num_polys == 0)
        return;
    long polys_done = 0;
    CArray<bool> poly_mask(num_polys, true);
    poly_list.clear();
    strip.clear();
    CArray<CArray<long> > vertex_to_poly_map;
    CArray<CArray<unsigned short> > vertex_to_vertex_map;
    BuildVertexAndPolyMaps(poly_buffer, vertex_to_poly_map, vertex_to_vertex_map);
    bool reversed = false;
    long last_vertex = -1;
    while (polys_done < num_polys) {
        CArray<long> sub_section_to_global_poly_map;
        CArray<unsigned short> subsection;
        GetMeshIslandSubsection(poly_buffer, poly_mask, vertex_to_poly_map, subsection, sub_section_to_global_poly_map);
        CArray<CArray<long> > sub_vertex_to_poly_map;
        CArray<CArray<unsigned short> > sub_vertex_to_vertex_map;
        BuildVertexAndPolyMaps(subsection, sub_vertex_to_poly_map, sub_vertex_to_vertex_map);
        long start_vertex = FindBestStartVertex(subsection, sub_vertex_to_poly_map, sub_vertex_to_vertex_map);
        CArray<unsigned short> vertex_ring;
        CArray<unsigned short> vertex_ring_position;
        CArray<CArray<unsigned short> > rings;
        BuildRings(start_vertex, subsection, sub_vertex_to_poly_map, sub_vertex_to_vertex_map, vertex_ring, vertex_ring_position, rings);
        CArray<unsigned short> vertex_list;
        BuildCacheOptimizedVertexList(subsection, sub_vertex_to_poly_map, sub_vertex_to_vertex_map, vertex_ring, rings, vertex_list);
        CArray<bool> section_poly_mask;
        bool all_polys = false;
        if (CalcSectionPolyMask(subsection, sub_vertex_to_poly_map, vertex_list, section_poly_mask)) {
            StripUnusedVerts(vertex_list, sub_vertex_to_poly_map, section_poly_mask);
            CalcSectionPolyMask(subsection, sub_vertex_to_poly_map, vertex_list, section_poly_mask);
        } else {
            section_poly_mask.clear();
            section_poly_mask.resize(subsection.size() / 3, true);
            all_polys = true;
        }
        CArray<long> poly_order;
        CArray<unsigned short> section_polys;
        CArray<unsigned short> section_strip;
        FillInPolygons(subsection, sub_vertex_to_poly_map, vertex_list, section_poly_mask, poly_order, section_polys, all_polys);
        FillInTriStrip(subsection, sub_vertex_to_poly_map, vertex_list, section_poly_mask, section_strip, all_polys, reversed);
        if (last_vertex >= 0) {
            strip.push_back(last_vertex);
            strip.push_back(section_strip[0]);
        }
        for (unsigned long i = 0; i < section_strip.size(); ++i)
            strip.push_back(section_strip[i]);
        last_vertex = strip.back();
        polys_done += poly_order.size();
        for (unsigned long p = 0; p < poly_order.size(); ++p) {
            poly_mask[sub_section_to_global_poly_map[poly_order[p]]] = false;
            poly_list.push_back(section_polys[p * 3]);
            poly_list.push_back(section_polys[p * 3 + 1]);
            poly_list.push_back(section_polys[p * 3 + 2]);
        }
            }
    long num_indices = num_polys * 3;
    CArray<bool> vertex_used;
    for (long i = 0; i < num_indices; ++i) {
        unsigned short vert = poly_buffer[i];
        if (vert >= vertex_used.size())
            vertex_used.resize(vert + 1, false);
        if (!vertex_used[vert])
            vertex_used[vert] = true;
    }
}
