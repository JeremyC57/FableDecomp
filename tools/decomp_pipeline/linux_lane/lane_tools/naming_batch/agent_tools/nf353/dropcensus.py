# dropcensus.py <batch-rows file> <census-rows file> : cross-batch drop census (agent NF353).
# 1. Union of every symbol a batch row's HEAD object (work/obj/H_<a>.obj) defines and its new object (N_<a>.obj) no longer defines, with
#    the HEAD pk_all verdict / address of that copy (evidence/pk/H_<a>.pkall.tsv; EQ only if the copy was EQ at that address).
# 2. Per dropped symbol, over the census objects OUTSIDE the batches (work/cobj/<c>.obj: landed HEAD sources compiled read-only; batch rows
#    simulated landed with their N objects, which must not define it): definers, and for symbols with a retail address the EQ definers
#    (pk_all.check on the census object with overrides {symbol: address}, relocation-masked body compare at that address);
#    UND referencers outside (census objects) and inside (batch N objects); verdict LEFT_UNDEFINED when something references it and
#    nothing outside defines it. Holes (T3 column-2 rows, file work/holes9.tsv) are counted separately: outside_all and outside_no_holes.
# Writes evidence/drop_census.tsv and evidence/drop_census_definers.tsv. Run with the lane env (FABLE_EXE, FABLE_ROOT).
import sys, os, collections, contextlib, io
A = '/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad/w22/agentNF353'
R = '/home/user/FableDecomp'
sys.path.insert(0, A + '/tools'); sys.path.insert(0, R + '/tools/decomp_pipeline/linux_lane/lane_tools/pk_all'); sys.path.insert(0, R + '/tools/decomp_pipeline')
sys.dont_write_bytecode = True
import nf, pk_all
B = [l.strip() for l in open(sys.argv[1]) if l.strip()]; C = [l.strip() for l in open(sys.argv[2]) if l.strip()]
holes = {l.split('\t')[0] for l in open(A + '/work/holes9.tsv') if l.strip()}
def defs(p): return {x[4:] for x in nf.ext_syms(p) if x.startswith('DEF ')}
def unds(p): return {x[4:] for x in nf.ext_syms(p) if x.startswith('UND ')}
dropped = collections.defaultdict(set); addr = {}; hv = collections.defaultdict(set)
for a in B:
    H, N = A + '/work/obj/H_%s.obj' % a, A + '/work/obj/N_%s.obj' % a
    for s in defs(H) - defs(N): dropped[s].add(a)
    for l in open(A + '/evidence/pk/H_%s.pkall.tsv' % a):
        f = l.rstrip('\n').split('\t')
        if l.startswith('#') or len(f) < 10: continue
        if f[9] in dropped and a in dropped[f[9]] and f[2] not in ('-', ''):
            addr.setdefault(f[9], set()).add(f[2]); hv[f[9]].add('%s@%s' % (f[1], f[2]))
over = {s: int(sorted(addr[s])[0], 16) for s in addr if len(addr[s]) == 1}
cdefs = {}; cunds = {}; ceq = collections.defaultdict(dict)
for c in C:
    p = A + '/work/cobj/%s.obj' % c
    if not os.path.exists(p): continue
    cdefs[c], cunds[c] = defs(p), unds(p)
    want = {s: v for s, v in over.items() if s in cdefs[c]}
    if want:
        src = nf.src_of(c).read_text(encoding='utf-8')
        with contextlib.redirect_stdout(io.StringIO()), contextlib.redirect_stderr(io.StringIO()):
            try:
                res = pk_all.check(p, c, leaf=nf.leaf_of(src, c), row_length=int(nf.catalog()[c]['length']), overrides=want)
            except Exception as ex:
                res = {'lines': []}
        for l in res['lines']:
            if l['symbol'] in want and l['address'] == '%08x' % want[l['symbol']]: ceq[c][l['symbol']] = l['status']
        for s in want: ceq[c].setdefault(s, 'SELF_OR_LEAF' if nf.leaf_of(src, c) in s else 'UNREAD')
nund = {a: unds(A + '/work/obj/N_%s.obj' % a) for a in B}; ndef = {a: defs(A + '/work/obj/N_%s.obj' % a) for a in B}
out = ['symbol\tdropped_by\thead_copy\taddr\tbatch_N_definers\toutside_definers_all\toutside_EQ_all\toutside_definers_no_holes\toutside_EQ_no_holes\tUND_outside\tUND_batch_new\tverdict\tsome_EQ_definers']
det = ['symbol\tcensus_row\thole\tstatus_at_addr']
for s in sorted(dropped):
    d_all = [c for c in cdefs if s in cdefs[c]]; d_nh = [c for c in d_all if c not in holes]
    a_ = '%08x' % over[s] if s in over else '-'
    eq_all = [c for c in d_all if ceq[c].get(s) in ('EQ', 'SELF_OR_LEAF')]; eq_nh = [c for c in eq_all if c not in holes]
    u_out = [c for c in cunds if s in cunds[c]]; u_in = [a for a in B if s in nund[a]]; bd = [a for a in B if s in ndef[a]]
    if s in over:
        v = 'ok' if eq_nh else ('HOLES_ONLY' if eq_all else 'NO_EQ_DEFINER')
    else:
        v = 'ok-noaddr'
    if (u_out or u_in) and not d_all and not bd: v += ',LEFT_UNDEFINED'
    out.append('\t'.join([s, str(len(dropped[s])), ','.join(sorted(hv.get(s, []))) or 'NOADDR', a_, str(len(bd)), str(len(d_all)), str(len(eq_all)) if s in over else '-',
                          str(len(d_nh)), str(len(eq_nh)) if s in over else '-', str(len(u_out)), str(len(u_in)), v, ','.join(eq_nh[:8]) if s in over else ','.join(d_nh[:6])]))
    if s in over:
        for c in d_all: det.append('%s\t%s\t%s\t%s' % (s, c, 'hole' if c in holes else '-', ceq[c].get(s)))
open(A + '/evidence/drop_census.tsv', 'w').write('\n'.join(out) + '\n')
open(A + '/evidence/drop_census_definers.tsv', 'w').write('\n'.join(det) + '\n')
print('census objects', len(cdefs), 'dropped symbols', len(dropped), 'with address', len(over))
for l in out[1:]:
    f = l.split('\t')
    if f[3] != '-' or 'LEFT' in f[11]: print('\t'.join(f[:12])[:400])
