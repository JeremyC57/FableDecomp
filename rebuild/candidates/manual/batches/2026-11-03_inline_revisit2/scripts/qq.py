#!/usr/bin/env python3
"""qq.py <addr> <src.cpp> [--leaf L] [--flags ...] [--calls] [--dis]
Private copy of q.py: work dir $S/inlinesplit/work/<addr>. Prints grade, masked diff-line count and,
with --calls, the retail vs built call target sequence (difflib aligned)."""
import sys, os, csv, glob, re, difflib, subprocess
from pathlib import Path
sys.path.insert(0, "/home/user/FableDecomp/tools/decomp_pipeline")
import verify_and_land as V
from capstone import Cs, CS_ARCH_X86, CS_MODE_32
S = Path(os.environ["S"])
a = sys.argv[1:]
addr = a[0].lower().replace("0x",""); srcp = Path(a[1]); src = srcp.read_text(encoding="utf-8")
extra=[]; leaf=None; show_calls='--calls' in a; dis='--dis' in a
a=[x for x in a if x not in ('--calls','--dis')]
if '--flags' in a: i=a.index('--flags'); extra=a[i+1:]; a=a[:i]
if not extra:
    mf=re.search(r'^// Flags:(.*)$',src,re.M)
    if mf: extra=mf.group(1).split()
if '--leaf' in a: leaf=a[a.index('--leaf')+1]
if not leaf:
    m=re.search(r'^// Leaf: (.*)$',src,re.M); leaf=m.group(1).strip() if m else None
retail=None; name=None
for f in ([os.environ["ORACLE"]] if os.environ.get("ORACLE") else []) + ["/home/user/FableDecomp/rebuild/oracles/auto-re-candidates.tsv"] + sorted(glob.glob(str(S/"batch"/"*_oracle.tsv"))) + sorted(glob.glob("/home/user/FableDecomp/rebuild/candidates/manual/batches/*/oracle.tsv")):
    for r in csv.DictReader(open(f, encoding="utf-8-sig"), delimiter="\t"):
        if r["address"].lower().replace("0x","") == addr:
            retail = bytes.fromhex(r["bytes"]); name = r["name"]; break
    if retail: break
while retail and retail[-1] in (0xCC,0x90): retail=retail[:-1]
leaf = leaf or name.rsplit("::",1)[-1]
work = S/"inlinerevisit2"/"work"/addr; work.mkdir(parents=True, exist_ok=True)
e = V.env()
st, built, sec = V.parity_of(V.vc71(src), addr, leaf, retail, work, e, extra=tuple(extra))
md = Cs(CS_ARCH_X86, CS_MODE_32)
base=int(addr,16)
def norm(b):
    out=[]
    for i in md.disasm(b,base):
        op=re.sub(r'0x[0-9a-f]{5,}','X',i.op_str)
        out.append(i.mnemonic+' '+op)
    return out
if built is None:
    print(addr, st); sys.exit(1)
ra=norm(retail); ba=norm(built)
dl=sum(1 for l in difflib.unified_diff(ra,ba,lineterm='',n=0) if l[:1] in '+-' and not l.startswith(('+++','---')))
print(f"{addr} {st} dl={dl} leaf={leaf} flags={extra}")
if show_calls:
    rc=[]
    for i in md.disasm(retail,base):
        if i.mnemonic=='call': rc.append(i.op_str if not i.op_str.startswith('0x') else i.op_str)
    obj=work/f"{addr}.obj"
    txt=subprocess.run(['objdump','-d','-r','-M','intel','--no-show-raw-insn',str(obj)],capture_output=True,text=True).stdout
    blk=None; bc=[]
    for ln in txt.splitlines():
        m=re.match(r'^[0-9a-f]+ <(.*)>:',ln)
        if m:
            if m.group(1).startswith("$"): continue
            blk=(m.group(1)==leaf or m.group(1).startswith("?"+leaf+"@")); continue
        if not blk: continue
        m=re.search(r'(DISP32|DIR32|REL32)\s+(\S+)',ln)
        if m and bc and bc[-1]=='?': bc[-1]=m.group(2); continue
        m=re.match(r'\s+[0-9a-f]+:\s+call\s+(.*)',ln)
        if m: bc.append('?' if 'PTR' not in m.group(1) else m.group(1).replace('DWORD PTR ','dword ptr ').replace('+',' + '))
    B={}
    for ln in open('/home/user/FableDecomp/rebuild/oracles/reloc_bindings.tsv'):
        p=ln.rstrip('\n').split('\t')
        if len(p)>=2 and p[0] not in B: B[p[0]]='0x'+p[1].lstrip('0')
    bc=[B.get(x,x) for x in bc]
    def k(s): return 'IND' if '[' in s else s
    sm=difflib.SequenceMatcher(None,[k(x) for x in rc],[k(x) for x in bc],autojunk=False)
    print(f"calls retail={len(rc)} built={len(bc)}")
    for tag,i1,i2,j1,j2 in sm.get_opcodes():
        if tag=='equal': print(f"  = {i2-i1} equal ({', '.join(sorted(set(rc[i1:i2])))[:150]})")
        else: print(f"  {tag}: retail {rc[i1:i2]}  built {bc[j1:j2]}")
if dis:
    for l in difflib.unified_diff(ra,ba,lineterm='',n=2): print(l)
