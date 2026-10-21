# growsweep.py <out.tsv> <addr>...: same-TU body for declared-only vector growth members (_M_insert_overflow / InsertGrow /
# InsertOverflow): reads the value through its const reference, leaves the tag alone; probe-grade with pv.py
import re,sys,os,subprocess
S="/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad"; W=f"{S}/w22/retry10"
out=open(sys.argv[1],"a")
DECL=re.compile(r"^([ \t]*)(template\s*<[^>]*>\s*)?(void|bool|long)\s+(_M_insert_overflow|InsertGrow|InsertOverflow|_M_insert_overflow_aux)\s*\(([^;{}()]*)\)\s*;[^\n]*$",re.M)
for a in sys.argv[2:]:
    src=open(f"{W}/src/{a}.cpp",errors="replace").read()
    m=DECL.search(src)
    if not m: continue
    name=m.group(4); params=[p.strip() for p in m.group(5).split(",")]
    pre=src[:m.start()]
    sm=list(re.finditer(r"^(template\s*<([^>]*)>\s*)?(?:struct|class)\s+(\w+)\b[^;{]*\{",pre,re.M))
    if not sm: continue
    tmpl=sm[-1].group(1); targs=sm[-1].group(2); sname=sm[-1].group(3)
    if re.search(r"\b"+re.escape(sname)+r"\s*(<[^>]*>)?\s*::\s*"+name+r"\s*\(",src): continue
    end=src.find("\n};",m.end())
    if end<0: continue
    end+=3
    # name params
    np=[]; vname=None; wtype=None
    for i,p in enumerate(params):
        q=p.split("=")[0].strip()
        toks=re.findall(r"[\w:<>]+|[*&]",q)
        if len(re.findall(r"\w+",q))>=2 and not q.endswith(("*","&")): pn=re.findall(r"\w+",q)[-1]; qq=q
        else: pn=f"p{i}"; qq=q+" "+pn
        np.append(qq)
        if i==0: wtype=re.sub(r"\s*\w+$","",qq).strip()
        if i==1: vname=pn
    if tmpl:
        tparam=re.findall(r"(?:class|typename)\s+(\w+)",targs)
        head=f"template <{targs}>\n__declspec(noinline) {m.group(3)} {sname}<{', '.join(tparam)}>::{name}({', '.join(np)})"
    else:
        head=f"__declspec(noinline) {m.group(3)} {sname}::{name}({', '.join(np)})"
    ret={"void":"","bool":"return true;","long":"return 0;"}[m.group(3)]
    body=f"""
void* GrowVectorStorage_s7(void* vector, unsigned long count);
#pragma auto_inline(off)
// Same-TU body of the vector growth template ({name}, instantiated in retail's TU): reads the value through its const
// reference into the new storage and never touches the empty tag (the landed push_back idiom).
{head}
{{
    {wtype} p = ({wtype})GrowVectorStorage_s7(this, {np[3].split()[-1] if len(np)>3 else 1});
    if (p)
        *(void**)p = *(void* const*)&{vname};
    {ret}
}}
#pragma auto_inline(on)"""
    v=src[:end]+body+src[end:]
    vp=f"{W}/var/{a}_s7grow.cpp"; open(vp,"w").write(v)
    env=dict(os.environ); env["S"]=S
    r=subprocess.run(["python3",f"{W}/var/pv.py",a,vp],capture_output=True,text=True,env=env,cwd="/home/user/FableDecomp").stdout
    last=(r.strip().splitlines() or ["?"])[-1]
    out.write(f"{a}\t{last[:160]}\n"); out.flush()
