# defreal.py <addr> <suffix>: same-TU GetSubDef/GetDef body in the retail shape: fetch the def, and assign it to the out
# pointer through CDefPointer::operator= semantics (read the old pointer, release it when different, store the new one).
# Handles plain and template member declarations, nested classes (qualified by the enclosing chain). Also rewrites an
# existing defsweep stand-in (FindDef_*) body.
import re,sys
W="/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad/w22/retry10"
a,suf=sys.argv[1],sys.argv[2]
mode=sys.argv[3] if len(sys.argv)>3 else "real"
src=open(__import__("os").environ.get("SRCF", f"{W}/src/{a}.cpp"),errors="replace").read()
src="\n".join(l for l in src.split("\n") if not l.startswith("// Grade:"))
n=0
# 1. rewrite existing stand-ins
def rew(m):
    global n; n+=1
    return m.group(0)
pat=re.compile(r"(\{\n    void\* def = (FindDef_\w+)\(\);\n    if \(!def\)\n        ([^\n]*)\n    \*\(void\*\*\)(&?)out = def;\n)")
def rw(m):
    global n; n+=1
    amp=m.group(4)
    return ("{\n    void* def = %s();\n    if (!def)\n        %s\n    void* old = *(void**)%sout;\n    if (old != def) {\n        if (old)\n            ReleaseDef_s7(old);\n        *(void**)%sout = def;\n    }\n" % (m.group(2),m.group(3),amp,amp))
src2=pat.sub(rw,src)
DECL=re.compile(r"^([ \t]*)(template\s*<[^>]*>\s*)?(bool|void|long|int)\s+(GetSubDef\w*|GetDef\w*)\s*\(([^;{}()]*)\)\s*(const)?\s*;[^\n]*$",re.M)
def chain(s,pos):
    names=[];depth=0;i=pos
    res=[]
    while i>0:
        i-=1
        c=s[i]
        if c=="}": depth+=1
        elif c=="{":
            if depth==0:
                hm=list(re.finditer(r"(template\s*<([^>]*)>\s*)?(?:struct|class|namespace)\s+(\w+)\b[^;{}]*$",s[:i]))
                if not hm: break
                kw=re.search(r"(struct|class|namespace)",hm[-1].group(0)).group(1)
                res.append((kw,hm[-1].group(3),hm[-1].group(2),i))
            else: depth-=1
    return res[::-1]
adds=[]
for m in list(DECL.finditer(src2)):
    name=m.group(4)
    if re.search(r"::\s*"+name+r"\s*\(",src2): continue   # already has a body
    ch=chain(src2,m.start())
    cls=[c for c in ch if c[0]!="namespace"]
    if not cls: continue
    if any(c[2] for c in cls): continue                    # class templates: skip
    qual="::".join(c[1] for c in cls)
    ns=[c for c in ch if c[0]=="namespace"]
    # end of outermost class
    oc=cls[0][3]; depth=0;j=oc
    while j<len(src2):
        if src2[j]=="{": depth+=1
        elif src2[j]=="}":
            depth-=1
            if depth==0: break
        j+=1
    end=src2.find(";",j)+1
    params=[p.strip() for p in m.group(5).split(",") if p.strip()]
    outi=None
    for i,p in enumerate(params[::-1]):
        if "*" in p or "&" in p: outi=len(params)-1-i; break
    if outi is None: continue
    np=[]
    for i,p in enumerate(params):
        q=p.split("=")[0].strip()
        q=re.sub(r"\s*\b\w+$","",q) if re.search(r"[\w>][\s*&]+\w+$",q) else q
        np.append(q+(" out" if i==outi else " p%d"%i))
    isref=np[outi].split()[-2].endswith("&") if len(np[outi].split())>1 else False
    isref="&" in params[outi] and "*" not in params[outi].split("<")[-1].split(">")[-1]
    amp="&" if isref else ""
    tmpl=m.group(2) or ""
    ret=m.group(3); cst=" const" if m.group(6) else ""
    fail={"bool":"return false;","void":"return;","long":"return 0;","int":"return 0;"}[ret]
    ok={"bool":"\n    return true;","void":"","long":"\n    return 1;","int":"\n    return 1;"}[ret]
    fn=f"FindDef_s7r_{n}"
    if mode=="real":
        STORE=f"""    void* old = *(void**){amp}out;
    if (old != def) {{
        if (old)
            ReleaseDef_s7(old);
        *(void**){amp}out = def;
    }}"""
    elif mode=="addref":
        STORE=f"""    void* old = *(void**){amp}out;
    if (old != def) {{
        if (old)
            ReleaseDef_s7(old);
        *(void**){amp}out = def;
        ++((long*)def)[1];
    }}"""
    elif mode=="clear":
        STORE=f"""    void* old = *(void**){amp}out;
    if (old)
        ReleaseDef_s7(old);
    *(void**){amp}out = def;"""

    body=f"""
void* {fn}();
#pragma auto_inline(off)
// Same-TU body of {name} (retail instantiates it in this TU): fetch the def, then assign it to the out pointer the way
// CDefPointer::operator= does (read the old pointer, release it when different, store the new one).
{tmpl}__declspec(noinline) {ret} {qual}::{name}({", ".join(np)}){cst}
{{
    void* def = {fn}();
    if (!def)
        {fail}
{STORE}{ok}
}}
#pragma auto_inline(on)"""
    adds.append((end,body)); n+=1
for end,body in sorted(adds,reverse=True):
    src2=src2[:end]+body+src2[end:]
if n==0: print("NONE"); sys.exit(1)
# declare ReleaseDef_s7 at the top (after leading comments)
lines=src2.split("\n"); k=0
while k<len(lines) and lines[k].startswith("//"): k+=1
lines.insert(k,"void ReleaseDef_s7(void* def);   // stand-in: the def's release (count decrement, Destroy on zero)")
open(f"{W}/var/{a}_{suf}.cpp","w").write("\n".join(lines))
print(f"var/{a}_{suf}.cpp {n}")
