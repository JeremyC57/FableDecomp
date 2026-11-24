# owner313.py <entry line>...: the owner row's source, its Leaf line and the lines that define Class::Member (cheap self screen)
import sys,subprocess,re
sys.path.insert(0,'/home/user/FableDecomp/tools/decomp_pipeline/naming'); import msvc_demangle as MD
A='/tmp/claude-0/-home-user-FableDecomp/5221bbec-fb18-575f-8649-a32d96bb1c2d/scratchpad/w22/agentNF313'
L=open(A+'/ENTRIES.tsv').read().splitlines()
for n in sys.argv[1:]:
    c=L[int(n)].split('\t'); t=c[0]; d=MD.demangle(c[6]); mem=d['comps'][-1].split('<')[0] if d.get('ok') else '?'
    cls=d['comps'][-2].split('<')[0] if d.get('ok') and len(d['comps'])>1 else ''
    src=subprocess.run(['bash',A+'/tools/srcof.sh',t],capture_output=True,text=True).stdout.strip()
    print('== %s %s %s::%s  %s'%(n,t,cls,mem,src.split('/')[-1] if src else 'NOSRC'))
    if not src: continue
    for i,x in enumerate(open(src,encoding='utf-8',errors='replace').read().splitlines()):
        if x.startswith('// Leaf:') or (re.search(r'\b%s\s*::\s*~?%s\s*\('%(re.escape(cls),re.escape(mem.lstrip('~'))),x) and not x.lstrip().startswith('//')):
            print('   %d: %s'%(i+1,x.strip()[:220]))
