H='/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad/w24/hand2c/hdr/engine/CPerlinNoise.h'
SC='return t*t*(3.0f-(2.0f*t));'; IN='return a+((b-a)*t);'
def hv(sc, inp):
    def f(s):
        h=open(H).read(); assert SC in h and IN in h
        return (s,{H:h.replace(SC,'return '+sc+';').replace(IN,'return '+inp+';')})
    return f
G='(t*t*(3.0f-2.0f*t))'
VARIANTS=[
 ('g_i_noparen', hv(G,'a+(b-a)*t')),
 ('g_i_outer', hv(G,'(a+(b-a)*t)')),
 ('g_i_outer2', hv(G,'(a+((b-a)*t))')),
 ('g_i_rev', hv(G,'((b-a)*t)+a')),
 ('g_i_rev2', hv(G,'(b-a)*t+a')),
]
