#!/usr/bin/env python3
"""fixtest_ci.py <dir with <a>.cpp and <a>_test.cpp>...: CLONE_INLINE tests compared the whole object after the base, which includes
padding the implicit copy constructor never copies; compare the base bytes and then each member (offset, width from the row) instead."""
import sys, re, glob, os
W = {'long': 4, 'int': 4, 'unsigned long': 4, 'unsigned int': 4, 'float': 4, 'bool': 1, 'char': 1, 'unsigned char': 1, 'short': 2, 'unsigned short': 2, 'signed char': 1}
for d in sys.argv[1:]:
    for t in glob.glob(d + '/*_test.cpp'):
        a = os.path.basename(t)[:8]; src = open(os.path.join(os.path.dirname(t).replace('/tests', '/rows') if '/tests' in t else d, a + '.cpp')).read()
        mem = []
        for m in re.finditer(r'^    (.+?) (\w+);\s+// \+0x([0-9A-F]+) \(Ego_r\.pdb (.+?) \w+ at', src, re.M):
            ty = m.group(1).strip(); w = 4 if ty.endswith('*') or ty.startswith('long /*') else W[ty]
            mem.append((int(m.group(3), 16), w))
        base = 0x114 if 'public CCombatActionBase' in src else 0xA8
        tx = open(t).read()
        cmp_ = ' && '.join(['memcmp(d + 4, s + 4, 0x%X - 4) == 0' % base] + ['memcmp(d + 0x%X, s + 0x%X, %d) == 0' % (o, o, w) for o, w in mem])
        tx2 = re.sub(r'memcmp\(d \+ 4, s \+ 4, 0x[0-9A-F]+ - 4\) == 0', cmp_, tx)
        if tx2 != tx: open(t, 'w').write(tx2.replace('the class\'s own members are copied;', 'the class\'s own members are copied (base bytes, then each member; padding is not copied);'))
        print(a, len(mem), 'patched' if tx2 != tx else 'UNCHANGED')
