#!/usr/bin/env python3
"""linkgate425.py <batch> <tag> [definer addr...]: /FORCE link (link377.py) of the batch's HEAD objects + definer objects (work/g/<a>_D)
v the new objects + the same definers. Watch = every HEAD self / new self and every inferred callee rename (objgate renames).
Pass: duplicates new <= HEAD; every new self DEFINED once; no name HEAD resolved left unresolved, except a callee whose HEAD spelling was
itself unresolved at HEAD (unresolved before and after: the owner row still defines a stand-in, queued in NAME_FIXES).
Writes batches/<b>/evidence/linkgate.txt."""
import sys, os, glob, re, subprocess, csv, shutil
W = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
b, tag = sys.argv[1:3]; defs = sys.argv[3:]; B = W + '/batches/' + b
rows = open(B + '/rows.txt').read().split()
sm = [r for r in csv.reader(open(B + '/selfmap.tsv'), delimiter='\t') if r]
ren = []
for l in open(B + '/evidence/objgate_summary.tsv'):
    f = l.rstrip('\n').split('\t')
    if len(f) > 2:
        for kv in f[2].split(' | '):
            if ' -> ' in kv: ren.append(kv.split(' -> '))
old = {x[1] for x in sm}; new = {x[2] for x in sm}; cold = {o for o, n in ren} - old; cnew = {n for o, n in ren} - new
watch = '^(' + '|'.join(re.escape(s) for s in sorted(old | new | cold | cnew)) + ')$'
D = [W + '/work/g/%s_D/%s.obj' % (a, a) for a in defs]
H = [glob.glob('%s/work/wd_%sh/*/*/%s/%s.obj' % (W, tag, a, a))[0] for a in rows]
N = [glob.glob('%s/work/wd_%sn/*/*/%s/%s.obj' % (W, tag, a, a))[0] for a in rows]
subprocess.run([sys.executable, W + '/tools/link377.py', b, watch] + D + H + ['--'] + D + N, capture_output=True, text=True)
lk = open(W + '/evidence/link_%s.txt' % b).read(); shutil.copy(W + '/evidence/link_%s.txt' % b, B + '/evidence/link.txt')
d = re.findall(r'^duplicates (\d+)', lk, re.M); parts = lk.split('## new set')
headpart = parts[0]; newpart = parts[1].split('## diff')[0]
hdef = set(re.findall(r'^DEFINED (\S+) in', headpart, re.M)); ndef = set(re.findall(r'^DEFINED (\S+) in', newpart, re.M))
hun = set(re.findall(r'^UNRESOLVED \d+ (\S+)', headpart, re.M)); nun = set(re.findall(r'^UNRESOLVED \d+ (\S+)', newpart, re.M))
pairs = [(o, n) for o, n in ren if o not in old]
lost = [(o, n) for o, n in pairs if o in hdef and n not in ndef]          # HEAD resolved the callee, new leaves it unresolved
both = [(o, n) for o, n in pairs if o not in hdef and n not in ndef]      # unresolved before and after
gain = [(o, n) for o, n in pairs if o not in hdef and n in ndef]
selfok = new <= ndef
ok = d and int(d[1]) <= int(d[0]) and not lost and selfok
s = ('%s: %d rows + %d definers; duplicates HEAD %s -> new %s; new selves defined %d/%d; callee renames %d: resolved->resolved %d, '
     'unresolved->resolved %d, unresolved both %d, LOST %d %s; %s'
     % (b, len(rows), len(D), d[0], d[1], len(new & ndef), len(new), len(pairs), len([p for p in pairs if p[0] in hdef and p[1] in ndef]),
        len(gain), len(both), len(lost), lost[:5], 'PASS' if ok else 'FAIL'))
open(B + '/evidence/linkgate.txt', 'w').write(s + '\n' + ''.join('UNRESOLVED_BOTH %s -> %s\n' % p for p in both)); print(s)
