// CTriangleStripifier2::FillInTriStrip(const CArray<unsigned short>& poly_buffer, const CArray<CArray<long> >& vertex_to_poly_map, const CArray<unsigned short>& vertex_list, const CArray<bool>& poly_mask, CArray<unsigned short>& strip, bool, bool& reversed) -- walk the cache-ordered vertex list and grow one triangle strip: for each list vertex pick, by a 3..19 priority score over its unused masked polygons (how many of the poly's corners are already in the strip, whether the last two strip vertices are a swap pair, and the winding the strip is on), the vertex that best extends the strip; push it, flip the winding, and repeat until no polygon of the vertex scores; a vertex still missing is then pushed after a swap vertex
// Leaf: ?FillInTriStrip@CTriangleStripifier2@@AAEXABV?$CArray@G@@ABV?$CArray@V?$CArray@J@@@@0ABV?$CArray@_N@@AAV2@_NAA_N@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00F3BDD0, 2998 bytes, `ret 1Ch`; the sixth argument (bool) is not read.
// Grade: DIFFER(2976v2998) -- register allocation only in the scoring loop: retail keeps the poly index in ebp and the best score in edi, the build swaps them (declaration order, a hoisted poly local and loop-index types do not move it); the store order inside the found-branches follows
// Callees: vector<long>(n, value, alloc) 0x00512630, vector<unsigned short>::_M_insert_overflow 0x005135D0 (first two pushes;
//   the later pushes expand it: malloc 0x00BFEA0E / memmove 0x00BFEAE6 / free 0x00BFEA14).
// Locals: the list position of each vertex (fill 0x80000000), a done flag per poly_buffer entry, a used flag per vertex and
//   the strip counter per vertex (fill 0xC0000000); destroyed in reverse order.
#define FABLE_RETAIL_CTRIANGLESTRIPIFIER2
#define FABLE_RETAIL_STLP_VECTOR
#include "engine/CTriangleStripifier2.h"

void CTriangleStripifier2::FillInTriStrip(const CArray<unsigned short>& poly_buffer, const CArray<CArray<long> >& vertex_to_poly_map, const CArray<unsigned short>& vertex_list, const CArray<bool>& poly_mask, CArray<unsigned short>& strip, bool, bool& reversed)
{
    unsigned long num_indices = poly_buffer.size();
    unsigned long num_verts = vertex_to_poly_map.size();
    CArray<long> vertex_list_position(num_verts, 0x80000000);
    for (unsigned long i = 0; i < vertex_list.size(); ++i)
        vertex_list_position[vertex_list[i]] = i;
    strip.clear();
    CArray<bool> poly_used(num_indices, false);
    CArray<bool> vertex_in_strip(num_verts, false);
    CArray<long> vertex_strip_index(num_verts, 0xc0000000);
    vertex_in_strip[vertex_list[0]] = true;
    vertex_in_strip[vertex_list[1]] = true;
    strip.push_back(vertex_list[0]);
    strip.push_back(vertex_list[1]);
    vertex_strip_index[vertex_list[0]] = 0;
    vertex_strip_index[vertex_list[1]] = 1;
    long strip_count = 2;
    long last_last_vertex = vertex_list[0];
    long last_vertex = vertex_list[1];
    for (unsigned long i = 2; i < vertex_list.size(); ++i) {
        long vert = vertex_list[i];
        long next_vert = -1;
        if (i < vertex_list.size() - 1)
            next_vert = vertex_list[i + 1];
        for (;;) {
            long best_vert = -1;
            long best_poly = -1;
            long best_score = 0;
            for (unsigned long j = 0; j < vertex_to_poly_map[vert].size(); ++j) {
                long poly = vertex_to_poly_map[vert][j];
                if (poly_used[poly] || !poly_mask[poly])
                    continue;
                long used = 0;
                bool later = false;
                for (long k = 0; k < 3; ++k) {
                    long pv = poly_buffer[poly * 3 + k];
                    if (vertex_in_strip[pv])
                        ++used;
                    else if (vertex_list_position[pv] > (long)i) {
                        later = true;
                        break;
                    }
                }
                if (later)
                    continue;
                if (vertex_in_strip[vert] && used < 3)
                    continue;
                if (used == 0) {
                    if (best_score < 3 && last_vertex == last_last_vertex) {
                        best_vert = vert; best_poly = -1; best_score = 3;
                    }
                    if (best_score < 2 && last_vertex != last_last_vertex) {
                        best_vert = last_vertex; best_poly = -1; best_score = 2;
                    }
                } else if (used == 1) {
                    if (best_score < 7 && last_vertex == last_last_vertex) {
                        for (long m = 0; m < 3; ++m) {
                            long a = (m + 1) % 3;
                            long b = (a + 1) % 3;
                            if (!reversed) { long t = a; a = b; b = t; }
                            if (poly_buffer[poly * 3 + m] == last_vertex && poly_buffer[poly * 3 + a] == vert && poly_buffer[poly * 3 + b] == next_vert) {
                                best_vert = vert; best_poly = -1; best_score = 7;
                                break;
                            }
                        }
                    }
                    if (best_score < 6 && last_vertex == last_last_vertex) {
                        best_vert = vert; best_poly = -1; best_score = 6;
                    }
                    if (best_score < 5 && last_vertex != last_last_vertex) {
                        best_vert = last_vertex; best_poly = -1; best_score = 5;
                    }
                } else if (used == 2) {
                    if (best_score < 13) {
                        for (long m = 0; m < 3; ++m) {
                            long a = (m + 1) % 3;
                            long b = (a + 1) % 3;
                            if (reversed) { long t = a; a = b; b = t; }
                            if (poly_buffer[poly * 3 + m] == vert && poly_buffer[poly * 3 + a] == last_last_vertex && poly_buffer[poly * 3 + b] == last_vertex) {
                                best_vert = vert; best_poly = poly; best_score = 13;
                                break;
                            }
                        }
                    }
                    if (best_score < 12 && last_vertex == last_last_vertex) {
                        for (long m = 0; m < 3; ++m) {
                            long a = (m + 1) % 3;
                            long b = (a + 1) % 3;
                            if (!reversed) { long t = a; a = b; b = t; }
                            if (poly_buffer[poly * 3 + m] == last_vertex && poly_buffer[poly * 3 + b] == vert) {
                                best_vert = poly_buffer[poly * 3 + a]; best_poly = -1; best_score = 12;
                                break;
                            }
                        }
                    }
                    if (best_score < 11 && last_vertex != last_last_vertex) {
                        for (long m = 0; m < 3; ++m) {
                            long a = (m + 1) % 3;
                            if (reversed)
                                a = (a + 1) % 3;
                            if (poly_buffer[poly * 3 + m] == vert && poly_buffer[poly * 3 + a] == last_vertex) {
                                best_vert = last_vertex; best_poly = -1; best_score = 11;
                                break;
                            }
                        }
                    }
                    if (best_score < 10 && last_vertex == last_last_vertex) {
                        best_vert = vert; best_poly = -1; best_score = 10;
                    }
                    if (best_score < 9 && last_vertex != last_last_vertex) {
                        best_vert = last_vertex; best_poly = -1; best_score = 9;
                    }
                } else {
                    if (best_score < 19) {
                        for (long m = 0; m < 3; ++m) {
                            long a = (m + 1) % 3;
                            long b = (a + 1) % 3;
                            if (reversed) { long t = a; a = b; b = t; }
                            if (poly_buffer[poly * 3 + a] == last_last_vertex && poly_buffer[poly * 3 + b] == last_vertex) {
                                best_vert = poly_buffer[poly * 3 + m]; best_poly = poly; best_score = 19;
                                break;
                            }
                        }
                    }
                    if (best_score < 17 && last_vertex != last_last_vertex) {
                        for (long m = 0; m < 3; ++m) {
                            if (poly_buffer[poly * 3 + m] == last_vertex) {
                                best_vert = last_vertex; best_poly = -1; best_score = 17;
                                break;
                            }
                        }
                    }
                    if (best_score < 18 && last_vertex == last_last_vertex) {
                        for (long m = 0; m < 3; ++m) {
                            long a = (m + 1) % 3;
                            if (!reversed)
                                a = (a + 1) % 3;
                            if (poly_buffer[poly * 3 + m] == last_vertex) {
                                best_vert = poly_buffer[poly * 3 + a]; best_poly = -1; best_score = 18;
                                break;
                            }
                        }
                    }
                    if (best_score < 16 && last_vertex == last_last_vertex) {
                        best_vert = vert; best_poly = -1; best_score = 16;
                    }
                    if (best_score < 15 && last_vertex != last_last_vertex) {
                        best_vert = last_vertex; best_poly = -1; best_score = 15;
                    }
                }
            }
            if (best_vert < 0)
                break;
            if (best_poly >= 0)
                poly_used[best_poly] = true;
            strip.push_back((unsigned short)best_vert);
            if (!vertex_in_strip[best_vert]) {
                vertex_in_strip[best_vert] = true;
                vertex_strip_index[vert] = ++strip_count;
            }
            last_last_vertex = last_vertex;
            last_vertex = best_vert;
            reversed = !reversed;
        }
        if (!vertex_in_strip[vert]) {
            if (last_vertex != last_last_vertex) {
                strip.push_back((unsigned short)last_vertex);
                last_last_vertex = last_vertex;
                reversed = !reversed;
            }
            strip.push_back((unsigned short)vert);
            vertex_in_strip[vert] = true;
            vertex_strip_index[vert] = ++strip_count;
            reversed = !reversed;
            last_vertex = vert;
        }
    }
}
