# vtslot.py <addr..>: every retail vftable slot holding <addr>: the vftable start (the dword after the COL pointer), the class named by
# the COL's type descriptor, and the slot index. A vftable is found by walking back from the hit over dwords that point into .text until
# the previous dword points at a CompleteObjectLocator (signature 0) whose type descriptor name starts .?AV / .?AU. Screen only.
import sys, struct
sys.path.insert(0, '/tmp/claude-0/-home-user-FableDecomp/5221bbec-fb18-575f-8649-a32d96bb1c2d/scratchpad/w22/agentNF319/tools')
from nf import exe
secs = exe()
def sec_of(v):
    for n, va, b, ch in secs:
        if va <= v < va + len(b): return n, va, b
    return None
def rd(v):
    s = sec_of(v)
    if not s or v + 4 > s[1] + len(s[2]): return None
    return struct.unpack_from('<I', s[2], v - s[1])[0]
def is_text(v):
    s = sec_of(v); return bool(s) and s[0] == '.text'
def col_name(c):
    if rd(c) != 0: return None
    td = rd(c + 12)
    if td is None: return None
    s = sec_of(td + 8)
    if not s: return None
    o = td + 8 - s[1]; e = s[2].find(b'\0', o)
    nm = s[2][o:e]
    return nm.decode('latin1') if nm.startswith((b'.?AV', b'.?AU')) else None
for a in sys.argv[1:]:
    t = int(a, 16); hits = []
    for n, va, b, ch in secs:
        if n not in ('.rdata', '.data'): continue
        pat = struct.pack('<I', t); i = b.find(pat)
        while i >= 0:
            if i % 4 == 0: hits.append(va + i)
            i = b.find(pat, i + 1)
    out = []
    for h in hits:
        p = h
        while True:
            prev = rd(p - 4)
            if prev is None: break
            nm = col_name(prev) if not is_text(prev) else None
            if nm: out.append('%08X:%s[%d]' % (p, nm, (h - p) // 4)); break
            if not is_text(prev): out.append('%08X:?' % h); break
            p -= 4
    print('%s\t%s' % (a, ' '.join(out) if out else 'no-vftable-hit'))
