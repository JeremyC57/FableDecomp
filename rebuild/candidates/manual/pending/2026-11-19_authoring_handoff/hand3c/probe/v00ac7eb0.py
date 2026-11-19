T="    float tex_diff = (vertex2.TextureCoord - vertex1.TextureCoord).GetMagnitude();\n"
N="    float normal_diff = (vertex2.Normal - vertex1.Normal).GetMagnitude();\n"
C="    float cost = tex_diff * tex_diff * Parameters.UnsplitTextureWeight + normal_diff * normal_diff * Parameters.UnsplitNormalWeight;\n"
def mk(body):
    return lambda s: s.replace(T+N+C, body)
VARIANTS=[
 ("sq_after_t", mk(T+"    tex_diff *= tex_diff;\n"+N+"    float cost = tex_diff * Parameters.UnsplitTextureWeight + normal_diff * normal_diff * Parameters.UnsplitNormalWeight;\n")),
 ("sq_after_n", mk(T+N+"    tex_diff *= tex_diff;\n"+"    float cost = tex_diff * Parameters.UnsplitTextureWeight + normal_diff * normal_diff * Parameters.UnsplitNormalWeight;\n")),
 ("both_sq", mk(T+N+"    tex_diff *= tex_diff;\n    normal_diff *= normal_diff;\n"+"    float cost = tex_diff * Parameters.UnsplitTextureWeight + normal_diff * Parameters.UnsplitNormalWeight;\n")),
 ("cost_split", mk(T+N+"    float cost = tex_diff * tex_diff * Parameters.UnsplitTextureWeight;\n    cost += normal_diff * normal_diff * Parameters.UnsplitNormalWeight;\n")),
 ("paren", mk(T+N+"    float cost = (tex_diff * tex_diff) * Parameters.UnsplitTextureWeight + (normal_diff * normal_diff) * Parameters.UnsplitNormalWeight;\n")),
 ("sq_n_cost_split", mk(T+N+"    tex_diff *= tex_diff;\n    float cost = tex_diff * Parameters.UnsplitTextureWeight;\n    cost += normal_diff * normal_diff * Parameters.UnsplitNormalWeight;\n")),
]
