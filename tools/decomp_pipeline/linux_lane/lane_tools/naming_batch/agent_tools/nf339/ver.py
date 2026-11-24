# ver.py (NF339): HEAD v new proof for changed rows, the access_fix.py way (both compiled by vland at ONE path, then moved aside).
#   ver.py pay <tag> <ren.tsv>   ren.tsv: addr <TAB> new_self_symbol_or_- <TAB> old=new [old=new ...]   (rows/<addr>.cpp, rows/<addr>_test.cpp)
#                                -> work/v_<tag>/{head,new}/payload.json + oracle.tsv
#   ver_run.sh <tag> head|new    background vland (timeout 900 on its own line)
#   ver.py read <tag> <ren.tsv>  objsec under the renames, DEF / UND symbol deltas, graded symbol, grade -> work/v_<tag>/RESULTS.tsv
import os, re, sys, json, subprocess, contextlib, io, shutil
R = '/home/user/FableDecomp'; A = '/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad/w22/agentNF339'
W = A + '/work'; LT = R + '/tools/decomp_pipeline/linux_lane/lane_tools'
OBJSEC = LT + '/naming_batch/agent_tools/nf314/objsec.py'
sys.path.insert(0, A + '/tools'); import nf
os.chdir(W); os.environ.setdefault('FABLE_OBJDUMP', 'objdump')
cmd, tag, renf = sys.argv[1], sys.argv[2], sys.argv[3]
d = f'{W}/v_{tag}'
ren = {}
for l in open(renf).read().splitlines():
    if not l.strip() or l.startswith('#'): continue
    f = l.split('\t'); ren[f[0]] = (f[1], [tuple(x.split('=', 1)) for x in f[2].split()] if len(f) > 2 and f[2].strip() else [])
def leaf(t):
    m = re.search(r'^//\s*Leaf:\s*(\S+)', t, re.M); return m.group(1) if m else None
if cmd == 'pay':
    os.makedirs(d + '/head', exist_ok=True); os.makedirs(d + '/new', exist_ok=True)
    r = subprocess.run(['python3', LT + '/recheck.py', d + '/head'] + list(ren), cwd=R, capture_output=True, text=True); print(r.stdout[-300:])
    pay = json.load(open(d + '/head/payload.json'))
    for x in pay['result']['authored']:
        a = x['address']
        x['source_cpp'] = open(f'{A}/rows/{a}.cpp', encoding='utf-8').read()
        if os.path.exists(f'{A}/rows/{a}_test.cpp'): x['test_cpp'] = open(f'{A}/rows/{a}_test.cpp', encoding='utf-8').read()
        lf = leaf(x['source_cpp'])
        if lf: x['leaf'] = lf
    json.dump(pay, open(d + '/new/payload.json', 'w')); shutil.copy(d + '/head/oracle.tsv', d + '/new/oracle.tsv')
    print('rows', len(pay['result']['authored'])); sys.exit(0)
# read
sys.path.insert(0, R + '/tools/decomp_pipeline'); sys.argv = ['x']
import verify_and_land as V
def st(p):
    s = {}
    for l in open(p, errors='replace'):
        m = re.match(r'([0-9a-f]{8})\s+(\S+)\s+(\S+)\s', l)
        if m and m.group(2) != 'WARN': s.setdefault(m.group(1), (m.group(2), m.group(3)))
    return s
sh, sn = st(d + '/head/vland.log'), st(d + '/new/vland.log')
ph = {x['address']: x for x in json.load(open(d + '/head/payload.json'))['result']['authored']}
pn = {x['address']: x for x in json.load(open(d + '/new/payload.json'))['result']['authored']}
out = []
for a, (newself, pairs) in ren.items():
    oh = f'{d}/head/wd/{a[:2]}/{a[2:4]}/{a}/{a}.obj'; on = f'{d}/new/wd/{a[:2]}/{a[2:4]}/{a}/{a}.obj'
    gh, gn = sh.get(a, ('NOT_GRADED', '-')), sn.get(a, ('NOT_GRADED', '-'))
    if not (os.path.exists(oh) and os.path.exists(on)):
        out.append([a, 'REJECT', '', newself, f'no object (head {gh}, new {gn})']); continue
    with contextlib.redirect_stderr(io.StringIO()):
        sym_h = V.obj_text(oh, ph[a]['leaf'])[2]; sym_n = V.obj_text(on, pn[a]['leaf'])[2]
    mp = dict(pairs)
    o = subprocess.run(['python3', OBJSEC, oh, on] + [f'{k}={v}' for k, v in pairs], capture_output=True, text=True).stdout
    eh, en = nf.ext_syms(oh), nf.ext_syms(on)
    eh2 = {k[:4] + mp.get(k[4:], k[4:]) for k in eh}
    delta = sorted(['-' + x for x in eh2 - en] + ['+' + x for x in en - eh2])
    ok_self = (newself == '-' and sym_h == sym_n) or (newself != '-' and sym_n == newself and mp.get(sym_h, sym_h) == newself)
    good = 'ALL_EQ' in o and not delta and ok_self and gn[0].startswith(('MATCH', 'RELOCATION_MATCH')) and gn[1] == 'PASS' and gn[0] == gh[0]
    why = f"objsec {'ALL_EQ' if 'ALL_EQ' in o else ' / '.join(o.strip().splitlines()[-3:])}; symdelta {delta[:6]}; graded {sym_h} -> {sym_n}; grade head {gh[0]} {gh[1]} new {gn[0]} {gn[1]}"
    old_self = sym_h if newself != '-' else (pairs[0][0] if pairs else sym_h)
    new_col = newself if newself != '-' else (pairs[0][1] if pairs else sym_n)
    out.append([a, 'ACCEPT' if good else 'REJECT', old_self, new_col, why])
with open(d + '/RESULTS.tsv', 'w') as f:
    f.write('address\tverdict\told\tnew\treason\n' + ''.join('\t'.join(r) + '\n' for r in out))
for r in out: print(r[0], r[1], r[4][:300])
