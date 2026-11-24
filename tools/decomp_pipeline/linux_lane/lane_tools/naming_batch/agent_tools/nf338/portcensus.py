# portcensus.py <idx..>: NF299 Q4 census form. For every table line (full spelling, reloc_bindings.tsv) of an ENTRIES target that differs
# from the PDB spelling: the witness row's flags (OFF / OE / OW), whether its landed source defines a local STL model (namespace std
# { template ... vector / list / map / deque / pair / allocator }, or a global template vector / pair), the classes of the wrong
# spelling found in neither PDB name set (stand-ins), the source lines naming the method, and the shared headers it includes that declare
# the method name (NF299 Q4: such entries must be opened by hand). -> stdout (tsv)
import sys, re, os
A = os.path.dirname(os.path.dirname(os.path.abspath(__file__))); R = '/home/user/FableDecomp'
sys.path.insert(0, A + '/tools'); import nf
G = R + '/ghidra_out/'
pdbtxt = open(G + 'egor_pdb_names.tsv', encoding='utf-8', errors='replace').read() + open(G + 'fablewin_pdb_names.tsv', encoding='utf-8', errors='replace').read()
off = set(open(A + '/work/offlimits.txt').read().split()); oe = set(open(A + '/work/other_agents_entries.txt').read().split())
ow = set(open(A + '/work/other_agents_witnesses.txt').read().split())
T = {}
for l in open(R + '/rebuild/oracles/reloc_bindings.tsv'):
    p = l.rstrip('\n').split('\t')
    if len(p) == 3: T.setdefault(p[1].lower().replace('0x', ''), []).append((p[0], p[2].lower()))
STL = re.compile(r'^\s*(namespace\s+std\b|template\s*<[^>]*>\s*(class|struct)\s+(vector|list|map|deque|pair|allocator|set)\b)', re.M)
def classes(s): return set(re.findall(r'[VU]([A-Za-z_]\w*)@', s))
def meth(pdb):
    m = re.match(r'\?\?\$(\w+)@|\?(\w+)@', pdb)
    if pdb.startswith('??0'): return re.match(r'\?\?0\?\$(\w+)@|\?\?0(\w+)@', pdb).group(1) or 'ctor'
    if pdb.startswith('??4'): return 'operator='
    if pdb.startswith('??A'): return 'operator['
    if pdb.startswith('??R'): return 'operator('
    return (m.group(1) or m.group(2)) if m else '?'
E = nf.entries()
print('idx\ttarget\twitness\tflags\tlocal_stl\tstandin_classes\tshared_hdr_declaring\tmethod_lines')
for i in map(int, sys.argv[1:]):
    e = E[i - 1]; t = e[0]; pdb = e[6]; mn = meth(pdb)
    for s, w in T.get(t, []):
        if s == pdb: continue
        fl = ','.join(x for x, ss in (('OFF', off), ('OE', oe), ('OW', ow)) if w in ss) or '-'
        src = nf.src_of(w); txt = open(src, encoding='utf-8', errors='replace').read() if src and src.exists() else ''
        stl = 'yes' if STL.search(txt) else 'no'
        bad = sorted(c for c in classes(s) - classes(pdb) if not re.search(r'[VU]%s@' % re.escape(c), pdbtxt))
        hdrs = []
        for h in re.findall(r'#include\s+"([^"]+)"', txt):
            hp = R + '/rebuild/include/' + h
            if os.path.exists(hp) and mn.rstrip('(=[') and re.search(r'\b%s\s*[\(<]' % re.escape(mn.split('operator')[0] or 'operator'), open(hp, errors='replace').read()) and mn[:8] != 'operator': hdrs.append(h)
        lines = [str(k + 1) + ':' + L.strip()[:90] for k, L in enumerate(txt.split('\n')) if mn in L and not L.strip().startswith('//')][:3]
        print('%d\t%s\t%s\t%s\t%s\t%s\t%s\t%s' % (i, t, w, fl, stl, ','.join(bad) or '-', ','.join(hdrs) or '-', ' | '.join(lines)))
