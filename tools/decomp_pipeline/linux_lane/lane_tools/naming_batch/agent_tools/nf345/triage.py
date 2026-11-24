# triage.py: per ENTRIES line: table lines of the leaf, table lines at the bound address and at the row, the witness rows' retail
# references (rel32 call/jmp or dword) to the row R and to the bound address A, and whether each witness source names the identifier.
import sys, os, re, struct, collections
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__))); import nf
R = '/home/user/FableDecomp'
E = [l.rstrip('\n').split('\t') for l in open(nf.A / 'ENTRIES.tsv')][1:]
T = [l.rstrip('\n').split('\t') for l in open(R + '/rebuild/oracles/reloc_bindings.tsv')][1:]
bysym = collections.defaultdict(list); byt = collections.defaultdict(list)
for s, t, w in T: bysym[s].append((t, w)); byt[t].append((s, w))
cat = nf.catalog(); secs = nf.exe()
def body(a):
    r = cat.get(a); n = int(r['length']) if r and r.get('length', '').isdigit() else 0
    va = int(a, 16)
    for nm, b, d, ch in secs:
        if b <= va < b + len(d): return va, d[va - b: va - b + n]
    return va, b''
def refs(w, targets):
    va, d = body(w); out = collections.Counter()
    for p in range(len(d) - 4):
        if d[p] in (0xE8, 0xE9) and p + 5 <= len(d):
            t = (va + p + 5 + struct.unpack_from('<i', d, p + 1)[0]) & 0xffffffff
            if t in targets: out['%s:rel@%x' % ('%08x' % t, va + p)] += 1
        x = struct.unpack_from('<I', d, p)[0]
        if x in targets: out['%s:abs@%x' % ('%08x' % x, va + p)] += 1
    return sorted(out)
sel = sys.argv[1:]
for e in E:
    r, L, a, ws = e[0], e[1], e[2], e[3].split(',')
    if sel and r not in sel: continue
    m = re.match(r'\?\?[0-9A-Z_]{1,2}(\w+)@', L) or re.match(r'\?(\w+)@', L) or re.match(r'_?(\S+)', L)
    ident = m.group(1)
    print('=' * 100); print('ROW', r, cat.get(r, {}).get('name', '-')[:90]); print(' L ', L[:200]); print(' A ', a, cat.get(a, {}).get('name', '-')[:90])
    print(' table(L):', [(t, w) for t, w in bysym[L]])
    print(' table@A:', [(s[:110], w) for s, w in byt[a]][:8])
    print(' table@R:', [(s[:110], w) for s, w in byt[r]][:8])
    for w in ws:
        sp = nf.src_of(w); txt = open(sp, encoding='utf-8', errors='replace').read() if sp and sp.exists() else ''
        print('  W', w, cat.get(w, {}).get('name', '-')[:70], '| src names ident:', len(re.findall(r'(?<!\w)' + re.escape(ident) + r'(?!\w)', txt)), '| retail refs:', refs(w, {int(r, 16), int(a, 16)})[:6])
