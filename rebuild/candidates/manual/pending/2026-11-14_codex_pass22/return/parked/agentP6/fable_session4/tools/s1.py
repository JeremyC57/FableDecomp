BASE='base_pdb_order.cpp'
P2G="""            if (vertex_to_poly_map[start_vert].size() == 0)
                continue;
                CArray<unsigned short> vertex_to_ring_map;"""
V=[
('b1','phase 3 nested in phase 2 if',[("""                    break;
        }
    }
        if (best_score == 0.0f) {""","""                    break;
        }
        if (best_score == 0.0f) {"""),("""                return start_vert;
            }
        }
""","""                return start_vert;
            }
        }
    }
""")]),
('b2','early return after phase 1 when scored',[("""    if (best_score == 0.0f) {
        for (long start_vert = 0; start_vert < vertex_count; ++start_vert) {
            if (vertex_to_poly_map[start_vert].size() == 0)
                continue;
                CArray""","""    if (best_score != 0.0f)
        return best_start_vert;
    {
        for (long start_vert = 0; start_vert < vertex_count; ++start_vert) {
            if (vertex_to_poly_map[start_vert].size() == 0)
                continue;
                CArray""")]),
('b3','phase 3 positive test returns',[("""                if (vertex_to_poly_map[start_vert].size() == 0)
                    continue;
                return start_vert;""","""                if (vertex_to_poly_map[start_vert].size() != 0)
                    return start_vert;""")]),
('b4','phase 3 size() > 0 returns',[("""                if (vertex_to_poly_map[start_vert].size() == 0)
                    continue;
                return start_vert;""","""                if (vertex_to_poly_map[start_vert].size() > 0)
                    return start_vert;""")]),
('b5','!best_score for both zero tests',[("if (best_score == 0.0f)@all","if (!best_score)")]),
('b6','best_score == 0 int literal',[("if (best_score == 0.0f)@all","if (best_score == 0)")]),
('b7','best_score == 0.0 double literal',[("if (best_score == 0.0f)@all","if (best_score == 0.0)")]),
('b8','float best_score = 0',[("float best_score = 0.0f;","float best_score = 0;")]),
('b9','1e30 double literal compare',[("if (best_score == 1e30f)@all","if (best_score == 1e30)")]),
('b10','start_surround_vert defined first',[("""        long surround_vert = vertex_link_map[start_vert][0];
        long start_surround_vert = surround_vert;""","""        long start_surround_vert = vertex_link_map[start_vert][0];
        long surround_vert = start_surround_vert;""")]),
('b11','id declared in for-init inside own scope',[("""                long id = 0;
                for (; id < 3; ++id)""","""                long id;
                for (id = 0; id < 3; ++id)""")]),
('b12','phase 2 body wrapped in positive if',[(P2G,"""            if (vertex_to_poly_map[start_vert].size() != 0) {
                CArray<unsigned short> vertex_to_ring_map;"""),("""                if (best_score == 1e30f)
                    break;
        }
    }""","""                if (best_score == 1e30f)
                    break;
            }
        }
    }""")]),
]
