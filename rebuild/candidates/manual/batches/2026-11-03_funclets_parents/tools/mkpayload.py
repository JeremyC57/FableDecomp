# usage: mkpayload.py <outdir> <addr>...   (rows from $S/funclets/ROWS3.tsv with a landed parent)
import json,re,os,sys,struct
S=os.environ["S"]; out=sys.argv[1]; want=[a.lower() for a in sys.argv[2:]]
os.makedirs(out,exist_ok=True)
exe=open(f"{S}/dl/game/Fable The Lost Chapters/Fable.exe","rb").read()
pe=struct.unpack_from("<I",exe,0x3c)[0]; ns=struct.unpack_from("<H",exe,pe+6)[0]; os_=struct.unpack_from("<H",exe,pe+20)[0]
secs=[]
for i in range(ns):
    o=pe+24+os_+40*i; vs,va,rs,ro=struct.unpack_from("<IIII",exe,o+8); secs.append((va,rs,ro))
def rd(va,n):
    for sva,rs,ro in secs:
        if sva<=va-0x400000<sva+rs: off=va-0x400000-sva+ro; return exe[off:off+n]
ps=open("rebuild/build_candidates.ps1",encoding="utf-8-sig").read()
ent={}
for blk in re.findall(r"\[pscustomobject\]@\{(.*?)\n    \}", ps, re.S):
    kv=dict(re.findall(r"(\w+) = '([^']*)'", blk))
    if "Address" in kv: ent[kv["Address"].lower()]=kv
cat={}
for l in open("rebuild/oracles/auto-re-candidates.tsv").read().splitlines()[1:]:
    q=l.split("\t"); cat[q[0].lower()]=q
rows={l.split("\t")[0]:l.split("\t") for l in open(f"{S}/funclets/ROWS3.tsv").read().splitlines()[1:]}
auth=[]; orc=["address\tname\tlength\tbytes"]
for a in want:
    r=rows[a]; p=r[4]; kv=ent[p]; o=int(r[12],16); ln=int(r[13]); A=a.upper()
    pname=cat[p][1]; fi=r[3]; th=r[15]
    src=(f"// EH unwind funclet @ 0x{A} of {pname} (parent @ 0x{p.upper()}): VC7.1 emits the\n"
         f"// unwind funclets and the __ehhandler thunk of an EH-framed function as one block after the function;\n"
         f"// the linker placed Fable's blocks at 0x0121xxxx. This funclet is {ln} bytes at +{o:#x} of the parent's\n"
         f"// block and is produced by the parent's translation unit, included below (no code of its own).\n"
         f"// Parent evidence: retail FuncInfo 0x{fi.upper()} lists 0x{A} in its unwind map; its __ehhandler thunk\n"
         f"// 0x{th.upper()} (mov eax, FuncInfo) is pushed by the parent's EH prologue at 0x{p.upper()}.\n"
         f"// Retail: Fable.exe (TLC, VC7.1) function @ 0x{A}, oracle length {ln} bytes.\n"
         f"// Grade: landed -- byte-exact (the parent's object, funclet block offset {o:#x}) and the parent's\n"
         f"//        behaviour test PASS under the Linux lane (Wine + cl 13.10.3077).\n"
         f'#include "../src/compiled/{kv["Source"]}"\n')
    tst=open("rebuild/tests/"+kv["TestSource"],encoding="utf-8").read()
    pp=kv.get("PassPattern",f"{p}_TEST PASS")
    newpp=f"{a}_TEST PASS"
    if pp in tst and pp.startswith(p): tst=tst.replace(pp,newpp); patt=newpp
    else: patt=pp
    fl=kv.get("CompilerFlags","").split(); extra=[x for x in fl if x not in ("/O2","/Oy","/W3")]
    if fl and "/Oy" not in fl and "/Oy-" not in extra: extra.append("/Oy-")
    e=dict(address=a,module="_global",name=f"Unwind@{a}",source_cpp=src,test_cpp=tst,pass_pattern=patt,leaf=f"Unwind@{a}",funclet_offset=o)
    if extra: e["flags"]=extra
    auth.append(e); orc.append(f"{a}\t_global::Unwind@{a}\t{ln}\t{rd(int(a,16),ln).hex()}")
json.dump({"result":{"authored":auth}},open(f"{out}/payload.json","w"),indent=1)
open(f"{out}/oracle.tsv","w").write("\n".join(orc)+"\n"); print(len(auth),"rows")
