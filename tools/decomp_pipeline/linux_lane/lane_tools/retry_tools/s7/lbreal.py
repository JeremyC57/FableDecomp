# lbreal.py <addr> <suffix>: replace tmplbody's generic CInterfaceMap::LowerBound stand-ins with the real binary search
import re,sys
W="/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad/w22/retry10"
a,suf=sys.argv[1],sys.argv[2]
s=open(f"{W}/src/{a}.cpp").read()
pat=re.compile(r"long (TmplSink_s7_\d+)\(const void\* self, long seen\);   // stand-in: the work LowerBound does \(outside the bytes graded here\)\n#pragma auto_inline\(off\)\n// Same-TU stand-in for the LowerBound instantiation[^\n]*\n[^\n]*\n(__declspec\(noinline\) \S+ (\w+)::LowerBound\(const long\* type\)( const)?)\n\{\n    long r = \1\(this, 0 \+ \(type \? \*\(const long\*\)type : 0\)\);\n    return \(SEntry\*\)r;\n\}")
n=0
def rep(m):
    global n; n+=1
    return f"""#pragma auto_inline(off)
// Same-TU body of the CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound instantiation (a template member, compiled in the
// caller's TU): binary search for the first entry whose type is not below the key read through the pointer.
{m.group(2)}
{{
    SEntry* it = first;
    long count = last - first;
    while (count > 0) {{
        long half = count / 2;
        if (it[half].type < *type) {{
            it += half + 1;
            count -= half + 1;
        }} else
            count = half;
    }}
    return it;
}}"""
s2=pat.sub(rep,s)
if n==0: print("NONE"); sys.exit(1)
open(f"{W}/var/{a}_{suf}.cpp","w").write(s2); print(f"var/{a}_{suf}.cpp {n}")
