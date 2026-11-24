# stale338.py (NF338): per wrong table line of each ENTRIES target: does the witness's CURRENT landed source still carry the wrong
# spelling's distinguishing identifiers (stand-in scope / class names absent from the PDB spelling, else the method token)?
# GONE = none of them left in the witness source (table line may be stale: compile the witness to prove, NF309 Q4) ; KEEP = still there.
import sys, re, os
A = os.path.dirname(os.path.dirname(os.path.abspath(__file__))); R = '/home/user/FableDecomp'
sys.path.insert(0, A + '/tools'); import nf
T = {}
for l in open(R + '/rebuild/oracles/reloc_bindings.tsv'):
    p = l.rstrip('\n').split('\t')
    if len(p) == 3: T.setdefault(p[1].lower().replace('0x', ''), []).append((p[0], p[2].lower().replace('0x', '')))
def ids(s): return set(re.findall(r'[A-Za-z_][A-Za-z0-9_]*', s))
E = nf.entries()
for i, e in enumerate(E, 1):
    t, pdb = e[0], e[6]
    for s, w in T.get(t, []):
        if s == pdb: continue
        extra = sorted(x for x in ids(s) - ids(pdb) if len(x) > 2 and not re.match(r'^[A-Z]{1,4}$', x))
        src = nf.src_of(w)
        if not src or not src.exists(): print('%d\t%s\t%s\tNOSRC\t%s\t%s' % (i, t, w, ','.join(extra), s[:150])); continue
        txt = re.sub(r'//.*', '', src.read_text(encoding='utf-8', errors='replace'))
        left = [x for x in extra if re.search(r'\b%s\b' % re.escape(x), txt)]
        st = 'KEEP' if left else ('GONE' if extra else 'SAMEIDS')
        print('%d\t%s\t%s\t%s\t%s\t%s' % (i, t, w, st, ','.join(left or extra), s[:150]))
