T="    float tex_diff = (vertex2.TextureCoord - vertex1.TextureCoord).GetMagnitude();\n    tex_diff *= tex_diff;\n"
N="    float normal_diff = (vertex2.Normal - vertex1.Normal).GetMagnitude();\n"
C="    float cost = tex_diff * Parameters.UnsplitTextureWeight + normal_diff * normal_diff * Parameters.UnsplitNormalWeight;\n"
T0="    float tex_diff = (vertex2.TextureCoord - vertex1.TextureCoord).GetMagnitude();\n"
W1="Parameters.UnsplitTextureWeight"; W2="Parameters.UnsplitNormalWeight"
def mk(body):
    def fn(s):
        assert T+N+C in s
        s=s.replace(T+N+C, body)
        s=s.replace('#include "engine/C3DVector.h"\n','#include "engine/C3DVector.h"\n#include "engine/GFWorldDistance.h"\n')
        return s
    return fn
VARIANTS=[
 ("fw_cost0_pluseq", mk("    float cost = 0.0f;\n"+T0+N+"    cost += GFSqr(tex_diff) * %s;\n    cost += GFSqr(normal_diff) * %s;\n"%(W1,W2))),
 ("cost_eq_pluseq", mk(T0+N+"    float cost = GFSqr(tex_diff) * %s;\n    cost += GFSqr(normal_diff) * %s;\n"%(W1,W2))),
 ("sum_gfsqr", mk(T0+N+"    float cost = GFSqr(tex_diff) * %s + GFSqr(normal_diff) * %s;\n"%(W1,W2))),
 ("w_first", mk(T0+N+"    float cost = %s * GFSqr(tex_diff) + %s * GFSqr(normal_diff);\n"%(W1,W2))),
 ("cost0_w_first", mk("    float cost = 0.0f;\n"+T0+N+"    cost += %s * GFSqr(tex_diff);\n    cost += %s * GFSqr(normal_diff);\n"%(W1,W2))),
 ("t_sq_nsq_gf", mk(T+N+"    float cost = tex_diff * %s + GFSqr(normal_diff) * %s;\n"%(W1,W2))),
]
