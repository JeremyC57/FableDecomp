#!/usr/bin/env python3
"""pk_all.py <row.obj> <row-address> [--leaf=SUBSTR] [--funclet] [--no-leaf-relocs] [--tsv] [--row-length=N] [--addrs=FILE]

All-copies pick-any checker (agent NF340, 2026-11-24). Compares EVERY function the row object defines (other than the row's own
leaf) with Fable.exe at that function's retail address, relocation fields masked on both sides (pickany_vs_retail.py's masking).

Address sources, per copy symbol:
  a   the row's own leaf relocations: reloc_identity.check (the grader's own symbol -> retail-target mapping, the one
      verify_and_land / vland use) on the leaf graded against the retail row bytes. Only available when the leaf is exactly the
      retail length (MATCH / RELOCATION_MATCH); the grader's mapping is authoritative for this row.
  a2  transitively, the relocations of a copy already proven EQ (same formula as `a`, applied to that copy at its retail address).
  b   rebuild/oracles/reloc_bindings.tsv (every target listed for the symbol).
  c   rebuild/oracles/self_symbols.tsv (the graded symbol of each landed row; last line per address wins).
  o   --addrs=FILE (symbol<TAB>hex address lines): caller-given addresses, authoritative like a (controls whose leaf no longer
      grades, e.g. the same row recompiled without its /d2inl flag, with the addresses its flagged build mapped).
TU-local symbols (storage class 3, and anonymous-namespace `?A0x` names) are resolved by a / a2 only: their names are per TU.
When a / a2 give an address, b / c addresses that differ are reported as ADDR_CONFLICT (not compared as NEQ).

Output, one line per (copy, address): verdict  status  address  sources  kind  size  leafref  reach  symbol
  status  of this address: EQ / NEQ / NOADDR / NOREAD / ADDR_CONFLICT(EQ|NEQ) (a b / c address other than the a / a2 one)
  verdict of the copy: the a / a2 address decides (EQ only if every such address is EQ); with none, EQ if any b / c address is EQ
          (EQ_AMBIG when several table addresses disagree: a COMDAT has one retail address, the others are stand-in or
          mis-graded table bindings), else NEQ (one address) or NEQ_AMBIG (several); NOADDR; SELF (only the row's own address)
  catalog_len  the catalog (auto-re-candidates.tsv) length of the retail address, '-' when it is not a landed row
  kind    pickany (COMDAT select-any) / strong (COMDAT nodup or non-COMDAT extern) / comdat<N> / static (storage class 3)
  leafref y = the leaf relocates directly to the symbol; n = it does not; ? = leaf unknown (funclet / no leaf)
  reach   y = reachable from the leaf through relocations of other copies (direct included)
Exit 0. With --tsv the same fields are tab-separated (no header).
Run with python3 -I -B (reads Fable.exe; imports verify_and_land / reloc_identity read-only)."""
import os, re, struct, sys, csv, collections, contextlib, io

ROOT = os.environ.get('FABLE_ROOT', '/home/user/FableDecomp')
sys.path.insert(0, os.path.join(ROOT, 'tools', 'decomp_pipeline'))
sys.dont_write_bytecode = True
import verify_and_land as V          # noqa: E402  (obj_text: the grader's leaf choice)
import reloc_identity as R           # noqa: E402  (check: the grader's relocation -> retail target mapping)

IMAGE_SCN_CNT_CODE = 0x20
IMAGE_SCN_LNK_COMDAT = 0x1000
REL_DIR32, REL_REL32 = 0x06, 0x14

# ---------------------------------------------------------------- retail image
_EXE = None
def exe():
    global _EXE
    if _EXE is None:
        p = os.environ.get('FABLE_EXE'); d = open(p, 'rb').read()
        pe = struct.unpack_from('<I', d, 0x3c)[0]; ns = struct.unpack_from('<H', d, pe + 6)[0]; opt = struct.unpack_from('<H', d, pe + 20)[0]
        base = struct.unpack_from('<I', d, pe + 24 + 28)[0]; xs = []
        for k in range(ns):
            vs, va, rs, rp = struct.unpack_from('<IIII', d, pe + 24 + opt + 40 * k + 8); xs.append((va, max(vs, rs), rp, rs))
        _EXE = (d, base, xs)
    return _EXE

def rd(va, n):
    d, base, xs = exe(); rva = va - base
    for s, sz, rp, rs in xs:
        if s <= rva and rva + n <= s + sz:
            o = rva - s
            b = d[rp + o:rp + min(o + n, rs)] if o < rs else b''
            return b + b'\0' * (n - len(b))
    return None

# ---------------------------------------------------------------- COFF object
def coff(path):
    d = open(path, 'rb').read()
    _, nsec, _, symp, nsym, opt, _ = struct.unpack_from('<HHIIIHH', d, 0)
    strt = symp + nsym * 18
    def nm(o):
        r = d[o:o + 8]
        if r[:4] == b'\0\0\0\0':
            off = struct.unpack_from('<I', r, 4)[0]; e = d.index(b'\0', strt + off); return d[strt + off:e].decode('latin1')
        return r.rstrip(b'\0').decode('latin1')
    secs = [None]
    for k in range(nsec):
        s = struct.unpack_from('<8sIIIIIIHHI', d, 20 + opt + k * 40)
        secs.append(dict(name=s[0].rstrip(b'\0').decode('latin1'), size=s[3], ptr=s[4], rel=s[5], nrel=s[7], flags=s[9], sel=None))
    syms = {}; i = 0
    while i < nsym:
        o = symp + i * 18; val, sec, typ, cls, aux = struct.unpack_from('<IhHBB', d, o + 8); name = nm(o)
        syms[i] = dict(name=name, val=val, sec=sec, typ=typ, cls=cls)
        if cls == 3 and aux and val == 0 and 0 < sec < len(secs) and name == secs[sec]['name'] and secs[sec]['sel'] is None:
            a = symp + (i + 1) * 18
            secs[sec]['sel'] = d[a + 14]          # section-definition aux: COMDAT selection
        i += 1 + aux
    def raw(secno):
        s = secs[secno]
        return bytes(d[s['ptr']:s['ptr'] + s['size']]) if s['ptr'] else b'\0' * s['size']
    def relocs(secno):
        s = secs[secno]; out = []
        for k in range(s['nrel']):
            off, si, ty = struct.unpack_from('<IIH', d, s['rel'] + k * 10)
            out.append((off, syms.get(si, {}).get('name', '?'), ty))
        return out
    return syms, secs, raw, relocs

def functions(syms, secs):
    """Function symbols defined in code sections: name -> (sec, start, end, cls)."""
    fs = [s for s in syms.values() if s['sec'] > 0 and s['typ'] == 0x20 and s['cls'] in (2, 3)
          and secs[s['sec']]['flags'] & IMAGE_SCN_CNT_CODE]
    bysec = collections.defaultdict(list)
    for s in fs: bysec[s['sec']].append(s['val'])
    out = {}
    for s in fs:
        later = [v for v in bysec[s['sec']] if v > s['val']]
        out[s['name']] = (s['sec'], s['val'], min(later) if later else secs[s['sec']]['size'], s['cls'])
    return out

def kind_of(secs, sec, cls):
    if cls == 3: return 'static'
    f = secs[sec]['flags']
    if not f & IMAGE_SCN_LNK_COMDAT: return 'strong'
    sel = secs[sec]['sel']
    return {2: 'pickany', 1: 'strong'}.get(sel, f'comdat{sel}')

def tu_local(name, cls):
    return cls == 3 or '?A0x' in name

# ---------------------------------------------------------------- tables
_TAB = None
def tables():
    global _TAB
    if _TAB is None:
        b = collections.defaultdict(set)
        for r in csv.DictReader(open(os.path.join(ROOT, 'rebuild/oracles/reloc_bindings.tsv'), encoding='utf-8', errors='replace'), delimiter='\t'):
            if r.get('symbol') and r.get('target'):
                try: b[r['symbol']].add(int(r['target'], 16))
                except ValueError: pass
        last = {}
        for l in open(os.path.join(ROOT, 'rebuild/oracles/self_symbols.tsv'), encoding='utf-8', errors='replace').read().splitlines()[1:]:
            f = l.split('\t')
            if len(f) >= 2: last[f[0].lower()] = f[1]
        c = collections.defaultdict(set)
        for a, s in last.items():
            if s:
                try: c[s].add(int(a, 16))
                except ValueError: pass
        cat = {}
        for l in open(os.path.join(ROOT, 'rebuild/oracles/auto-re-candidates.tsv'), encoding='utf-8', errors='replace').read().splitlines()[1:]:
            f = l.split('\t')
            if len(f) >= 3:
                try: cat[f[0].lower()] = int(f[2])
                except ValueError: pass
        _TAB = (b, c, cat)
    return _TAB

# ---------------------------------------------------------------- core
def target_of(kind_reloc, at, off, retail, built):
    """Retail target of a relocation at `off` of a function placed at `at` (reloc_identity.check's formula)."""
    if off + 4 > len(retail) or off + 4 > len(built): return None
    if kind_reloc == REL_REL32:
        return (at + off + 4 + struct.unpack_from('<i', retail, off)[0] - struct.unpack_from('<i', built, off)[0]) & 0xFFFFFFFF
    if kind_reloc == REL_DIR32:
        return (struct.unpack_from('<I', retail, off)[0] - struct.unpack_from('<I', built, off)[0]) & 0xFFFFFFFF
    return None

def check(obj, row, leaf=None, funclet=False, use_leaf_relocs=True, row_length=None, overrides=None):
    """Return dict(leaf=symbol|None, leaf_note=str, lines=[dict(...)])."""
    row = int(str(row), 16) if isinstance(row, str) else row
    syms, secs, raw, relocs = coff(obj)
    fns = functions(syms, secs)
    btab, ctab, cat = tables()
    note = []
    # 1. the grader's leaf
    leafsym = None; leafbuilt = None
    if not funclet:
        try:
            with contextlib.redirect_stderr(io.StringIO()):
                leafbuilt, _, leafsym = V.obj_text(obj, leaf)
        except Exception as ex:
            note.append(f'no leaf ({type(ex).__name__})')
    else:
        note.append('funclet row: leaf is a $L slice, source a unavailable')
    leafsec = fns[leafsym][0] if leafsym in fns else None
    if leafsym and leafsym not in fns: note.append('leaf symbol not a function symbol'); leafsym = None
    # 2. source a: the grader's mapping of the leaf's relocations
    addr = collections.defaultdict(dict)         # symbol -> {address: set(sources)}
    def add(sym, t, src): addr[sym].setdefault(t, set()).add(src)
    leaf_ok = False
    if leafsym and use_leaf_relocs:
        n = row_length or tables()[2].get(f'{row:08x}')
        if not n: note.append('row length unknown: source a unavailable')
        else:
            retail = rd(row, n)
            if retail is None: note.append('row bytes unreadable')
            else:
                hard, warn, pairs = R.check(ROOT, V.OBJDUMP, obj, leafsym, f'{row:08x}', retail, built=leafbuilt)
                if any(w.startswith('RELOC_UNCHECKED') for w in warn):
                    note.append(f'source a unavailable: reloc_identity RELOC_UNCHECKED (leaf {len(leafbuilt)} bytes v retail {n}; no single object block of the retail length)')
                else:
                    leaf_ok = True
                    for off, k, s, t in pairs:
                        if s in fns: add(s, t, 'a')
                    if hard: note.append('leaf RELOC hard: ' + '; '.join(hard)[:300])
    # 2b. source o: caller-given addresses (symbol -> address), authoritative like a (used for controls whose leaf no longer grades)
    for s, t in (overrides or {}).items():
        if s in fns: add(s, t, 'o')
    # 3. sources b, c (never for TU-local names)
    for s, (sec, a0, a1, cls) in fns.items():
        if s == leafsym or sec == leafsec: continue
        if tu_local(s, cls): continue
        for t in btab.get(s, ()): add(s, t, 'b')
        for t in ctab.get(s, ()): add(s, t, 'c')
    # 4. leaf reference sets
    leafrefs = set()
    if leafsec is not None:
        lsec, l0, l1, _ = fns[leafsym]
        leafrefs = {n for o, n, _ in relocs(lsec) if l0 <= o < l1}
    graph = {}
    for s, (sec, a0, a1, cls) in fns.items():
        graph[s] = {n for o, n, _ in relocs(sec) if a0 <= o < a1 and n in fns}
    reach = set(); stack = [n for n in leafrefs if n in fns]
    while stack:
        x = stack.pop()
        if x in reach or x == leafsym: continue
        reach.add(x); stack.extend(graph.get(x, ()))
    # 5. compare (iterate: EQ copies feed source a2)
    def compare(s, t):
        sec, a0, a1, cls = fns[s]; body = raw(sec)[a0:a1]
        r = rd(t, len(body))
        if r is None: return 'NOREAD', body
        rels = [o - a0 for o, _, _ in relocs(sec) if a0 <= o < a1]
        return ('EQ' if V.mask(r, rels) == V.mask(body, rels) else 'NEQ'), body
    results = {}
    changed = True
    while changed:
        changed = False
        for s in list(addr):
            if s == leafsym or fns[s][0] == leafsec: continue
            for t in list(addr[s]):
                if t == row or (s, t) in results: continue
                st, body = compare(s, t); results[(s, t)] = st
                if st == 'EQ':
                    sec, a0, a1, cls = fns[s]; r = rd(t, len(body))
                    for o, n, ty in relocs(sec):
                        if not (a0 <= o < a1) or n not in fns or n == leafsym or fns[n][0] == leafsec: continue
                        tt = target_of(ty, t, o - a0, r, body)
                        if tt is not None and tt not in addr[n]:
                            add(n, tt, 'a2'); changed = True
                        elif tt is not None: addr[n][tt].add('a2')
    # 6. lines
    lines = []
    for s, (sec, a0, a1, cls) in sorted(fns.items(), key=lambda kv: (kv[1][0], kv[1][1])):
        if s == leafsym or sec == leafsec: continue
        k = kind_of(secs, sec, cls)
        lr = '?' if leafsec is None else ('y' if s in leafrefs else 'n')
        rc = '?' if leafsec is None else ('y' if s in reach else 'n')
        ts = {t: srcs for t, srcs in addr.get(s, {}).items() if t != row}
        if not ts:
            own = row in addr.get(s, {})
            lines.append(dict(status='SELF' if own else 'NOADDR', address='-', sources='-', kind=k, size=a1 - a0, leafref=lr, reach=rc, symbol=s,
                              verdict='SELF' if own else 'NOADDR', catalog_len='-'))
            continue
        auth = {t for t, srcs in ts.items() if srcs & {'a', 'a2', 'o'}}
        sts = {t: (results.get((s, t)) or compare(s, t)[0]) for t in ts}
        # one verdict per copy: the grader-mapped address(es) decide; otherwise any EQ table address wins (a COMDAT has one retail
        # address, so the other table addresses are stand-in / mis-graded bindings); otherwise NEQ, or NEQ_AMBIG over several
        pool = auth or set(ts)
        vs = {sts[t] for t in pool}
        if 'EQ' in vs and auth and vs == {'EQ'}: verdict = 'EQ'
        elif 'EQ' in vs and not auth: verdict = 'EQ' if len(pool) == 1 else 'EQ_AMBIG'
        elif auth: verdict = 'NEQ' if 'NEQ' in vs else sorted(vs)[0]
        elif vs == {'NOREAD'}: verdict = 'NOREAD'
        else: verdict = 'NEQ' if len(pool) == 1 else 'NEQ_AMBIG'
        for t in sorted(ts):
            st = sts[t]
            if auth and t not in auth: st = f'ADDR_CONFLICT({st})'
            lines.append(dict(status=st, address=f'{t:08x}', sources=','.join(sorted(ts[t])), kind=k, size=a1 - a0, leafref=lr, reach=rc,
                              symbol=s, verdict=verdict, catalog_len=str(cat.get(f'{t:08x}', '-'))))
    return dict(leaf=leafsym, leaf_ok=leaf_ok, leaf_note='; '.join(note), lines=lines)

FIELDS = ('verdict', 'status', 'address', 'sources', 'kind', 'size', 'catalog_len', 'leafref', 'reach', 'symbol')

def main():
    a = [x for x in sys.argv[1:] if not x.startswith('--')]
    opt = {x.split('=', 1)[0]: (x.split('=', 1)[1] if '=' in x else True) for x in sys.argv[1:] if x.startswith('--')}
    if len(a) != 2: sys.exit(__doc__)
    res = check(a[0], a[1], leaf=opt.get('--leaf'), funclet=bool(opt.get('--funclet')), use_leaf_relocs=not opt.get('--no-leaf-relocs'),
                row_length=int(opt['--row-length']) if opt.get('--row-length') else None,
                overrides={l.split('\t')[0]: int(l.split('\t')[1], 16) for l in open(opt['--addrs']).read().splitlines() if '\t' in l}
                if opt.get('--addrs') else None)
    print(f'# leaf {res["leaf"]}  source_a={"yes" if res["leaf_ok"] else "no"}  {res["leaf_note"]}')
    for l in res['lines']:
        if opt.get('--tsv'): print('\t'.join(str(l[f]) for f in FIELDS))
        else: print(f'{l["verdict"]:<9} {l["status"]:<18} {l["address"]:>8} {l["sources"]:<8} {l["kind"]:<8} {l["size"]:>5} leaf={l["leafref"]} reach={l["reach"]} {l["symbol"]}')

if __name__ == '__main__':
    main()
