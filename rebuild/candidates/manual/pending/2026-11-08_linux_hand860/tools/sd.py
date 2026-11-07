# sd.py <addr> <tag> [--slots]: structural diff of retail v ours with call/jump targets masked; without --slots also masks [ebp+-N]/[esp+N]
import sys,os,re,csv,difflib
sys.path.insert(0,"/home/user/FableDecomp/tools/decomp_pipeline")
import verify_and_land as V
from capstone import Cs,CS_ARCH_X86,CS_MODE_32
S=os.environ['S']; A=S+'/w22/agent860'; a,tag=sys.argv[1:3]; slots='--slots' in sys.argv; regs='--regs' in sys.argv
for r in csv.DictReader(open(S+'/batch/chunk860_oracle.tsv',encoding='utf-8-sig'),delimiter='\t'):
    if r['address'].lower()==a: rb=bytes.fromhex(r['bytes'])
src=open(f'{A}/w/{a}_{tag}/{a}.cpp').read()
m=re.search(r"^// Leaf: *(\S+)",src,re.M); leaf=m.group(1)
ob=V.obj_text(f"{A}/w/{a}_{tag}/{a}.obj",leaf)[0]
cs=Cs(CS_ARCH_X86,CS_MODE_32); base=int(a,16)
def dis(b): return [(i.address,i.mnemonic+' '+i.op_str) for i in cs.disasm(b,base)]
def norm(s):
    s=re.sub(r"\[0\]","[ADDR]",s); s=re.sub(r"^push 0$","push ADDR",s); s=re.sub(r", 0$",", ADDR",s)
    s=re.sub(r"0x[0-9a-f]{5,}","ADDR",s); s=re.sub(r"^(call|j[a-z]+) (0x)?[0-9a-f]+$",r"\1 X",s)
    if not slots: s=re.sub(r"\[(ebp|esp) [-+] (0x[0-9a-f]+|[0-9])\]",r"[\1+N]",s); s=re.sub(r"\[(ebp|esp)\]",r"[\1+N]",s)
    if regs: s=re.sub(r'\b(e[abcd]x|e[sd]i|ebp|[abcd]l)\b','R',s)
    return s
ra,oa=dis(rb),dis(ob); rn=[norm(x[1]) for x in ra]; on=[norm(x[1]) for x in oa]
sm=difflib.SequenceMatcher(None,rn,on,autojunk=False); n=0
for op,i1,i2,j1,j2 in sm.get_opcodes():
    if op=='equal': continue
    for k in range(i1,i2): print(f"  - {ra[k][0]:08x} {ra[k][1]}"); n+=1
    for k in range(j1,j2): print(f"  + {oa[k][0]:08x} {oa[k][1]}"); n+=1
    print('  --')
sys.stdout.flush(); print('lines',n,'len',len(rb),len(ob))
