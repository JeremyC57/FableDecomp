# callers.py <target...>: landed rows whose catalog (retail) bytes hold an E8 call / E9 jmp rel32 to each target
import sys, os, struct
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from rows import rows, ROOT
R = rows()
T = {int(a, 16): a for a in sys.argv[1:]}
hits = {a: [] for a in sys.argv[1:]}
for l in open(ROOT + '/rebuild/oracles/auto-re-candidates.tsv'):
    s = l.rstrip('\n').split('\t')
    if len(s) < 4 or s[0] == 'address' or s[0].lower() not in R: continue
    a = int(s[0], 16)
    try: b = bytes.fromhex(s[3])
    except ValueError: continue
    for i in range(len(b) - 4):
        if b[i] in (0xE8, 0xE9):
            t = (a + i + 5 + struct.unpack_from('<i', b, i + 1)[0]) & 0xffffffff
            if t in T: hits[T[t]].append('%s%s' % (s[0].lower(), '' if b[i] == 0xE8 else 'j'))
for a, h in hits.items(): print(a, len(set(h)), ' '.join(sorted(set(h))))
