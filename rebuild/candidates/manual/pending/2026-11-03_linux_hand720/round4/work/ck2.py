# ck2.py <addr> <leafprefix>: zip retail call targets (with bound names) against our call relocation symbols
import sys,os,subprocess,re,csv,collections
from capstone import *
S=os.environ["S"]; a=sys.argv[1].lower(); leaf=sys.argv[2]
obj=f"{S}/wine/q/{a}/{a}.obj"
out=subprocess.run(["objdump","-dr","--no-show-raw-insn",obj],capture_output=True,text=True).stdout
cands=[]; cur=None
for l in out.splitlines():
    m=re.match(r"([0-9a-f]{8}) <(.*)>:",l)
    if m and not m.group(2).startswith("$"):
        cur=[] if m.group(2).startswith(leaf) else None
        if cur is not None: cands.append(cur)
        continue
    if cur is not None: cur.append(l)
lines=max(cands,key=len); ours=[]
for i,l in enumerate(lines):
    if "\tcall " in l:
        if "*" in l: ours.append("IND")
        else:
            nx=lines[i+1] if i+1<len(lines) else ""
            ours.append(nx.split()[-1] if ("DISP32" in nx or "PC32" in nx) else "??")
bind=collections.defaultdict(collections.Counter)
for l in open("/home/user/FableDecomp/rebuild/oracles/reloc_bindings.tsv"):
    p=l.rstrip("\n").split("\t")
    if len(p)==3 and p[1]!="target": bind[int(p[1],16)][p[0]]+=1
rows=[r for f in os.listdir(f"{S}/batch") if f.endswith("_oracle.tsv") for r in csv.DictReader(open(f"{S}/batch/{f}"),delimiter="\t") if r["address"].lower()==a]
code=bytes.fromhex(rows[0]["bytes"]); ret=[]
for ins in Cs(CS_ARCH_X86,CS_MODE_32).disasm(code,int(a,16)):
    if ins.mnemonic=="call": ret.append("IND" if "ptr" in ins.op_str else ins.op_str)
for i in range(max(len(ret),len(ours))):
    r=ret[i] if i<len(ret) else "-"; o=ours[i] if i<len(ours) else "-"
    if r=="IND" and o=="IND": continue
    b=""
    if r.startswith("0x"):
        c=bind.get(int(r,16)); b=" ".join(s for s,_ in c.most_common(2)) if c else ""
    flag="" if (o in b) else "  <<"
    print(f"{r:9} {o[:100]:100} | {b[:100]}{flag}")
