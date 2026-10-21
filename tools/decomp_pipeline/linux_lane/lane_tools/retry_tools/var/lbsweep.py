# lbsweep.py <addr>...: add a same-TU stand-in LowerBound body (auto_inline off) after the map struct and probe-grade.
import re,sys,subprocess,os
W=os.environ["S"]+"/w22/retry10"
def convert(src):
    m=re.search(r"^(\s*)(const\s+)?(SEntry|Entry|S\w+)\*\s+LowerBound\((const long[*&]) (\w+)\)( const)?;.*$",src,re.M)
    if not m: return None,"no decl"
    # enclosing struct
    pre=src[:m.start()]
    sm=list(re.finditer(r"^struct (\w+)\s*\{",pre,re.M))
    if not sm: return None,"no struct"
    sname=sm[-1].group(1); ent=m.group(3); cq=m.group(2) or ""; ptype=m.group(4); pname=m.group(5); cst=m.group(6) or ""
    # struct end
    end=src.find("\n};",m.end())
    if end<0: return None,"no end"
    end+=3
    deref="*"+pname if ptype.endswith("*") else pname
    body=f'''
#pragma auto_inline(off)
// Same-TU stand-in for CVectorMap::LowerBound (STLport lower_bound over the 8-byte {{type, component}} entries): compiled above
// the caller so VC7.1 knows the key's address does not escape and packs the lookup keys as retail (the landed 0x00848730 idiom).
{cq}{sname}::{ent}* {sname}::LowerBound({ptype} {pname}){cst}
{{
    {cq}{ent}* it = first;
    int len = int(last - first);
    while (len > 0) {{
        int half = len / 2;
        {cq}{ent}* middle = it + half;
        if (middle->type < {deref}) {{
            it = middle + 1;
            len = len - half - 1;
        }} else
            len = half;
    }}
    return it;
}}
#pragma auto_inline(on)'''
    return src[:end]+body+src[end:],"ok"
for a in sys.argv[1:]:
    src=open(f"{W}/src/{a}.cpp").read()
    if "#pragma auto_inline(off)" in src and "LowerBound(" in src.split("#pragma auto_inline(off)")[1][:3000]:
        print(a,"already has stand-in"); continue
    out,why=convert(src)
    if out is None: print(a,"skip:",why); continue
    p=f"{W}/var/{a}_lb.cpp"; open(p,"w").write(out)
    r=subprocess.run(["python3",f"{W}/var/pv.py",a,p],capture_output=True,text=True)
    lines=r.stdout.strip().splitlines()
    head=lines[0] if lines else r.stdout[-300:]
    tail=lines[-1] if lines else ""
    print(a,"|",head[:90],"|",tail, flush=True)
