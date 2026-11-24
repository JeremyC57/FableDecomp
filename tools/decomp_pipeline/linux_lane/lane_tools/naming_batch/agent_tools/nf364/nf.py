# nf.py: NF323's naming-batch helper (rebuilt 2026-11-23 after the restart). Subcommands:
#   census <idx...>            exe scan: per ENTRIES line, the landed rows whose retail body calls / jumps to / holds the entry address
#   scen <idx...>              source census: per row (self + refs) its declaration lines of the method, access in effect, const / virtual,
#                              or the shared-header lines declaring it (call-only rows)
#   pre <label> <file> [nowchar]   clang -fsyntax-only precheck, every error classified by file -> work/pre.log
#                              (NF_TREE=<full include tree> replaces hdr/ + rebuild/include; VC7.1 steps take FABLE_HDR_OVERLAY=<tree>)
#   zs <tag> <file> <src|test> [flags...]   D1 VC7.1 `cl /Zs` (vc71() text, row flags; tests /Od /W3) -> work/pre_zs.log
#   grade <tag> <addr...>      VC7.1 HEAD (repo source) + new (rows/<addr>.cpp) objects, graded against the catalog oracle
#   hgrade <tag> <addr...>     VC7.1 HEAD object only (controls) -> work/hobj/<addr>.obj
#   proof <tag> <addr...>      objsec HEAD v new: every non-debug section's bytes + relocations equal under the symbol renames
#   link <tag> <watch> <objs...> -- <objs...>   /FORCE link of a HEAD set and a new set: duplicates + unresolved names matching watch
#   xr <addr> <obj>            the leaf function's relocations resolved against the retail bytes: retail target -> symbol (count)
#   syms <obj>                 DEF / UND external symbols
#   pay <prefix> <addr...>     vland --recheck-landed payload (rows/<addr>.cpp + rows/<addr>_test.cpp when present, else landed)
# Every compile here is a grade step: launch the whole command in the background with `timeout 900` on its own line.
import sys, os, re, struct, subprocess, collections, json, shutil
from pathlib import Path
A = Path('/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad/w22/agentNF364')
S = Path('/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad')
R = Path('/home/user/FableDecomp')
W = A / 'work'; W.mkdir(exist_ok=True)
sys.path.insert(0, str(R / 'tools/decomp_pipeline'))
PS = (R / 'rebuild/build_candidates.ps1').read_text(encoding='utf-8')

_BLK = None
def blk(a):
    # bounded to the row's own [pscustomobject]@{ ... } block (NF294 found a flags lookup that read the NEXT entry's fields)
    global _BLK
    if _BLK is None:
        _BLK = {}
        for m in re.finditer(r"\[pscustomobject\]@\{(.*?)\n\s*\}", PS, re.S):
            k = re.search(r"\bAddress = '([0-9A-Fa-f]{8})'", m.group(1))
            if k: _BLK.setdefault(k.group(1).lower(), m.group(1))
    return _BLK.get(a.lower())
def field(a, k):
    b = blk(a)
    m = re.search(r"\b%s = '([^']*)'" % k, b) if b else None
    return m.group(1) if m else None
def src_of(a): s = field(a, 'Source'); return R / 'rebuild/src/compiled' / s if s else None
def test_of(a): t = field(a, 'TestSource'); return R / 'rebuild/tests' / t if t else None
def lock_owner(a):
    p = S / 'w22/nflocks' / a / 'owner'
    return p.read_text().strip() if p.exists() else 'released'
def entries():
    L = [l.rstrip('\n').split('\t') for l in open(A / 'ENTRIES.tsv', encoding='utf-8')]
    return L[1:]
_cat = None
def catalog():
    global _cat
    if _cat is None:
        _cat = {}
        import csv; csv.field_size_limit(10**9)
        for r in csv.DictReader(open(R / 'rebuild/oracles/auto-re-candidates.tsv', encoding='utf-8-sig'), delimiter='\t'):
            _cat[r['address'].lower().replace('0x', '')] = r
    return _cat
def exe():
    d = open(os.environ.get('FABLE_EXE', str(S / 'dl/game/Fable The Lost Chapters/Fable.exe')), 'rb').read()
    pe = struct.unpack_from('<I', d, 0x3C)[0]; ns = struct.unpack_from('<H', d, pe + 6)[0]; osz = struct.unpack_from('<H', d, pe + 20)[0]
    base = struct.unpack_from('<I', d, pe + 24 + 28)[0]; secs = []
    for i in range(ns):
        o = pe + 24 + osz + 40 * i; vs, va, rs, rp = struct.unpack_from('<IIII', d, o + 8); ch = struct.unpack_from('<I', d, o + 36)[0]
        secs.append((d[o:o + 8].rstrip(b'\0').decode(), base + va, d[rp:rp + rs], ch))
    return secs

# ---------- COFF ----------
def coff(path):
    d = open(path, 'rb').read()
    ns = struct.unpack_from('<H', d, 2)[0]; psym, nsym = struct.unpack_from('<II', d, 8); oh = struct.unpack_from('<H', d, 16)[0]
    strt = d[psym + 18 * nsym:]
    def sname(raw):
        if raw[:4] == b'\0\0\0\0':
            off = struct.unpack_from('<I', raw, 4)[0]; return strt[off:strt.index(b'\0', off)].decode('latin1')
        return raw.rstrip(b'\0').decode('latin1')
    syms = {}; i = 0; symlist = []
    while i < nsym:
        o = psym + 18 * i; raw = d[o:o + 8]; val, sec, typ, scl, naux = struct.unpack_from('<IhHBB', d, o + 8)
        syms[i] = (sname(raw), val, sec, typ, scl); symlist.append((i, sname(raw), val, sec, typ, scl)); i += 1 + naux
    secs = []
    for k in range(ns):
        o = 20 + oh + 40 * k; raw = d[o:o + 8]
        nm = raw.rstrip(b'\0').decode('latin1')
        if nm.startswith('/'): off = int(nm[1:]); nm = strt[off:strt.index(b'\0', off)].decode('latin1')
        vs, va, rs, rp, rr, rl, nr, nl, ch = struct.unpack_from('<IIIIIIHHI', d, o + 8)
        data = d[rp:rp + rs] if rp else b''
        rel = [struct.unpack_from('<IIH', d, rr + 10 * j) for j in range(nr)]
        secs.append(dict(name=nm, ch=ch, data=data, rel=[(va_, syms[si][0], t) for va_, si, t in rel]))
    return secs, symlist
def ext_syms(path):
    _, sl = coff(path); out = set()
    for i, n, v, sec, typ, scl in sl:
        if scl == 2: out.add(('DEF ' if sec > 0 else 'UND ') + n)
    return out

def leaf_of(src, a):
    m = re.search(r'^// Leaf:\s*(\S+)', src, re.M)
    if m: return m.group(1)
    nm = catalog()[a]['name']; return nm.rsplit('::', 1)[-1]
def flags_of(a, src):
    cf = field(a, 'CompilerFlags')
    if cf is not None: return [x for x in cf.split() if x not in ('/O2', '/Oy', '/W3')]
    m = re.search(r'^// Flags:[ \t]*(.*)', src, re.M)
    return [x for x in m.group(1).split() if x not in ('none', '(none)')] if m else []

def compile_one(a, srcpath, tag):
    import verify_and_land as V
    src = Path(srcpath).read_text(encoding='utf-8')
    retail = bytes.fromhex(catalog()[a]['bytes'])
    while retail and retail[-1] in (0xCC, 0x90): retail = retail[:-1]
    wd = W / 'g' / (a + '_' + tag); wd.mkdir(parents=True, exist_ok=True)
    leaf = leaf_of(src, a); fl = flags_of(a, src)
    st, built, sec = V.parity_of(V.vc71(src), a, leaf, retail, wd, V.env(), extra=tuple(fl))
    obj = wd / (a + '.obj')
    return st, leaf, fl, obj, wd

def cmd_grade(tag, addrs, head_only=False):
    log = open(W / ('gr_%s.log' % tag), 'a')
    for a in addrs:
        if lock_owner(a) != 'NF364': print('NOLOCK', a, file=log); log.flush(); continue
        jobs = [('H', src_of(a))] + ([] if head_only else [('N', A / 'rows' / (a + '.cpp'))])
        for t, p in jobs:
            st, leaf, fl, obj, wd = compile_one(a, p, t)
            dst = (W / 'hobj' / (a + '.obj')) if head_only else (W / 'obj' / (('H_' if t == 'H' else '') + a + '.obj'))
            dst.parent.mkdir(exist_ok=True)
            if obj.exists(): shutil.copy(obj, dst)
            print('%s %s %s leaf=%s flags=%s' % ('pH' if t == 'H' else 'pS', a, st, leaf, ' '.join(fl)), file=log); log.flush()
            subprocess.run(['python3', str(R / 'tools/decomp_pipeline/linux_lane/lane_tools/naming_batch/clean.py'), str(A), str(wd)], capture_output=True)   # NF323: clean.py, not rmtree
    print('GRADE_DONE', file=log); log.close()

DEBUG = re.compile(r'^\.debug')
def cmd_proof(tag, addrs):
    out = open(A / 'evidence' / ('proof_%s.txt' % tag), 'w')
    for a in addrs:
        h, n = W / 'obj' / ('H_' + a + '.obj'), W / 'obj' / (a + '.obj')
        sh, sn = ext_syms(h), ext_syms(n)
        old = sorted(x[4:] for x in sh - sn); new = sorted(x[4:] for x in sn - sh)
        oldn = sorted(set(old)); newn = sorted(set(new))
        mp = dict(zip(oldn, newn)) if len(oldn) == len(newn) else None
        print('##### %s renames: %s' % (a, ' '.join('%s=%s' % kv for kv in (mp or {}).items()) if mp is not None else 'UNPAIRED old=%s new=%s' % (oldn, newn)), file=out)
        if mp is None: print('NOT_EQ unpaired renames', file=out); continue
        A_, _ = coff(h); B_, _ = coff(n)
        A_ = [s for s in A_ if not DEBUG.match(s['name'])]; B_ = [s for s in B_ if not DEBUG.match(s['name'])]
        bad = []
        if len(A_) != len(B_): bad.append('section count %d v %d' % (len(A_), len(B_)))
        for x, y in zip(A_, B_):
            if x['name'] != y['name'] or x['ch'] != y['ch'] or x['data'] != y['data']: bad.append('section %s data/flags' % x['name'])
            if [(v, mp.get(s, s), t) for v, s, t in x['rel']] != y['rel']: bad.append('section %s relocs' % x['name'])
        fh = {mp.get(x[4:], x[4:]) for x in sh if x.startswith('DEF ')}; fn = {x[4:] for x in sn if x.startswith('DEF ')}
        if fh != fn: bad.append('DEF sets differ: %s' % sorted(fh ^ fn))
        tabs = sorted(x[4:] for x in sn if x.startswith('DEF ??_7'))
        print(('ALL_EQ' if not bad else 'NOT_EQ ' + '; '.join(bad[:5])) + ' sections=%d DEF=%d tables=%s' % (len(B_), len(fn), ','.join(tabs) or '-'), file=out)
    out.close(); print(open(A / 'evidence' / ('proof_%s.txt' % tag)).read())

def cmd_link(tag, watch, objs):
    import verify_and_land as V
    i = objs.index('--'); sets = (('HEAD', objs[:i]), ('new', objs[i + 1:]))
    e = V.env(); e.pop('LINK', None); w = re.compile(watch)
    out = open(A / 'evidence' / ('link_%s.txt' % tag), 'w')
    for nm, ob in sets:
        dll = W / ('lk_%s_%s' % (tag, nm))
        r = V.run_tool(V.VC / 'bin' / 'link.exe', ['/nologo', '/DLL', '/NOENTRY', '/FORCE', '/NODEFAULTLIB', '/out:%s.dll' % dll] + ob, e)
        t = (r.stdout or '') + (r.stderr or '')
        d = [l for l in t.splitlines() if 'LNK4006' in l or 'LNK2005' in l]
        print('## %s set: %s' % (nm, ' '.join(Path(x).name for x in ob)), file=out)
        print('duplicates', len(d), file=out)
        for l in d:
            m = re.search(r'\((\?\S+|\?\?\S+)\) already defined in (\S+)', l); print('  DUP', l.split(':')[0].strip(), (m.group(1) + ' in ' + m.group(2)) if m else l[-200:], file=out)
        u = collections.Counter()
        for l in t.splitlines():
            m = re.search(r'(LNK2001|LNK2019): unresolved external symbol (?:"[^"]*" \((\S+)\)|(\S+))', l)
            if m:
                s = m.group(2) or m.group(3)
                if w.search(s): u[s] += 1
        for s, c in sorted(u.items()): print('UNRESOLVED', c, s, file=out)
        # NF323: link outputs go through the toolkit's clean.py (never a direct unlink / rm of a variable path)
        subprocess.run(['python3', str(R / 'tools/decomp_pipeline/linux_lane/lane_tools/naming_batch/clean.py'), str(A)] + [str(dll) + x for x in ('.dll', '.lib', '.exp')], capture_output=True)
    print('LK_DONE', file=out); out.close()

def cmd_xr(a, obj):
    import verify_and_land as V
    src = (A / 'rows' / (a + '.cpp')); src = src if src.exists() else src_of(a)
    leaf = leaf_of(src.read_text(encoding='utf-8'), a)
    retail = bytes.fromhex(catalog()[a]['bytes']); va0 = int(a, 16)
    secs, sl = coff(obj)
    # the leaf function's section: the text section whose COMDAT symbol contains the leaf (else the largest text section)
    cand = [(i, n, v, sec) for i, n, v, sec, t, scl in sl if scl == 2 and sec > 0 and secs[sec - 1]['name'].startswith('.text') and leaf in n]
    if not cand:
        big = max((k for k, s in enumerate(secs) if s['name'].startswith('.text')), key=lambda k: len(secs[k]['data']))
        cand = [(0, '?', 0, big + 1)]
    i, n, v, sec = cand[0]; s = secs[sec - 1]; c = collections.Counter()
    for off, sym, typ in s['rel']:
        o = off - v
        if o < 0 or o + 4 > len(retail): continue
        x = struct.unpack_from('<I', retail, o)[0]
        tgt = (va0 + o + 4 + x) & 0xffffffff if typ == 0x14 else x
        c[('%08x' % tgt, sym)] += 1
    for (t, sym), k in sorted(c.items()): print('%s\t%s\tx%d' % (t, sym, k))

def cmd_census(idxs):
    E = entries(); secs = exe(); ln = {}; cat = catalog()
    landed = set(m.group(1).lower() for m in re.finditer(r"Address = '([0-9a-fA-F]{8})'", PS))
    for a in landed:
        r = cat.get(a)
        if r and r.get('length', '').isdigit(): ln[int(a, 16)] = int(r['length'])
    import bisect; st = sorted(ln)
    def row(site):
        k = bisect.bisect_right(st, site) - 1
        return '%08x' % st[k] if k >= 0 and st[k] <= site < st[k] + ln[st[k]] else None
    T = {int(E[i - 1][0], 16): i for i in idxs}; refs = collections.defaultdict(set)
    for nm, va, d, ch in secs:
        if ch & 0x20:
            for m in re.finditer(rb'[\xe8\xe9]', d):
                p = m.start()
                if p + 5 > len(d): break
                site = va + p; tgt = (site + 5 + struct.unpack_from('<i', d, p + 1)[0]) & 0xffffffff
                if tgt in T:
                    rw = row(site)
                    if rw: refs[tgt].add(rw)
        for t in T:
            for m in re.finditer(re.escape(struct.pack('<I', t)), d):
                rw = row(va + m.start())
                if rw: refs[t].add(rw)
    out = open(W / 'census.tsv', 'a')
    for t, i in sorted(T.items(), key=lambda x: x[1]):
        a = '%08x' % t; print('%d\t%s\t%s' % (i, a, ','.join(sorted(refs[t] - {a}))), file=out)
    out.close()

def access(L, n, meth):
    own = L[n - 1].split('//')[0]; k = own.find(meth)
    labs = re.findall(r'\b(public|private|protected)\s*:', own[:k]) if k >= 0 else []
    if labs: return labs[-1] + '@same'
    for i in range(n - 2, -1, -1):
        c = L[i].split('//')[0]
        labs = re.findall(r'\b(public|private|protected)\s*:', c)
        if labs: return labs[-1] + '@L%d' % (i + 1)
        m = re.match(r'^\s*(class|struct)\s+(\w+)', c)
        if m and '{' in c and not c.rstrip().endswith(';'): return '%s-default(L%d %s)' % ('public' if m.group(1) == 'struct' else 'private', i + 1, m.group(2))
    return '?'
def cmd_scen(idxs):
    E = entries(); C = {}
    for l in open(W / 'census.tsv'):
        p = l.rstrip('\n').split('\t'); C[int(p[0])] = [x for x in p[2].split(',') if x]
    for i in idxs:
        e = E[i - 1]; pdb = e[6]; m = re.match(r'\?\?0(\w+)@', pdb); meth = m.group(1) if m else re.match(r'\?(\w+)@', pdb).group(1)
        print('#%d %s %s %s want=%s' % (i, e[0], e[4], meth, pdb.split('@@')[1][:3]))
        for r in [e[0]] + C.get(i, []):
            p = src_of(r)
            if not p or not p.exists(): print('  %s NOSRC' % r); continue
            L = p.read_text(encoding='utf-8', errors='replace').split('\n')
            lf = [l for l in L if l.startswith('// Leaf:')]; dec = []; df = []
            for n, l in enumerate(L, 1):
                cc = l.split('//')[0]
                if l.lstrip().startswith('//'): continue
                if re.search(r'::\s*%s\s*\(' % re.escape(meth), cc) and not cc.rstrip().endswith(';') and not re.search(r'(->|\.)\s*%s' % re.escape(meth), cc): df.append('L%d' % n)
                elif re.search(r'[\w>*&]\s+%s\s*\(' % re.escape(meth), cc) and not re.search(r'\b(return|if|while)\b|=', cc.split(meth)[0]):
                    dec.append('L%d[%s]%s%s' % (n, access(L, n, meth), 'C' if re.search(r'\)\s*const', cc) else '', 'V' if 'virtual' in cc.split(meth)[0] else ''))
            hd = []
            if not dec:
                for l in L:
                    mm = re.match(r'#include\s+"([^"]+)"', l)
                    if mm:
                        hp = R / 'rebuild/include' / mm.group(1)
                        if hp.exists():
                            HL = hp.read_text(encoding='utf-8', errors='replace').split('\n')
                            for k, hl in enumerate(HL, 1):
                                c2 = hl.split('//')[0]
                                if re.search(r'[\w>*&]\s+%s\s*\(' % re.escape(meth), c2) and '::' not in c2.split(meth)[0][-3:]:
                                    hd.append('%s:L%d[%s]%s%s' % (mm.group(1).split('/')[-1], k, access(HL, k, meth), 'C' if re.search(r'\)\s*const', c2) else '', 'V' if 'virtual' in c2 else ''))
            print('  %s%s lock=%s leaf=%s DECL=%s DEF=%s %s' % (r, '(self)' if r == e[0] else '', lock_owner(r), (lf[0][9:9 + len(pdb) + 2] if lf else '-'), ','.join(dec) or '-', ','.join(df) or '-', ('HDR=' + ','.join(hd[:4])) if hd else ''))

def cmd_pre(label, f, nowchar=False):
    cmd = ['clang++', '-target', 'i686-pc-windows-msvc', '-fms-extensions', '-fms-compatibility', '-fms-compatibility-version=13.10', '-fsyntax-only', '-w',
           '-Wno-address-of-temporary', '-ferror-limit=500'] + (['-Xclang', '-fno-wchar'] if nowchar else []) + (['-I' + x for x in os.environ['NF_TREE'].split(':') if x] if os.environ.get('NF_TREE') else ['-I' + str(A / 'hdr'), '-I' + str(R / 'rebuild/include')]) + ['-isystem', str(S / 'wine/vc71/include'), f]
    try: r = subprocess.run(cmd, capture_output=True, text=True, timeout=90, cwd=str(W)); t = r.stdout + r.stderr
    except subprocess.TimeoutExpired: t = 'x: error: TIMEOUT'
    errs = [l for l in t.splitlines() if re.search(r': (fatal )?error', l)]
    def where(l):
        p = re.sub(r':\d+:\d+: .*', '', l)
        if p == f: return 'OWN'
        if '/wine/vc71/include/' in p: return 'VC71:' + p.split('/wine/vc71/include/')[1]
        if '/rebuild/include/' in p: return 'INC:' + p.split('/rebuild/include/')[1]
        return p
    c = collections.Counter(where(l) for l in errs)
    line = '%s %s %s%s errors=%d [%s] %s' % (__import__('time').strftime('%H:%M:%S'), label, 'PASS' if not errs else 'FAIL', ' NOWCHAR' if nowchar else '', len(errs), ', '.join('%d %s' % (v, k) for k, v in c.items()), f)
    open(W / 'pre.log', 'a').write(line + '\n')
    if errs: open(W / ('pre_err_%s.txt' % label), 'w').write('\n'.join(errs) + '\n')
    print(line)

def cmd_zs(tag, f, kind, extra):
    import verify_and_land as V
    wd = W / 'zs' / tag; wd.mkdir(parents=True, exist_ok=True)
    sp = wd / (tag + '.cpp'); sp.write_text(V.vc71(Path(f).read_text(encoding='utf-8')), encoding='utf-8')
    fl = ['/Od', '/W3'] if kind == 'test' else V.BASE_FLAGS + extra
    cp = V.cl(['/nologo', '/Zs'] + fl + [str(sp)], V.env()); t = (cp.stdout or '') + (cp.stderr or '')
    errs = [l for l in t.splitlines() if ' error ' in l or 'fatal error' in l]
    line = '%s %s %s %s %s %s' % (__import__('time').strftime('%H:%M:%S'), 'ZS_PASS' if cp.returncode == 0 and not errs else 'ZS_FAIL rc=%d' % cp.returncode, tag, kind, f, ' '.join(fl))
    open(W / 'pre_zs.log', 'a').write(line + '\n' + ''.join('   ' + l[-250:] + '\n' for l in errs[:5]))
    subprocess.run(['python3', str(R / 'tools/decomp_pipeline/linux_lane/lane_tools/naming_batch/clean.py'), str(A), str(wd)], capture_output=True); print(line)   # NF323: clean.py, not rmtree

def cmd_pay(prefix, addrs):
    out = []; orc = []; cat = catalog(); hdr = open(R / 'rebuild/oracles/auto-re-candidates.tsv', encoding='utf-8').readline().rstrip('\n')
    lines = {l.split('\t')[0]: l.rstrip('\n') for l in open(R / 'rebuild/oracles/auto-re-candidates.tsv', encoding='utf-8')}
    for a in addrs:
        rd = Path(os.environ.get('ROWDIR', str(A / 'rows')))   # NF352: ROWDIR picks another row-text folder (the r1 ruling texts)
        sp = rd / (a + '.cpp'); tp = rd / (a + '_test.cpp')
        head = os.environ.get('HEADPAY')   # NF341: HEADPAY=1 builds the HEAD (landed) payload
        s = (sp if sp.exists() and not head else src_of(a)).read_text(encoding='utf-8'); t = (tp if tp.exists() and not head else test_of(a)).read_text(encoding='utf-8')
        nm = cat[a]['name']; d = 0; cut = 0
        for k, ch in enumerate(nm):
            if ch == '<': d += 1
            elif ch == '>': d -= 1
            elif ch == ':' and d == 0 and nm[k:k + 2] == '::': cut = k + 2
        lf = re.search(r'// Leaf:\s*(\S+)', s)
        out.append(dict(address=a, module=field(a, 'Module'), name=nm[cut:], source_cpp=s, test_cpp=t, pass_pattern=field(a, 'PassPattern'),
                        flags=flags_of(a, s), leaf=lf.group(1) if lf else nm[cut:]))
        orc.append(lines[a])
    json.dump({'result': {'authored': out}}, open(prefix + '_payload.json', 'w'), indent=1)
    open(prefix + '_oracle.tsv', 'w').write(hdr + '\n' + '\n'.join(orc) + '\n')

if __name__ == '__main__':
    c = sys.argv[1]; a = sys.argv[2:]
    if c == 'census': cmd_census([int(x) for x in a])
    elif c == 'scen': cmd_scen([int(x) for x in a])
    elif c == 'pre': cmd_pre(a[0], a[1], len(a) > 2)
    elif c == 'zs': cmd_zs(a[0], a[1], a[2], a[3:])
    elif c == 'grade': cmd_grade(a[0], a[1:])
    elif c == 'hgrade': cmd_grade(a[0], a[1:], head_only=True)
    elif c == 'proof': cmd_proof(a[0], a[1:])
    elif c == 'link': cmd_link(a[0], a[1], a[2:])
    elif c == 'xr': cmd_xr(a[0], a[1])
    elif c == 'syms': print('\n'.join(sorted(ext_syms(a[0]))))
    elif c == 'pay': cmd_pay(a[0], a[1:])
