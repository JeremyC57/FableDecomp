# nearbody.py <addr> <suffix> [maxdist]: for every declared-only callee (address comment within maxdist of the row, default
# 0x1000) that is a member of a non-template class in this TU, add a same-TU noinline stand-in body that reads its const
# reference / pointer-to-const parameters (first word), never lets a parameter escape, and returns an opaque value
# (NearSink_s7_N(this)). Writes var/<addr>_<suffix>.cpp; prints the stand-ins made.
import re,sys
W="/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad/w22/retry10"
a,suf=sys.argv[1],sys.argv[2]; maxd=int(sys.argv[3],16) if len(sys.argv)>3 else 0x1000
src=open(__import__("os").environ.get("SRCF", f"{W}/src/{a}.cpp"),errors="replace").read()
src="\n".join(l for l in src.split("\n") if not l.startswith("// Grade:"))
def chain(s,pos):
    res=[];depth=0;i=pos
    while i>0:
        i-=1
        c=s[i]
        if c=="}": depth+=1
        elif c=="{":
            if depth==0:
                hm=list(re.finditer(r"(template\s*<([^<>]*(?:<[^<>]*>[^<>]*)*)>\s*)?(?<![<,]\s)(?<![<,])\b(struct|class|namespace)\s+(\w+)\b[^;{}()]*$",s[:i]))
                if not hm: return None
                res.append((hm[-1].group(3),hm[-1].group(4),hm[-1].group(2),i))
            else: depth-=1
    return res[::-1]
def split_params(p):
    out=[];d=0;cur=""
    for ch in p:
        if ch in "<(": d+=1
        elif ch in ">)": d-=1
        if ch=="," and d==0: out.append(cur); cur=""
        else: cur+=ch
    if cur.strip(): out.append(cur)
    return [x.strip() for x in out]
DECL=re.compile(r"^([ \t]*)((?:virtual|static|inline)\s+)?([\w:<>,* &]+?)\s*\b(~?\w+)\s*\(([^;{}()]*(?:\([^()]*\)[^;{}()]*)*)\)\s*(const)?\s*;[ \t]*//[^\n]*?0x([0-9A-Fa-f]{6,8})",re.M)
adds=[];n=0;made=[]
for m in DECL.finditer(src):
    kw,ret,name,params,cst,addr=m.group(2) or "",m.group(3).strip(),m.group(4),m.group(5),m.group(6),m.group(7)
    if "virtual" in kw: continue
    if name.startswith("~") or name=="operator": continue
    d=int(addr,16)-int(a,16)
    if abs(d)>maxd: continue
    if re.search(r"::\s*"+re.escape(name)+r"\s*\(",src): continue
    ch=chain(src,m.start())
    if not ch: continue
    cls=[c for c in ch if c[0]!="namespace"]
    if not cls or any(c[2] for c in cls): continue
    if ret in ("",) : continue   # constructor
    if re.match(r"^(class|struct)\b",ret): continue
    rt=ret.replace("static","").strip()
    # return kinds
    if rt=="void": rk="void"
    elif rt.endswith("*") or rt.endswith("&"): rk="ptr"
    elif rt in ("bool",): rk="bool"
    elif rt in ("long","int","unsigned long","unsigned int","short","unsigned short","char","unsigned char","DWORD","size_t"): rk="int"
    elif rt in ("float","double"): rk="float"
    else: continue      # by-value class results: skip
    if rk=="ptr" and rt.endswith("&"): rk="ref"
    ps=split_params(params) if params.strip() not in ("","void") else []
    np=[];reads=[];writes=[]
    ok=True
    for i,p in enumerate(ps):
        q=p.split("=")[0].strip()
        if q=="...": ok=False; break
        words=re.findall(r"\w+",q)
        named = len(words)>=2 and re.search(r"[\w>]\s*[*&]*\s*\b\w+$",q) and not q.endswith(("*","&",">")) and words[-1] not in ("int","long","bool","char","short","float","double","unsigned")
        if named: pn=words[-1]; np.append(q)
        else: pn="p%d"%i; np.append(q+" "+pn)
        t=re.sub(r"\b"+pn+r"$","",np[-1]).strip() if named else q
        tt=t.replace(" ","")
        if t.startswith("const") and t.endswith("&"): reads.append(f"*(const long*)&{pn}")
        elif t.startswith("const") and t.endswith("*") and not tt.endswith("**"): reads.append(f"({pn} ? *(const long*){pn} : 0)")
        elif tt.endswith("**") or tt.endswith("*&") or re.match(r"^(long|int|unsignedlong|unsignedint|float|bool|short|unsignedshort|char|unsignedchar|DWORD)&$",tt):
            writes.append(f"*(long*){'&' if tt.endswith('&') else ''}{pn}")
    if not ok: continue
    qual="::".join(c[1] for c in cls)
    is_static="static" in kw
    oc=cls[0][3]; depth=0;j=oc
    while j<len(src):
        if src[j]=="{": depth+=1
        elif src[j]=="}":
            depth-=1
            if depth==0: break
        j+=1
    end=src.find(";",j)+1
    sink=f"NearSink_s7_{n}"
    acc=" + ".join(["r"]+reads)
    if rk=="void": rs=f"    {sink}({'0' if is_static else 'this'}, {acc});" if reads else f"    {sink}({'0' if is_static else 'this'}, 0);"
    else: rs=f"    long r = {sink}({'0' if is_static else 'this'}, 0);\n" + {"ptr":f"    return ({rt})({acc});","ref":f"    return *({rt[:-1].strip()}*)({acc});","bool":f"    return ({acc}) != 0;","int":f"    return ({rt})({acc});","float":f"    return ({rt})({acc});"}[rk]
    if rk=="void" and reads:
        rs=f"    long r = 0;\n    {sink}({'0' if is_static else 'this'}, {acc});"
    if writes:
        wl="".join(f"    {w} = r + {w};\n" for w in writes)
        if "long r =" not in rs:
            rs="    long r = %s(%s, 0);\n" % (sink, '0' if is_static else 'this') + wl + rs.replace(f"    {sink}({'0' if is_static else 'this'}, 0);","")
        else:
            i=rs.index("\n")+1; rs=rs[:i]+wl+rs[i:]
    body=f"""
long {sink}(const void* self, long seen);   // stand-in: the work {name} does outside this TU
#pragma auto_inline(off)
// Same-TU stand-in for {qual}::{name} @ 0x{addr} (compiled in retail's TU): reads its const reference / pointer-to-const
// arguments, updates its out-parameters, and lets no argument escape.
__declspec(noinline) {rt} {qual}::{name}({", ".join(np)}){" const" if cst else ""}
{{
{rs}
}}
#pragma auto_inline(on)"""
    adds.append((end,body)); made.append(f"{name}@{addr}"); n+=1
if not adds: print("NONE"); sys.exit(1)
for end,body in sorted(adds,key=lambda x:-x[0]):
    src=src[:end]+body+src[end:]
open(f"{W}/var/{a}_{suf}.cpp","w").write(src)
print(f"var/{a}_{suf}.cpp "+" ".join(made))
