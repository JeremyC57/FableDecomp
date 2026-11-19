import os
H=os.environ["W"]+"/hdr/engine/CPerlinNoise.h"; base_h=open(H).read()
LI=["IndexTable[i&0xff]","(IndexTable[i&0xff])","IndexTable[(i&0xff)]"]
LG=["VectorTable[i&0xff]","(VectorTable[i&0xff])","VectorTable[(i&0xff)]","C3DVector(VectorTable[i&0xff])"]
IP=["a+((b-a)*t)","((b-a)*t)+a","a+(t*(b-a))","a+((b-a)*(t))","(a)+((b-a)*t)"]
def mk(i,j,k):
    def fn(s):
        h=base_h.replace("{return IndexTable[i&0xff];}","{return %s;}"%LI[i]).replace("{return VectorTable[i&0xff];}","{return %s;}"%LG[j]).replace("{return a+((b-a)*t);}","{return %s;}"%IP[k])
        return s,{H:h}
    return fn
VARIANTS=[(f"li{i}_lg{j}_ip{k}",mk(i,j,k)) for i in range(3) for j in range(4) for k in range(5)]
