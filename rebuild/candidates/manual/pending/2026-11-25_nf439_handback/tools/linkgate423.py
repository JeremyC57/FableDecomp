#!/usr/bin/env python3
"""linkgate423.py <batch> <tag> <definer addr...>: /FORCE link (link377.py) of the batch's HEAD objects + definer objects (work/g/<a>_D)
v the new objects + the same definers. Watch = every old and new symbol of renames.tsv. Pass: duplicates new <= HEAD, NEW_UNRESOLVED 0,
every new name DEFINED, every old stand-in name gone from the new set. Writes batches/<b>/evidence/linkgate.txt."""
import sys, os, glob, re, subprocess, csv, shutil
W = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
b, tag = sys.argv[1:3]; defs = sys.argv[3:]; B = W + '/batches/' + b
rows = open(B + '/rows.txt').read().split()
ren = [r for r in csv.reader(open(B + '/renames.tsv'), delimiter='\t') if r]
import os.path
defs = [a for a in defs if os.path.exists(W + '/work/g/%s_D/%s.obj' % (a, a))]
old = {r[1] for r in ren}; new = {r[2] for r in ren if r[3] in defs}; new_all = {r[2] for r in ren}
watch = '^(' + '|'.join(re.escape(s) for s in sorted(old | new_all)) + ')$'
D = [W + '/work/g/%s_D/%s.obj' % (a, a) for a in defs]
H = [glob.glob('%s/work/wd_%sh/*/*/%s/%s.obj' % (W, tag, a, a))[0] for a in rows]
N = [glob.glob('%s/work/wd_%sn/*/*/%s/%s.obj' % (W, tag, a, a))[0] for a in rows]
subprocess.run([sys.executable, W + '/tools/link377.py', b, watch] + D + H + ['--'] + D + N, capture_output=True, text=True)
lk = open(W + '/evidence/link_%s.txt' % b).read(); shutil.copy(W + '/evidence/link_%s.txt' % b, B + '/evidence/link.txt')
d = re.findall(r'^duplicates (\d+)', lk, re.M); parts = lk.split('## new set')
nu = re.findall(r'^NEW_UNRESOLVED (\S+)', lk, re.M)
newpart = parts[1].split('## diff')[0]
defined = set(re.findall(r'^DEFINED (\S+) in', newpart, re.M)); unres_new = set(re.findall(r'^UNRESOLVED \d+ (\S+)', newpart, re.M))
headunres = set(re.findall(r'^UNRESOLVED \d+ (\S+)', parts[0], re.M))
nu = [x for x in nu if x not in (new_all - new)]
ok = d and int(d[1]) <= int(d[0]) and not nu and new <= defined and not (old & defined)   # NF439: an old stand-in may stay UND where another row still calls it (objgate proves each respelt row dropped it)
nodef = sorted(new_all - new)
s = ('%s: %d rows + %d definers; duplicates HEAD %s -> new %s; NEW_UNRESOLVED %d; new names defined %d/%d; HEAD unresolved stand-ins %d -> new %d; %s'
     % (b, len(rows), len(D), d[0], d[1], len(nu), len(new & defined), len(new), len(old & headunres), len(old & unres_new), ('(%d new names whose target has no landed definer: expected unresolved) ' % len(nodef)) + ('PASS' if ok else 'FAIL')))
open(B + '/evidence/linkgate.txt', 'w').write(s + '\n'); print(s)
