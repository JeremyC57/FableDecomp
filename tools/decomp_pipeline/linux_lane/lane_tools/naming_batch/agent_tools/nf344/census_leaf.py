#!/usr/bin/env python3
"""census_leaf.py <out.tsv> [<summary.txt>]  (agent NF344, 2026-11-24; no compile, read-only on the repo)

Leaf-vs-table census: every landed catalog row (rebuild/build_candidates.ps1 Address + Source) whose own `// Leaf:` symbol (first Leaf line,
the one recheck.py uses) is bound in rebuild/oracles/reloc_bindings.tsv to a DIFFERENT address.

Matching: exact symbol, after stripping `?A0x<8 hex>` hashes and folding `??_E` onto `??_G` (one alias pair). Leaves that are not a full
mangled name (bare names, `_$E` initializers, `Unwind@` funclets, prefixes that equal no table symbol) are counted as
UNRESOLVED_FORM and never guessed; a prefix leaf that is a strict prefix of table symbols is listed apart (PREFIX class) with its targets.

Classes:  ELSEWHERE = bound only at other address(es);  SPLIT = bound at the row's address and at other(s).
Cheap side check (no bytes written out): catalog bytes (auto-re-candidates.tsv) of the row v each other address: IDENTICAL (folded / twin
bodies) or DIFFERENT, plus self_symbols.tsv's graded symbol at the other address and whether the other address is itself a landed row
and what its own Leaf is.
"""
import re, sys, os
from collections import defaultdict
ROOT = os.environ.get('FABLE_ROOT', '/home/user/FableDecomp')
HASH = re.compile(r'\?A0x[0-9a-f]{8}')

_MD = None
def masked(hexs, va):
    """capstone listing with absolute addresses (>= 0x400000) and out-of-body branch targets masked; in-body targets as offsets"""
    global _MD
    import capstone
    if _MD is None: _MD = capstone.Cs(capstone.CS_ARCH_X86, capstone.CS_MODE_32)
    b = bytes.fromhex(hexs); out = []
    for i in _MD.disasm(b, va):
        ops = i.op_str
        if i.mnemonic.startswith(('j', 'call', 'loop')) and ops.startswith('0x'):
            tgt = int(ops, 16); ops = ('+%x' % (tgt - va)) if va <= tgt < va + len(b) else 'EXT'
        else:
            ops = re.sub(r'0x[0-9a-f]+', lambda m: 'ADDR' if int(m.group(0), 16) >= 0x400000 else m.group(0), ops)
        out.append((i.mnemonic, ops))
    return out

def norm(s):
    s = HASH.sub('?A0x', s)
    if s.startswith('??_E'): s = '??_G' + s[4:]
    return s

def main():
    out = sys.argv[1]; summ = sys.argv[2] if len(sys.argv) > 2 else None
    ps = open(os.path.join(ROOT, 'rebuild/build_candidates.ps1'), encoding='utf-8-sig').read()
    ent = {}
    for blk in re.findall(r"\[pscustomobject\]@\{(.*?)\n    \}", ps, re.S):
        kv = dict(re.findall(r"(\w+) = '([^']*)'", blk))
        if 'Address' in kv and 'Source' in kv: ent[kv['Address'].lower().replace('0x', '')] = kv
    tab = defaultdict(lambda: defaultdict(set))       # norm symbol -> target -> witnesses
    rawsym = defaultdict(set)
    for l in open(os.path.join(ROOT, 'rebuild/oracles/reloc_bindings.tsv'), encoding='utf-8').read().splitlines()[1:]:
        f = l.split('\t')
        if len(f) < 3: continue
        n = norm(f[0]); tab[n][f[1].lower()].add(f[2].lower()); rawsym[n].add(f[0])
    selfs = {}
    for l in open(os.path.join(ROOT, 'rebuild/oracles/self_symbols.tsv'), encoding='utf-8').read().splitlines()[1:]:
        f = l.split('\t')
        if len(f) >= 4: selfs[f[0].lower()] = (f[1] or f[3], f[2])
    cat = {}
    import csv; csv.field_size_limit(10 ** 9)
    for l in open(os.path.join(ROOT, 'rebuild/oracles/auto-re-candidates.tsv'), encoding='utf-8').read().splitlines()[1:]:
        f = l.split('\t')
        if len(f) >= 4: cat[f[0].lower()] = (f[1], f[3].lower())
    syms_sorted = sorted(tab.keys())
    import bisect
    leafof = {}
    stats = defaultdict(int); rows = []
    for a, kv in sorted(ent.items()):
        p = os.path.join(ROOT, 'rebuild/src/compiled', kv['Source'])
        try: src = open(p, encoding='utf-8', errors='replace').read()
        except OSError: stats['no_source_file'] += 1; continue
        m = re.search(r"^//\s*Leaf:\s*(\S+)", src, re.M)
        leafof[a] = m.group(1) if m else ''
    def consistent(L, G):
        # the graded symbol must agree with the Leaf text: the same symbol, a mangled prefix of it, or a bare name inside it
        if norm(L) == norm(G) or norm(G).startswith(norm(L)): return True
        return bool(re.fullmatch(r'[A-Za-z_]\w*', L)) and ('?' + L + '@') in G
    for a in sorted(leafof):
        L = leafof[a]; ss = selfs.get(a); G = ss[0] if ss and ss[0] and ss[0].startswith('?') else ''
        stats['rows'] += 1
        if L and norm(L) in tab:
            leaf, n, mt = L, norm(L), 'Leaf:exact'
        elif G and (not L or consistent(L, G)) and not L.startswith('_$E') and 'Unwind@' not in L:
            leaf, n, mt = G, norm(G), ('self_symbols:no-Leaf-line' if not L else 'self_symbols:Leaf-' + ('mangled-unbound' if L.startswith('?') and norm(L) == norm(G) else 'prefix-or-bare'))
            if n not in tab: stats['unbound'] += 1; continue
        elif L and L.startswith('?') and not L.startswith('_$E') and 'Unwind@' not in L and (L.endswith('Z') or L.endswith('A') or L.endswith('B')) and not G:
            stats['unbound_or_unresolved_mangled_no_self_symbol'] += 1; continue
        else:
            stats['unresolved_form' + ('_no_leaf' if not L else '_nonmangled' if not L.startswith('?') else '_prefix') + ('_no_self_symbol' if not G else '_self_symbol_inconsistent')] += 1; continue
        tg = tab[n]; others = sorted(t for t in tg if t != a)
        if not others: stats['bound_here_only'] += 1; continue
        cls = 'SPLIT' if a in tg else 'ELSEWHERE'
        if '?A0x' in n: cls = 'ANON_' + cls      # per-TU anonymous-namespace name: another TU's twin is another function, not a split
        stats[cls] += 1; stats[cls + '_' + mt] += 1
        for t in others:
            rows.append((a, leaf, t, ','.join(sorted(tg[t])), cls, mt))
    with open(out, 'w') as o:
        o.write('row\tleaf\tbound_address\twitness_rows\tclass\tmatch\tbytes_row_v_bound\tbound_addr_landed_row\tbound_addr_own_leaf\t'
                'self_symbols_at_bound\tcatalog_name_row\tcatalog_name_bound\tside\n')
        for a, leaf, t, ws, cls, mt in rows:
            ca = cat.get(a); ct = cat.get(t)
            if ca and ct:
                if ca[1] == ct[1]: bc = 'IDENTICAL'
                elif masked(ca[1], int(a, 16)) == masked(ct[1], int(t, 16)): bc = 'EQUAL_MASKED(len %d)' % (len(ca[1]) // 2)
                else: bc = 'DIFFERENT(len %d v %d)' % (len(ca[1]) // 2, len(ct[1]) // 2)
            else: bc = 'n/a'
            landed = 'y' if t in leafof else 'n'
            ol = leafof.get(t, '-') or '(no Leaf line)'
            ss = selfs.get(t, ('-', '-'))
            eff = ol if ol not in ('-', '(no Leaf line)') else (ss[0] if landed == 'y' and ss[0].startswith('?') else '')
            same_leaf = landed == 'y' and eff != '' and norm(eff) == norm(leaf)
            if cls.startswith('ANON_'): side = 'not a conflict: anonymous-namespace name, the bound address is another TU\'s instance (per-TU ?A0x hash)'
            elif bc == 'IDENTICAL' or bc.startswith('EQUAL_MASKED'): side = 'folded/twin bodies (identical bytes): one symbol may legitimately cover both; research which owns the name'
            elif same_leaf: side = 'two landed rows define the same Leaf (identity claim: hold both as one unit); research'
            elif landed == 'y' and eff and norm(eff) != norm(leaf) and cls.endswith('ELSEWHERE'):
                side = 'research (bound address is a landed row with another Leaf: table line likely stale / binder wrong)'
            else: side = 'research'
            o.write('\t'.join([a, leaf, t, ws, cls, mt, bc, landed, ol, ss[0] or '-', (ca[0] if ca else '-'), (ct[0] if ct else '-'), side]) + '\n')
    s = '\n'.join('%s\t%d' % (k, v) for k, v in sorted(stats.items()))
    print(s)
    if summ: open(summ, 'w').write(s + '\n')

if __name__ == '__main__':
    main()
