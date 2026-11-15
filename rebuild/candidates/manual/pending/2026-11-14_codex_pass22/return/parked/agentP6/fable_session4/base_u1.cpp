// 00F3E120 CTriangleStripifier2::FindBestStartVertex -- select an open-fan strip start, then score all used vertices as fallback.
// Authenticated vendor context: protected in-class overflow, resize-to-insert forwarding, and protected allocate-and-copy; native const-reference resize ABI retained.
// Applying this genuine vendor declaration context to the native TU is inferred; no target match is claimed.
// Inline-budget: T70 retains native ring growth; the incidence while keeps filter growth called.
// Leaf: ?FindBestStartVertex@CTriangleStripifier2@@AAEJABV?$CArray@G@@ABV?$CArray@V?$CArray@J@@@@ABV?$CArray@V?$CArray@G@@@@@Z
// Flags: /Gr /d2inlT70
// Retail: Fable.exe TLC VC7.1 @ 0x00F3E120, 1411 bytes, thiscall ret12.
// Root locals, including poly/id/id1/id2, are recovered from original PDB scopes; see NAMING.md for helper-local evidence and remaining uncertainty.
#define FABLE_RETAIL_CBASECLASSNONCOPYABLE_ORIGINAL_ASSIGNMENT_RESULT_PASS22
#define FABLE_RETAIL_PASS22_SCOREBOARD_ORIGINAL_OVERFLOW_SCOPE
#define FABLE_RETAIL_STLP_VECTOR
#define FABLE_RETAIL_STLP_ORIGINAL_COPY_DISPATCH_PASS22
#define FABLE_RETAIL_STLP_ORIGINAL_TRUE_UNINIT_COPY_PASS22
#define FABLE_RETAIL_STLP_ORIGINAL_COPY_TRIVIAL_BACKWARD_PASS22
#define FABLE_RETAIL_STLP_VECTOR_C
#define FABLE_RETAIL_STLP_VECTOR_ORIGINAL_RESIZE_INSERT_CHAIN_PASS22
#define FABLE_RETAIL_STLP_VECTOR_ALLOCATE_COPY_ORIGINAL_SCOPE_PASS22
#define FABLE_RETAIL_CARRAY_PUSHBACK
#define FABLE_RETAIL_CARRAY
#define FABLE_RETAIL_CARRAY_ORIGINAL_COMPLETE_DECLARATIONS_PASS22
#define FABLE_RETAIL_CARRAY_ORIGINAL_USER_SPECIAL_MEMBERS_PASS22
#define FABLE_RETAIL_CARRAY_ORIGINAL_INDEX_BASE_PASS22
#define FABLE_RETAIL_PASS22_TRIANGLE_BEST_START
#define FABLE_RETAIL_PASS22_TRIANGLE_ORIGINAL_COMPLETE_DECLARATIONS
#include "engine/CTriangleStripifier2.h"

// Original definition order proven by both raw PDB C13 line tables.

// Original source line 183.
void CTriangleStripifier2::BuildRings(long start_vert,
    const CArray<unsigned short>& poly_buffer,
    const CArray<CArray<long> >& vertex_to_poly_map,
    const CArray<CArray<unsigned short> >& vertex_link_map,
    CArray<unsigned short>& vertex_to_ring_map,
    CArray<unsigned short>& vertex_to_ring_pos_map,
    CArray<CArray<unsigned short> >& rings)
{
    long vertex_count = vertex_to_poly_map.size();
    CArray<long> vertex_to_prev_ring_vert_map(vertex_count, -1L);
    vertex_to_ring_map.clear();
    vertex_to_ring_map.resize(vertex_count, (unsigned short)0xffff);
    vertex_to_ring_pos_map.clear();
    vertex_to_ring_pos_map.resize(vertex_count, (unsigned short)0xffff);
    rings.clear();
    CArray<unsigned short> prev_ring_list;
    prev_ring_list.push_back(start_vert);
    vertex_to_ring_map[start_vert] = 0;
    vertex_to_ring_pos_map[start_vert] = 0;
    rings.push_back(prev_ring_list);
    long ring_id = 1;
    while (prev_ring_list.size() > 0) {
        if (ring_id == 0xffff)
            break;
        CArray<unsigned short> ring_list;
        for (long pv = 0; pv < prev_ring_list.size(); ++pv) {
            long prev_ring_vert = prev_ring_list[pv];
            for (long cv = 0; cv < vertex_link_map[prev_ring_vert].size(); ++cv) {
                long new_vert = vertex_link_map[prev_ring_vert][cv];
                if (vertex_to_ring_map[new_vert] == 0xffff) {
                    vertex_to_ring_map[new_vert] = ring_id;
                    vertex_to_prev_ring_vert_map[new_vert] = prev_ring_vert;
                    ring_list.push_back(new_vert);
                }
            }
        }
        if (ring_list.size() == 0)
            break;
        CArray<unsigned short> ordered_ring_list;
        CArray<bool> verts_in_ordered_list(vertex_count, false);
        long current_vert = ring_list[0];
        long prev_ring_vert_pos = 0;
        while (true) {
            bool start_searching = true;
            long verts_searched = 0;
            long current_prev_ring_link = rings[ring_id-1][prev_ring_vert_pos];
            while (start_searching) {
                start_searching = false;
                for (long pl = 0; pl < vertex_to_poly_map[current_vert].size(); ++pl) {
                    long poly = vertex_to_poly_map[current_vert][pl];
                    long id = 0;
                    for (; id < 3; ++id)
                        if (poly_buffer[poly*3+id] == current_vert)
                            break;
                    long id1 = (id+1)%3;
                    long id2 = (id1+1)%3;
                    if (poly_buffer[poly*3+id1] == current_prev_ring_link) {
                        long fvert = poly_buffer[poly*3+id2];
                        if (vertex_to_ring_map[fvert] == ring_id && !verts_in_ordered_list[fvert]) {
                            current_vert = fvert;
                            ++verts_searched;
                            if (verts_searched <= ring_list.size())
                                start_searching = true;
                        }
                        break;
                    }
                }
            }
            vertex_to_ring_pos_map[current_vert] = ordered_ring_list.size();
            ordered_ring_list.push_back(current_vert);
            verts_in_ordered_list[current_vert] = true;
            if (ordered_ring_list.size() == ring_list.size())
                break;
            for (;;) {
                for (long pl = 0; pl < vertex_to_poly_map[current_vert].size(); ++pl) {
                    long poly = vertex_to_poly_map[current_vert][pl];
                    long id = 0;
                    for (; id < 3; ++id)
                        if (poly_buffer[poly*3+id] == current_vert)
                            break;
                    long id1 = (id+2)%3;
                    long id2 = (id1+2)%3;
                    if (poly_buffer[poly*3+id1] == current_prev_ring_link) {
                        long fvert = poly_buffer[poly*3+id2];
                        if (vertex_to_ring_map[fvert] == ring_id && !verts_in_ordered_list[fvert]) {
                            current_vert = fvert;
                            goto found_next_vertex;
                        }
                    }
                }
                for (long vt = 0; vt < vertex_link_map[current_prev_ring_link].size(); ++vt) {
                    long link_vert = vertex_link_map[current_prev_ring_link][vt];
                    if (vertex_to_ring_map[link_vert] == ring_id && !verts_in_ordered_list[link_vert]) {
                        current_vert = link_vert;
                        goto found_next_vertex;
                    }
                }
                ++prev_ring_vert_pos;
                current_prev_ring_link = rings[ring_id-1][prev_ring_vert_pos];
            }
        found_next_vertex:
            continue;
        }
        rings.push_back(ordered_ring_list);
        prev_ring_list = ordered_ring_list;
        ++ring_id;
    }
}

// Original source line 383.
float CTriangleStripifier2::CalcSectionScore(const CArray<unsigned short>& vert_list,
    const CArray<unsigned short>& poly_buffer, const CArray<CArray<long> >& vertex_to_poly_map)
{
    CArray<bool> poly_mask;
    long poly_count = CalcSectionPolyMask(poly_buffer, vertex_to_poly_map, vert_list, poly_mask);
    if (poly_count == 0)
        return 0.0f;
    CArray<unsigned short> new_vert_list(vert_list);
    StripUnusedVerts(new_vert_list, vertex_to_poly_map, poly_mask);
    long unfinished_vertex_count = 0;
    for (long vt = 0; vt < new_vert_list.size(); ++vt) {
        bool finished = true;
        long vert = new_vert_list[vt];
        for (long pl = 0; pl < vertex_to_poly_map[vert].size(); ++pl) {
            long poly = vertex_to_poly_map[vert][pl];
            if (!poly_mask[poly]) {
                finished = false;
                break;
            }
        }
        if (!finished)
            ++unfinished_vertex_count;
    }
    if (unfinished_vertex_count == 0)
        return 1e30f;
    else
        return (float)poly_count / unfinished_vertex_count;
}

// Original source line 442.
long CTriangleStripifier2::CalcSectionPolyMask(const CArray<unsigned short>& poly_buffer,
    const CArray<CArray<long> >& vertex_to_poly_map,
    const CArray<unsigned short>& vertex_list, CArray<bool>& mask)
{
    CArray<bool> polys_processed(poly_buffer.size()/3, false);
    CArray<bool> verts_in_section(vertex_to_poly_map.size(), false);
    CArray<long> vert_cache_stamp(vertex_to_poly_map.size(), -0x40000000L);
    long current_cache_stamp = 0;
    for (long vt = 0; vt < vertex_list.size(); ++vt) {
        long vert = vertex_list[vt];
        verts_in_section[vert] = true;
        ++current_cache_stamp;
        vert_cache_stamp[vert] = current_cache_stamp;
    }
    mask.clear();
    mask.resize(poly_buffer.size()/3, false);
    long poly_count = 0;
    for (long vt = 0; vt < vertex_list.size(); ++vt) {
        long vert = vertex_list[vt];
        for (long pl = 0; pl < vertex_to_poly_map[vert].size(); ++pl) {
            long poly = vertex_to_poly_map[vert][pl];
            if (!polys_processed[poly]) {
                if (verts_in_section[poly_buffer[poly*3]] &&
                    verts_in_section[poly_buffer[poly*3+1]] &&
                    verts_in_section[poly_buffer[poly*3+2]]) {
                    long min_stamp = ((vert_cache_stamp[poly_buffer[poly*3]] < vert_cache_stamp[poly_buffer[poly*3+1]] ? vert_cache_stamp[poly_buffer[poly*3]] : vert_cache_stamp[poly_buffer[poly*3+1]]) < vert_cache_stamp[poly_buffer[poly*3+2]] ? (vert_cache_stamp[poly_buffer[poly*3]] < vert_cache_stamp[poly_buffer[poly*3+1]] ? vert_cache_stamp[poly_buffer[poly*3]] : vert_cache_stamp[poly_buffer[poly*3+1]]) : vert_cache_stamp[poly_buffer[poly*3+2]]);
                    long max_stamp = ((vert_cache_stamp[poly_buffer[poly*3]] > vert_cache_stamp[poly_buffer[poly*3+1]] ? vert_cache_stamp[poly_buffer[poly*3]] : vert_cache_stamp[poly_buffer[poly*3+1]]) > vert_cache_stamp[poly_buffer[poly*3+2]] ? (vert_cache_stamp[poly_buffer[poly*3]] > vert_cache_stamp[poly_buffer[poly*3+1]] ? vert_cache_stamp[poly_buffer[poly*3]] : vert_cache_stamp[poly_buffer[poly*3+1]]) : vert_cache_stamp[poly_buffer[poly*3+2]]);
                    if (max_stamp-min_stamp < 15) {
                        mask[poly] = true;
                        ++poly_count;
                    }
                }
                polys_processed[poly] = true;
            }
        }
    }
    return poly_count;
}

// Original source line 501.
void CTriangleStripifier2::StripUnusedVerts(CArray<unsigned short>& vertex_list,
    const CArray<CArray<long> >& vertex_to_poly_map, const CArray<bool>& poly_mask)
{
    CArray<unsigned short> new_list;
    for (long vt = 0; vt < vertex_list.size(); ++vt) {
        long vert = vertex_list[vt];
        long pl = 0;
        while (pl < vertex_to_poly_map[vert].size()) {
            long poly = vertex_to_poly_map[vert][pl];
            if (poly_mask[poly]) {
                new_list.push_back(vert);
                break;
            }
            ++pl;
        }
    }
    vertex_list = new_list;
}

// Original source line 536.
void CTriangleStripifier2::BuildCacheOptimizedVertexList(
    const CArray<unsigned short>& poly_buffer,
    const CArray<CArray<long> >& vertex_to_poly_map,
    const CArray<CArray<unsigned short> >& vertex_link_map,
    const CArray<unsigned short>& vertex_to_ring_map,
    const CArray<CArray<unsigned short> >& rings,
    CArray<unsigned short>& dest_vert_list)
{
    long vertex_count = vertex_to_poly_map.size();
    dest_vert_list.clear();
    CArray<bool> verts_processed(vertex_count, false);
    CArray<long> vert_cache_stamp(vertex_count, -1L);
    long current_cache_stamp = 0;
    for (long ring = 0; ring < rings.size(); ++ring) {
        for (long vt = 0; vt < rings[ring].size(); ++vt) {
            long vert = rings[ring][vt];
            long look_ahead_stamp = current_cache_stamp+1;
            if (ring > 0 && ring < rings.size()-1) {
                for (long lavt = 0; lavt < rings[ring+1].size(); ++lavt) {
                    long lavert = rings[ring+1][lavt];
                    bool found_linked_vert = false;
                    ++look_ahead_stamp;
                    for (long lv = 0; lv < vertex_link_map[lavert].size(); ++lv) {
                        long link_vert = vertex_link_map[lavert][lv];
                        if (vertex_to_ring_map[link_vert] == ring) {
                            if (vert_cache_stamp[link_vert] >= 0) {
                                if (look_ahead_stamp-vert_cache_stamp[link_vert] >= 16)
                                    goto skip_to_next_ring;
                                found_linked_vert = true;
                            } else if (link_vert == vert) {
                                if (look_ahead_stamp-(current_cache_stamp+1) >= 16)
                                    goto skip_to_next_ring;
                                found_linked_vert = true;
                            }
                        }
                    }
                    if (!found_linked_vert)
                        break;
                }
            }
            verts_processed[vert] = true;
            dest_vert_list.push_back(vert);
            ++current_cache_stamp;
            vert_cache_stamp[vert] = current_cache_stamp;
        }
    skip_to_next_ring:
        continue;
    }
}

// Original source line 1303.
long CTriangleStripifier2::FindBestStartVertex(const CArray<unsigned short>& poly_buffer,
    const CArray<CArray<long> >& vertex_to_poly_map, const CArray<CArray<unsigned short> >& vertex_link_map)
{
    long vertex_count;
    float best_score = 0.0f;
    long best_start_vert = -1;
    vertex_count = vertex_to_poly_map.size();
    for (long start_vert = 0; start_vert < vertex_count; ++start_vert) {
        if (vertex_to_poly_map[start_vert].size() == 0)
            continue;
        long surround_vert = vertex_link_map[start_vert][0];
        long start_surround_vert = surround_vert;
        for (long verts_searched = 0; verts_searched < vertex_link_map[start_vert].size(); ++verts_searched) {
            for (long pl = 0; pl < vertex_to_poly_map[surround_vert].size(); ++pl) {
                long poly = vertex_to_poly_map[surround_vert][pl];
                long id = 0;
                for (; id < 3; ++id)
                    if (poly_buffer[poly*3+id] == surround_vert)
                        break;
                long id1 = (id+1)%3;
                long id2 = (id1+1)%3;
                if (poly_buffer[poly*3+id1] == start_vert) {
                    surround_vert = poly_buffer[poly*3+id2];
                    if (surround_vert == start_surround_vert)
                        goto skip_to_next_start_vert;
                    break;
                }
            }
        }
        {
            CArray<unsigned short> vertex_to_ring_map;
            CArray<unsigned short> vertex_to_ring_pos_map;
            CArray<CArray<unsigned short> > rings;
            BuildRings(start_vert, poly_buffer, vertex_to_poly_map, vertex_link_map, vertex_to_ring_map, vertex_to_ring_pos_map, rings);
            CArray<unsigned short> vertex_list;
            BuildCacheOptimizedVertexList(poly_buffer, vertex_to_poly_map, vertex_link_map, vertex_to_ring_map, rings, vertex_list);
            float score = CalcSectionScore(vertex_list, poly_buffer, vertex_to_poly_map);
            if (score > best_score) {
                best_score = score;
                best_start_vert = start_vert;
            }
            if (best_score == 1e30f)
                break;
        }
    skip_to_next_start_vert:
        continue;
    }
    if (best_score == 0.0f) {
        for (long start_vert = 0; start_vert < vertex_count; ++start_vert) {
            if (vertex_to_poly_map[start_vert].size() == 0)
                continue;
                CArray<unsigned short> vertex_to_ring_map;
                CArray<unsigned short> vertex_to_ring_pos_map;
                CArray<CArray<unsigned short> > rings;
                CArray<unsigned short> vertex_list;
                BuildRings(start_vert, poly_buffer, vertex_to_poly_map, vertex_link_map, vertex_to_ring_map, vertex_to_ring_pos_map, rings);
                BuildCacheOptimizedVertexList(poly_buffer, vertex_to_poly_map, vertex_link_map, vertex_to_ring_map, rings, vertex_list);
                float score = CalcSectionScore(vertex_list, poly_buffer, vertex_to_poly_map);
                if (score > best_score) {
                    best_score = score;
                    best_start_vert = start_vert;
                }
                if (best_score == 1e30f)
                    break;
        }
    }
        if (best_score == 0.0f) {
            for (long start_vert = 0; start_vert < vertex_count; ++start_vert) {
                if (vertex_to_poly_map[start_vert].size() == 0)
                    continue;
                return start_vert;
            }
        }
    return best_start_vert;
}
