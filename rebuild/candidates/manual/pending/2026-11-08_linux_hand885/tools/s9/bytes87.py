#!/usr/bin/env python3
"""bytes87.py <m.obj> <m.asm> <src.cpp>: where ours is longer than retail. Shape-aligns ours/retail (shape.py keys), sums
instruction bytes per hunk, attributes each hunk to the enclosing verb (nearest preceding StartsWith line) and prints the
per-verb byte surplus and the biggest hunks."""
import sys, re, difflib, collections
srcf = sys.argv[3]
sys.argv = [sys.argv[0], sys.argv[1], sys.argv[2], '/dev/null']
src = open('tools/pk/cdiff.py').read()
exec(src[:src.index('sm = difflib')])
exec(open('s8/tools/shape.py').read().split("KO = [k2")[0].split("exec(src[:src.index('sm = difflib')])")[1])
KO = [k2(i) for i in O]; KR = [k2(i) for i in R]
S = open(srcf, encoding='utf-8').read().split('\n')
verb = {}; cur = 'prologue'
for n, l in enumerate(S, 1):
    m = re.search(r'StartsWith\("([^"]+)"\)', l)
    if m and ('if (command' in l or 'else if' in l): cur = m.group(1)
    if l.startswith('    // ---') : pass
    verb[n] = cur
sm2 = difflib.SequenceMatcher(None, KO, KR, autojunk=False)
per = collections.Counter(); hk = []
for tag, a1, a2, b1, b2 in sm2.get_opcodes():
    if tag == 'equal':
        for i, j in zip(range(a1, a2), range(b1, b2)):
            d = O[i].size - R[j].size
            if d:
                ln = lines[i] if i < len(lines) else -1; per[verb.get(ln, '?')] += d
                hk.append((d, 'size', i, ln, f'{O[i].mnemonic} {O[i].op_str} | {R[j].mnemonic} {R[j].op_str}'))
        continue
    ob = sum(i.size for i in O[a1:a2]); rb = sum(i.size for i in R[b1:b2])
    ln = lines[a1] if a1 < len(lines) else -1
    per[verb.get(ln, '?')] += ob - rb
    hk.append((ob - rb, tag, a1, ln, f'{a2-a1}v{b2-b1} ins @{R[b1].address if b1 < len(R) else 0:08x}'))
print('total ours-retail bytes:', sum(i.size for i in O) - sum(i.size for i in R))
print('per verb (ours - retail bytes), largest first:')
for v, d in sorted(per.items(), key=lambda x: -x[1])[:25]: print(f'  {d:+5d}  {v}')
print('most negative:')
for v, d in sorted(per.items(), key=lambda x: x[1])[:10]: print(f'  {d:+5d}  {v}')
print('largest hunks:')
for h in sorted(hk, key=lambda x: -x[0])[:30]: print(f'  {h[0]:+4d} {h[1]:8s} ours#{h[2]} line {h[3]} {h[4][:110]}')
print("most negative hunks:")
for h in sorted(hk, key=lambda x: x[0])[:20]: print(f"  {h[0]:+4d} {h[1]:8s} ours#{h[2]} line {h[3]} {h[4][:110]}")
print('sum of same-shape size diffs (push 0 v push reg, or -1 v mov reg, ...):', sum(h[0] for h in hk if h[1] == 'size'))
print('sum of shape hunks:', sum(h[0] for h in hk if h[1] != 'size'))
