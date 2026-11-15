LINES=0
PD="""    long vertex_count = vertex_to_poly_map.size();
    float best_score = 0.0f;
    long best_start_vert = -1;
"""
U1="""    long vertex_count;
    float best_score = 0.0f;
    long best_start_vert = -1;
    vertex_count = vertex_to_poly_map.size();
"""
T='REGION-prologue '
def both(name,desc,new_pdb,new_u1):
    r=[]
    if new_pdb is not None: r.append((name+'p',T+'(pdb base) '+desc,[(PD,new_pdb)],'base_pdb_order.cpp'))
    if new_u1 is not None: r.append((name+'u',T+'(u1 base) '+desc,[(U1,new_u1)],'base_u1.cpp'))
    return r
W=[]
W+=both('p1','size through a named const CArray reference',
"""    const CArray<CArray<long> >& polys_of_vert = vertex_to_poly_map;
    long vertex_count = polys_of_vert.size();
    float best_score = 0.0f;
    long best_start_vert = -1;
""","""    const CArray<CArray<long> >& polys_of_vert = vertex_to_poly_map;
    long vertex_count;
    float best_score = 0.0f;
    long best_start_vert = -1;
    vertex_count = polys_of_vert.size();
""")
W+=both('p2','size read into an unsigned temporary first',
"""    unsigned long map_size = vertex_to_poly_map.size();
    long vertex_count = map_size;
    float best_score = 0.0f;
    long best_start_vert = -1;
""","""    unsigned long map_size = vertex_to_poly_map.size();
    long vertex_count;
    float best_score = 0.0f;
    long best_start_vert = -1;
    vertex_count = map_size;
""")
W+=both('p3','all three declared bare, three assignment statements c,s,b',
"""    long vertex_count;
    float best_score;
    long best_start_vert;
    vertex_count = vertex_to_poly_map.size();
    best_score = 0.0f;
    best_start_vert = -1;
""",None)
W+=both('p4','initialisers in their own scope',
"""    long vertex_count;
    float best_score;
    long best_start_vert;
    {
        vertex_count = vertex_to_poly_map.size();
        best_score = 0.0f;
        best_start_vert = -1;
    }
""","""    long vertex_count;
    float best_score;
    long best_start_vert;
    {
        best_score = 0.0f;
        best_start_vert = -1;
    }
    vertex_count = vertex_to_poly_map.size();
""")
W+=both('p5','score and vertex as one comma statement',
"""    long vertex_count = vertex_to_poly_map.size();
    float best_score;
    long best_start_vert;
    best_score = 0.0f, best_start_vert = -1;
""","""    long vertex_count;
    float best_score;
    long best_start_vert;
    best_score = 0.0f, best_start_vert = -1;
    vertex_count = vertex_to_poly_map.size();
""")
W+=both('p6','constructor-style initialisers',
"""    long vertex_count(vertex_to_poly_map.size());
    float best_score(0.0f);
    long best_start_vert(-1);
""",None)
W+=both('p7','size as end() - begin()',
"""    long vertex_count = vertex_to_poly_map.end() - vertex_to_poly_map.begin();
    float best_score = 0.0f;
    long best_start_vert = -1;
""","""    long vertex_count;
    float best_score = 0.0f;
    long best_start_vert = -1;
    vertex_count = vertex_to_poly_map.end() - vertex_to_poly_map.begin();
""")
W+=both('p8','vertex_count assigned in the for-init of phase 1',None,
"""    long vertex_count;
    float best_score = 0.0f;
    long best_start_vert = -1;
    long start_vert;
    for (vertex_count = vertex_to_poly_map.size(), start_vert = 0; start_vert < vertex_count; ++start_vert) {
        if (vertex_to_poly_map[start_vert].size() == 0)
            continue;
        long surround_vert = vertex_link_map[start_vert][0];""")
W+=both('p9','score and vertex initialised by chained assignment through a long zero/minus-one pair',
"""    long vertex_count = vertex_to_poly_map.size();
    float best_score;
    long best_start_vert;
    best_start_vert = -1;
    best_score = 0.0f;
""","""    long vertex_count;
    float best_score;
    long best_start_vert;
    best_start_vert = -1;
    best_score = 0.0f;
    vertex_count = vertex_to_poly_map.size();
""")
W+=both('p10','best_start_vert first then count then score (b,c,s)',
"""    long vertex_count;
    float best_score;
    long best_start_vert = -1;
    vertex_count = vertex_to_poly_map.size();
    best_score = 0.0f;
""",None)
W+=both('p11','redundant second -1 store after the count (first store read by nothing)',
"""    long vertex_count;
    float best_score;
    long best_start_vert = -1;
    vertex_count = vertex_to_poly_map.size();
    best_score = 0.0f;
    best_start_vert = -1;
""",None)
W+=both('p12','size cast to long explicitly; score from integer zero',
"""    long vertex_count = (long)vertex_to_poly_map.size();
    float best_score = 0;
    long best_start_vert = -1L;
""","""    long vertex_count;
    float best_score = 0;
    long best_start_vert = -1L;
    vertex_count = (long)vertex_to_poly_map.size();
""")
W+=both('p13','count declared const after the two results (PDB order broken on purpose)',
None,"""    float best_score = 0.0f;
    long best_start_vert = -1;
    const long vertex_count = vertex_to_poly_map.size();
""")
W+=both('p14','count through a conditional that folds: size() of a reference temp via static_cast base',
"""    long vertex_count = static_cast<const std::vector<CArray<long> >&>(vertex_to_poly_map).size();
    float best_score = 0.0f;
    long best_start_vert = -1;
""","""    long vertex_count;
    float best_score = 0.0f;
    long best_start_vert = -1;
    vertex_count = static_cast<const std::vector<CArray<long> >&>(vertex_to_poly_map).size();
""")
V=[]
BASE=None
