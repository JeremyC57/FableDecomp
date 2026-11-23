# sig313.py <census file> [entry...]: for every SIG / OPEN stand-in line of the census, the demangled parameter / return difference
# against the PDB spelling and the witness's source lines that name the member (declaration lines first). Read-only.
import sys,re,subprocess
sys.path.insert(0,'/home/user/FableDecomp/tools/decomp_pipeline/naming'); import msvc_demangle as MD
A='/tmp/claude-0/-home-user-FableDecomp/5221bbec-fb18-575f-8649-a32d96bb1c2d/scratchpad/w22/agentNF315'
L=open(A+'/ENTRIES.tsv').read().splitlines()
full={}
for l in open('/home/user/FableDecomp/rebuild/oracles/reloc_bindings.tsv',encoding='utf-8'):
    r=l.rstrip('\n').split('\t')
    if len(r)==3: full[(r[1].lower(),r[2])]=full.get((r[1].lower(),r[2]),[])+[r[0]]
def dm(s):
    try: d=MD.demangle(s); return d if d.get('ok') else None
    except Exception: return None
want=set(sys.argv[2:])
for l in open(sys.argv[1]):
    c=l.rstrip('\n').split('\t')
    if not c[0].startswith('#'): continue
    n=c[0][1:]
    if want and n not in want: continue
    if not (c[3].startswith('SIG') or c[3].startswith('OPEN')): continue
    t,w=c[1],c[2]; pdb=L[int(n)].split('\t')[6]
    for s in full.get((t,w),[]):
        if s==pdb: continue
        P,D=dm(pdb),dm(s)
        print('#%s %s W=%s %s'%(n,t,w,c[3]))
        f=lambda d,x: ('%s %s(%s)%s'%(d.get('ret'),d.get('qual'),','.join(d.get('params') or []),' const' if d.get('this_cv') else '') if d else x); print('   PDB: '+f(P,pdb)[:230]); print('   OLD: '+f(D,s)[:230])
        mem=(P['comps'][-1] if P else '').split('<')[0]
        for wi in w.split(','):
            src=subprocess.run(['bash',A+'/tools/srcof.sh',wi],capture_output=True,text=True).stdout.strip()
            if not src: print('   %s: NOSRC'%wi); continue
            hits=[(i+1,x) for i,x in enumerate(open(src,encoding='utf-8',errors='replace').read().splitlines()) if mem and mem in x and not x.lstrip().startswith('//')]
            print('   %s %s (%d lines)'%(wi,src.split('/')[-1],len(hits)))
            for i,x in hits[:4]: print('      %d: %s'%(i,x.strip()[:200]))
