// Behaviour test for 0x00F3BDD0 CTriangleStripifier2::FillInTriStrip: from a cache-ordered vertex list, grow one triangle strip over the
// masked polygons: the first two list vertices start the strip; each later vertex is extended by the best-scoring unused polygon
// (3..19 by corners already in the strip, swap pairs and the winding), pushing vertices and flipping `reversed`, and a vertex still
// missing is pushed after a swap vertex. Model of the row's logic (std::vector for CArray) checked against hand-traced strips.
#include <cstdio>
#include <vector>
typedef std::vector<unsigned short> A16;
static void FillInTriStrip(const A16& poly_buffer, const std::vector<std::vector<long> >& vertex_to_poly_map, const A16& vertex_list, const std::vector<bool>& poly_mask, A16& strip, bool, bool& reversed)
{
    unsigned long num_indices = poly_buffer.size();
    unsigned long num_verts = vertex_to_poly_map.size();
    std::vector<long> vertex_list_position(num_verts, (long)0x80000000);
    for (unsigned long i = 0; i < vertex_list.size(); ++i)
        vertex_list_position[vertex_list[i]] = i;
    strip.clear();
    std::vector<bool> poly_used(num_indices, false);
    std::vector<bool> vertex_in_strip(num_verts, false);
    std::vector<long> vertex_strip_index(num_verts, (long)0xc0000000);
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
                                best_poly = -1; best_vert = vert; best_score = 7;
                                break;
                            }
                        }
                    }
                    if (best_score < 6 && last_vertex == last_last_vertex) {
                        best_vert = vert; best_poly = -1; best_score = 6;
                    }
                    if (best_score < 5 && last_vertex != last_last_vertex) {
                        best_poly = -1; best_vert = last_vertex; best_score = 5;
                    }
                } else if (used == 2) {
                    if (best_score < 13) {
                        for (long m = 0; m < 3; ++m) {
                            long a = (m + 1) % 3;
                            long b = (a + 1) % 3;
                            if (reversed) { long t = a; a = b; b = t; }
                            if (poly_buffer[poly * 3 + m] == vert && poly_buffer[poly * 3 + a] == last_last_vertex && poly_buffer[poly * 3 + b] == last_vertex) {
                                best_score = 13; best_poly = poly; best_vert = vert;
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
                                best_poly = -1; best_vert = poly_buffer[poly * 3 + a]; best_score = 12;
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
                                best_poly = -1; best_vert = last_vertex; best_score = 11;
                                break;
                            }
                        }
                    }
                    if (best_score < 10 && last_vertex == last_last_vertex) {
                        best_vert = vert; best_poly = -1; best_score = 10;
                    }
                    if (best_score < 9 && last_vertex != last_last_vertex) {
                        best_poly = -1; best_vert = last_vertex; best_score = 9;
                    }
                } else {
                    if (best_score < 19) {
                        for (long m = 0; m < 3; ++m) {
                            long a = (m + 1) % 3;
                            long b = (a + 1) % 3;
                            if (reversed) { long t = a; a = b; b = t; }
                            if (poly_buffer[poly * 3 + a] == last_last_vertex && poly_buffer[poly * 3 + b] == last_vertex) {
                                best_score = 19; best_poly = poly; best_vert = poly_buffer[poly * 3 + m];
                                break;
                            }
                        }
                    }
                    if (best_score < 17 && last_vertex != last_last_vertex) {
                        for (long m = 0; m < 3; ++m) {
                            if (poly_buffer[poly * 3 + m] == last_vertex) {
                                best_poly = -1; best_vert = last_vertex; best_score = 17;
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
                                best_poly = -1; best_vert = poly_buffer[poly * 3 + a]; best_score = 18;
                                break;
                            }
                        }
                    }
                    if (best_score < 16 && last_vertex == last_last_vertex) {
                        best_poly = -1; best_vert = vert; best_score = 16;
                    }
                    if (best_score < 15 && last_vertex != last_last_vertex) {
                        best_poly = -1; best_vert = last_vertex; best_score = 15;
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
            last_vertex = vert;
            reversed = !reversed;
        }
    }
}

static void Map(const A16& pb, unsigned long nv, std::vector<std::vector<long> >& m)
{
    m.assign(nv, std::vector<long>());
    for (unsigned long p = 0; p < pb.size() / 3; ++p) for (int k = 0; k < 3; ++k) m[pb[p * 3 + k]].push_back((long)p);
}
static bool Run(const unsigned short* pb, int np, unsigned long nv, const unsigned short* vl, int nl, bool rev0, const unsigned short* want, int nw, bool rev_want)
{
    A16 poly_buffer(pb, pb + np * 3), vertex_list(vl, vl + nl), strip;
    std::vector<std::vector<long> > map; Map(poly_buffer, nv, map);
    std::vector<bool> mask(poly_buffer.size(), true);
    bool reversed = rev0;
    FillInTriStrip(poly_buffer, map, vertex_list, mask, strip, false, reversed);
    bool ok = (int)strip.size() == nw && reversed == rev_want;
    for (int i = 0; ok && i < nw; ++i) ok = strip[i] == want[i];
    if (!ok) { std::printf("got"); for (size_t i = 0; i < strip.size(); ++i) std::printf(" %d", strip[i]); std::printf(" rev %d\n", reversed); }
    return ok;
}
int main()
{
    bool ok = true;
    const unsigned short want1[] = { 0, 1, 2, 3 };
    const unsigned short want2[] = { 0, 1, 1, 0, 2, 3, 3, 0, 4, 1 };
    const unsigned short want3[] = { 0, 1, 2, 3, 4 };
    const unsigned short quad[] = { 0, 1, 2, 2, 1, 3 };
    const unsigned short quad_list[] = { 0, 1, 2, 3 };
    ok = Run(quad, 2, 4, quad_list, 4, false, want1, 4, false) && ok;
    const unsigned short fan[] = { 0, 1, 2, 0, 2, 3, 0, 3, 4, 0, 4, 1 };
    const unsigned short fan_list[] = { 0, 1, 2, 3, 4 };
    ok = Run(fan, 4, 5, fan_list, 5, true, want2, 10, true) && ok;
    const unsigned short strip3[] = { 0, 1, 2, 1, 3, 2, 2, 3, 4 };
    const unsigned short strip3_list[] = { 0, 1, 2, 3, 4 };
    ok = Run(strip3, 3, 5, strip3_list, 5, false, want3, 5, true) && ok;
    std::printf(ok ? "00f3bdd0_TEST PASS\n" : "00f3bdd0_TEST FAIL\n");
    return ok ? 0 : 1;
}
