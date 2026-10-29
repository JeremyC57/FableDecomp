# fam_cbo8.py <file>...: CBaseObject's default ctor and copy assignment are protected (Ego_r.pdb + FableWin.pdb
#   ??0CBaseObject@@IAE@XZ 0x0099A380, ??4CBaseObject@@IAEAAV0@ABV0@@Z 0x0099A3B0). Each `CBaseObject();` / `CBaseObject& operator=(
#   const CBaseObject& ...);` declaration line inside a CBaseObject class body is wrapped in `protected:` ... `public:`
#   (access labels do not move members). Idempotent: a line already under a `protected:` label just above it is left.
import sys, re
LINE = re.compile(r'^([ \t]*)(CBaseObject\(\s*\)\s*;|CBaseObject\s*&\s*operator=\(\s*const\s+CBaseObject\s*&[^)]*\)\s*;)([^\n]*)$', re.M)
for p in sys.argv[1:]:
    t = open(p, encoding='utf-8', errors='surrogateescape', newline='').read(); o = t
    out = []; pos = 0
    for m in LINE.finditer(t):
        # only inside a class CBaseObject body: the nearest preceding class head is CBaseObject's
        head = list(re.finditer(r'\b(class|struct)\s+(\w+)[^;{]*\{', t[:m.start()]))
        if not head or head[-1].group(2) != 'CBaseObject': continue
        prev = t[:m.start()].rstrip().rsplit('\n', 1)[-1].strip()
        if prev == 'protected:': continue
        nl = '\r\n' if '\r\n' in t else '\n'
        out.append(t[pos:m.start()] + 'protected:' + nl + m.group(0) + nl + 'public:'); pos = m.end()
    t = ''.join(out) + t[pos:]
    t = re.sub(r'(protected:\r?\n[^\n]*\r?\n)public:(\r?\n)protected:\r?\n', r'\1', t)   # adjacent pair -> one protected block
    if t != o: open(p, 'w', encoding='utf-8', errors='surrogateescape', newline='').write(t); print('edited', p)
    else: print('skip', p, 'no public CBaseObject ctor / operator= line')
