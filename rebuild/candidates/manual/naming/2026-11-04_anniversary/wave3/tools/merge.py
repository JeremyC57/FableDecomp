import os, re, csv, collections
W = os.path.dirname(os.path.abspath(__file__)) + '/..'; R = '/home/user/FableDecomp'
parts = [('entity', 'labels_entity.tsv'), ('script_alloc', 'labels_scriptalloc.tsv'), ('template', 'labels_templates.tsv'), ('script_reg', 'labels_scriptreg.tsv'), ('base', 'labels_base.tsv')]
landed = set(re.findall(r"Address = '([0-9a-f]{8})'", open(R + '/rebuild/build_candidates.ps1', encoding='utf-8').read()))
high = {l.split('\t')[0]: l.split('\t')[1] for l in open(R + '/rebuild/candidates/manual/naming/2026-11-04_anniversary/labels_high.tsv') if not l.startswith('address')}
rows = {}; conflicts = []; cnt = collections.Counter()
for tag, fn in parts:
    for r in csv.DictReader(open(W + '/' + fn), delimiter='\t'):
        a = r['address']
        if a in rows and rows[a][0] != r['name']: conflicts.append((a, rows[a][0], r['name'])); continue
        if a in high and high[a] != r['name']: conflicts.append((a, 'labels_high ' + high[a], r['name']))
        rows[a] = (r['name'], r['evidence'], tag); cnt[tag] += 1
names = collections.Counter(v[0] for v in rows.values())
dups = [n for n, c in names.items() if c > 1]
with open(W + '/labels_alloc.tsv', 'w') as f:
    f.write('address\tname\tevidence\n')
    for a in sorted(rows):
        n, ev, tag = rows[a]
        f.write('%s\t%s\t%s\n' % (a, n, ev + ('; LANDED row (respell source)' if a in landed else '')))
kinds = collections.Counter()
for a, (n, ev, tag) in rows.items():
    k = 'wrong' if 'replaces ' in ev else 'qualify' if 'under-qualified' in ev else 'placeholder'
    kinds[(tag, k)] += 1
print('rows', len(rows), cnt, 'conflicts', conflicts, 'dup names', dups[:10], len(dups))
print(sorted(kinds.items())); print('landed', sum(1 for a in rows if a in landed))
print('in labels_high', sum(1 for a in rows if a in high))
