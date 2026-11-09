#!/usr/bin/env python3
"""pblk.py <c2prio.json> <id>[,<id>...] [--lines lo-hi] [--after EV]: for candidate ids (as c2 numbers them at the time), print every
priority-pass block contribution (blkend events: w, wK, ref cost or live-through) whose lines intersect lo-hi (relative lines),
grouped by pass (a pass = the run of blkend events between two non-block events), with the pass's index and the next event."""
import json, sys
J = json.load(open(sys.argv[1])); ids = set(int(x) for x in sys.argv[2].split(','))
lo, hi = -1, 10**9
if '--lines' in sys.argv:
    lo, hi = map(int, sys.argv[sys.argv.index('--lines') + 1].split('-'))
after = int(sys.argv[sys.argv.index('--after') + 1]) if '--after' in sys.argv else 0
ev = J['functions'][0]['events']
blines = {}
cur = []; npass = 0; inpass = False
def flush(i):
    global cur, npass
    if cur:
        tot = {}
        print(f'--- pass {npass} ending before ev{i} ({ev[i]["e"] if i < len(ev) else "end"} {json.dumps(ev[i])[:120] if i < len(ev) else ""})')
        for b, line in cur: print(line)
        npass += 1
    cur = []
for i, e in enumerate(ev):
    if e['e'] == 'blk':
        blines[e['blk']] = e['lines']; continue
    if e['e'] == 'blkend':
        if i < after: continue
        ls = blines.get(e['blk'], [])
        if ls and (max(ls) < lo or min(ls) > hi): continue
        K = e['wK'] // max(e['w'], 1)
        for r in e['refl']:
            if r[0] in ids: cur.append((e['blk'], f'  ev{i} blk {e["blk"]} lines {min(ls) if ls else "?"}-{max(ls) if ls else "?"} w {e["w"]} K {K}: #{r[0]} ref cost {r[2]} -> +{e["wK"]*r[2]}  {r}'))
        for r in e['live']:
            if r[0] in ids: cur.append((e['blk'], f'  ev{i} blk {e["blk"]} lines {min(ls) if ls else "?"}-{max(ls) if ls else "?"} w {e["w"]} K {K}: #{r[0]} live -> -{e["wK"]}  {r}'))
        continue
    flush(i)
flush(len(ev))
