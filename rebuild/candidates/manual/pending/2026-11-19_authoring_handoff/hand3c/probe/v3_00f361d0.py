import itertools
def r(s,a,b):
    assert a in s, a
    return s.replace(a,b)
MODS={
 "frac_paren": lambda s: s.replace("= point.X - x_int;","= (point.X - x_int);").replace("= point.Y - y_int;","= (point.Y - y_int);").replace("= point.Z - z_int;","= (point.Z - z_int);"),
 "v1_paren": lambda s: s.replace("v0_x - 1.0f;","(v0_x - 1.0f);").replace("v0_y - 1.0f;","(v0_y - 1.0f);").replace("v0_z - 1.0f;","(v0_z - 1.0f);"),
 "b_paren": lambda s: s.replace("LookupIndex(x_int) + y_int + 1","(LookupIndex(x_int) + y_int) + 1").replace("LookupIndex(x_int + 1) + y_int + 1","(LookupIndex(x_int + 1) + y_int) + 1"),
 "g_paren": lambda s: s.replace("z_int + 1)","(z_int + 1))"),
 "g1_early": lambda s: move_g1(s),
 "z0_split": lambda s: s.replace("float result_z0 = Interp(Interp(value000, value100, tx), Interp(value010, value110, tx), ty);","float result_z0 = Interp(Interp(value000, value100, tx),\n                            Interp(value010, value110, tx), ty);"),
}
def move_g1(s):
    L=s.split("\n"); g=[l for l in L if "LookupGradient(b" in l and "+ 1)" in l]
    L=[l for l in L if l not in g]; i=[k for k,l in enumerate(L) if "gradient110 = " in l][0]
    return "\n".join(L[:i+1]+g+L[i+1:])
keys=list(MODS)
VARIANTS=[]
for k in keys: 
    VARIANTS.append((k, (lambda kk: (lambda s: MODS[kk](s)))(k)))
for a,b in itertools.combinations(["frac_paren","v1_paren","b_paren","g_paren"],2):
    VARIANTS.append((a+"+"+b,(lambda aa,bb:(lambda s: MODS[bb](MODS[aa](s))))(a,b)))
