INC_F='#include "engine/GFFloatToLongFloor.h"\n'; INC_P='#include "engine/CPerlinNoise.h"\n'
INC3='#include "engine/C3DVector.h"\n'; INC2='#include "engine/C2DVector.h"\n'
def inc(order):
    def f(s):
        s=s.replace(INC_F,'').replace(INC_P,'')
        k=s.index('\nnamespace NFractal')
        return s[:k]+'\n'+''.join(order)+s[k:]
    return f
TX='    float tx = SCurve(v0_x);\n'; TY='    float ty = SCurve(v0_y);\n'; TZ='    float tz = SCurve(v0_z);\n'
def mv(lines, before):
    def f(s):
        for l in lines: s=s.replace(l,'',1)
        k=s.index(before); return s[:k]+''.join(lines)+s[k:]
    return f
V1='    float v1_x = v0_x - 1.0f;\n'
B00='    unsigned long b00 ='
G000='    C3DVector gradient000'
VARIANTS=[
 ('inc_P_F', inc([INC_P,INC_F])),
 ('inc_3_2_F_P', inc([INC3,INC2,INC_F,INC_P])),
 ('inc_2_3_F_P', inc([INC2,INC3,INC_F,INC_P])),
 ('inc_3_F_P', inc([INC3,INC_F,INC_P])),
 ('inc_2_F_P', inc([INC2,INC_F,INC_P])),
 ('inc_P_F_3_2', inc([INC_P,INC_F,INC3,INC2])),
 ('tx_before_v0y', mv([TX],'    float v0_y')),
 ('txyz_before_v1', mv([TX,TY,TZ],V1)),
 ('txyz_before_b00', mv([TX,TY,TZ],B00)),
 ('txy_before_b00', mv([TX,TY],B00)),
 ('tx_before_b00', mv([TX],B00)),
 ('tz_before_tx', mv([TZ],TX)),
 ('tz_after_g000', mv([TZ],'    float value000')),
 ('ty_before_tx', mv([TY],TX)),
]
