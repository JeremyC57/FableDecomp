# ovowners.py <linkdiff file>: for every NEWLY_UNRESOLVED symbol (a vftable slot the new Clone TUs now reference), the retail address
# (labels_rtti_port / _dtors), the landed row at that address and the row's `// Leaf:` -> SAME (the row defines the PDB name) /
# LEAF_DIFF (a landed self row with another spelling: NAME_FIXES) / NO_ROW. No bytes.
import sys, re
sys.path.insert(0, '/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad/w22/agentNF328/tools'); import nf
L = {}
for f in ('labels_rtti_port.tsv', 'labels_rtti_port_dtors.tsv'):
    for l in open('/home/user/FableDecomp/ghidra_out/' + f):
        p = l.rstrip('\n').split('\t')
        if len(p) > 1: L.setdefault(p[1], []).append(p[0].lower())
for l in open(sys.argv[1]):
    if 'NEWLY_UNRESOLVED' not in l: continue
    s = l.split()[1]; ads = L.get(s, [])
    for a in ads or ['-']:
        src = nf.src_of(a) if a != '-' else None
        if not src or not src.exists(): print('NO_ROW\t%s\t%s' % (a, s)); continue
        t = src.read_text(encoding='utf-8', errors='replace'); m = re.search(r'^//\s*Leaf:\s*(\S+)', t, re.M)
        lf = m.group(1) if m else '(no Leaf)'
        print('%s\t%s\t%s\t%s' % ('SAME' if lf == s else 'LEAF_DIFF', a, s, lf))
