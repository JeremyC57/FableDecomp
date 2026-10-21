# defsweep.py <addr>...: add a same-TU noinline stand-in body for every out-of-line GetSubDef/GetDef declaration (out pointer or
# reference written through a type-punned word store, so the out slot's address does not escape) and probe-grade.
import re,sys,subprocess,os
W=os.environ["S"]+"/w22/retry10"
DECL=re.compile(r"^(\s*)(?:template\s*<\s*class\s+T\s*>\s*)?(bool|void|long|int)\s+(GetSubDef\w*|GetDef\w*|GetPDefFromGlobalIndex\w*)\s*\(([^;{}]*)\)\s*(const)?\s*;[^\n]*$",re.M)
def convert(src):
    out=src; n=0; notes=[]
    for m in list(DECL.finditer(src))[::-1]:
        ret,name,params,cst=m.group(2),m.group(3),m.group(4),m.group(5) or ""
        pre=src[:m.start()]
        # skip template member declarations
        line_start=src.rfind("\n",0,m.start())+1
        tmpl="template" in src[line_start:m.start()]
        sm=list(re.finditer(r"^(?:struct|class) (\w+)\b[^;{]*\{",pre,re.M))
        if not sm: notes.append(name+":nostruct"); continue
        sname=sm[-1].group(1)
        # the struct must still be open at the decl: count braces between struct start and decl
        body=src[sm[-1].end():m.start()]
        if body.count("{")!=body.count("}"): notes.append(name+":nested"); continue
        end=src.find("\n};",m.end())
        if end<0: notes.append(name+":noend"); continue
        end+=3
        plist=[p.strip() for p in params.split(",") if p.strip()]
        # find the out parameter: the last pointer/reference parameter
        outp=None
        for p in plist[::-1]:
            if "*" in p or "&" in p: outp=p; break
        if outp is None: notes.append(name+":noout"); continue
        pname=re.split(r"[\s*&]+",outp)[-1]
        if not pname or pname==outp: pname="out"
        # normalise parameter names
        np=[]
        for i,p in enumerate(plist):
            q=p.split("=")[0].strip()
            if re.search(r"[\w>]\s*[*&]?\s*\w+$",q) and not q.endswith("*") and not q.endswith("&") and len(re.split(r"[\s*&]+",q))>1:
                np.append(q)
            else:
                np.append(q+(" p%d"%i))
        # rename the out param consistently
        plist2=[]
        for i,q in enumerate(np):
            if plist[i]==outp:
                base=re.sub(r"\s*\w+$","",q) if len(re.split(r"[\s*&]+",q))>1 else q
                if outp.endswith("*") or outp.endswith("&"): base=outp
                plist2.append(base+" out")
            else: plist2.append(q)
        isref="&" in outp
        store="*(void**)&out = def;" if isref else "*(void**)out = def;"
        retstmt={"bool":"return true;","void":"","long":"return 1;","int":"return 1;"}[ret]
        failstmt={"bool":"return false;","void":"return;","long":"return 0;","int":"return 0;"}[ret]
        body=f'''
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
        out=out[:end]+body+out[end:]; n+=1
    return (out if n else None), (";".join(notes) or f"{n} stand-ins")
for a in sys.argv[1:]:
    p=f"{W}/src/{a}.cpp"
    if not os.path.exists(p): print(a,"no src"); continue
    src=open(p).read()
    out,why=convert(src)
    if out is None: print(a,"skip:",why, flush=True); continue
    vp=f"{W}/var/{a}_def2.cpp"; open(vp,"w").write(out)
    r=subprocess.run(["python3",f"{W}/var/pv.py",a,vp],capture_output=True,text=True)
    lines=r.stdout.strip().splitlines()
    head=lines[0] if lines else r.stdout[-400:].replace("\n"," ")
    tail=lines[-1] if lines else ""
    print(a,"|",why,"|",head[:80],"|",tail, flush=True)
