# grow3.py <addr> <suffix>: replace (or add) the vector growth member's body with a realistic reallocation: new storage
# from an opaque allocator (no caller pointers), head copy, `count` copies of the value read through its const reference,
# tail copy unless at the end, release of the old block, then the three members updated.
import re,sys,os
W="/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad/w22/retry10"
a,suf=sys.argv[1],sys.argv[2]
src=open(os.environ.get("SRCF", f"{W}/src/{a}.cpp"),errors="replace").read()
src="\n".join(l for l in src.split("\n") if not l.startswith("// Grade:"))
# 1. drop an existing GrowVectorStorage_s7-based body
src=re.sub(r"\nvoid\* GrowVectorStorage_s7\(void\* vector, unsigned long count\);[^\n]*","",src)
m=re.search(r"\n#pragma auto_inline\(off\)\n(?://[^\n]*\n)*((?:template <[^>]*>\n)?__declspec\(noinline\) void ([\w:<>, ]+)::(_M_insert_overflow|InsertGrow|InsertOverflow)\(([^)]*)\))\n\{\n(?:[^}]|\}(?!\n#pragma))*\}\n#pragma auto_inline\(on\)",src)
if not m: print("NOBODY"); sys.exit(1)
head,cls,name,params=m.group(1),m.group(2),m.group(3),m.group(4)
ps=[p.strip() for p in params.split(",")]
nm=[re.findall(r"\w+",p)[-1] for p in ps]
pos,val,cnt=nm[0],nm[1],nm[3] if len(nm)>3 else "1"
atend=nm[4] if len(nm)>4 else None
etype=re.sub(r"\s*\*\s*\w+$","",ps[0]).strip()
# vector class body: find the three pointer members
base=cls.split("<")[0].split("::")[-1]
cm=re.search(r"(?:struct|class)\s+"+re.escape(base)+r"\b[^{;]*\{(.*?)\n\};",src,re.S)
if not cm: print("NOCLASS"); sys.exit(1)
mem=re.findall(r"^\s*[\w:<>, ]+?\s*\*\s*(\w+)\s*;",cm.group(1),re.M)
proxy=re.search(r"^\s*_STLP_alloc_proxy<[^>]*>\s+(\w+)\s*;",cm.group(1),re.M)
if len(mem)<2: print("NOMEM",mem); sys.exit(1)
f0,f1=mem[0],mem[1]
f2=(proxy.group(1)+"._M_data") if (proxy and len(mem)<3) else (mem[2] if len(mem)>2 else None)
if not f2: print("NOEND"); sys.exit(1)
tail=f"    if (!{atend})\n        for ({etype}* in = {pos}; in != {f1}; ++in, ++out)\n            new (out) {etype}(*in);\n" if atend else ""
body=f"""
void* AllocateStorage_s7(unsigned long bytes);   // stand-in: the allocator (no caller pointers)
void FreeStorage_s7(void* block);                // stand-in: the allocator's release
#pragma auto_inline(off)
// Same-TU body of the vector growth instantiation ({name}): new storage of max(2*size, size+count), head copy, `count`
// copies of the value read through its const reference, tail copy unless at the end; the empty tag is never touched.
{head}
{{
    unsigned long size = (unsigned long)({f1} - {f0});
    unsigned long len = size + (size > {cnt} ? size : {cnt});
    {etype}* storage = ({etype}*)AllocateStorage_s7(len * sizeof({etype}));
    {etype}* out = storage;
    for ({etype}* in = {f0}; in != {pos}; ++in, ++out)
        new (out) {etype}(*in);
    for (unsigned long i = 0; i < {cnt}; ++i, ++out)
        new (out) {etype}({val});
{tail}    FreeStorage_s7({f0});
    {f0} = storage;
    {f1} = out;
    {f2} = storage + len;
}}
#pragma auto_inline(on)"""
src=src[:m.start()]+body+src[m.end():]
if "operator new(unsigned int, void*" not in src and "#include <new>" not in src:
    src="inline void* operator new(unsigned int, void* where) { return where; }\n"+src
open(f"{W}/var/{a}_{suf}.cpp","w").write(src); print(f"var/{a}_{suf}.cpp {cls}::{name} T={etype} members={f0},{f1},{f2}")
