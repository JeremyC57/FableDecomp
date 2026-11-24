# who.py <addr..> [-n name-substring..]: for each retail address: landed self row (file, Leaf, defined name guesses), table spellings, RTTI label,
# and whether it is in work/offlimits.txt. With -n, also grep both PDB name sets for each substring.
import sys, re, collections
sys.path.insert(0, __file__.rsplit('/', 1)[0]); import nf
R = nf.R; A = nf.A
args = sys.argv[1:]; names = []
if '-n' in args: k = args.index('-n'); names = args[k + 1:]; args = args[:k]
off = set(open(A / 'work/offlimits.txt').read().split())
RT = {}
for f in ('labels_rtti_port.tsv', 'labels_rtti_port_dtors.tsv'):
    for l in open(R / 'ghidra_out' / f, encoding='utf-8', errors='replace'):
        p = l.rstrip('\n').split('\t')
        if len(p) > 1: RT.setdefault(p[0].lower(), []).append(p[1])
TAB = collections.defaultdict(list)
for l in open(R / 'rebuild/oracles/reloc_bindings.tsv', encoding='utf-8', errors='replace'):
    p = l.rstrip('\n').split('\t')
    if len(p) == 3: TAB[p[1].lower().replace('0x', '')].append((p[0], p[2]))
for a in args:
    a = a.lower().replace('0x', '')
    s = nf.src_of(a); lf = '-'
    if s and s.exists():
        t = s.read_text(encoding='utf-8', errors='replace'); m = re.search(r"^//\s*Leaf:\s*(\S+)", t, re.M); lf = m.group(1) if m else '-'
    print('%s %s row=%s leaf=%s rtti=%s lock=%s' % (a, 'OFF' if a in off else '', s.name if s else None, lf, RT.get(a, '-'), nf.lock_owner(a)))
    for sym, w in TAB.get(a, []): print('    tab %s  (%s)' % (sym[:200], w))
for n in names:
    for f in ('egor_pdb_names.tsv', 'fablewin_pdb_names.tsv'):
        hits = sorted(set(l.split('\t')[1].strip() for l in open(R / 'ghidra_out' / f, encoding='utf-8', errors='replace') if n in l))
        print('  %s %s: %s' % (f[:6], n, ' | '.join(hits[:8])))
