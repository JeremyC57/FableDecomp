# ck.py <addr> <leafprefix> : list (retail call target, our reloc symbol) pairs in order, plus data relocs
import sys,os,subprocess,re,csv,struct
from capstone import *
S=os.environ["S"]; a=sys.argv[1].lower(); leaf=sys.argv[2]
obj=f"{S}/wine/q/{a}/{a}.obj"
out=subprocess.run(["objdump","-dr","--no-show-raw-insn",obj],capture_output=True,text=True).stdout
# find the function block whose symbol contains leaf
cands=[]; cur=None
for l in out.splitlines():
    m=re.match(r"([0-9a-f]{8}) <(.*)>:",l)
    if m and not m.group(2).startswith("$"):
        cur=[] if m.group(2).startswith(leaf) else None
        if cur is not None: cands.append(cur)
        continue
    if l.startswith("Disassembly of section"): cur=None
    if cur is not None: cur.append(l)
best="\n".join(max(cands,key=len))
ours=[]
lines=best.splitlines()
for i,l in enumerate(lines):
    if "\tcall " in l:
        if "*" in l: ours.append("(indirect) "+l.split("call")[1].strip())
        elif i+1<len(lines) and ("DISP32" in lines[i+1] or "R_386_PC32" in lines[i+1]): ours.append(lines[i+1].split()[-1])
        else: ours.append("?? "+l)
exe=open(f"{S}/dl/game/Fable The Lost Chapters/Fable.exe","rb").read()
rows=[r for f in os.listdir(f"{S}/batch") if f.endswith("_oracle.tsv") for r in csv.DictReader(open(f"{S}/batch/{f}"),delimiter="\t") if r["address"].lower().replace("0x","")==a]
code=bytes.fromhex(rows[0]["bytes"]); base=int(a,16)
md=Cs(CS_ARCH_X86,CS_MODE_32); ret=[]
for ins in md.disasm(code,base):
    if ins.mnemonic=="call":
        ret.append(ins.op_str)
for i in range(max(len(ret),len(ours))):
    print(f"{ret[i] if i<len(ret) else '-':14} {ours[i] if i<len(ours) else '-'}")
