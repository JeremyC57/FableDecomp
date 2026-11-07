# rels.py <obj> <leaf>: list relocations (offset, symbol) inside the leaf's section
import sys,subprocess,re
obj,leaf=sys.argv[1:3]
out=subprocess.run(['objdump','-dr','--no-show-raw-insn',obj],capture_output=True,text=True).stdout
cur=None; res=[]
for l in out.splitlines():
    m=re.match(r'^[0-9a-f]+ <(.+)>:',l)
    if m: cur=m.group(1); continue
    if cur and leaf in cur:
        m=re.match(r'\s+([0-9a-f]+): (?:R_386_|dir32|DISP32|IMAGE_REL_I386_)(\S*)\s+(\S+)',l)
        if 'R_386' in l or 'DIR32' in l or 'REL32' in l: res.append(l.strip())
seen=[]
for r in res:
    s=r.split()[-1]
    if s not in seen: seen.append(s)
for s in seen: print(s)
