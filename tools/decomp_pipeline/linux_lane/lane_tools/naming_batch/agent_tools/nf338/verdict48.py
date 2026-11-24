"""verdict48.py: verdicts for the TRIAGE reread-needed entries whose only free binder had a signature-only / stand-in-gone state: each
witness object was compiled at HEAD (work/hobj, census grade lanes) and work/witbind.tsv shows what it binds at the target. A witness that
still binds a non-PDB spelling confirms the recorded reason (the fix is the measured / stand-in port the line names). Skips idx already in
work/verdict.tsv and the idx given in SKIP (worked in place)."""
import os, sys
A = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
skip = set(sys.argv[1:])
V = {l.split('\t')[0] for l in open(A + '/work/verdict.tsv')}
E = [l.split('\t') for l in open(A + '/ENTRIES.tsv').read().splitlines()[1:]]
T = [l.rstrip('\n').split('\t') for l in open(A + '/TRIAGE.tsv')][1:]
need = {r[0] for r in T if r[6] == 'reread-needed' and r[0] not in V and r[0] not in skip}
wb = {}
for l in open(A + '/work/witbind.tsv'):
    f = l.rstrip('\n').split('\t'); wb.setdefault(f[0], []).append(f)
out = []
for i in sorted(need, key=int):
    e = E[int(i) - 1]
    ws = wb.get(i, [])
    if ws and all(w[5] == 'notPDB' for w in ws):
        b = '; '.join('witness %s compiled at HEAD binds %s (%s)' % (w[2], w[3], 'table line' if w[6] == 'TABLE' else 'no table line') for w in ws)
        out.append('%s\tholds\tre-read: %s, not the PDB %s; the recorded reason (stand-in / measured port) is confirmed on today\'s tree\n' % (i, b, e[6]))
    else:
        out.append('%s\treread-open\tno compiled witness verdict (%s)\n' % (i, ws))
open(A + '/work/verdict.tsv', 'a').write(''.join(out))
print(len(out), sum(1 for x in out if '\tholds\t' in x))
