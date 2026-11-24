# gen311.py: STATUS verdict lines for entries 24..212 -> tools/v311b.tsv: census (work/census.txt '=' lines, FULL table spellings, NF299 Q4
# form) for every entry not in the hand-verdict dict H below; H holds the entries opened by hand (sources read and / or compiled).
import re,collections
A='/tmp/claude-0/-home-user-FableDecomp/5221bbec-fb18-575f-8649-a32d96bb1c2d/scratchpad/w22/agentNF313'
C=collections.defaultdict(list); V={}
for l in open(A+'/work/census.txt'):
    c=l.rstrip('\n').split('\t')
    if l.startswith('#'): C[int(c[0][1:])].append(c)
    elif l.startswith('='): V[int(c[0][1:])]=(c[2],c[3])
SAMPLE={30,40,50,60,70,80,90,100,110,120,130,140,150,160,170,180,190,200,210}   # 1-in-10 census sample, opened (declaration read)
H={}
def h(n,st,b,g,note): H[n]=(st,b,g,note)
exec(open(A+'/tools/hand311.py').read())
# NF313 (advisor review): entries with a SIG-only stand-in line (same PDB class, parameter / return types differ) are OUTSIDE NF299 Q4;
# they were not opened, so they are left UNREACHED (no STATUS line) and listed in NOTES for the next batch.
UNREACHED=set()
for n,cs in C.items():
    if n not in H and any(c[3]=='SIG' for c in cs): UNREACHED.add(n)
open(A+'/work/unreached.txt','w').write(' '.join(str(n) for n in sorted(UNREACHED))+'\n')
out=[]
for n in range(24,213):
    if n in UNREACHED: continue
    if n in H: st,b,g,note=H[n]
    else:
        ver,tags=V[n]; assert ver=='port',(n,ver)
        b=' / '.join(sorted(set('0x'+w for c in C[n] for w in c[2].split(',') if re.fullmatch(r'[0-9a-f]{8}',w))))
        wo=sorted(set(c[4][2:] for c in C[n] if c[4]!='W:-'))
        g='census (tools/census311.py, FULL reloc_bindings spellings) + stale screen OK (tools/stale311.py)'+('; 1-in-10 sample: binder declaration opened, still the stand-in' if n in SAMPLE else '; binder not opened (NF299 Q4 form)')+'; not compiled'
        note='every stand-in line binds through %s (MADEUP = a class / scope / type token in neither PDB name set; WRONGCLASS = another class; FREE = free function or C stub; HASH = anonymous-namespace hash, TU-path dependent; SIG = parameter / return types differ): port'%tags+(' [witness off-limits: %s]'%','.join(wo) if wo else '')
        st='skipped-port'
    out.append('%d\t%s\t%s\t%s\t%s'%(n,st,b,g,note))
open(A+'/tools/v311b.tsv','w').write('\n'.join(out)+'\n'); print(len(out))
