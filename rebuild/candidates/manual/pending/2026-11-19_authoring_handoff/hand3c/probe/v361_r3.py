H='/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad/w24/hand2c/hdr/engine/CPerlinNoise.h'
NP=open(H).read().replace('return t*t*(3.0f-(2.0f*t));','return t*t*(3.0f-2.0f*t);')
assert NP!=open(H).read()
X='float v0_x = point.X - x_int;'; Y='float v0_y = point.Y - y_int;'
def v(rep, np=True):
    def f(s):
        for a,b in rep: assert a in s; s=s.replace(a,b)
        return (s,{H:NP}) if np else s
    return f
VARIANTS=[
 ('np_castx', v([(X,'float v0_x = point.X - (float)x_int;')])),
 ('np_parx', v([(X,'float v0_x = point.X - (x_int);')])),
 ('np_parcastx', v([(X,'float v0_x = point.X - ((float)x_int);')])),
 ('np_parv0x', v([(X,'float v0_x = (point.X - x_int);')])),
 ('np_pary', v([(Y,'float v0_y = point.Y - (y_int);')])),
 ('np_parcasty', v([(Y,'float v0_y = point.Y - ((float)y_int);')])),
 ('np_parv0y', v([(Y,'float v0_y = (point.Y - y_int);')])),
 ('p_parcastx', v([(X,'float v0_x = point.X - ((float)x_int);')],False)),
 ('p_parv0x', v([(X,'float v0_x = (point.X - x_int);')],False)),
]
