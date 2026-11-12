"""combine_wf.py <wf-dir> <out-dir> addr...: merge per-row workflow payloads into one landing payload + oracle.
Normalises address (8 lowercase hex), name (method only), strips trailing int3/nop padding from oracle bytes. Never raises."""
import json, os, sys, csv, re
wf, out = sys.argv[1:3]
os.makedirs(out, exist_ok=True)
rows, orc = [], []
for a in sys.argv[3:]:
    try:
        d = json.load(open(f'{wf}/{a}/payload.json'))['result']
        rs = [r for v in d.values() for r in (v if isinstance(v, list) else [v]) if isinstance(r, dict) and r.get('address')]
        for r in rs:
            r['address'] = r['address'].lower().replace('0x', '').zfill(8)
            if '::' in r['name'] and not r['name'].startswith('?'):
                r['name'] = r['name'].rsplit('::', 1)[1]
            rows.append(r)
        t = open(f'{wf}/{a}/oracle.tsv').read().strip().split('\n')
        for l in t:
            c = l.split('\t')
            if c[0].lower().replace('0x', '').zfill(8) == a:
                b = c[-1].strip().lower()
                n0 = len(b)
                while b.endswith('cc') or b.endswith('90'):
                    b = b[:-2]
                # only strip when the length column says so
                ln = int(c[2]) if len(c) > 3 and c[2].isdigit() else len(b) // 2
                b = c[-1].strip().lower()[:ln * 2]
                orc.append((a, c[1] if len(c) > 3 else r['name'], ln, b))
    except Exception as e:
        print('ERR', a, e)
json.dump({'result': {'authored': rows}}, open(f'{out}/payload.json', 'w'), indent=1)
with open(f'{out}/oracle.tsv', 'w') as f:
    f.write('address\tname\tlength\tbytes\n')
    for a, n, ln, b in orc:
        f.write(f'{a}\t{n}\t{ln}\t{b}\n')
print('rows', len(rows), 'oracle', len(orc))
for r in rows:
    print(' ', r['address'], r['module'], r['name'], r.get('flags', ''))
