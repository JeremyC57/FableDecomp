H='/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad/w24/hand2c/hdr/engine/CPerlinNoise.h'
ORIG='return t*t*(3.0f-(2.0f*t));'
def hv(expr):
    def f(s):
        h=open(H).read(); assert ORIG in h
        return (s,{H:h.replace(ORIG,'return '+expr+';')})
    return f
VARIANTS=[
 ('b_lpar', hv('((3.0f-2.0f*t)*t)*t')),
 ('d_rassoc', hv('t*(t*(3.0f-2.0f*t))')),
 ('f_mid', hv('t*((3.0f-2.0f*t)*t)')),
 ('g_outer', hv('(t*t*(3.0f-2.0f*t))')),
 ('g2_outer', hv('((3.0f-2.0f*t)*t*t)')),
 ('h_ttpar', hv('(t*t)*(3.0f-2.0f*t)')),
]
