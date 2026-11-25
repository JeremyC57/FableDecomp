#!/usr/bin/env python3
"""hdrvar.py <src batch> <tag> <dst batch>: for every row of <src batch> whose new vland log gained WARN SHARED_HEADER_AVAILABLE for the
class the generator declared locally, write a variant into <dst batch> that includes the shared header (engine/<C>.h, path from the warn)
instead of the bodiless local declaration. orig/ copies and renames.tsv follow."""
import sys, os, re, shutil, collections
W = '/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad/w22/agentNF443'
src, tag, dst = sys.argv[1:4]; S_ = W + '/batches/' + src; D = W + '/batches/' + dst
h = collections.defaultdict(set); n = collections.defaultdict(dict)
for l in open('%s/work/vland_%sh.log' % (W, tag), errors='replace'):
    m = re.match(r'^([0-9a-f]{8})\s+WARN SHARED_HEADER_AVAILABLE (\S+) \((\S+)\)', l)
    if m: h[m.group(1)].add(m.group(2))
for l in open('%s/work/vland_%sn.log' % (W, tag), errors='replace'):
    m = re.match(r'^([0-9a-f]{8})\s+WARN SHARED_HEADER_AVAILABLE (\S+) \((\S+)\)', l)
    if m: n[m.group(1)][m.group(2)] = m.group(3)
for d in ('rows', 'tests', 'orig/rows', 'orig/tests', 'evidence', 'head'): os.makedirs(D + '/' + d, exist_ok=True)
ren = [l for l in open(S_ + '/renames.tsv')]; out = []
for a in open(S_ + '/rows.txt').read().split():
    new = {c: p for c, p in n[a].items() if c not in h[a]}
    if not new: continue
    s = open(S_ + '/rows/%s.cpp' % a).read(); ok = True
    for c, p in new.items():
        pat = re.compile(r'^((?:namespace \w+ \{ )*)class %s \{ public: (virtual )?~%s\(\); \};( \})*\n' % (c, c), re.M)
        if len(pat.findall(s)) != 1: ok = False; break
        s = pat.sub('#include "%s"\n' % p, s)
        s = s.replace('called qualified on a declaration of the class (no body, no layout)', 'called qualified through the shared %s' % p)
    if not ok: print('SKIP', a); continue
    open(D + '/rows/%s.cpp' % a, 'w').write(s)
    for x in ('rows/%s.cpp', 'tests/%s_test.cpp'):
        if os.path.exists(S_ + '/orig/' + x % a): shutil.copy(S_ + '/orig/' + x % a, D + '/orig/' + x % a)
    out.append(a)
open(D + '/rows.txt', 'w').write(''.join(a + '\n' for a in out))
open(D + '/renames.tsv', 'w').write(''.join(l for l in ren if l.split('\t')[0] in out))
print(dst, len(out))
