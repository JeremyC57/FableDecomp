# census.py (NF342; after NF339's census.py): HEAD census of landed rows, no source change.
#   census.py pay <tag> <addr file>   recheck.py payload of the landed rows -> work/cen_<tag>/ (cwd = repo for recheck.py; it writes only <outdir>)
#                                     rows whose lock is held by another agent, or whose HEAD source / test has no passing precheck, are left out
#                                     (work/cen_<tag>/skipped.tsv)
#   cen_run.sh <tag>                  (background, timeout 900 on its own line) vland --recheck-landed -> work/cen_<tag>/vland.log
#   census.py read <tag>              per row: grade, graded symbol (verify_and_land.obj_text), DEF / UND externals, and every relocation of the
#                                     graded function resolved against the retail bytes (retail target -> symbol) -> evidence/census/<addr>.json
#                                     (last write per address wins) and work/census.tsv (appended); then clean.py the chunk's wd
import os, re, sys, json, struct, subprocess, contextlib, io, collections
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import common as C
import nf
R = C.R; A = C.A; W = C.W; LT = R + '/tools/decomp_pipeline/linux_lane/lane_tools'
os.environ['PYTHONDONTWRITEBYTECODE'] = '1'; sys.dont_write_bytecode = True
os.environ.setdefault('FABLE_OBJDUMP', 'objdump')
cmd, tag = sys.argv[1], sys.argv[2]; KEEP = '--keep' in sys.argv[3:]
d = f'{W}/cen_{tag}'


def pre_ok():
    """label -> True when the newest clang line passed, or a ZS_PASS line exists (D1)"""
    ok = {}
    if os.path.exists(W + '/pre.log'):
        for l in open(W + '/pre.log'):
            p = l.split()
            if len(p) > 2: ok[p[1]] = (p[2] == 'PASS')
    if os.path.exists(W + '/pre_zs.log'):
        for l in open(W + '/pre_zs.log'):
            p = l.split()
            if len(p) > 2 and p[1] == 'ZS_PASS': ok[p[2]] = True
    return ok


if cmd == 'pay':
    os.makedirs(d, exist_ok=True)
    ok = pre_ok(); go = []; sk = []
    for a in open(sys.argv[3]).read().split():
        own = C.lock_owner(a)
        if own != 'released' and own != 'NF342': sk.append((a, 'locked by ' + own)); continue
        if C.src_path(a) and not ok.get('H_' + a): sk.append((a, 'HEAD source precheck not passed')); continue
        if C.test_path(a) and not ok.get('T_' + a): sk.append((a, 'HEAD test precheck not passed')); continue
        go.append(a)
    with open(d + '/skipped.tsv', 'w') as f:
        for a, why in sk: f.write(f'{a}\t{why}\n')
    r = subprocess.run(['python3', LT + '/recheck.py', d] + go, cwd=R, capture_output=True, text=True,
                       env=dict(os.environ, PYTHONDONTWRITEBYTECODE='1'))
    print(r.stdout[-300:], r.stderr[-300:], 'skipped', len(sk))
    sys.exit(0)

# read
os.chdir(W)
sys.path.insert(0, R + '/tools/decomp_pipeline'); sys.argv = ['x']
import verify_and_land as V
st = {}; warn = collections.defaultdict(list)
for l in open(d + '/vland.log', errors='replace'):
    m = re.match(r'([0-9a-f]{8})\s+(\S+)\s+(\S+)\s', l)
    if not m: continue
    if m.group(2) == 'WARN': warn[m.group(1)].append(l.strip()[9:][:300]); continue
    st.setdefault(m.group(1), (m.group(2), m.group(3)))
pay = json.load(open(d + '/payload.json'))['result']['authored']
cat = nf.catalog()
os.makedirs(A + '/evidence/census', exist_ok=True)
def comdat_sel(path):
    d = open(path, 'rb').read()
    ns = struct.unpack_from('<H', d, 2)[0]; psym, nsym = struct.unpack_from('<II', d, 8); oh = struct.unpack_from('<H', d, 16)[0]
    strt = d[psym + 18 * nsym:]
    ch = [struct.unpack_from('<I', d, 20 + oh + 40 * k + 36)[0] for k in range(ns)]
    sel = {}; out = {}; i = 0; ext = []
    while i < nsym:
        o_ = psym + 18 * i; raw = d[o_:o_ + 8]; val, sec, typ, scl, naux = struct.unpack_from('<IhHBB', d, o_ + 8)
        nm = strt[struct.unpack_from('<I', raw, 4)[0]:].split(b'\0')[0].decode('latin1') if raw[:4] == b'\0\0\0\0' else raw.rstrip(b'\0').decode('latin1')
        if scl == 3 and naux and val == 0 and sec > 0 and (ch[sec - 1] & 0x1000): sel.setdefault(sec, d[o_ + 18 + 14])
        if scl == 2 and sec > 0: ext.append((nm, sec))
        i += 1 + naux
    for nm, sec in ext: out[nm] = sel.get(sec, 0) if (ch[sec - 1] & 0x1000) else 0
    return out


rows = []
for x in pay:
    a = x['address']; o = f'{d}/wd/{a[:2]}/{a[2:4]}/{a}/{a}.obj'
    g = st.get(a, ('NOT_GRADED', '-'))
    rec = dict(address=a, grade=g[0], behaviour=g[1], leaf=x['leaf'], warn=warn.get(a, []), src_sha=__import__('hashlib').sha1(x['source_cpp'].encode()).hexdigest()[:12])
    if not os.path.exists(o):
        rec.update(obj=False); rows.append(rec)
        json.dump(rec, open(f'{A}/evidence/census/{a}.json', 'w'), indent=1); continue
    syms = nf.ext_syms(o)
    rec['DEF'] = sorted(s[4:] for s in syms if s.startswith('DEF '))
    rec['DEF_SEL'] = comdat_sel(o)    # external DEF -> COMDAT selection (1 NODUPLICATES = strong under /Gy, 2 ANY = pick-any), 0 = not COMDAT
    rec['UND'] = sorted(s[4:] for s in syms if s.startswith('UND '))
    try:
        with contextlib.redirect_stderr(io.StringIO()): gb, gsec, gs = V.obj_text(o, x['leaf'])
    except Exception as ex: gb, gsec, gs = b'', -1, 'ERR ' + str(ex)[:80]
    rec['graded'] = gs
    # relocations of the graded function resolved against the retail bytes
    bind = collections.Counter()
    try:
        retail = bytes.fromhex(cat[a]['bytes']); va0 = int(a, 16)
        secs, sl = nf.coff(o)
        cand = [(n, v, sec) for i, n, v, sec, t, scl in sl if n == gs and sec > 0]
        if cand:
            n, v, sec = cand[0]; s = secs[sec - 1]
            for off, sym, typ in s['rel']:
                ofs = off - v
                if ofs < 0 or ofs + 4 > len(retail) or sym.startswith('$'): continue
                add = struct.unpack_from('<i' if typ == 0x14 else '<I', s['data'], off)[0] if off + 4 <= len(s['data']) else 0
                if typ == 0x14: tgt = (va0 + ofs + 4 + struct.unpack_from('<i', retail, ofs)[0] - add) & 0xffffffff
                elif typ == 0x06: tgt = (struct.unpack_from('<I', retail, ofs)[0] - add) & 0xffffffff
                else: continue
                bind[('%08x' % tgt, sym)] += 1
    except Exception as ex:
        rec['bind_err'] = str(ex)[:120]
    rec['bind'] = [[t, s, k] for (t, s), k in sorted(bind.items())]
    rec['obj'] = True
    rows.append(rec)
    json.dump(rec, open(f'{A}/evidence/census/{a}.json', 'w'), indent=1)
with open(W + '/census.tsv', 'a') as f:
    for r in rows:
        f.write('\t'.join([tag, r['address'], r['grade'], r['behaviour'], r.get('graded', '-'), str(len(r.get('DEF', []))), str(len(r.get('bind', [])))]) + '\n')
print(collections.Counter(r['grade'] for r in rows), 'rows', len(rows))
if not KEEP and os.path.isdir(d + '/wd'):
    subprocess.run(['python3', LT + '/naming_batch/clean.py', A, d + '/wd'], capture_output=True)
