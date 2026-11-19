#!/usr/bin/env python3
"""Grow the native-decomp address list from what runs.

    pick_functions.py --current addresses.txt --report out/report.tsv --matched matched.txt
                      --gen gen_win [--log log.txt ...] [--callee-depth 1] [-o next.txt]

Takes the current list (build_native.sh's input) and adds:
  * every guest function the logs' profiler lines name ("profile: ... guest 0xADDR (n)"),
  * the direct callees (from the lifted code in --gen) of every function already native, to
    --callee-depth levels: a call that stays native skips nd_call's guest-stack bridge,
keeping only addresses that FullDecomp has matched (--matched: one hex address per line, e.g.
from the branch's rebuild/src/compiled/*_<addr>.cpp names) and that report.tsv does not already
show as lowered or as left lifted for a reason the lowering still has. Prints what was added.
"""
import argparse
import collections
import glob
import os
import re


def hexes(path):
    out = []
    for line in open(path):
        line = line.strip().split('\t')[0].split()[0] if line.strip() else ''
        if line:
            out.append('%08X' % int(line, 16))
    return out


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument('--current', required=True)
    ap.add_argument('--report', required=True)
    ap.add_argument('--matched', required=True)
    ap.add_argument('--gen', required=True)
    ap.add_argument('--log', action='append', default=[])
    ap.add_argument('--callee-depth', type=int, default=1)
    ap.add_argument('-o', '--out')
    a = ap.parse_args()

    current = hexes(a.current)
    matched = set(hexes(a.matched))
    native, lifted = set(), {}
    for row in open(a.report).read().splitlines()[1:]:
        f = row.split('\t')
        if len(f) >= 3:
            (native.add(f[0]) if f[2] == 'native' else lifted.__setitem__(f[0], f[3] if len(f) > 3 else ''))

    # profiled guest functions
    seen = collections.Counter()
    for log in a.log:
        for m in re.finditer(r'guest 0x([0-9A-F]{8}) \((\d+)\)', open(log, errors='replace').read()):
            seen[m.group(1)] += int(m.group(2))

    # the lifted code's direct calls, per function
    calls = {}
    head = re.compile(r'^void F_([0-9A-F]{8})(?:_orig)?\(Ctx\* restrict c\) \{')
    call = re.compile(r'\bF_([0-9A-F]{8})\(c\)')
    for path in glob.glob(os.path.join(a.gen, 'recomp_*.c')):
        cur = None
        for line in open(path, errors='replace'):
            m = head.match(line)
            if m:
                cur = m.group(1)
                calls.setdefault(cur, set())
            elif cur:
                for c in call.findall(line):
                    if c != cur:
                        calls[cur].add(c)

    have = set(current) | native
    added = collections.OrderedDict()

    def consider(addr, why):
        if addr in have or addr in added or addr not in matched:
            return
        if addr in lifted:  # tried before; still a reason
            return
        added[addr] = why

    for addr, n in seen.most_common():
        consider(addr, 'profiled (%d samples)' % n)
    frontier = set(native) | set(added)
    for depth in range(a.callee_depth):
        nxt = set()
        for f in sorted(frontier):
            for c in sorted(calls.get(f, ())):
                before = len(added)
                consider(c, 'callee of %s' % f)
                if len(added) > before:
                    nxt.add(c)
        frontier = nxt

    out = current + [x for x in added]
    if a.out:
        with open(a.out, 'w') as w:
            w.write('\n'.join(out) + '\n')
    why = collections.Counter(v.split(' (')[0].split(' of ')[0] for v in added.values())
    print('current %d, added %d (%s) -> %d' % (len(current), len(added), ', '.join('%s %d' % kv for kv in why.items()), len(out)))
    for k, v in list(added.items())[:40]:
        print('  %s  %s' % (k, v))


if __name__ == '__main__':
    main()
