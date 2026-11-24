#!/usr/bin/env python3
"""droptest.py <addr> [--h=H] [--n=D] [--addrs=FILE] [--hleaf=SYM (NF356: the HEAD leaf when the self symbol changes)] : drop test for a row change that removes external symbols (agent NF347).
For every external symbol the HEAD object (work/obj/<h>_<addr>.obj) defines or references that the new object (work/obj/<n>_<addr>.obj)
no longer has (DEF- and UND-), report:
  1. reloc_bindings.tsv lines whose witness is this row (a rewitness is needed if the new object no longer names the symbol);
  2. every reloc_bindings.tsv line for the symbol (target, witness) and the landed self row at each target (self_symbols.tsv / catalog);
  3. the HEAD copy's pk_all verdict (sources a / a2 / b / c, plus o = --addrs symbol<TAB>hexaddr lines for known NOADDR addresses).
Prints TSV: kind symbol witness_lines table_targets owner_rows pk_verdict pk_addr pk_sources size. Run with the lane env (python3 -I -B)."""
import sys, os, csv, collections
A = '/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad/w22/agentNF363'
R = '/home/user/FableDecomp'
sys.path.insert(0, A + '/tools'); sys.path.insert(0, R + '/tools/decomp_pipeline/linux_lane/lane_tools/pk_all'); sys.path.insert(0, R + '/tools/decomp_pipeline'); sys.dont_write_bytecode = True
import nf, pk_all
args = [x for x in sys.argv[1:] if not x.startswith('--')]
opt = {x.split('=', 1)[0]: x.split('=', 1)[1] for x in sys.argv[1:] if x.startswith('--') and '=' in x}
a = args[0].lower().replace('0x', ''); ht, nt = opt.get('--h', 'H'), opt.get('--n', 'D')
ov = {l.split('\t')[0]: int(l.split('\t')[1], 16) for l in open(opt['--addrs']).read().splitlines() if '\t' in l} if opt.get('--addrs') else None
H = nf.W / 'obj' / ('%s_%s.obj' % (ht, a)); N = nf.W / 'obj' / ('%s_%s.obj' % (nt, a))
he, ne = nf.ext_syms(H), nf.ext_syms(N)
nnames = {x[4:] for x in ne}
dropped = sorted(x for x in he - ne if x[4:] not in nnames)
tab = collections.defaultdict(list)
for l in open(R + '/rebuild/oracles/reloc_bindings.tsv', encoding='utf-8').read().splitlines()[1:]:
    f = l.split('\t')
    if len(f) >= 3: tab[f[0]].append((f[1].lower(), f[2].lower()))
selfrow = {}
for l in open(R + '/rebuild/oracles/self_symbols.tsv', encoding='utf-8').read().splitlines()[1:]:
    f = l.split('\t')
    if len(f) >= 2: selfrow[f[0].lower().replace('0x', '')] = f[1]
src = open(nf.src_of(a), encoding='utf-8').read()
res = pk_all.check(str(H), a, leaf=opt.get('--hleaf') or nf.leaf_of(src, a), row_length=len(bytes.fromhex(nf.catalog()[a]['bytes'])), overrides=ov)
pk = collections.defaultdict(list)
for l in res['lines']: pk[l['symbol']].append(l)
print('kind\tsymbol\twitness_is_row\ttable(target:witness)\tself_at_target\tpk_verdict\tpk_status@addr(src)\tsize')
for x in dropped:
    k, s = x[:3], x[4:]
    wl = [t for t, w in tab.get(s, []) if w == a]
    tl = ','.join('%s:%s' % tw for tw in tab.get(s, [])) or '-'
    own = ','.join('%s=%s%s' % (t, 'SAME' if selfrow.get(t) == s else 'OTHER', '' if selfrow.get(t) == s else '(' + (selfrow.get(t) or 'none')[:60] + ')')
                   for t in sorted({t for t, w in tab.get(s, [])} | {l['address'] for l in pk.get(s, []) if l['address'] != '-'})) or '-'
    pl = pk.get(s, [])
    v = pl[0]['verdict'] if pl else ('UND' if k == 'UND' else 'none')
    st = ','.join('%s@%s(%s)' % (l['status'], l['address'], l['sources']) for l in pl) or '-'
    sz = pl[0]['size'] if pl else '-'
    print('\t'.join([k, s, ','.join(wl) or '-', tl, own, v, st, str(sz)]))
