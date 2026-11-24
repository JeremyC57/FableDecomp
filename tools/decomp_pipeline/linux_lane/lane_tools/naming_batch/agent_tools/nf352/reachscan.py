# reachscan.py <tree> <closure addr file> <out.tsv>: the independent cross-check of the planted control. For each closure row's source and
# test, preprocess with gcc cpp against <tree> (equiv_check's flags and stub dir) and record whether the TU text contains the
# CCountedPointer.h-only token `CCPPointerInfo(unsigned long refCount` (the CCPPointerInfo constructor, defined nowhere else).
# Writes `<addr>\t<src|test>\tREACH|NO` lines; the REACH set must equal equiv_check's planted-control DIFF set.
import sys, os, re, subprocess
from concurrent.futures import ThreadPoolExecutor
T, cl, out = sys.argv[1:4]; R = '/home/user/FableDecomp'; A = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
only = set(open(cl).read().split())
bc = open(R + '/rebuild/build_candidates.ps1', encoding='utf-8', errors='replace').read(); rows = []
for m in re.finditer(r"\[pscustomobject\]@\{(.*?)\n\s*\}", bc, re.S):
    d = dict(re.findall(r"(\w+) = '([^']*)'", m.group(1)))
    if 'Address' in d and 'Source' in d and d['Address'].lower() in only:
        rows.append((d['Address'].lower(), R + '/rebuild/src/compiled/' + d['Source'], R + '/rebuild/tests/' + d['TestSource'] if d.get('TestSource') else None))
seen = set(); rows = [r for r in rows if not (r[0] in seen or seen.add(r[0]))]
def one(r):
    a, s, t = r; res = []
    for k, f in (('src', s), ('test', t)):
        if not f or not os.path.exists(f): continue
        p = subprocess.run(['cpp', '-P', '-nostdinc', '-undef', '-D_MSC_VER=1310', '-D_M_IX86=600', '-D_WIN32', '-x', 'c++', '-I', T, '-I', A + '/stubinc_equiv', '-idirafter', R + '/rebuild/include', f],
                           capture_output=True, text=True, errors='replace', cwd=R)
        res.append('%s\t%s\t%s' % (a, k, 'REACH' if re.search(r'CCPPointerInfo\s*\(\s*unsigned long refCount', p.stdout) else 'NO'))
    return res
with open(out, 'w') as o, ThreadPoolExecutor(4) as ex:
    for res in ex.map(one, rows, chunksize=16):
        for l in res: o.write(l + '\n')
print('REACHSCAN_DONE', len(rows))
