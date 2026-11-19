import itertools, re
B={"00":"    unsigned long b00 = IndexTable[(IndexTable[x_int & 0xff] + y_int) & 0xff];\n",
   "01":"    unsigned long b01 = IndexTable[(IndexTable[x_int & 0xff] + y_int + 1) & 0xff];\n",
   "10":"    unsigned long b10 = IndexTable[(IndexTable[(x_int + 1) & 0xff] + y_int) & 0xff];\n",
   "11":"    unsigned long b11 = IndexTable[(IndexTable[(x_int + 1) & 0xff] + y_int + 1) & 0xff];\n"}
G={k:f"    C2DVector gradient{k} = VectorTable[b{k} & 0xff];\n" for k in B}
V={"00":"    float value00 = gradient00.X * v0_x + gradient00.Y * v0_y;\n","10":"    float value10 = gradient10.X * v1_x + gradient10.Y * v0_y;\n",
   "01":"    float value01 = gradient01.X * v0_x + gradient01.Y * v1_y;\n","11":"    float value11 = gradient11.X * v1_x + gradient11.Y * v1_y;\n"}
def mk(ob, og, ov):
    def fn(s):
        for d in (B,G,V):
            for k in d: s=s.replace(d[k],"")
        anchor="    float result ="
        blk="".join(B[k] for k in ob)+"".join(G[k] for k in og)+"".join(V[k] for k in ov)
        return s.replace(anchor, blk+anchor)
    return fn
orders=[("00","01","10","11"),("00","10","01","11")]
VARIANTS=[(f"b{''.join(a)}_g{''.join(b)}_v{''.join(c)}", mk(a,b,c)) for a in orders for b in orders for c in orders]
