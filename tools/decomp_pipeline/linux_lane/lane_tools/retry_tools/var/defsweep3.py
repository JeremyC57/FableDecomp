# defsweep3.py <addr>...: like defsweep2 but (a) replaces an existing out-of-line body of the callee with the stand-in and
# (b) renames the out parameter by substituting its trailing identifier (handles CDefPointer<const T>* out).
import re,sys,subprocess,os
W=os.environ["S"]+"/w22/retry10"
DECL=re.compile(r"^(\s*)(?:template\s*<\s*class\s+T\s*>\s*)?(bool|void|long|int)\s+(GetSubDef\w*|GetDef\w*|GetPDefFromGlobalIndex\w*)\s*\(([^;{}]*)\)\s*(const)?\s*;[^\n]*$",re.M)
def cut_body(src, sname, name):
    # remove an existing out-of-line definition `... sname::name(...) ... { ... }` (with any template/pragma lines directly above)
    m=re.search(r"^[^\n;]*\b"+re.escape(sname)+r"::"+re.escape(name)+r"\s*\([^;{]*\)[^;{]*\{",src,re.M)
    if not m: return src,False
    i=m.end(); depth=1
    while depth and i<len(src):
        if src[i]=="{": depth+=1
        elif src[i]=="}": depth-=1
        i+=1
    start=m.start()
    # also drop a preceding `template <class T>` line
    ls=src.rfind("\n",0,start-1)+1
    if src[ls:start].strip().startswith("template"): start=ls
    return src[:start]+src[i:].lstrip("\n"),True
def convert(src):
    out=src; n=0; notes=[]
    for m in list(DECL.finditer(src))[::-1]:
        ret,name,params,cst=m.group(2),m.group(3),m.group(4),m.group(5) or ""
        pre=src[:m.start()]
        line_start=src.rfind("\n",0,m.start())+1
        tmpl="template" in src[m.start():m.end()]
        sm=list(re.finditer(r"^(?:struct|class) (\w+)\b[^;{]*\{",pre,re.M))
        if not sm: notes.append(name+":nostruct"); continue
        sname=sm[-1].group(1)
        body=src[sm[-1].end():m.start()]
        if body.count("{")!=body.count("}"): notes.append(name+":nested"); continue
        end=out.find("\n};",m.end())
        if end<0: notes.append(name+":noend"); continue
        end+=3
        plist=[p.strip() for p in params.split(",") if p.strip()]
        outp=None
        for p in plist[::-1]:
            if "*" in p or "&" in p: outp=p; break
        if outp is None: notes.append(name+":noout"); continue
        plist2=[]
        for i,p in enumerate(plist):
            q=p.split("=")[0].strip()
            if p==outp:
                if re.search(r"[*&]\s*\w+$",q): q=re.sub(r"\w+$","out",q)
                else: q=q+" out"
            elif not re.search(r"[\w>*&]\s+\w+$",q) or q.endswith("*") or q.endswith("&"):
                q=q+(" p%d"%i)
            plist2.append(q)
        isref="&" in outp
        store="*(void**)&out = def;" if isref else "*(void**)out = def;"
        retstmt={"bool":"return true;","void":"","long":"return 1;","int":"return 1;"}[ret]
        failstmt={"bool":"return false;","void":"return;","long":"return 0;","int":"return 0;"}[ret]
        sb=f'''
void* FindDef_{sname}_{name}_{n}();   // stand-in callee of the stand-in body below
// Stand-in body (the real one is out of line in retail): it writes the out slot only inline, so VC7.1 knows the def
// pointer's address does not escape the call (the landed 0x00848730 idiom).
#pragma auto_inline(off)
{"template <class T> " if tmpl else "__declspec(noinline) "}{ret} {sname}::{name}({", ".join(plist2)}){" "+cst if cst else ""}
{{
    void* def = FindDef_{sname}_{name}_{n}();
    if (!def)
        {failstmt}
    {store}
    {retstmt}
}}
#pragma auto_inline(on)'''
        out2,cut=cut_body(out[end:],sname,name)
        out=out[:end]+sb+out2; n+=1
        if cut: notes.append(name+":replaced-body")
    return (out if n else None), (";".join(notes) or "")+f" {n} stand-ins"
for a in sys.argv[1:]:
    p=f"{W}/src/{a}.cpp"
    if not os.path.exists(p): print(a,"no src"); continue
    src=open(p).read()
    out,why=convert(src)
    if out is None: print(a,"skip:",why, flush=True); continue
    vp=f"{W}/var/{a}_def3.cpp"; open(vp,"w").write(out)
    r=subprocess.run(["python3",f"{W}/var/pv.py",a,vp],capture_output=True,text=True)
    lines=r.stdout.strip().splitlines()
    head=lines[0] if lines else r.stdout[-400:].replace("\n"," ")
    tail=lines[-1] if lines else ""
    print(a,"|",why,"|",head[:80],"|",tail, flush=True)
