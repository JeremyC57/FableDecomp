"""Build the open EH-framed oracle pool used by next.py from a famscan dump.
Usage (cwd = repo, env S = scratch root):  python3 mk_eh_oracle.py $S/batch/families512.json > $S/batch/eh_open_oracle.tsv
Rows: address, module::name, length, bytes -- bodies whose first bytes are `push -1 ; push offset` (6a ff 68) and that are not in the catalog."""
import json, sys
cat = {l.split('\t')[0].lower() for l in open('rebuild/oracles/auto-re-candidates.tsv')}
print('address\tname\tlength\tbytes')
seen = set()
for shape, fam in json.load(open(sys.argv[1])):
    for a, m, n, l, h in fam:
        if a.lower() in cat or a in seen or not h.lower().startswith('6aff68'):
            continue
        seen.add(a)
        print(f'{a}\t{m}::{n}\t{l}\t{h}')
