# qscan32.py (from qscan31.py) <idx.jsonl> <out.tsv>: per queue row (QUEUE_W32_input.tsv): final symbol, PDB presence, bound spellings at the target,
# rows naming each non-final bound spelling (u / d), own row (landed at the target) and whether it defines / names the final.
import sys, os, re, json, collections
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import inputs
from rows import rows
R = rows(); NM = inputs.names(); BT, BS = inputs.bindings()
U = collections.defaultdict(set); D = collections.defaultdict(set); IX = {}
for l in open(sys.argv[1]):
    r = json.loads(l); IX[r['a']] = r
    for s in r.get('u', []): U[s].add(r['a'])
    for s in r.get('d', []): D[s].add(r['a'])
out = open(sys.argv[2], 'w')
out.write('chunk\ttarget\tfinal\tpdb\tfinal_bound_to\town_row\town_defines_final\tfinal_named_by\twrong_bound\twrong_rows\n')
for i, l in enumerate(open(inputs.W + '/QUEUE_W32_input.tsv')):
    q = l.rstrip('\n').split('\t') + [''] * 5
    ch, addr, cor = q[0], q[1], q[2]
    t = inputs.na(addr) if re.match(r'^(0x)?[0-9a-fA-F]{6,8}$', addr.strip()) else addr
    fin = cor.split()[0] if cor.startswith('?') or cor.startswith('_') else cor
    pdb = ('E' if fin in NM['Ego_r.pdb'] else '-') + ('F' if fin in NM['FableWin.pdb'] else '-')
    fb = BS.get(fin, ('', ''))[0]
    own = t if t in R else ''
    od = ''
    if own and own in IX:
        od = 'Y' if fin in IX[own].get('d', []) else ('err' if 'err' in IX[own] else 'N:' + ' '.join(s for s in IX[own].get('d', []) if not s.startswith('??_C@') and not s.startswith('__real'))[:300])
    named = sorted(U.get(fin, set()) | D.get(fin, set()))
    wb = sorted(s for s in BT.get(t, ()) if s != fin)
    wr = {}
    for s in wb:
        rs = sorted(U.get(s, set()) | D.get(s, set()))
        if rs: wr[s] = rs
    out.write('\t'.join([ch, t, fin, pdb, fb, own, od, ' '.join(named[:40]) + (' ...%d' % len(named) if len(named) > 40 else ''),
                         ' | '.join(wb)[:2000], ' | '.join('%s: %s' % (s, ' '.join(r)) for s, r in wr.items())[:4000]]) + '\n')
