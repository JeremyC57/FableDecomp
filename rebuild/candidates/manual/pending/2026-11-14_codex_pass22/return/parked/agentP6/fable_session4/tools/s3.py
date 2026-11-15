LINES=0
BASE='base_pdb_order.cpp'
A='REGION-reload1 '; B='REGION-reload2 '
GT="""                    if (surround_vert == start_surround_vert)
                        goto skip_to_next_start_vert;
                    break;"""
LBL="""    skip_to_next_start_vert:
        continue;
    }
    if (best_score == 0.0f) {"""
SC1="""            float score = CalcSectionScore(vertex_list, poly_buffer, vertex_to_poly_map);
            if (score > best_score) {
                best_score = score;
                best_start_vert = start_vert;
            }
            if (best_score == 1e30f)
                break;
        }
    skip_to_next_start_vert:"""
VL1="""            BuildRings(start_vert, poly_buffer, vertex_to_poly_map, vertex_link_map, vertex_to_ring_map, vertex_to_ring_pos_map, rings);
            CArray<unsigned short> vertex_list;
            BuildCacheOptimizedVertexList(poly_buffer, vertex_to_poly_map, vertex_link_map, vertex_to_ring_map, rings, vertex_list);
            float score"""
OPEN="""        {
            CArray<unsigned short> vertex_to_ring_map;
            CArray<unsigned short> vertex_to_ring_pos_map;
            CArray<CArray<unsigned short> > rings;
            BuildRings(start_vert,"""
V=[
('q1',A+'closed-fan test reversed operands',[("if (surround_vert == start_surround_vert)\n                        goto","if (start_surround_vert == surround_vert)\n                        goto")]),
('q2',A+'open fan breaks first, closed fan falls to the goto',[(GT,"""                    if (surround_vert != start_surround_vert)
                        break;
                    goto skip_to_next_start_vert;""")]),
('q3',A+'closed-fan goto with braces and else break',[(GT,"""                    if (surround_vert == start_surround_vert) {
                        goto skip_to_next_start_vert;
                    } else {
                        break;
                    }""")]),
('q4',A+'four arrays declared before BuildRings in phase 1 as in the fallback',[(VL1,"""            CArray<unsigned short> vertex_list;
            BuildRings(start_vert, poly_buffer, vertex_to_poly_map, vertex_link_map, vertex_to_ring_map, vertex_to_ring_pos_map, rings);
            BuildCacheOptimizedVertexList(poly_buffer, vertex_to_poly_map, vertex_link_map, vertex_to_ring_map, rings, vertex_list);
            float score""")]),
('q5',A+'calls through this-> in the scoring block',[("            BuildRings(start_vert, poly_buffer, vertex_to_poly_map, vertex_link_map, vertex_to_ring_map, vertex_to_ring_pos_map, rings);\n            CArray","            this->BuildRings(start_vert, poly_buffer, vertex_to_poly_map, vertex_link_map, vertex_to_ring_map, vertex_to_ring_pos_map, rings);\n            CArray"),("            BuildCacheOptimizedVertexList(poly_buffer, vertex_to_poly_map, vertex_link_map, vertex_to_ring_map, rings, vertex_list);\n            float score = CalcSectionScore(","            this->BuildCacheOptimizedVertexList(poly_buffer, vertex_to_poly_map, vertex_link_map, vertex_to_ring_map, rings, vertex_list);\n            float score = this->CalcSectionScore(")]),
('q6',A+'scoring block opened by a start-vertex copy local',[(OPEN,"""        {
            long ring_start_vert = start_vert;
            CArray<unsigned short> vertex_to_ring_map;
            CArray<unsigned short> vertex_to_ring_pos_map;
            CArray<CArray<unsigned short> > rings;
            BuildRings(ring_start_vert,""")]),
('q7',A+'vertex list and score in an inner scope of the scoring block',[(VL1,"""            BuildRings(start_vert, poly_buffer, vertex_to_poly_map, vertex_link_map, vertex_to_ring_map, vertex_to_ring_pos_map, rings);
            float score;
            {
            CArray<unsigned short> vertex_list;
            BuildCacheOptimizedVertexList(poly_buffer, vertex_to_poly_map, vertex_link_map, vertex_to_ring_map, rings, vertex_list);
            score"""),("""            score = CalcSectionScore(vertex_list, poly_buffer, vertex_to_poly_map);
            if (score > best_score) {""","""            score = CalcSectionScore(vertex_list, poly_buffer, vertex_to_poly_map);
            }
            if (score > best_score) {""")]),
('q8',A+'fan walk in its own scope before the scoring scope',[("""        long surround_vert = vertex_link_map[start_vert][0];
        long start_surround_vert = surround_vert;
        for (long verts_searched""","""        {
        long surround_vert = vertex_link_map[start_vert][0];
        long start_surround_vert = surround_vert;
        for (long verts_searched"""),("""                    break;
                }
            }
        }
        {
            CArray<unsigned short> vertex_to_ring_map;""","""                    break;
                }
            }
        }
        }
        {
            CArray<unsigned short> vertex_to_ring_map;""")]),
('q9',A+'walk loops as while with explicit increments',[("        for (long verts_searched = 0; verts_searched < vertex_link_map[start_vert].size(); ++verts_searched) {\n            for (long pl","        long verts_searched = 0;\n        for (; verts_searched < vertex_link_map[start_vert].size(); ++verts_searched) {\n            for (long pl")]),
('q10',A+'post-increment walk counters',[("++verts_searched) {","verts_searched++) {"),("        for (long start_vert = 0; start_vert < vertex_count; ++start_vert) {\n        if (vertex_to_poly_map[start_vert].size() == 0)\n            continue;\n        long surround","        for (long start_vert = 0; start_vert < vertex_count; start_vert++) {\n        if (vertex_to_poly_map[start_vert].size() == 0)\n            continue;\n        long surround")]),
('r1',B+'skip label on an empty statement',[(LBL,"""    skip_to_next_start_vert:
        ;
    }
    if (best_score == 0.0f) {""")]),
('r2',B+'explicit continue closes the scoring block, label after it',[(SC1,"""            float score = CalcSectionScore(vertex_list, poly_buffer, vertex_to_poly_map);
            if (score > best_score) {
                best_score = score;
                best_start_vert = start_vert;
            }
            if (best_score == 1e30f)
                break;
            continue;
        }
    skip_to_next_start_vert:""")]),
('r3',B+'perfect-score exit by goto to a label after the loop',[(SC1,"""            float score = CalcSectionScore(vertex_list, poly_buffer, vertex_to_poly_map);
            if (score > best_score) {
                best_score = score;
                best_start_vert = start_vert;
            }
            if (best_score == 1e30f)
                goto phase1_done;
        }
    skip_to_next_start_vert:"""),(LBL,"""    skip_to_next_start_vert:
        continue;
    }
phase1_done:
    if (best_score == 0.0f) {""")]),
('r4',B+'perfect test nested in the improvement test',[(SC1,"""            float score = CalcSectionScore(vertex_list, poly_buffer, vertex_to_poly_map);
            if (score > best_score) {
                best_score = score;
                best_start_vert = start_vert;
                if (best_score == 1e30f)
                    break;
            }
        }
    skip_to_next_start_vert:""")]),
('r5',B+'score update vertex first',[("""            if (score > best_score) {
                best_score = score;
                best_start_vert = start_vert;
            }
            if (best_score == 1e30f)
                break;
        }
    skip""","""            if (score > best_score) {
                best_start_vert = start_vert;
                best_score = score;
            }
            if (best_score == 1e30f)
                break;
        }
    skip""")]),
('r6',B+'perfect test negated with continue',[("""            if (best_score == 1e30f)
                break;
        }
    skip_to_next_start_vert:""","""            if (best_score != 1e30f)
                continue;
            break;
        }
    skip_to_next_start_vert:""")]),
('r7',B+'score update as else-if chain with the perfect test',[("""            if (best_score == 1e30f)
                break;
        }
    skip_to_next_start_vert:""","""            if (!(best_score != 1e30f))
                break;
        }
    skip_to_next_start_vert:""")]),
('r8',B+'label inside the loop tail followed by the for increment only (no statement after scope) via if(1)',[("""        {
            CArray<unsigned short> vertex_to_ring_map;
            CArray<unsigned short> vertex_to_ring_pos_map;
            CArray<CArray<unsigned short> > rings;
            BuildRings(start_vert, poly_buffer, vertex_to_poly_map, vertex_link_map, vertex_to_ring_map, vertex_to_ring_pos_map, rings);
            CArray<unsigned short> vertex_list;""","""        if (true) {
            CArray<unsigned short> vertex_to_ring_map;
            CArray<unsigned short> vertex_to_ring_pos_map;
            CArray<CArray<unsigned short> > rings;
            BuildRings(start_vert, poly_buffer, vertex_to_poly_map, vertex_link_map, vertex_to_ring_map, vertex_to_ring_pos_map, rings);
            CArray<unsigned short> vertex_list;""")]),
]
