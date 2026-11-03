# symmap.py <addr> <obj> <leaf> : symbol -> retail address for every relocation in the leaf (offsets must align: MATCH rows)
import sys,subprocess,re,struct
a=int(sys.argv[1],16); obj=sys.argv[2]; leaf=sys.argv[3]
S='/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad'
ret=bytes.fromhex([l.split('\t')[3].strip() for l in open(S+'/batch/chunk733_oracle.tsv') if l.split('\t')[0].lower()==sys.argv[1].lower()][0])
out=subprocess.run(['objdump','-dr','--no-show-raw-insn',obj],capture_output=True,text=True).stdout
fn=None; seen={}
for l in out.split('\n'):
    m=re.match(r'^([0-9a-f]+) <(.*)>:$',l)
    if m: fn=m.group(2); base=int(m.group(1),16); continue
    m=re.match(r'^\s+([0-9a-f]+): (DISP32|dir32)\s+(\S+)$',l)
    if fn and fn==leaf and m:
        off=int(m.group(1),16)-base; v=struct.unpack('<I',ret[off:off+4])[0]
        t=(a+off+4+v)&0xffffffff if m.group(2)=='DISP32' else v
        seen.setdefault(m.group(3),set()).add(t)
for k,v in sorted(seen.items(), key=lambda kv: min(kv[1])):
    print(' '.join('%08X'%x for x in sorted(v)), k)
