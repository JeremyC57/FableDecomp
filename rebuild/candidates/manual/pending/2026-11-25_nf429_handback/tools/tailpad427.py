#!/usr/bin/env python3
"""tailpad427.py <Class> <retail row>: in hdr/engine/<Class>.h's FABLE_RETAIL_<C>_VIRTUAL_MODEL block, drop the trailing `_pad_*`
member (its offsetof assert too) and the block's `#pragma pack(push, 1)` / `#pragma pack(pop)`: retail's inlined copy constructor
(the Clone row) does not copy those bytes, so they are the class's natural tail padding, not a member. sizeof and every other offset
stay asserted (natural alignment gives the same values). Writes the file in place; prints a one-line report."""
import sys, re
from pathlib import Path
A = Path(__file__).resolve().parent.parent; X, row = sys.argv[1:3]
p = A / 'hdr/engine' / (X + '.h'); t = p.read_text().split('\n'); SW = 'FABLE_RETAIL_%s_VIRTUAL_MODEL' % X.upper()
i = t.index('#ifdef ' + SW); j = next(k for k in range(i, len(t)) if t[k] == '#else')
blk = t[i:j]
ce = max(k for k, l in enumerate(blk) if l == '};')
last = blk[ce - 1]; m = re.match(r'\s*unsigned char (_pad_0x[0-9a-f]+)\[', last)
if not m: sys.exit(X + ' NO_TAIL_PAD')
pad = m.group(1)
blk = blk[:ce - 1] + blk[ce:]
blk = [l for l in blk if l not in ('#pragma pack(push, 1)', '#pragma pack(pop)') and not re.search(r'offsetof\(%s, %s\)' % (X, pad), l)]
k = next(k for k, l in enumerate(blk) if l.startswith('// Members:'))
blk.insert(k + 1, '// NF427: no `#pragma pack` and no trailing `%s` member: retail\'s inlined copy constructor (Clone, 0x%s) does not copy those bytes, so they are tail padding (sizeof and offsets asserted).' % (pad, row.upper()))
t[i:j] = blk; p.write_text('\n'.join(t)); print(X, 'OK dropped', pad)
