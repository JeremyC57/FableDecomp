# refsweep.py [--only-ref] <addr>...: lb + def stand-ins (as combosweep) plus a generic same-TU noinline stand-in for EVERY other
# out-of-line declared function (member or free) that takes a class-type reference/pointer parameter: the stand-in writes one
# word through each such parameter (so the caller's address-taken locals do not escape) and returns a default. Probe-graded.
import re,sys,os,subprocess,importlib.util
W=os.environ["S"]+"/w22/retry10"
ARGS=[a for a in sys.argv[1:] if not a.startswith("--")]
ONLY_REF="--only-ref" in sys.argv
def load(n):
    spec=importlib.util.spec_from_file_location(n,f"{W}/var/{n}.py"); m=importlib.util.module_from_spec(spec)
    sa=sys.argv; sys.argv=[n]; spec.loader.exec_module(m); sys.argv=sa; return m
lb=load("lbsweep"); d3=load("defsweep3")
PRIM=set("void bool char short int long float double unsigned signed __int64 size_t".split())
SKIPNAMES=re.compile(r"^(operator|~|GetSubDef|GetDef|GetPDefFromGlobalIndex|LowerBound)")
DECL=re.compile(r"^([ \t]*)((?:template\s*<[^>]*>\s*)?)((?:static\s+|inline\s+|virtual\s+)*)([A-Za-z_][\w:<>*&\s]*?)\s+(__fastcall\s+|__cdecl\s+|__stdcall\s+)?([A-Za-z_]\w*)\s*\(([^;{}()]*)\)\s*(const)?\s*;[^\n]*$",re.M)
def split_params(ps):
    out=[];depth=0;cur=""
    for ch in ps:
        if ch in "<(": depth+=1
        if ch in ">)": depth-=1
        if ch=="," and depth==0: out.append(cur.strip()); cur=""
        else: cur+=ch
    if cur.strip(): out.append(cur.strip())
    return out
def base_type(p):
    q=p.split("=")[0].strip()
    q=re.sub(r"\b(const|volatile)\b","",q)
    q=re.sub(r"[*&]"," ",q)
    toks=[t for t in q.split() if t]
    return toks
def is_classref(p):
    q=p.split("=")[0].strip()
    if "*" not in q and "&" not in q: return False
    if q.count("*")>=2: return False
    toks=base_type(q)
    if not toks: return False
    # drop the parameter name (last token) if there are >=2 tokens and the last is a plain identifier not a known type keyword
    tt=toks[:-1] if len(toks)>=2 else toks
    if any(t in PRIM for t in tt): return False
    if any(t in PRIM for t in toks) and len(toks)==1: return False
    return True
def convert(src):
    out=src; n=0; notes=[]
    # functions that already have a body in the TU
    defined=set(re.findall(r"^[^\n;/]*?\b(\w+)::(\w+)\s*\([^;{]*\)[^;{]*\{",src,re.M))
    defined_free=set(re.findall(r"^(?:static\s+|inline\s+)?[A-Za-z_][\w:<>*&\s]*?\s+(\w+)\s*\([^;{]*\)\s*\{",src,re.M))
    for m in list(DECL.finditer(src))[::-1]:
        indent,tmpl,quals,ret,cc,name,params,cst=m.groups(); cst=cst or ""
        if "virtual" in quals or "=" in params and "= 0" in m.group(0): continue
        if SKIPNAMES.match(name): continue
        if "operator" in ret: continue
        ret=re.sub(r"\b(public|private|protected)\s*:\s*","",ret).strip()
        if ret in ("return","else","new","delete") or ret.startswith("//"): continue
        plist=split_params(params)
        refs=[i for i,p in enumerate(plist) if is_classref(p)]
        if not refs: continue
        # return type must be void/bool/integer/float/pointer
        rt=ret.replace("const","").strip()
        if "*" in rt: retstmt="return 0;"
        elif rt=="void": retstmt=""
        elif rt=="bool": retstmt="return false;"
        elif rt in ("float","double"): retstmt="return 0.0f;"
        elif all(t in PRIM for t in rt.split()): retstmt="return 0;"
        else: notes.append(name+":retclass"); continue
        # member or free?
        pre=src[:m.start()]
        sm=list(re.finditer(r"^(?:struct|class) (\w+)\b[^;{]*\{",pre,re.M))
        sname=None
        if sm:
            body=src[sm[-1].end():m.start()]
            if body.count("{")>=body.count("}"): sname=sm[-1].group(1)
        if sname and name==sname: continue   # constructor
        if sname and (sname,name) in defined: continue
        if not sname and name in defined_free: continue
        if sname and ("static" in quals): sname_call=sname; static=True
        else: static=False
        # name the parameters
        np=[]
        for i,p in enumerate(plist):
            q=p.split("=")[0].strip()
            if re.search(r"[\w>*&]\s+\w+$",q) and not q.endswith("*") and not q.endswith("&") and len(base_type(q))>=2: np.append(q)
            else: np.append(q+" p%d"%i)
        stores=[]
        for i in refs:
            q=np[i]; pname=q.split()[-1]
            if "&" in q: stores.append(f"    *(void**)&{pname} = p;")
            else: stores.append(f"    *(void**){pname} = p;")
        if sname:
            end=src.find("\n};",m.end())
            if end<0: continue
            end+=3
            if tmpl: head=f"{tmpl.strip()} {ret} {sname}::{name}({', '.join(np)}){' '+cst if cst else ''}"
            else: head=f"__declspec(noinline) {ret} {cc or ''}{sname}::{name}({', '.join(np)}){' '+cst if cst else ''}"
        else:
            end=m.end()
            if tmpl: head=f"{tmpl.strip()} {ret} {cc or ''}{name}({', '.join(np)})"
            else: head=f"__declspec(noinline) {ret} {cc or ''}{name}({', '.join(np)})"
        helper=f"FindRef_{sname or 'free'}_{name}_{n}"
        sb=f'''
void* {helper}();   // stand-in callee of the stand-in body below
// Stand-in body (the real one is out of line in retail): it only writes a word through the reference parameter inline, so
// VC7.1 knows the caller's address-taken local does not escape the call.
#pragma auto_inline(off)
{head}
{{
    void* p = {helper}();
    if (!p)
        {retstmt or "return;"}
{chr(10).join(stores)}
    {retstmt}
}}
#pragma auto_inline(on)'''
        # insert into `out` at the position corresponding to `end` (out and src aligned since we go backwards)
        out=out[:end]+sb+out[end:]; n+=1
    return (out if n else None), (";".join(notes)+f" {n} ref stand-ins")
if __name__=="__main__":
    for a in ARGS:
        p=f"{W}/src/{a}.cpp"
        if not os.path.exists(p): print(a,"no src"); continue
        src=open(p).read()
        if not ONLY_REF:
            r1=lb.convert(src); s1=r1[0] if isinstance(r1,tuple) else r1
            if not s1: s1=src
            s2,_=d3.convert(s1)
            if s2 is None: s2=s1
        else: s2=src
        s3,why=convert(s2)
        if s3 is None: print(a,"skip: no ref stand-ins",flush=True); continue
        vp=f"{W}/var/{a}_ref.cpp"; open(vp,"w").write(s3)
        r=subprocess.run(["python3",f"{W}/var/pv.py",a,vp],capture_output=True,text=True)
        lines=r.stdout.strip().splitlines()
        head=lines[0] if lines else r.stdout[-300:].replace("\n"," ")
        tail=lines[-1] if lines else ""
        print(a,"|",why,"|",head[:70],"|",tail,flush=True)
