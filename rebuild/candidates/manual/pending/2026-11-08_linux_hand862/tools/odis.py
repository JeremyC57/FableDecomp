# odis.py <tag> [addr] [leafsubstr]: our listing of the graded function with relocation names, rebased to the row address
import sys,subprocess,re,os
A=os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
t=sys.argv[1]; ad=sys.argv[2] if len(sys.argv)>2 else '00531ec0'; leaf=sys.argv[3] if len(sys.argv)>3 else 'Update@CComponent'
out=subprocess.run(['objdump','-dr','--no-show-raw-insn','-M','intel',f'{A}/work/g_{ad}_{t}/{ad}.obj'],capture_output=True,text=True).stdout
base=int(ad,16); inf=False
for l in out.splitlines():
    m=re.match(r'^[0-9a-f]+ <(.*)>:',l)
    if m: inf=leaf in m.group(1); continue
    if not inf: continue
    m=re.match(r'^\s+([0-9a-f]+):\s+(.*)',l)
    if m:
        if 'DISP32' in l or 'dir32' in l.lower(): print('         -> '+l.split()[-1][:90])
        else: print(f'{int(m.group(1),16)+base:08x} {m.group(2)}')
