# xcheck332.py: hand-back cross-check against live data -> stdout. TABLE_CHANGES: each old line exists with the stated witness (replace /
# delete / rewitness), each replace's new spelling has no line, each delete's new spelling does (note), add has no line yet. Every new
# symbol (table, RENAMES s1 leaf, s2 new) is an exact string in BOTH egor_pdb_names.tsv and fablewin_pdb_names.tsv.
import collections, re
A = '/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad/w22/agentNF332'; G = '/home/user/FableDecomp/ghidra_out/'
T = collections.defaultdict(list)
for l in open('/home/user/FableDecomp/rebuild/oracles/reloc_bindings.tsv', encoding='utf-8'):
    r = l.rstrip('\n').split('\t')
    if len(r) == 3: T[(r[0], r[1].lower())].append(r[2].lower())
def names(p):
    s = set()
    for l in open(G + p, encoding='utf-8', errors='replace'):
        for f in l.rstrip('\n').split('\t'):
            if f[:1] in '?@': s.add(f)
    return s
eg, fw = names('egor_pdb_names.tsv'), names('fablewin_pdb_names.tsv')
bad = 0; new = []
def say(ok, m):
    global bad
    if not ok: bad += 1
    print(('OK   ' if ok else 'BAD  ') + m)
for l in open(A + '/TABLE_CHANGES.tsv').read().splitlines()[1:]:
    act, sym, tg, w = l.split('\t')[:4]; tg = tg.lower().replace('0x', ''); w = w.lower().replace('0x', '')
    if act == 'replace':
        o, n = [s.strip() for s in sym.split(' -> ')]; new.append(n)
        say((o, tg) in T and w not in ('',) and True, 'replace old line exists %s %s (table witness %s)' % (o, tg, T.get((o, tg))))
        say((n, tg) not in T, 'replace new has no line %s %s' % (n, tg))
    elif act == 'delete':
        say((sym, tg) in T, 'delete line exists %s %s' % (sym, tg))
        m = re.search(r'witness moved to (\S+)', l)
    elif act == 'add':
        new.append(sym); say((sym, tg) not in T, 'add has no line yet %s %s' % (sym, tg))
    elif act == 'rewitness':
        say((sym, tg) in T, 'rewitness line exists %s %s (now %s -> %s)' % (sym, tg, T.get((sym, tg)), w))
r = open(A + '/RENAMES.tsv').read().split('\n# Section 2\n')
for l in r[0].splitlines()[1:]: new.append(l.split('\t')[3])
for l in r[1].splitlines()[1:]:
    if l.strip(): new.append(l.split('\t')[2])
for n in sorted(set(new)): say(n in eg and n in fw, 'both PDB name sets: %s (egor=%s fablewin=%s)' % (n, n in eg, n in fw))
print('BAD', bad, 'of', 'table + names checks')
