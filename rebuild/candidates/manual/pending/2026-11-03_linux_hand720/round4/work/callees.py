# callees.py <addr> : list call targets / abs refs in the oracle row with bound symbols + manifest names
import sys,os,csv,re,collections
from capstone import *
S=os.environ["S"]; R="/home/user/FableDecomp"
a=sys.argv[1].lower()
rows=[r for f in os.listdir(f"{S}/batch") if f.endswith("_oracle.tsv") for r in csv.DictReader(open(f"{S}/batch/{f}"),delimiter="\t") if r["address"].lower()==a]
code=bytes.fromhex(rows[0]["bytes"]); base=int(a,16)
bind=collections.defaultdict(collections.Counter)
for l in open(f"{R}/rebuild/oracles/reloc_bindings.tsv"):
    p=l.rstrip("\n").split("\t")
    if len(p)==3: bind[p[1].lower()][p[0]]+=1
man={}
for l in open(f"{R}/rebuild/manifest/functions.tsv"):
    p=l.split("\t"); man[p[0].lower()]=p[2]+"::"+p[1] if len(p)>2 else ""
md=Cs(CS_ARCH_X86,CS_MODE_32)
seen=collections.OrderedDict()
for ins in md.disasm(code,base):
    tg=None
    if ins.mnemonic=="call" and ins.op_str.startswith("0x"): tg=int(ins.op_str,16)
    else:
        for m in re.findall(r"0x([0-9a-f]{6,8})",ins.op_str):
            v=int(m,16)
            if 0x401000<=v<0x1500000: tg=v
    if tg is None: continue
    k="%08x"%tg
    seen.setdefault(k,[]).append("%x"%ins.address)
for k,v in seen.items():
    b=bind.get(k,collections.Counter())
    syms=" | ".join(f"{s}({n})" for s,n in b.most_common(3))
    print(f"{k} x{len(v)} man={man.get(k,'-')[:60]}  bound={syms[:300]}")
