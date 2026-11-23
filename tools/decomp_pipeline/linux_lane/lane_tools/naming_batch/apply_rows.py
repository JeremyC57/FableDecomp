#!/usr/bin/env python3
"""Coordinator: copy a hand-back's rows/<addr>.cpp (and rows/<addr>_test.cpp) over the landed files, found BY ADDRESS in
rebuild/build_candidates.ps1 (never by a remembered path).   apply_rows.py <handback>/rows [--dry]"""
import os, re, sys, shutil
R = os.environ.get('FABLE_ROOT', '/home/user/FableDecomp')
cat = open(R + '/rebuild/build_candidates.ps1', encoding='utf-8', errors='replace').read()
src, tst = {}, {}
for m in re.finditer(r"Address = '([0-9a-fA-F]{8})'(.*?)\n\s*\}", cat, re.S):
    a = m.group(1).lower(); body = m.group(2)
    s = re.search(r"\n\s*Source = '([^']+)'", body); t = re.search(r"TestSource = '([^']+)'", body)
    if s: src[a] = R + '/rebuild/src/compiled/' + s.group(1)
    if t: tst[a] = R + '/rebuild/tests/' + t.group(1)
d = sys.argv[1]; dry = '--dry' in sys.argv; n = bad = 0
for f in sorted(os.listdir(d)):
    m = re.match(r'([0-9a-fA-F]{8})(_test)?\.cpp$', f)
    if not m: print('skip', f); continue
    a = m.group(1).lower(); dst = (tst if m.group(2) else src).get(a)
    if not dst or not os.path.exists(dst): print('PROBLEM no landed file for', f); bad += 1; continue
    if not dry: shutil.copyfile(os.path.join(d, f), dst)
    n += 1
print('written' if not dry else 'would write', n, 'problems', bad)
