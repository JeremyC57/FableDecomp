# assemble2.py: RENAMES.tsv (section 1: one line per changed row, from evidence/libcp_*_vland.txt renames), rows.txt (every changed row:
# done + skipped-held READY), REJECTED.tsv (header + rejected lines from STATUS), TABLE_CHANGES.tsv / NAME_FIXES.tsv kept if present.
import re, glob, csv, sys
from pathlib import Path
A = Path('/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad/w22/agentNF332')
R = Path('/home/user/FableDecomp')
sys.path.insert(0, str(A / 'tools')); import nf
E = {l.split('\t')[0]: l.rstrip('\n').split('\t') for l in open(A / 'ENTRIES.tsv', encoding='utf-8')}
ST = {l.split('\t')[0]: l.rstrip('\n').split('\t') for l in open(A / 'STATUS.tsv').read().splitlines()[1:]}
ren = {}
for f in sorted(glob.glob(str(A / 'evidence/libcp_*_vland.txt'))):
    for l in open(f):
        a = l.split()[0]
        if 'LIBCP_OK' not in l: continue
        old, new = l.split('renames=')[1].split(' ;')[0].split('=', 1); ren[a] = (old, new, 'libcp' in l, Path(f).name)
changed = [a for a in ST if a in ren and (ST[a][1] == 'done' or ST[a][3].startswith('READY'))]
out = ['address\tmodule\tname\tmangled_leaf\told_catalog_name\told_leaf\tevidence']
for a in changed:
    old, new, lc, ev = ren[a]
    m = re.match(r"\?([^@]+)@([^@]+)@@", new); M, X = m.group(1), m.group(2)
    out.append('\t'.join(['0x' + a.upper(), X, M, new, nf.catalog()[a]['name'], old,
               'RTTI vftable placement (ghidra_out/labels_rtti_port.tsv) names %s here; PDB member (Ego_r / FableWin); switch route onto engine/%s.h; HEAD v new vland objects ALL_EQ under this one rename%s (evidence/%s); vland MATCH/RELOCATION_MATCH PASS' % (new, X, ', .drectve + /DEFAULTLIB:"libcp"' if lc else ', .drectve equal', ev)]))
out.append('# Section 2'); out.append('caller\told_symbol\tnew_symbol\tcallee')
for l in (A / 'work/ren2.tsv').read_text().splitlines() if (A / 'work/ren2.tsv').exists() else []: out.append(l)
(A / 'RENAMES.tsv').write_text('\n'.join(out) + '\n')
(A / 'rows.txt').write_text(''.join(a + '\n' for a in changed))
rej = ['address\treason'] + ['0x%s\t%s' % (a.upper(), v[3]) for a, v in ST.items() if v[1] == 'rejected']
(A / 'REJECTED.tsv').write_text('\n'.join(rej) + '\n')
if not (A / 'TABLE_CHANGES.tsv').exists(): (A / 'TABLE_CHANGES.tsv').write_text('action\tsymbol\ttarget\twitness\tnote\n')
if not (A / 'NAME_FIXES.tsv').exists(): (A / 'NAME_FIXES.tsv').write_text('address\tcorrect_symbol\tevidence\twrong_spellings_seen\n')
print(len(changed), 'changed rows')
