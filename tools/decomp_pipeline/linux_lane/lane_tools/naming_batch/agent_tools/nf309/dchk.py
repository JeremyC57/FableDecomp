# dchk.py <entry#>:<binder>[,<binder>...] ...: donor second source. For each binder row, read its `// Leaf:` symbol, list its callees in the
# donor FableWin.exe (dcalls.py) and report whether the entry's exact PDB spelling is among them (YES / NO / NOLEAF / NOTINFW).
import sys,subprocess,re
A='/tmp/claude-0/-home-user-FableDecomp/5221bbec-fb18-575f-8649-a32d96bb1c2d/scratchpad/w22/agentNF309'
L=[l.split('\t') for l in open(A+'/ENTRIES.tsv').read().splitlines()]
for spec in sys.argv[1:]:
    n,bs=spec.split(':'); pdb=L[int(n)][6]
    for b in bs.split(','):
        f=subprocess.run(['bash',A+'/tools/srcof.sh',b],capture_output=True,text=True).stdout.strip()
        lf=None
        if f:
            m=re.search(r'^// Leaf:\s*(\S+)',open(f,encoding='utf-8',errors='replace').read(),re.M); lf=m.group(1) if m else None
        qs=[]
        if lf and lf.startswith('?'): qs.append(lf if lf.endswith('Z') else lf+'*')
        if f:
            fn=f.rsplit('/',1)[-1][:-13]; ps=open('/home/user/FableDecomp/rebuild/build_candidates.ps1',encoding='utf-8').read()
            i=ps.find("Address = '%s'"%b); mm=re.search(r"Module = '([^']+)'",ps[i:i+400]); mod=mm.group(1) if mm else ''
            if mod and fn.startswith(mod+'_'): qs.append('?%s@%s@*'%(fn[len(mod)+1:],mod.split('::')[-1]))
        o=''
        for q in qs:
            o=subprocess.run(['python3',A+'/tools/dcalls.py',q],capture_output=True,text=True).stdout
            if 'NOT IN FableWin' not in o: lf=q; break
        if not o or 'NOT IN FableWin' in o: print('#%s %s NOTINFW %s'%(n,b,qs)); continue
        cal=[x.strip() for x in o.splitlines() if x.startswith('    ')]
        pid=re.match(r'\?([^@]+)@',pdb).group(1) if pdb.startswith('?') else pdb
        same=[c for c in cal if c.startswith('?%s@'%pid)]
        print('#%s %s %s leaf=%s %s'%(n,b,'YES' if pdb in cal else 'NO',lf[:70],('similar='+' '.join(same)) if pdb not in cal and same else ''))
