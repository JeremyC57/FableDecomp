# census.py (NF339): HEAD DEF / UND census of landed rows (no source change).
#   census.py pay <tag> <addr file>     recheck.py payload of the landed rows -> work/cen_<tag>/ (cwd repo for recheck.py, read-only there)
#   census.py read <tag>                after work/cen_<tag>/vland.log: per row grade, graded symbol, does the object DEFINE the PDB symbol,
#                                       does it still REFERENCE the old spelling -> work/census_def.tsv (appended, last line per addr wins)
# The vland run itself is launched by cen_run.sh (background, timeout 900 on its own line, cwd = work dir).
import os, re, sys, subprocess, contextlib, io
R = '/home/user/FableDecomp'; A = '/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad/w22/agentNF339'
W = A + '/work'; LT = R + '/tools/decomp_pipeline/linux_lane/lane_tools'
sys.path.insert(0, A + '/tools'); import nf
os.chdir(W); os.environ.setdefault('FABLE_OBJDUMP', 'objdump')
cmd, tag = sys.argv[1], sys.argv[2]
d = f'{W}/cen_{tag}'
if cmd == 'pay':
    os.makedirs(d, exist_ok=True)
    addrs = open(sys.argv[3]).read().split()
    r = subprocess.run(['python3', LT + '/recheck.py', d] + addrs, cwd=R, capture_output=True, text=True)
    print(r.stdout[-500:], r.stderr[-500:])
    sys.exit(0)
# read
tri = {}
for l in open(W + '/tri2.tsv').read().splitlines()[1:]:
    f = l.split('\t'); tri.setdefault(f[0], []).append(f)
st = {}
for l in open(d + '/vland.log', errors='replace'):
    m = re.match(r'([0-9a-f]{8})\s+(\S+)\s+(\S+)\s', l)
    if m and m.group(2) != 'WARN': st.setdefault(m.group(1), (m.group(2), m.group(3)))
import json
pay = json.load(open(d + '/payload.json'))['result']['authored']
sys.path.insert(0, R + '/tools/decomp_pipeline'); argv0 = sys.argv; sys.argv = ['x']
import verify_and_land as V
out = []
for x in pay:
    a = x['address']; o = f'{d}/wd/{a[:2]}/{a[2:4]}/{a}/{a}.obj'
    g = st.get(a, ('NOT_GRADED', '-'))
    if not os.path.exists(o): out.append([a, 'NO_OBJ', f'{g[0]} {g[1]}', '', '']); continue
    syms = nf.ext_syms(o)          # 'DEF <sym>' / 'UND <sym>' lines
    defs = {s[4:] for s in syms if s.startswith('DEF ')}; unds = {s[4:] for s in syms if s.startswith('UND ')}
    with contextlib.redirect_stderr(io.StringIO()): gs = V.obj_text(o, x['leaf'])[2]
    for f in tri.get(a, []):
        new, old = f[4], f[3]
        if f[2] == 'binder_line' or f[1] == 'binders1':
            verdict = ('UND_OLD' if old and old in unds else 'NO_OLD_REF') + ('+UND_NEW' if new and new in unds else '')
        else:
            verdict = 'DEF_PDB' if new in defs and gs == new else ('DEF_PDB_NOT_GRADED' if new in defs else ('DEF_OLD' if old in defs else 'DEF_OTHER'))
        out.append([a, verdict, f'{g[0]} {g[1]} graded {gs}', f[1], ''])
hdr = not os.path.exists(W + '/census_def.tsv')
with open(W + '/census_def.tsv', 'a') as fo:
    if hdr: fo.write('addr\tverdict\tgrade\ttag\tnote\n')
    for r in out: fo.write('\t'.join(r) + '\n')
import collections; print(collections.Counter(r[1] for r in out))
