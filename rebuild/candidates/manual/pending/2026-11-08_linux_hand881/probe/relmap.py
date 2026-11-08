import subprocess,sys,re
obj=sys.argv[1]; leaf=sys.argv[2]; base=int(sys.argv[3],16)
d=subprocess.run(['objdump','-dr','-M','intel',obj],capture_output=True,text=True).stdout
inf=False
for l in d.splitlines():
    m=re.match(r'[0-9a-f]+ <(.*)>:',l)
    if m:
        if not m.group(1).startswith('$'): inf = leaf in m.group(1)
        continue
    if l.startswith('Disassembly of section'): inf=False; continue
    if inf:
        m=re.match(r'\s+([0-9a-f]+): (?:dir32|DISP32|REL32|IMAGE_REL_I386_\w+)\s+(\S+)',l)
        if m: print('%x %s'%(base+int(m.group(1),16),m.group(2)))
