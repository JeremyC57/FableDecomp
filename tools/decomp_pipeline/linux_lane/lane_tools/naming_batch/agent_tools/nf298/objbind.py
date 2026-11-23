# objbind.py <binder>...: for every ENTRIES stand-in whose witness list holds the binder, report whether the binder's HEAD object
# (work/obj/H_<b>.obj) still references the stand-in symbol, and whether it references the entry's PDB spelling.
import re,sys,subprocess
A='/tmp/claude-0/-home-user-FableDecomp/5221bbec-fb18-575f-8649-a32d96bb1c2d/scratchpad/w22/agentNF298'
L=[l.split('\t') for l in open(A+'/ENTRIES.tsv').read().splitlines()]
for b in sys.argv[1:]:
    import os
    if not os.path.exists(A+'/work/obj/H_%s.obj'%b): print(b,'NO_OBJ (not compiled)'); continue
    try: syms=set(l.split()[-1] for l in subprocess.run(['objdump','-t',A+'/work/obj/H_%s.obj'%b],capture_output=True,text=True).stdout.splitlines() if '(scl   2)' in l)
    except Exception as e: print(b,'ERR',e); continue
    for n,c in enumerate(L[1:],1):
        pdb=c[6]
        for part in c[11].split(' || '):
            m=re.match(r'(\S+) \(([^)]*)\)',part)
            if m and m.group(1)!=pdb and b in m.group(2).split(','):
                print('#%d\t%s\tbinder %s\tstandin_in_obj=%s\tpdb_in_obj=%s\t%s'%(n,c[0],b,m.group(1) in syms,pdb in syms,m.group(1)[:90]))
