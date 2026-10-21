# tmplbody.py <addr> <suffix>: same-TU no-escape stand-ins for declared-only STL-like template instantiations (find, erase,
# insert, lower_bound, __copy, ...), free functions or members of non-template classes, at any distance (template bodies are
# visible in the caller's TU). Reads const-ref / pointer-to-const args (except empty tags), writes out pointers, returns an
# opaque value; a by-value iterator result returns one of the same-typed parameters (first-or-last).
import re,sys,os
W="/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad/w22/retry10"
a,suf=sys.argv[1],sys.argv[2]
src=open(os.environ.get("SRCF", f"{W}/src/{a}.cpp"),errors="replace").read()
src="\n".join(l for l in src.split("\n") if not l.startswith("// Grade:"))
NAMES=re.compile(r"^(_*[Ff]ind\w*|erase|Erase|insert|Insert\w*|lower_bound|upper_bound|LowerBound|UpperBound|equal_range|EqualRange|__copy\w*|copy|__uninitialized\w*|uninitialized\w*|__fill\w*|fill\w*|remove\w*|for_each|__unguarded\w*|__adjust_heap|__push_heap|__pop_heap|__linear_insert|__insertion_sort|__final_insertion_sort|__introsort_loop|unique|find_if|count|_M_erase|_M_insert\w*|_M_fill_insert|_M_range_insert)$")
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
DECL=re.compile(r"^([ \t]*)((?:static|inline)\s+)?([\w:<>,* &]+?)\s*\b(~?\w+)\s*\(([^;{}()]*(?:\([^()]*\)[^;{}()]*)*)\)\s*(const)?\s*;[^\n]*$",re.M)
adds=[];n=0;made=[]
for m in DECL.finditer(src):
    kw,ret,name,params,cst=m.group(2) or "",m.group(3).strip(),m.group(4),m.group(5),m.group(6)
    if not NAMES.match(name): continue
    if re.match(r"^(return|else|new|delete)\b",ret) or ret.endswith("::") : continue
    if re.search(r"(::|\b)"+re.escape(name)+r"\s*\([^;{}]*\)\s*(const)?\s*\{",src[m.end():]+src[:m.start()]) and re.search(r"\b"+re.escape(name)+r"\s*\([^;]*\)\s*(const)?\s*\n?\s*\{",src): continue
    line_start=src.rfind("\n",0,m.start())+1
    if "template" in src[max(0,line_start-200):m.start()].split("\n")[-1]: continue
    prev=src[:m.start()].rstrip()
    if prev.endswith(">") and re.search(r"template\s*<[^>]*>\s*$",prev): continue
    ch=chain(src,m.start())
    if ch is None: continue
    cls=[c for c in ch if c[0]!="namespace"]
    if any(c[2] for c in cls): continue
    rt=ret.replace("static","").strip()
    cc=""
    mcc=re.search(r"\b(__fastcall|__stdcall|__cdecl|__thiscall)\b",rt)
    if mcc: cc=mcc.group(1); rt=re.sub(r"\s*\b"+cc+r"\b","",rt).strip()
    ps=split_params(params) if params.strip() not in ("","void") else []
    np=[];names=[];types=[];reads=[];writes=[]
    ok=True
    for i,p in enumerate(ps):
        q=p.split("=")[0].strip()
        if q=="..." : ok=False; break
        words=re.findall(r"\w+",q)
        named = len(words)>=2 and re.search(r"[\w>]\s*[*&]*\s*\b\w+$",q) and not q.endswith(("*","&",">")) and words[-1] not in ("int","long","bool","char","short","float","double","unsigned")
        if named: pn=words[-1]; t=q[:q.rfind(pn)].strip()
        else: pn="p%d"%i; t=q
        np.append(t+" "+pn); names.append(pn); types.append(t)
        tt=t.replace(" ","")
        istag=bool(re.search(r"(tag|_type|Tag)\w*&$",tt)) or re.search(r"(iterator_tag|__true_type|__false_type|SScalarTag|SMoveTag|Tag)\w*",t) is not None
        if istag: continue
        if t.endswith("&") and re.search(r"\bconst\b",t): reads.append(f"*(const long*)&{pn}")
        elif t.startswith("const") and t.endswith("*") and not tt.endswith("**"): reads.append(f"({pn} ? *(const long*){pn} : 0)")
        elif tt.endswith("**") or tt.endswith("*&"): writes.append(f"*(long*){'&' if tt.endswith('&') else ''}{pn}")
    if not ok: continue
    is_member=bool(cls) and "static" not in kw
    self="this" if is_member else "0"
    sink=f"TmplSink_s7_{n}"
    acc=" + ".join(["0"]+reads)
    # return kinds
    if rt=="void": body=f"    long r = {sink}({self}, {acc});\n" + "".join(f"    {w} = r;\n" for w in writes) + "    (void)r;"
    elif rt in ("bool",): body=f"    long r = {sink}({self}, {acc});\n" + "".join(f"    {w} = r;\n" for w in writes) + "    return r != 0;"
    elif rt.endswith("*"): body=f"    long r = {sink}({self}, {acc});\n" + "".join(f"    {w} = r;\n" for w in writes) + f"    return ({rt})r;"
    elif rt in ("long","int","unsigned long","unsigned int","size_t","unsigned"): body=f"    long r = {sink}({self}, {acc});\n    return ({rt})r;"
    elif rt in types:
        idx=[i for i,t in enumerate(types) if t==rt]
        f0=names[idx[0]]; f1=names[idx[-1]]
        body=f"    long r = {sink}({self}, {acc});\n    return r ? {f0} : {f1};"
    else: continue
    qual="::".join(c[1] for c in cls) if cls else ""
    fname=(qual+"::" if is_member or cls else "")+name
    if not cls: fname=name
    if cls:
        oc=cls[0][3]; depth=0;j=oc
        while j<len(src):
            if src[j]=="{": depth+=1
            elif src[j]=="}":
                depth-=1
                if depth==0: break
            j+=1
        end=src.find(";",j)+1
    else:
        end=src.find("\n",m.end()-1)
        if end<0: end=len(src)
    statickw="" if is_member or not cls else ""
    rbase=re.sub(r"[*&\s]|\bconst\b","",rt)
    if cls:
        cbody=src[cls[-1][3]:end]
        if re.search(r"\b(struct|class|typedef)\b[^;{]*\b"+re.escape(rbase)+r"\b",cbody) and "::" not in rt:
            rt2=re.sub(r"\b"+re.escape(rbase)+r"\b",qual+"::"+rbase,rt,count=1)
        else: rt2=rt
    else: rt2=rt
    decl_ret=(rt2+(" "+cc if cc else ""))
    bodytxt=f"""
long {sink}(const void* self, long seen);   // stand-in: the work {name} does (outside the bytes graded here)
#pragma auto_inline(off)
// Same-TU stand-in for the {name} instantiation (template bodies are compiled in the caller's TU): reads the const-reference
// arguments (not the empty tags), stores the out pointers, lets no argument escape.
__declspec(noinline) {decl_ret} {fname}({", ".join(np)}){" const" if cst else ""}
{{
{body}
}}
#pragma auto_inline(on)"""
    adds.append((end,bodytxt)); made.append(name); n+=1
if not adds: print("NONE"); sys.exit(1)
for end,b in sorted(adds,key=lambda x:-x[0]):
    src=src[:end]+b+src[end:]
open(f"{W}/var/{a}_{suf}.cpp","w").write(src)
print(f"var/{a}_{suf}.cpp "+" ".join(made))
