cat = {}
for l in open(R + '/rebuild/oracles/auto-re-candidates.tsv', encoding='utf-8'):
    c = l.rstrip('\n').split('\t')
    if len(c) > 1: cat[c[0]] = c[1]
man = {}
for r in csv.DictReader(open(R + '/rebuild/manifest/functions.tsv', encoding='utf-8-sig'), delimiter='\t'):
    m = r['module']; n = r['name']
    man[r['address']] = n if m in ('_global', '_gapscan', '', '_unattributed') or n.startswith(m + '::') else m + '::' + n
pdb = set()
for f in ('ghidra_out/fablewin_pdb_names.tsv', 'ghidra_out/egor_pdb_names.tsv'):
    for l in open(R + '/' + f, errors='replace'):
        c = l.rstrip('\n').split('\t')
        if len(c) > 1: pdb.add(c[1])
PH = re.compile(r"sub_|CVtable_|CUnproven|vfunc_|^\?|_gapscan|^None$|Alloc_CVtable|^_unattributed::|_[0-9a-f]{8}$|^FUN_")
def curname(a): return cat.get(a) or man.get(a) or 'None'
def refs(va): return E.d.count(struct.pack('<I', va))
name_holders = collections.defaultdict(set)
for a, n in cat.items(): name_holders[n].add(a)
def propose(rows, log, addr, newname, ev):
    cur = curname(addr)
    if cur == newname: log.append((addr, 'SAME', '')); return
    if PH.search(cur): act, note = 'RENAME', 'was %s' % cur
    elif cur.replace('NScript::', '') == newname.replace('NScript::', ''): act, note = 'RENAME', 'was %s (namespace spelling)' % cur
    elif newname.replace('NScript::', '').endswith('::' + cur.replace('NScript::', '')): act, note = 'RENAME_QUAL', 'was under-qualified %s' % cur
    else:
        others = name_holders.get(cur, set()) - {addr}
        if others: act, note = 'RENAME_WRONG', 'replaces %s: current name also held by %s' % (cur, ','.join(sorted(others)[:3]))
        else: act, note = 'DISAGREE', 'current %s kept' % cur
    log.append((addr, act, note))
    if act.startswith('RENAME'): rows.append((addr, newname, ev + '; ' + note))
