#!/usr/bin/env python3
"""gatecmp.py <results.jsonl>... [--base=CAT] [--rows=a,b] (agent NF344): NF340 gate, HEAD (variant CAT) v every other variant per row.

Copies are keyed by symbol with `?A0x<hash>` stripped (one compile path per row, so the hashes agree anyway) and by retail address.
Per variant: leaf grade; LEAF-CALLED copies (leafref y or reach y) with their verdicts on both sides.
  BROKE   = leaf-called copy NEQ in the variant that was EQ at HEAD or absent at HEAD (holds the change: NF340 gate)
  REPAIR  = leaf-called copy NEQ at HEAD that is EQ in the variant (or no longer emitted while the leaf still grades)
  DEBT    = leaf-called copy NEQ on both sides
  old checker: counts of EQ / NEQ lines and the NEQ symbols that differ between the sides.
Verdict per variant: LANDABLE (grade holds, no BROKE, some REPAIR or no change at all), HOLD-grade, HOLD-broke, NO-GAIN (grade holds,
no BROKE, no REPAIR, but some copy changed)."""
import json, re, sys
from collections import defaultdict
HASH = re.compile(r'\?A0x[0-9a-f]{8}')
OK = ('MATCH', 'RELOCATION_MATCH')

def key(x): return HASH.sub('?A0x', x['symbol'])

def copies(rec):
    d = {}
    for x in rec.get('copies', []) or []:
        k = key(x)
        if k in d: continue
        d[k] = x
    return d

def leafcalled(x): return x.get('leafref') == 'y' or x.get('reach') == 'y'
def neq(v): return v.startswith('NEQ')

def oldset(rec):
    s = {'EQ': set(), 'NEQ': set()}
    for l in rec.get('old', []) or []:
        f = l.split()
        if len(f) >= 4 and f[0] in ('EQ', 'NEQ'): s[f[0]].add(HASH.sub('?A0x', f[3]) + ('@' + f[1]) + (' [' + ' '.join(f[4:]) + ']' if f[4:] else ''))
    return s

def main():
    files = [a for a in sys.argv[1:] if not a.startswith('--')]
    base = next((a.split('=', 1)[1] for a in sys.argv if a.startswith('--base=')), 'CAT')
    only = next((a.split('=', 1)[1].split(',') for a in sys.argv if a.startswith('--rows=')), None)
    recs = defaultdict(dict)
    for f in files:
        for l in open(f):
            r = json.loads(l); recs[r['row']][r['variant']] = r
    for row in sorted(recs):
        if only and row not in only: continue
        V = recs[row]
        if base not in V: print(row, 'no base'); continue
        h = V[base]; hc = copies(h); ho = oldset(h)
        print('=' * 100); print(f"{row} {base}: {h['grade']} flags={h['flags']}  copies={len(hc)} old EQ/NEQ={len(ho['EQ'])}/{len(ho['NEQ'])}")
        for k in sorted(hc):
            x = hc[k]
            if leafcalled(x) and neq(x['verdict']): print(f"    HEAD leaf-called NEQ: {x['address']} {k[:110]}")
        for vn, n in V.items():
            if vn == base: continue
            nc = copies(n); no = oldset(n)
            gok = n['grade'].startswith(OK)
            broke, rep, debt, gone, other = [], [], [], [], []
            for k in sorted(set(hc) | set(nc)):
                a, b = hc.get(k), nc.get(k)
                if b and leafcalled(b) and neq(b['verdict']):
                    if not a or not neq(a['verdict']): broke.append((k, b['address'], a['verdict'] if a else 'absent'))
                    else: debt.append((k, b['address']))
                elif a and leafcalled(a) and neq(a['verdict']):
                    if b and b['verdict'].startswith('EQ'): rep.append((k, a['address'], 'EQ'))
                    elif not b: gone.append((k, a['address']))
                    else: other.append((k, a['address'], a['verdict'], b['verdict'], 'leafcalled_head=y', 'leafcalled_new=' + ('y' if leafcalled(b) else 'n')))
                elif a and b and a['verdict'] != b['verdict']:
                    other.append((k, b['address'], a['verdict'], b['verdict'], 'lc=' + ('y' if leafcalled(a) else 'n') + ('y' if leafcalled(b) else 'n')))
                elif bool(a) != bool(b):
                    other.append((k, (b or a)['address'], a['verdict'] if a else 'absent', b['verdict'] if b else 'absent', 'lc=' + ('y' if a and leafcalled(a) else 'n') + ('y' if b and leafcalled(b) else 'n')))
            if not gok: verdict = 'HOLD-grade'
            elif broke: verdict = 'HOLD-broke'
            elif rep or gone: verdict = 'LANDABLE(repairs)'
            elif not other and no == ho: verdict = 'SAME-AS-HEAD(copy verdicts unchanged; for NOD = flag unneeded)'
            else: verdict = 'NO-GAIN(copies changed, none leaf-called NEQ)'
            print(f"  -- {vn} [{n['spec']}]: {n['grade']}  copies={len(nc)} old EQ/NEQ={len(no['EQ'])}/{len(no['NEQ'])}  => {verdict}")
            for t in broke: print(f"       BROKE  {t[1]} (head {t[2]}) {t[0][:110]}")
            for t in rep: print(f"       REPAIR {t[1]} {t[0][:110]}")
            for t in gone: print(f"       GONE   {t[1]} (head leaf-called NEQ copy not emitted) {t[0][:100]}")
            for t in debt: print(f"       DEBT   {t[1]} {t[0][:110]}")
            for t in other: print(f"       other  {t[1]} {t[2]}->{t[3]} {' '.join(t[4:])} {t[0][:90]}")
            od = (ho['NEQ'] ^ no['NEQ'])
            if od: print(f"       old-checker NEQ diff: +{sorted(no['NEQ'] - ho['NEQ'])[:3]} -{sorted(ho['NEQ'] - no['NEQ'])[:3]}")

if __name__ == '__main__':
    main()
