# probesum.py <probe log>...: per class from vprobe2 / vtcmp logs: slot count equal (retail == emitted), every DIFF slot is slot 0 with
# ??_E v ??_G and otherwise the same name (class + access tail), else FAIL. Prints one line per class + totals.
import sys, re
tot = {'OK': 0, 'FAIL': 0}
for f in sys.argv[1:]:
    cur = None; diffs = []
    for l in open(f):
        m = re.match(r'== (\S+) vftable (\S+)', l)
        if m: cur = m.group(1); vt = m.group(2); diffs = []; continue
        if l.startswith('slot') and (' DIFF ' in l or ' NOLABEL ' in l): diffs.append(l.strip())
        m = re.match(r'retail slots (\d+), emitted slots (\d+); SAME (\d+) DIFF (\d+) NOLABEL (\d+)', l)
        if m and cur:
            r, e, s, d, n = map(int, m.groups()); bad = []
            if r != e: bad.append('count %d v %d' % (r, e))
            if n: bad.append('nolabel %d' % n)
            for x in diffs:
                q = re.match(r'slot\s+(\d+) \S+ DIFF\s+ours=(\S+) retail_label=(\S+)', x)
                if not q or q.group(1) != '0' or not (q.group(2).startswith('??_E') and q.group(3).startswith('??_G')) or q.group(2)[4:] != q.group(3)[4:]:
                    bad.append('diff ' + x[:160])
            st = 'FAIL' if bad else 'OK'; tot[st] += 1
            print('%s\t%s\t%s\tretail=%d emitted=%d same=%d slot0_alias=%d\t%s' % (cur, vt, st, r, e, s, d, '; '.join(bad)))
            cur = None
        if 'COMPILE_FAIL' in l: print(l.strip()); tot['FAIL'] += 1
print('TOTAL', tot)
