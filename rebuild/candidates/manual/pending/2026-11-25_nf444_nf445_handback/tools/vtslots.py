#!/usr/bin/env python3
"""vtslots.py <obj>...: print each ??_7 vftable COMDAT an object defines with its slot symbols (relocation order = slot order)."""
import subprocess,sys,re
for o in sys.argv[1:]:
    t=subprocess.run(['objdump','-t',o],capture_output=True,text=True).stdout
    secs={}
    for l in t.splitlines():
        m=re.search(r'\(sec\s+(\d+)\).*\(scl\s+2\).*?0x[0-9a-f]+ (\?\?_7\S+)',l)
        if m: secs[int(m.group(1))]=m.group(2)
    r=subprocess.run(['objdump','-r',o],capture_output=True,text=True).stdout
    cur=None; slots={}
    for l in r.splitlines():
        m=re.match(r'RELOCATION RECORDS FOR \[(.*)\]',l)
        if m: cur=m.group(1); continue
        p=l.split()
        if cur and len(p)==3 and re.match(r'[0-9a-f]{8}',p[0]): slots.setdefault(cur,[]).append((int(p[0],16),p[2]))
    # section names are all .rdata; map by index order of headers
    h=subprocess.run(['objdump','-h',o],capture_output=True,text=True).stdout
    names=[]
    for l in h.splitlines():
        m=re.match(r'\s*(\d+)\s+(\S+)',l)
        if m: names.append((int(m.group(1))+1,m.group(2)))
    # objdump -r groups by section name; ambiguous for multiple .rdata -> fall back: print all .rdata reloc groups in order
    for k,v in slots.items():
        if k.startswith('.rdata'): print(o.split('/')[-1], k, [s for _,s in sorted(v)])
