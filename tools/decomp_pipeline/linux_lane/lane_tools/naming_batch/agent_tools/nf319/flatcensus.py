# flatcensus.py: NF299 Q4-form census of the entries whose engine/<X>.h has no polymorphic model (work/nobase.txt, virt=0).
# Per entry: the header's `void* __vftable` member count, `virtual` count, any `class|struct X :` (a based model) anywhere in rebuild/include,
# and the RTTI slot of the address in X's own retail vftable (vtslot.py). -> evidence/flat_census.tsv
import re, subprocess
from pathlib import Path
A = Path('/tmp/claude-0/-home-user-FableDecomp/5221bbec-fb18-575f-8649-a32d96bb1c2d/scratchpad/w22/agentNF319')
INC = Path('/home/user/FableDecomp/rebuild/include')
E = [l.rstrip('\n').split('\t') for l in open(A / 'ENTRIES.tsv', encoding='utf-8')][1:]
o = open(A / 'evidence/flat_census.tsv', 'w')
print('idx\taddress\tclass\tvftable_members\tvirtual_count\tbased_model_anywhere\tretail_slot\tpdb_spelling', file=o)
for l in open(A / 'work/nobase.txt'):
    p = l.rstrip('\n').split('\t')
    if p[2] != 'virt=0': continue
    i = int(p[0]); e = E[i - 1]; X = p[1]; h = INC / 'engine' / (X + '.h'); t = h.read_text(encoding='utf-8', errors='replace')
    vf = len(re.findall(r'void\*\s+__vftable', t)); nv = len(re.findall(r'\bvirtual\b', t.split('//')[0] if False else re.sub(r'//.*', '', t)))
    g = subprocess.run(['grep', '-rlE', r'(class|struct)\s+' + X + r'\s*:[^:]', str(INC)], capture_output=True, text=True).stdout.split()
    v = subprocess.run(['python3', str(A / 'tools/vtslot.py'), e[0]], capture_output=True, text=True).stdout
    m = re.search(r'([0-9A-F]{8}):\.\?AV' + re.escape(X) + r'@@\[(\d+)\]', v)
    print(i, e[0], X, vf, nv, ','.join(x.replace(str(INC) + '/', '') for x in g) or '-', ('0x%s[%s]' % m.groups()) if m else '-', e[3], sep='\t', file=o)
o.close()
