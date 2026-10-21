# lbreal2.py <addr> <suffix>: add the real binary-search body for every declared-only CInterfaceMap-style
# `SEntry* LowerBound(const long* type) [const];` (struct with `first`/`last` SEntry pointers and SEntry{long type; ...})
import re,sys,os
W="/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad/w22/retry10"
a,suf=sys.argv[1],sys.argv[2]
s=open(os.environ.get("SRCF", f"{W}/src/{a}.cpp"),errors="replace").read()
s="\n".join(l for l in s.split("\n") if not l.startswith("// Grade:"))
n=0
for m in list(re.finditer(r"^struct (\w+) \{\n(.*?)\n\};",s,re.M|re.S))[::-1]:
    name,body=m.group(1),m.group(2)
    if not re.search(r"\bSEntry\*\s+first;",body) or not re.search(r"\bSEntry\*\s+last;",body): continue
    decls=list(re.finditer(r"^\s*(const\s+)?SEntry\*\s+LowerBound\(const long\* (\w+)\)( const)?;[^\n]*$",body,re.M))
    if not decls: continue
    if re.search(name+r"::LowerBound\s*\(",s): continue
    add=""
    for d in decls:
        cret=d.group(1) or ""; cst=d.group(3) or ""; pn=d.group(2)
        add+=f"""
#pragma auto_inline(off)
// Same-TU body of the CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound template member (compiled in the caller's TU):
// binary search for the first entry whose type is not below the key read through the pointer.
__declspec(noinline) {cret}{name}::SEntry* {name}::LowerBound(const long* {pn}){cst}
{{
    {cret}SEntry* it = first;
    long count = last - first;
    while (count > 0) {{
        long half = count / 2;
        if (it[half].type < *{pn}) {{
            it += half + 1;
            count -= half + 1;
        }} else
            count = half;
    }}
    return it;
}}
#pragma auto_inline(on)"""
        n+=1
    s=s[:m.end()]+add+s[m.end():]
if n==0: print("NONE"); sys.exit(1)
open(f"{W}/var/{a}_{suf}.cpp","w").write(s); print(f"var/{a}_{suf}.cpp {n}")
