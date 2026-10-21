# resize2.py (from grow2.py): same-TU body for a declared-only vector resize(n, const T& value)
# grow2.py <addr> <mode> [suffix]: write var/<addr>_<suffix>.cpp with a same-TU out-of-class body for the declared-only
# growth member (_M_insert_overflow / InsertGrow / InsertOverflow). mode: copy (placement copy-construct of the value),
# assign (*p = value), word (first word only). Brace-matched enclosing class; multi-line declarations handled.
import re,sys
W="/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad/w22/retry10"
a,mode=sys.argv[1],sys.argv[2]; suf=sys.argv[3] if len(sys.argv)>3 else "s7g"+mode[0]
src=open(f"{W}/src/{a}.cpp",errors="replace").read()
src="\n".join(l for l in src.split("\n") if not l.startswith("// Grade:"))
m=re.search(r"^([ \t]*)(void)\s+(resize)\s*\(([^;{}]*?,[^;{}]*?)\)\s*;",src,re.M)
if not m: print("NODECL"); sys.exit(1)
ret,name,params=m.group(2),m.group(3),m.group(4)
# strip comments inside params
params=re.sub(r"//[^\n]*","",params)
# enclosing class chain (outermost first): scan back for unmatched '{'
def chain(s,pos):
    res=[];depth=0;i=pos
    while i>0:
        i-=1
        c=s[i]
        if c=="}": depth+=1
        elif c=="{":
            if depth==0:
                hm=list(re.finditer(r"(template\s*<([^<>]*(?:<[^<>]*>[^<>]*)*)>\s*)?(?<![<,]\s)(?<![<,])\b(struct|class|namespace)\s+(\w+)\b[^;{}()]*$",s[:i]))
                if not hm: break
                res.append((hm[-1].group(3),hm[-1].group(4),hm[-1].group(2),i))
            else: depth-=1
    return res[::-1]
ch=chain(src,m.start())
cls=[c for c in ch if c[0]!="namespace"]
if not cls: print("NOCLASS"); sys.exit(1)
inner=cls[-1]; cname="::".join(c[1] for c in cls); tmpl=inner[2] is not None and inner[2]!=""; targs=inner[2]
if any(c[2] for c in cls[:-1]): print("NESTEDTEMPLATE"); sys.exit(1)
oc=cls[0][3]; depth=0;j=oc
while j<len(src):
    if src[j]=="{": depth+=1
    elif src[j]=="}":
        depth-=1
        if depth==0: break
    j+=1
end=src.find(";",j)+1
ps=[p.strip() for p in params.split(",")]
np=[];names=[]
for k,p in enumerate(ps):
    q=p.split("=")[0].strip()
    words=re.findall(r"\w+",q)
    last=words[-1] if words else ""
    is_named = len(words)>=2 and not re.search(r"[*&]\s*$",q) and last not in("int","long","bool","char","short","type","T") and not q.endswith(">")
    if q.split()[-1] in ("int","long","bool","char"): is_named=False
    if is_named: np.append(q); names.append(last)
    else: np.append(q+" p%d"%k); names.append("p%d"%k)
vt=re.sub(r"\s*\w+$","",np[1]).strip(); etype=re.sub(r"^const\s+|\s*&$","",vt).strip(); etype=re.sub(r"\s*&\s*$","",etype)
ptype=etype+"*"
vname=names[1]; count=names[0]
if tmpl:
    tp=re.findall(r"(?:class|typename)\s+(\w+)",targs)
    tdef=re.sub(r"\s*=\s*[^,]+","",targs)
    qual=f"template <{tdef}>\n__declspec(noinline) {ret} {cname}<{', '.join(tp)}>::{name}({', '.join(np)})"
else:
    qual=f"__declspec(noinline) {ret} {cname}::{name}({', '.join(np)})"
store={"copy":f"new (p) {etype}({vname});","assign":f"*p = {vname};","word":f"*(void**)p = *(void* const*)&{vname};"}[mode]
r={"void":"","bool":"\n    return true;","long":"\n    return 0;"}[ret]
needs_new = mode=="copy" and not re.search(r"operator\s+new\s*\(\s*unsigned int\s*,\s*void\s*\*",src) and "#include <new>" not in src
body=f"""
void* ResizeVectorStorage_s7(void* vector, unsigned long count);   // stand-in: resizes, returns the first new element or 0
{"inline void* operator new(unsigned int, void* where) { return where; }" if needs_new else ""}
#pragma auto_inline(off)
// Same-TU body of the vector resize instantiation (retail compiled it in this TU): the new elements are copies of the
// value read through its const reference.
{qual}
{{
    {ptype} p = ({ptype})ResizeVectorStorage_s7(this, {count});
    if (p)
        {store}{r}
}}
#pragma auto_inline(on)"""
out=src[:end]+body+src[end:]
open(f"{W}/var/{a}_{suf}.cpp","w").write(out)
print(f"var/{a}_{suf}.cpp class={cname} name={name} ptype={ptype} v={vname} count={count}")
