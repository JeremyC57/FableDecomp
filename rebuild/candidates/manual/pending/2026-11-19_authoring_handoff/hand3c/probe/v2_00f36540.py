import os
H=os.environ["W"]+"/hdr/engine/CPerlinNoise.h"; base_h=open(H).read()
SC=["t*t*(3.0f-(2.0f*t))","t*(t*(3.0f-2.0f*t))","t*(t*(3.0f-(2.0f*t)))"]
IP=["a+(b-a)*t","a+((b-a)*t)","(b-a)*t+a","((b-a)*t)+a","a+t*(b-a)","a+(t*(b-a))"]
LS=["VectorTable[i&0xff].X","(VectorTable[i&0xff]).X"]
def mk(i,j,k):
    def fn(s):
        h=base_h.replace("{return t*t*(3.0f-(2.0f*t));}","{return %s;}"%SC[i]).replace("{return a+(b-a)*t;}","{return %s;}"%IP[j]).replace("{return VectorTable[i&0xff].X;}","{return %s;}"%LS[k])
        return s,{H:h}
    return fn
VARIANTS=[(f"sc{i}_ip{j}_ls{k}",mk(i,j,k)) for i in range(len(SC)) for j in range(len(IP)) for k in (0,)]
