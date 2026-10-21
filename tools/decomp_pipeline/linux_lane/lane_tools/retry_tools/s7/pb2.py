# pb2.py <addr> [suffix]: same-TU out-of-line body for a declared-only `void push_back(const X& v);` of a vector-like
# class (first three pointer members = start/finish/end): placement copy when there is room, else a growth stand-in.
import re,sys
W="/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad/w22/retry10"
a=sys.argv[1]; suf=sys.argv[2] if len(sys.argv)>2 else "s7pb"
src=open(__import__("os").environ.get("SRCF", f"{W}/src/{a}.cpp"),errors="replace").read()
src="\n".join(l for l in src.split("\n") if not l.startswith("// Grade:"))
m=re.search(r"^([ \t]*)void\s+push_back\s*\(\s*(const\s+[^&]+?)\s*&\s*(\w+)?\s*\)\s*;",src,re.M)
if not m: print("NODECL"); sys.exit(1)
vtype=m.group(2).replace("const","",1).strip(); vname=m.group(3) or "value"
depth=0;i=m.start()
while i>0:
    i-=1
    c=src[i]
    if c=="}": depth+=1
    elif c=="{":
        if depth==0: break
        depth-=1
ob=i
hm=list(re.finditer(r"(template\s*<([^>]*)>\s*)?(?:struct|class)\s+(\w+)\b[^;{]*$",src[:ob]))
if not hm: print("NOCLASS"); sys.exit(1)
h=hm[-1]; tmpl=h.group(1); targs=h.group(2); cname=h.group(3)
depth=0;j=ob
while j<len(src):
    if src[j]=="{": depth+=1
    elif src[j]=="}":
        depth-=1
        if depth==0: break
    j+=1
end=src.find(";",j)+1
body_txt=src[ob+1:j]
# pointer members at class depth (strip nested braces)
flat=re.sub(r"\{[^{}]*\}","{}",body_txt); flat=re.sub(r"\{[^{}]*\}","{}",flat)
mem=re.findall(r"^\s*([\w:<>, ]+?)\s*\*\s*(\w+)\s*;",flat,re.M)
if len(mem)<3:
    # fall back to "T* a; T* b; T* c;" on one line
    mem=re.findall(r"([\w:<>]+)\s*\*\s*(\w+)\s*;",flat)
if len(mem)<3: print("NOMEMBERS",mem); sys.exit(1)
f0,f1,f2=[x[1] for x in mem[:3]]
if tmpl:
    tp=re.findall(r"(?:class|typename)\s+(\w+)",targs)
    qual=f"template <{targs}>\n__declspec(noinline) void {cname}<{', '.join(tp)}>::push_back(const {vtype}& {vname})"
else:
    qual=f"__declspec(noinline) void {cname}::push_back(const {vtype}& {vname})"
has_new=bool(re.search(r"operator\s+new\s*\(\s*(unsigned int|size_t|unsigned)\s*,\s*void\s*\*",src)) or "#include <new>" in src
body=f"""
void* GrowVectorStorage_s7(void* vector, unsigned long count);
{"" if has_new else "inline void* operator new(unsigned int, void* where) { return where; }"}
#pragma auto_inline(off)
// Same-TU body of the out-of-line STLport push_back (the template instantiation retail compiled in this TU): placement
// copy of the value read through its const reference when there is room, else the growth path with the same copy.
{qual}
{{
    if ({f1} != {f2}) {{
        new ({f1}) {vtype}({vname});
        ++{f1};
    }} else {{
        {vtype}* p = ({vtype}*)GrowVectorStorage_s7(this, 1);
        if (p)
            new (p) {vtype}({vname});
    }}
}}
#pragma auto_inline(on)"""
out=src[:end]+body+src[end:]
open(f"{W}/var/{a}_{suf}.cpp","w").write(out)
print(f"var/{a}_{suf}.cpp class={cname} T={vtype} members={f0},{f1},{f2}")
