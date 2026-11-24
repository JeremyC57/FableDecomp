#!/usr/bin/env python3
"""report.py <outdir> <results.jsonl>... [--control=<control results.jsonl>]
AUDIT.tsv, DEBT_QUEUE.tsv and counts.json from audit_run.py results (agent NF340). Writes no retail bytes.
--control: the /d2inl control run (same rows, /d2inl* dropped, addresses carried over as source o); adds the copy's verdict
without the flag to AUDIT.tsv / DEBT_QUEUE.tsv and counts how many NEQ copies the flag causes."""
import json, sys, collections, re
args = [a for a in sys.argv[1:] if not a.startswith('--')]
ctlp = next((a.split('=', 1)[1] for a in sys.argv[1:] if a.startswith('--control=')), None)
out = args[0]

def load(p):
    seen = {}
    for l in open(p):
        r = json.loads(l); seen[r['row']] = r          # last line per row wins
    return seen
recs = []
for p in args[1:]: recs += list(load(p).values())
ctl = load(ctlp) if ctlp else {}

def old_map(r):
    m = {}
    for l in r.get('old', []):
        f = l.split()
        if len(f) >= 4 and f[0] in ('EQ', 'NEQ'): m[(f[3], f[1].lower())] = f[0]
        elif len(f) >= 3 and f[0] == 'NOREAD': m[(f[2], f[1].lower())] = 'NOREAD'
    return m

def per_copy(r):
    d = collections.OrderedDict()
    for c in r.get('copies', []): d.setdefault(c['symbol'], []).append(c)
    return d

H = ['set', 'row', 'catalog_flags', 'grade', 'source_a', 'copy_symbol', 'kind', 'size', 'retail_address', 'catalog_len', 'address_source',
     'status', 'verdict', 'leaf_referenced', 'reachable_from_leaf', 'old_checker', 'verdict_without_d2inl']
rows = []; debt = []; conf = []
C = collections.defaultdict(collections.Counter)
for r in sorted(recs, key=lambda r: (r['set'], r['row'])):
    s = r['set']; om = old_map(r); C[s]['rows'] += 1
    g = r.get('grade', '?'); C[s]['grade_' + re.sub(r'\(.*', '', g)] += 1
    if r.get('source_a'): C[s]['rows_source_a'] += 1
    if 'funclet' in (r.get('note') or ''): C[s]['rows_funclet'] += 1
    if r.get('old_error'): C[s]['rows_old_checker_error'] += 1
    if r.get('pk_error'): C[s]['rows_pk_error'] += 1
    cc = per_copy(ctl[r['row']]) if r['row'] in ctl else {}
    pc = per_copy(r)
    if any(c['verdict'] != 'NOADDR' for cs in pc.values() for c in cs): C[s]['rows_with_addressed_copy'] += 1
    rowflag = collections.Counter()
    for sym, cs in pc.items():
        c0 = cs[0]; k = 'static' if c0['kind'] == 'static' else 'extern'; v = c0['verdict']
        vctl = cc[sym][0]['verdict'] if sym in cc else '-'
        for c in cs:
            rows.append([s, r['row'], r.get('catalog_flags', ''), g, 'y' if r.get('source_a') else 'n', sym, c['kind'], str(c['size']),
                         c['address'], c.get('catalog_len', '-'), c['sources'], c['status'], v,
                         c['leafref'] if (r.get('source_a') or c['leafref'] == '?') else c['leafref'] + '(leaf-ungraded)',
                         c['reach'], om.get((sym, c['address']), '-'), vctl])
        C[s][f'copies_{k}'] += 1; C[s][f'copies_{k}_{v}'] += 1
        if k == 'extern': C[s][f'copies_extern_kind_{c0["kind"]}'] += 1
        # address-source agreement: grader-mapped (a / a2) v table (b / c)
        auth = {c['address'] for c in cs if set(c['sources'].split(',')) & {'a', 'a2'}}
        tab = {c['address'] for c in cs if set(c['sources'].split(',')) & {'b', 'c'}}
        for c in cs:
            if c['status'].startswith('ADDR_CONFLICT'):
                conf.append([s, r['row'], sym, ','.join(sorted(auth)), c['address'], c['sources'], c['status'], c.get('catalog_len', '-')])
        if auth and tab:
            C[s]['addr_a_and_table'] += 1
            C[s]['addr_a_table_agree' if tab <= auth else ('addr_a_table_partial' if tab & auth else 'addr_a_table_conflict')] += 1
        elif auth: C[s]['addr_a_only'] += 1
        elif tab: C[s]['addr_table_only'] += 1
        if k == 'extern' and v == 'NOADDR': C[s]['NOADDR_' + ('leafref' if c0['leafref'] == 'y' else 'reach_only' if c0['reach'] == 'y' else 'unreached')] += 1
        if k != 'extern' or v not in ('NEQ', 'NEQ_AMBIG'): continue
        lr = c0['leafref'] == 'y' and bool(r.get('source_a'))
        neq_addrs = [c['address'] for c in cs if c['status'] == 'NEQ']
        old_hit = any(om.get((sym, a)) == 'NEQ' for a in neq_addrs)
        C[s]['NEQ'] += 1
        C[s]['NEQ_old_also' if old_hit else 'NEQ_old_missed'] += 1
        if lr:
            C[s]['NEQ_leafref'] += 1; rowflag['leaf'] += 1
            C[s]['NEQ_leafref_old_also' if old_hit else 'NEQ_leafref_old_missed'] += 1
            if '?A0x' in sym: C[s]['NEQ_leafref_tu_local_name'] += 1
            if vctl != '-': C[s][f'NEQ_leafref_without_d2inl_{vctl}'] += 1
            srcs = ','.join(sorted({c['sources'] for c in cs if c['status'] == 'NEQ'}))
            clen = ','.join(c.get('catalog_len', '-') for c in cs if c['status'] == 'NEQ')
            debt.append([s, r['row'], r.get('catalog_flags', ''), sym, ','.join(neq_addrs), srcs, c0['kind'], str(c0['size']), clen,
                         'y' if old_hit else 'n', vctl])
        elif c0['reach'] == 'y': C[s]['NEQ_reach_only'] += 1
        else: C[s]['NEQ_unreferenced_or_table_only'] += 1
        if vctl != '-': C[s][f'NEQ_without_d2inl_{vctl}'] += 1
    # copies the control (no /d2inl) finds NEQ that the flagged build has EQ
    for sym, cs in cc.items():
        if sym in pc and pc[sym][0]['verdict'] == 'EQ' and cs[0]['verdict'] in ('NEQ', 'NEQ_AMBIG'): C[s]['EQ_with_d2inl_but_NEQ_without'] += 1
    if rowflag['leaf']: C[s]['rows_with_NEQ_leafref'] += 1
    C[s]['old_NEQ_lines'] += sum(1 for v in om.values() if v == 'NEQ')
    C[s]['old_EQ_lines'] += sum(1 for v in om.values() if v == 'EQ')
    if any(v == 'NEQ' for v in om.values()): C[s]['rows_old_any_NEQ'] += 1
with open(f'{out}/AUDIT.tsv', 'w') as f:
    f.write('\t'.join(H) + '\n')
    for x in rows: f.write('\t'.join(x) + '\n')
with open(f'{out}/DEBT_QUEUE.tsv', 'w') as f:
    f.write('set\trow\tcatalog_flags\tcopy_symbol\tretail_address\taddress_source\tkind\tcopy_size\tcatalog_len\told_checker_reports_it\tverdict_without_d2inl\n')
    for x in debt: f.write('\t'.join(x) + '\n')
with open(f'{out}/ADDR_CONFLICTS.tsv', 'w') as f:
    f.write('set\trow\tcopy_symbol\tleaf_mapped_address\ttable_address\ttable_source\tstatus_at_table_address\tcatalog_len_at_table_address\n')
    for x in conf: f.write('\t'.join(x) + '\n')
json.dump({k: dict(v) for k, v in C.items()}, open(f'{out}/counts.json', 'w'), indent=1, sort_keys=True)
for k, v in sorted(C.items()):
    print('==', k); [print(f'  {a}: {b}') for a, b in sorted(v.items())]
