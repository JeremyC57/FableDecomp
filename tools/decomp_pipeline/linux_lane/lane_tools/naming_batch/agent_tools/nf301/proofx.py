# proofx.py <tag> <addr>=<entry,entry..> ...: objsec of work/obj/H_<a>.obj v N_<a>.obj under an EXPLICIT rename map built from ENTRIES
# (each listed entry's stand-in spelling witnessed by <a> -> the entry's pdb_spelling); also checks that the map covers exactly the
# DEF/UND symbol diff. -> evidence/proof_<tag>.txt
import sys,re,subprocess
A='/tmp/claude-0/-home-user-FableDecomp/5221bbec-fb18-575f-8649-a32d96bb1c2d/scratchpad/w22/agentNF301'
L=open(A+'/ENTRIES.tsv').read().splitlines(); tag=sys.argv[1]; out=open(A+'/evidence/proof_%s.txt'%tag,'w')
def syms(p): return set(l.split()[-1] for l in subprocess.run(['objdump','-t',p],capture_output=True,text=True).stdout.splitlines() if '(scl   2)' in l)
for arg in sys.argv[2:]:
    a,ents=arg.split('='); mp={}
    for n in map(int,ents.split(',')):
        c=L[n].split('\t'); pdb=c[6]
        for part in c[11].split(' || '):
            m=re.match(r'(\S+) \(([^)]*)\)',part)
            if m and m.group(1)!=pdb and a in m.group(2).split(','): mp[m.group(1)]=pdb
    h=A+'/work/obj/H_%s.obj'%a; nn=A+'/work/obj/N_%s.obj'%a; sh,sn=syms(h),syms(nn)
    cover = (sh-sn)==set(mp) and (sn-sh)==set(mp.values())
    out.write('##### %s map (%d): %s\n'%(a,len(mp),' '.join('%s=%s'%kv for kv in mp.items())))
    out.write('map_covers_symbol_diff=%s\n'%cover)
    r=subprocess.run(['python3',A+'/tools/objsec.py',h,nn]+['%s=%s'%kv for kv in mp.items()],capture_output=True,text=True).stdout
    out.write(''.join(l+'\n' for l in r.splitlines() if re.match(r'(sections|DIFF|ALL_EQ|bad|  )',l)))
    out.write('tables H=%d N=%d\n'%(len([s for s in sh if s.startswith('??_7')]),len([s for s in sn if s.startswith('??_7')])))
out.close(); print(open(A+'/evidence/proof_%s.txt'%tag).read())
