# hasmodel338.py (NF338): per wrong table line with a free witness: does the witness TU already model the PDB scope (a local
# `class|struct <Name> {` for the PDB spelling's innermost class, or an #include of engine/<Name>.h / a header declaring it)?
# Prints idx target witness state MODEL:<how> pdb_pretty  (only lines where a model exists)
import sys, re, os
A = os.path.dirname(os.path.dirname(os.path.abspath(__file__))); R = '/home/user/FableDecomp'
sys.path.insert(0, A + '/tools'); import nf
sys.path.insert(0, R + '/tools/decomp_pipeline/naming'); import msvc_demangle as M
why = {l.split('\t')[0]: l.split('\t')[1].strip() for l in open(A + '/work/offlimits_why.tsv')}
E = nf.entries()
for l in open(A + '/work/stale2.tsv'):
    i, t, w, st, x, p = l.rstrip('\n').split('\t')
    if w in why or nf.lock_owner(w) not in ('released', 'NF338'): continue
    d = M.demangle(E[int(i) - 1][6]); cls = d.get('cls') or ''
    if not cls: continue
    last = re.sub(r'<.*', '', cls.split('::')[-1])
    src = nf.src_of(w)
    if not src or not src.exists(): continue
    txt = re.sub(r'//[^\n]*', '', src.read_text(encoding='utf-8', errors='replace'))
    how = []
    if re.search(r'\b(class|struct)\s+%s\b[^;{]*\{' % re.escape(last), txt): how.append('local')
    for inc in re.findall(r'#include\s+"engine/([^"]+)\.h"', txt):
        if inc == last or inc.endswith('_' + last): how.append('inc:' + inc)
    if how: print('%s\t%s\t%s\t%s\t%s\t%s' % (i, t, w, st, ','.join(how), M.pretty(E[int(i) - 1][6])[:150]))
