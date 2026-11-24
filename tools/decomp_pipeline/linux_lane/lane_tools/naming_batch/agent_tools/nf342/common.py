# common.py (NF342): shared loaders for the T3_PORT_QUEUE stale-line sweep. Read-only on the repo.
import os, re, collections, hashlib, glob
R = '/home/user/FableDecomp'
S = '/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad'
A = S + '/w22/agentNF342'
W = A + '/work'
Q = R + '/rebuild/candidates/manual/naming/T3_PORT_QUEUE.tsv'
MANG = re.compile(r'\?[A-Za-z0-9_@$?]+')
HEX = re.compile(r'(?<![0-9A-Za-z_])(?:0x)?(00[0-9a-fA-F]{6})(?![0-9A-Za-z_])')


def norm(a): return a.lower().replace('0x', '').zfill(8)


def queue():
    """[(line_no, sha1_12, fields)] ; line_no 1-based in the file (header = 1)."""
    out = []
    for i, l in enumerate(open(Q, encoding='utf-8'), 1):
        if i == 1: continue
        t = l.rstrip('\n')
        out.append((i, hashlib.sha1(t.encode()).hexdigest()[:12], t.split('\t')))
    return out


_pdb = None
def pdb():
    global _pdb
    if _pdb is None:
        _pdb = set()
        for fn in ('egor_pdb_names.tsv', 'fablewin_pdb_names.tsv'):
            for l in open(R + '/ghidra_out/' + fn, encoding='utf-8', errors='replace'):
                p = l.rstrip('\n').split('\t')
                if len(p) > 1: _pdb.add(p[1])
    return _pdb


_tab = None
def table():
    """target -> [(symbol, witness)]"""
    global _tab
    if _tab is None:
        _tab = collections.defaultdict(list)
        for l in open(R + '/rebuild/oracles/reloc_bindings.tsv', encoding='utf-8', errors='replace'):
            p = l.rstrip('\n').split('\t')
            if len(p) == 3 and p[0] != 'symbol': _tab[norm(p[1])].append((p[0], norm(p[2])))
    return _tab


_ent = None
def landed():
    """addr -> {Address, Module, Source, TestSource, CompilerFlags...} (bounded per block)"""
    global _ent
    if _ent is None:
        _ent = {}
        ps = open(R + '/rebuild/build_candidates.ps1', encoding='utf-8-sig').read()
        for m in re.finditer(r"\[pscustomobject\]@\{(.*?)\n\s*\}", ps, re.S):
            kv = dict(re.findall(r"(\w+)\s*=\s*'([^']*)'", m.group(1)))
            if 'Address' in kv: _ent.setdefault(kv['Address'].lower(), kv)
    return _ent


def src_path(a):
    e = landed().get(a); return (R + '/rebuild/src/compiled/' + e['Source']) if e and e.get('Source') else None


def test_path(a):
    e = landed().get(a); return (R + '/rebuild/tests/' + e['TestSource']) if e and e.get('TestSource') else None


def leaf(a):
    p = src_path(a)
    if not p or not os.path.exists(p): return None
    m = re.search(r'^//\s*Leaf:\s*(\S+)', open(p, encoding='utf-8', errors='replace').read(), re.M)
    return m.group(1) if m else ''


def src_text(a):
    p = src_path(a)
    return open(p, encoding='utf-8', errors='replace').read() if p and os.path.exists(p) else ''


_open = None
def tracker():
    """OPEN.tsv (committed): target -> dict"""
    global _open
    if _open is None:
        _open = {}
        L = open(R + '/rebuild/candidates/manual/naming/status/OPEN.tsv', encoding='utf-8', errors='replace').read().splitlines()
        h = L[0].split('\t')
        for l in L[1:]:
            p = l.split('\t'); _open[norm(p[0])] = dict(zip(h, p))
    return _open


def lock_owner(a):
    p = S + '/w22/nflocks/' + norm(a) + '/owner'
    return open(p).read().strip() if os.path.exists(p) else 'released'


_bat = None
def batches():
    """addr -> [landing / hand-back dirs that name it as a changed row or table change] (sorted). Sources: batches/*/rows.txt, RESULTS.tsv,
    TABLE_CHANGES.tsv (2026-11-20 on; READMEs left out: they name many unchanged addresses); pending/*_handback/RENAMES.tsv, APPLIED.tsv, TABLE_CHANGES*.tsv (2026-11-20 on)"""
    global _bat
    if _bat is None:
        _bat = collections.defaultdict(set)
        pats = [R + '/rebuild/candidates/manual/batches/2026-11-2*/' + f for f in ('rows.txt', 'RESULTS.tsv', 'TABLE_CHANGES.tsv')]
        pats += [R + '/rebuild/candidates/manual/pending/2026-11-2*_handback/' + f for f in ('RENAMES.tsv', 'APPLIED.tsv', 'TABLE_CHANGES*.tsv')]
        for pat in pats:
            for f in glob.glob(pat):
                b = f.split('/')[-2]
                if '/pending/' in f: b = 'pending/' + b
                for l in open(f, encoding='utf-8', errors='replace'):
                    if f.endswith('rows.txt') or f.endswith('RESULTS.tsv') or f.endswith('RENAMES.tsv') or f.endswith('APPLIED.tsv'):
                        t = re.split(r'[\t ]+', l.strip())
                        cand = [t[0]] + (t[1:2] if t and t[0].isdigit() else [])
                        for c in cand:
                            if re.fullmatch(r'(0x)?[0-9a-fA-F]{8}', c): _bat[norm(c)].add(b)
                    else:
                        for h in re.findall(r'(?<![0-9A-Za-z_])(?:0x)?(00[0-9a-fA-F]{6})(?![0-9A-Za-z])', l): _bat[norm(h)].add(b)
        _bat = {k: sorted(v) for k, v in _bat.items()}
    return _bat


def tagnum(tag):
    m = re.match(r'n[fF](\d+)', tag); return int(m.group(1)) if m else None


def later_batches(a, tag):
    """batches naming a that came in after the line's own source tag (nfNNN: landing_nfMMM with MMM > NNN, or a dated dir >= that landing's date)"""
    n = tagnum(tag); out = []
    own_dates = sorted({b.replace('pending/', '')[:10] for bs in batches().values() for b in bs if n is not None and re.search(r'landing_n[fF]0*%d(?!\d)' % n, b)})
    d0 = min(own_dates) if own_dates else '2026-11-22'
    for b in batches().get(a, []):
        if re.search(r'(landing_|pending/[0-9-]+_)%s(_b\w+|_handback)?$' % re.escape(tag.replace('-', '_')), b.replace('-', '_'), re.I): continue   # the line's own landing (flatten-switch1 = landing_flatten_switch1)
        m = re.search(r'(?:landing|pending/[0-9-]+)_n[fF](\d+)', b)
        if n is not None and m: ok = int(m.group(1)) > n
        else: ok = b.replace('pending/', '')[:10] >= d0
        if ok: out.append(b)
    return out
