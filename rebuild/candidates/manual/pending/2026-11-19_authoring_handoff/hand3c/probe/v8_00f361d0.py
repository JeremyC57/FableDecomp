import re
def paren_line(key):
    def fn(s):
        L=s.split("\n")
        for i,l in enumerate(L):
            if l.strip().startswith(key):
                a,b=l.split(" = ",1); L[i]=a+" = ("+b[:-1]+");"
        return "\n".join(L)
    return fn
keys=["float value000","float value100","float value010","float value110","float result_z0","float value001","float result_z1","float result =","float tx","float ty","float tz","unsigned long b00","unsigned long b11","float v0_x","float v1_x","float v1_z"]
VARIANTS=[(k.split()[-1].strip("="),paren_line(k)) for k in keys]
def inner(s):
    return s.replace("Interp(Interp(value000, value100, tx), Interp(value010, value110, tx), ty)","Interp((Interp(value000, value100, tx)), (Interp(value010, value110, tx)), ty)")
def inner2(s):
    return s.replace("Interp(Interp(value000, value100, tx), Interp(value010, value110, tx), ty)","Interp(Interp(value000, value100, tx), (Interp(value010, value110, tx)), ty)")
def inner3(s):
    return s.replace("Interp(Interp(value000, value100, tx), Interp(value010, value110, tx), ty)","Interp((Interp(value000, value100, tx)), Interp(value010, value110, tx), ty)")
VARIANTS+= [("inner",inner),("inner2",inner2),("inner3",inner3)]
