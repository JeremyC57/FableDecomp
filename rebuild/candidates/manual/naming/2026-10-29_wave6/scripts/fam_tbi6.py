# wave 6 family: the binary loaders of CPersistTraits<T> take the stream and the value by reference (Ego_r.pdb / FableWin.pdb
# ?TransferBinaryIn@?$CPersistTraits@<T>@@QAEXAAVCMemoryDataInputStream@@AA<T>@Z for bool 0x00403EB0, unsigned char
# 0x00405940, float 0x0040EFB0, long 0x0040FE60, unsigned __int64 0x0044B890, and the enum instances alike; QUEUE_561).
# Row-local CPersistTraits models took pointers (PAV...PA<T>), the same bytes.
# Transform (idempotent, source files): every `TransferBinaryIn(CMemoryDataInputStream* s, X* v)` member of a
# CPersistTraits model becomes `TransferBinaryIn(CMemoryDataInputStream& s, X& v)`; an inline forwarding body
# `{ v->TransferBinaryIn(s); }` becomes `{ v.TransferBinaryIn(&s); }`; the call `.TransferBinaryIn(<stream>, &<value>)`
# becomes `.TransferBinaryIn(*<stream>, <value>)`. Skips a row with any other two-argument TransferBinaryIn shape.
import re, sys
DECL = re.compile(r'\bTransferBinaryIn[ \t]*\([ \t]*CMemoryDataInputStream[ \t]*\*[ \t]*(\w+)[ \t]*,[ \t]*([\w:<> ]+?)[ \t]*\*[ \t]*(\w+)[ \t]*\)')
FWD = re.compile(r'\{[ \t]*(\w+)->TransferBinaryIn\([ \t]*(\w+)[ \t]*\)[ \t]*;[ \t]*\}')
CALL = re.compile(r'\.TransferBinaryIn\([ \t]*([\w>.-]+?)[ \t]*,[ \t]*&[ \t]*([\w>.\[\]-]+?)[ \t]*\)')
def fix(t):
    if not DECL.search(t): return None, 'no pointer-form loader'
    t2 = DECL.sub(lambda m: 'TransferBinaryIn(CMemoryDataInputStream& %s, %s& %s)' % (m.group(1), m.group(2), m.group(3)), t)
    # inline forwarders on the converted declarations
    def fwd(m):
        return '{ %s.TransferBinaryIn(&%s); }' % (m.group(1), m.group(2))
    lines = []
    for line in t2.split('\n'):
        if 'TransferBinaryIn(CMemoryDataInputStream& ' in line: line = FWD.sub(fwd, line)
        lines.append(line)
    t2 = '\n'.join(lines)
    t2 = CALL.sub(lambda m: '.TransferBinaryIn(*%s, %s)' % (m.group(1), m.group(2)), t2)
    left = re.findall(r'TransferBinaryIn[ \t]*\([ \t]*\w[\w:<> ]*\*[ \t]*\w+[ \t]*,[^()]*\)', re.sub(r'//[^\n]*', '', t2))
    if left: return None, 'other loader shape: ' + left[0][:70]
    return t2, 'ok'
if __name__ == '__main__':
    for p in sys.argv[1:]:
        t = open(p, encoding='utf-8', errors='surrogateescape', newline='').read()
        new, why = fix(t)
        if new is None: print('skip', p, why); continue
        open(p, 'w', encoding='utf-8', errors='surrogateescape', newline='').write(new); print('edited', p, why)
