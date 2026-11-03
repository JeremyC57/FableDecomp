import sys,os,csv,re,collections,struct
from capstone import *
S=os.environ["S"]; R="/home/user/FableDecomp"
a=sys.argv[1].lower()
rows=[r for f in os.listdir(f"{S}/batch") if f.endswith("_oracle.tsv") for r in csv.DictReader(open(f"{S}/batch/{f}"),delimiter="\t") if r["address"].lower()==a]
code=bytes.fromhex(rows[0]["bytes"]); base=int(a,16)
bind=collections.defaultdict(collections.Counter)
for l in open(f"{R}/rebuild/oracles/reloc_bindings.tsv"):
    p=l.rstrip("\n").split("\t")
    if len(p)==3 and p[1]!="target": bind[int(p[1],16)][p[0]]+=1
man={}
for l in open(f"{R}/rebuild/manifest/functions.tsv"):
    p=l.split("\t")
    try: man[int(p[0],16)]=p[2]+"::"+p[1]
    except: pass
exe=open(f"{S}/dl/game/Fable The Lost Chapters/Fable.exe","rb").read()
pe=struct.unpack_from("<I",exe,0x3c)[0]; n=struct.unpack_from("<H",exe,pe+6)[0]; opt=struct.unpack_from("<H",exe,pe+20)[0]
secs=[]
for i in range(n):
    o=pe+24+opt+i*40
    secs.append((struct.unpack_from("<I",exe,o+12)[0]+0x400000,struct.unpack_from("<I",exe,o+8)[0],struct.unpack_from("<I",exe,o+20)[0]))
def rd(x,k=64):
    for va,vs,rp in secs:
        if va<=x<va+vs: return exe[rp+x-va:rp+x-va+k]
    return b''
def sname(x):
    b=rd(x)
    if not b: return None
    s=b.split(b'\0')[0]
    if len(s)>=3 and all(32<=c<127 for c in s): return '"'+s.decode()+'"'
    w=b.split(b'\0\0')[0]
    if len(w)>=6 and len(w)%2==0:
        try:
            ws=w.decode('utf-16le')
            if all(32<=ord(c)<127 for c in ws): return 'L"'+ws+'"'
        except: pass
    return None
md=Cs(CS_ARCH_X86,CS_MODE_32)
for ins in md.disasm(code,base):
    note=""
    m=re.findall(r"0x([0-9a-f]{6,8})",ins.op_str)
    for h in m:
        v=int(h,16)
        if ins.mnemonic=="call" or v in bind:
            b=bind.get(v)
            nm=b.most_common(1)[0][0] if b else man.get(v,"")
            if v in man and (not b): nm="man:"+man[v]
            note+=" ; "+nm[:110]
        elif 0x1200000<=v<0x1500000:
            s=sname(v)
            if s: note+=" ; "+s[:80]
    print(f"{ins.address:08x} {ins.mnemonic:6} {ins.op_str:34}{note}")
