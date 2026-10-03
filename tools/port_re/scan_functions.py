import csv, re, bisect, json, collections, os
rows = list(csv.DictReader(open('rebuild/manifest/functions.tsv'), delimiter='\t'))
starts = sorted(int(r['address'], 16) for r in rows)
byaddr = {int(r['address'], 16): r for r in rows}
info = collections.defaultdict(lambda: dict(consts=0, n=0, calls=[], icalls=0, globals=0, fp=0, jmps_out=0, ret=None))
line_re = re.compile(r'^\s*([0-9a-f]+):\s+(\S+)\s*(.*)$')
RDATA_LO, RDATA_HI = 0x122d000, 0x1374000
DATA_LO, DATA_HI = 0x1374000, 0x143f000
cur = None; cur_end = None
for line in open(os.environ.get('FABLE_TEXT_ASM', 'text.asm')):
    m = line_re.match(line)
    if not m: continue
    a = int(m.group(1), 16)
    if cur is None or a >= cur_end or a < cur:
        i = bisect.bisect_right(starts, a) - 1
        if i < 0: continue
        cur = starts[i]; cur_end = starts[i+1] if i+1 < len(starts) else 0x122d000
    d = info[cur]; d['n'] += 1
    op, rest = m.group(2), m.group(3)
    if op == 'call':
        t = re.match(r'0x([0-9a-f]+)\s*$', rest)
        if t and 'PTR' not in rest: d['calls'].append(int(t.group(1), 16))
        else: d['icalls'] += 1
    elif op.startswith('f'): d['fp'] += 1
    if op.startswith('ret'): d['ret'] = rest.strip() or '0'
    for h in re.findall(r'0x([0-9a-f]{6,8})', rest):
        v = int(h, 16)
        if op.startswith('j') or op == 'call': continue
        if DATA_LO <= v < DATA_HI: d['globals'] += 1
        elif RDATA_LO <= v < RDATA_HI: d['consts'] = d.get('consts', 0) + 1
out = []
for a, d in info.items():
    r = byaddr.get(a)
    if not r: continue
    out.append(dict(addr=f"{a:08x}", name=r['name'], module=r['module'], cc=r['calling_convention'],
        params=r['parameter_types'], ret_type=r['return_type'], verified=r['behavior_test']=='PASS',
        parity=r['retail_parity'], n=d['n'], calls=[f"{c:08x}" for c in d['calls']], icalls=d['icalls'],
        globals=d['globals'], consts=d.get('consts',0), fp=d['fp'], ret=d['ret']))
json.dump(out, open(os.environ.get('FABLE_FUNCS_JSON', 'funcs.json'), 'w'))
un = [f for f in out if not f['verified']]
leaf = [f for f in un if not f['calls'] and not f['icalls']]
pure = [f for f in leaf if f['globals'] == 0]
print("functions:", len(out), "unverified:", len(un), "unverified leaf:", len(leaf), "leaf+no globals:", len(pure))
print("unverified instr total:", sum(f['n'] for f in un))
