import os
H=os.environ["W"]+"/hdr/engine/CPerlinNoise.h"; base_h=open(H).read()
SC=["t*t*(3.0f-(2.0f*t))","(t*t)*(3.0f-(2.0f*t))","t*(t*(3.0f-(2.0f*t)))","(t*t*(3.0f-(2.0f*t)))","t*t*((3.0f-(2.0f*t)))","((t*t)*(3.0f-(2.0f*t)))","t*t*(3.0f-2.0f*t)","t*(t*(3.0f-2.0f*t))","(t*t)*(3.0f-2.0f*t)"]
IP=["a+((b-a)*t)","(a+((b-a)*t))","a+(b-a)*t","((b-a)*t)+a","(b-a)*t+a","a+(t*(b-a))"]
def mk(i,j):
    def fn(s):
        h=base_h.replace("{return t*t*(3.0f-(2.0f*t));}","{return %s;}"%SC[i]).replace("{return a+((b-a)*t);}","{return %s;}"%IP[j])
        return s,{H:h}
    return fn
VARIANTS=[(f"sc{i}_ip{j}",mk(i,j)) for i in range(len(SC)) for j in range(len(IP))]
