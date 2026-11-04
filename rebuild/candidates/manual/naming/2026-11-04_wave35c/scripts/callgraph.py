# callgraph.py addr... : landed functions whose retail body calls/jumps to each address (by catalog extents)
import sys, struct, bisect, collections
sys.path.insert(0,'/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad/wave35c/scripts')
from c35 import *
cat=catalog(); ent=ps1_entries()
want=set(int(a,16) for a in sys.argv[1:])
ext=sorted((int(a,16),int(q[2]),q[3].strip()) for a,q in cat.items() if a in ent and q[2].isdigit())
S35={}
for f in ('wave35a','wave35b'):
    for l in open(os.environ['S']+'/'+f+'/ROWS.tsv'):
        S35[l.split('\t')[0]]=f
res=collections.defaultdict(list)
for base,ln,hx in ext:
    try: b=bytes.fromhex(hx)
    except: continue
    for i in range(len(b)-4):
        if b[i] in (0xe8,0xe9):
            t=(base+i+5+struct.unpack_from('<i',b,i+1)[0])&0xffffffff
            if t in want: res[t].append((base,i))
for t in sorted(want):
    for base,i in sorted(set(res[t])):
        a='%08x'%base
        print('%08x <- %s +%x %s %s'%(t,a,i,ent[a]['Source'],S35.get(a,'')))
