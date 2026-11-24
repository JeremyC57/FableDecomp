# assemble3.py (NF323 alias-sig): RENAMES.tsv (section 1 per done row from evidence/libcp_*_vland*.txt; NF304 Q1 COMDAT renames as
# extra lines), APPLIED.tsv, rows.txt (done rows), REJECTED.tsv, empty TABLE_CHANGES.tsv header when absent.
import re, glob, sys
from pathlib import Path
A = Path('/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad/w22/agentNF332')
sys.path.insert(0, str(A / 'tools')); import nf
E = {l.split('\t')[0]: l.rstrip('\n').split('\t') for l in open(A / 'ENTRIES.tsv', encoding='utf-8')}
ST = {l.split('\t')[0]: l.rstrip('\n').split('\t') for l in open(A / 'STATUS.tsv').read().splitlines()[1:]}
ren = {}
for f in sorted(glob.glob(str(A / 'evidence/libcp_*_vland*.txt'))):
    for l in open(f):
        a = l.split()[0]
        if 'LIBCP_OK' not in l: continue
        pairs = [x.split('=', 1) for x in l.split('renames=')[1].split(' ;')[0].split()]
        ren[a] = (pairs, 'libcp"' in l, Path(f).name)
done = [a for a in ST if ST[a][1] == 'done']
out = ['address\tmodule\tname\tmangled_leaf\told_catalog_name\told_leaf\tevidence']; app = ['address\tfile\tchange']
for a in done:
    pairs, lc, ev = ren[a]; pdb = E[a][3]
    self_old = next(o for o, n in pairs if n == pdb)
    m = re.match(r"\?([^@]+)@([^@]+)@@", pdb); M, X = m.group(1), m.group(2)
    extra = ''.join('; pick-any COMDAT renamed with it (NF304 Q1): %s -> %s' % (o, n) for o, n in pairs if n != pdb)
    out.append('\t'.join(['0x' + a.upper(), X, M, pdb, nf.catalog()[a]['name'], self_old,
        'RTTI vftable placement (ghidra_out/labels_rtti_port.tsv) names %s here; PDB member; signature port onto engine/%s.h under existing landed switches; HEAD v new vland objects ALL_EQ under the rename%s%s (evidence/%s); vland MATCH/RELOCATION_MATCH PASS, landed test unchanged' % (pdb, X, extra, ', .drectve + /DEFAULTLIB:"libcp"' if lc else ', .drectve equal', ev)]))
    app.append('\t'.join(['0x' + a.upper(), E[a][7], 'alias-sig signature port: %s -> %s (definition moved off the alias class onto engine/%s.h with the PDB parameter / return types; body bytes unchanged)' % (self_old, pdb, X)]))
out.append('# Section 2 (caller respellings): none'); out.append('caller\told_symbol\tnew_symbol\tcallee')
(A / 'RENAMES.tsv').write_text('\n'.join(out) + '\n'); (A / 'APPLIED.tsv').write_text('\n'.join(app) + '\n')
(A / 'rows.txt').write_text(''.join(a + '\n' for a in done))
rej = ['address\treason'] + ['0x%s\t%s' % (a.upper(), v[3]) for a, v in ST.items() if v[1] == 'rejected']
(A / 'REJECTED.tsv').write_text('\n'.join(rej) + '\n')
if not (A / 'TABLE_CHANGES.tsv').exists(): (A / 'TABLE_CHANGES.tsv').write_text('action\tsymbol\ttarget\twitness\tnote\n')
print(len(done), 'done rows')
