T="    float tex_diff = (vertex2.TextureCoord - vertex1.TextureCoord).GetMagnitude();\n    tex_diff *= tex_diff;\n"
N="    float normal_diff = (vertex2.Normal - vertex1.Normal).GetMagnitude();\n"
C="    float cost = tex_diff * Parameters.UnsplitTextureWeight + normal_diff * normal_diff * Parameters.UnsplitNormalWeight;\n"
T0="    float tex_diff = (vertex2.TextureCoord - vertex1.TextureCoord).GetMagnitude();\n"
H='#define FABLE_RETAIL_C3DVECTOR_NORMALISE_SQUARE_HELPER\n#define FABLE_RETAIL_C3DVECTOR\n'
def mk(body, helper=False):
    def fn(s):
        s=s.replace(T+N+C, body)
        if helper: s=s.replace('#define FABLE_RETAIL_C3DVECTOR\n',H)
        return s
    return fn
VARIANTS=[
 ("sqr_both", mk(T0+N+"    float cost = GFSqr(tex_diff) * Parameters.UnsplitTextureWeight + GFSqr(normal_diff) * Parameters.UnsplitNormalWeight;\n", True)),
 ("sqr_n", mk(T+N+"    float cost = tex_diff * Parameters.UnsplitTextureWeight + GFSqr(normal_diff) * Parameters.UnsplitNormalWeight;\n", True)),
 ("sqr_assign_t", mk(T0+"    tex_diff = GFSqr(tex_diff);\n"+N+"    float cost = tex_diff * Parameters.UnsplitTextureWeight + GFSqr(normal_diff) * Parameters.UnsplitNormalWeight;\n", True)),
 ("tex_sq_n_sq_sep", mk(T+N+"    normal_diff *= normal_diff;\n    float cost = tex_diff * Parameters.UnsplitTextureWeight + normal_diff * Parameters.UnsplitNormalWeight;\n")),
 ("t_sq_inline_both", mk(T0+N+"    float cost = tex_diff * tex_diff * Parameters.UnsplitTextureWeight;\n    cost += normal_diff * normal_diff * Parameters.UnsplitNormalWeight;\n")),
]
