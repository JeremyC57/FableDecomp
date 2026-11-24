# eqtable.py: per changed header, from work/equiv_all.tsv (equiv3.py): landed includers flagged by its planted control (rows, files),
# DIFF lines (old v new) among them, and whether each header-route row's own HEAD source is flagged for its header. -> evidence/equiv_per_header.tsv
import re
from pathlib import Path
A = Path('/tmp/claude-0/-home-user-FableDecomp/5221bbec-fb18-575f-8649-a32d96bb1c2d/scratchpad/w22/agentNF319')
L = [l.rstrip('\n').split('\t') for l in open(A / 'work/equiv_all.tsv') if not l.startswith('#')]
done = [l for l in open(A / 'work/equiv_all.tsv') if l.startswith('#DONE')]
diff = [l for l in L if l[2] == 'DIFF']; err = [l for l in L if l[2] == 'ERROR']
ctl = {}
for l in L:
    if l[2] == 'CTL':
        for h in l[3].split(','): ctl.setdefault(h, set()).add((l[0], l[1]))
E = {l.split('\t')[0]: l.rstrip('\n').split('\t') for l in open(A / 'ENTRIES.tsv')}
own = {}
for a in (A / 'rows.txt').read_text().split():
    X = re.match(r'\?[^@]+@([^@]+)@@', E[a][3]).group(1)
    if (A / 'hdr/engine' / (X + '.h')).exists(): own.setdefault(X, []).append(a)
o = open(A / 'evidence/equiv_per_header.tsv', 'w')
print('header\tcontrol_flagged_rows\tcontrol_flagged_files\tdiff_lines_old_v_new\town_rows_flagged', file=o)
for h in sorted(p.stem for p in (A / 'hdr/engine').glob('*.h')):
    s = ctl.get(h, set()); rows = {r for r, k in s}
    d = [l for l in diff if l[0] in rows]
    ow = ','.join('%s:%s' % (a, 'yes' if a in rows else 'NO') for a in own.get(h, []))
    print(h, len(rows), len(s), len(d), ow, sep='\t', file=o)
print('#summary: %s; DIFF lines %d; ERROR lines %d; headers with a control hit %d of %d' % (done[0].strip() if done else 'NOT DONE', len(diff), len(err), sum(1 for h in (A / 'hdr/engine').glob('*.h') if ctl.get(h.stem)), len(list((A / 'hdr/engine').glob('*.h')))), file=o)
o.close(); print(open(A / 'evidence/equiv_per_header.tsv').read())
