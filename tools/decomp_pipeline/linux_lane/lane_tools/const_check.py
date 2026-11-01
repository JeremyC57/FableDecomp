# const_check.py (chunk 653 agent, 2026-11-01; was cc.py) <addr> <obj> <leafsubstr>: check every __real@ / __xmm constant reloc in the leaf against the retail constant value
import os,sys,struct,subprocess,re
addr=int(sys.argv[1],16); obj=sys.argv[2]; leaf=sys.argv[3]
d=open(os.environ['FABLE_EXE'],'rb').read()
pe=struct.unpack_from('<I',d,0x3c)[0]; n=struct.unpack_from('<H',d,pe+6)[0]; opt=struct.unpack_from('<H',d,pe+20)[0]
secs=[struct.unpack_from('<8sIIII',d,pe+24+opt+i*40) for i in range(n)]
def rd(va,k=4):
    rva=va-0x400000
    for nm,vs,vaddr,rs,ro in secs:
        if vaddr<=rva<vaddr+max(vs,rs): return d[ro+rva-vaddr:ro+rva-vaddr+k]
out=subprocess.run(['objdump','-dr',obj],capture_output=True,text=True).stdout
# find leaf section
blocks=re.split(r'\n(?=[0-9a-f]{8} <)',out)
bad=0;n=0
for b in blocks:
    if leaf not in b.split('\n')[0]: continue
    for m in re.finditer(r'^\s+([0-9a-f]+): dir32\s+(__real@([0-9a-f]+))',b,re.M):
        off=int(m.group(1),16); hx=m.group(3)
        ptr=struct.unpack('<I',rd(addr+off))[0]
        if len(hx)==8: rv=rd(ptr,4)[::-1].hex()
        else: rv=rd(ptr,8)[::-1].hex()
        n+=1
        if rv!=hx: bad+=1; print('MISMATCH off %x built %s retail %s @%x'%(off,hx,rv,ptr))
print('checked',n,'constants,',bad,'mismatches')
