# retail Fable.exe reader: va <-> bytes, dword search, containing landed catalog row
import struct, bisect, os
EXE = '/tmp/claude-0/-home-user-FableDecomp/a013f3ec-d440-5b47-87a1-ed975cb80dfb/scratchpad/dl/game/Fable The Lost Chapters/Fable.exe'
B = open(EXE, 'rb').read()
pe = struct.unpack_from('<I', B, 0x3c)[0]
nsec = struct.unpack_from('<H', B, pe + 6)[0]; opt = struct.unpack_from('<H', B, pe + 20)[0]
IB = struct.unpack_from('<I', B, pe + 24 + 28)[0]
SEC = []
for i in range(nsec):
    o = pe + 24 + opt + 40 * i
    nm = B[o:o + 8].rstrip(b'\0').decode(); vs, va, rs, ro = struct.unpack_from('<IIII', B, o + 8)
    SEC.append((nm, IB + va, vs, ro, rs))
def rd(va, n):
    for nm, v, vs, ro, rs in SEC:
        if v <= va < v + max(vs, rs): return B[ro + va - v: ro + va - v + n]
    return None
def dw(va): return struct.unpack('<I', rd(va, 4))[0]
def find(val, secs=None):
    pat = struct.pack('<I', val); out = []
    for nm, v, vs, ro, rs in SEC:
        if secs and nm not in secs: continue
        blob = B[ro:ro + rs]; i = blob.find(pat)
        while i >= 0: out.append((nm, v + i)); i = blob.find(pat, i + 1)
    return out
_C = None
def cat():
    global _C
    if _C is None:
        L = []
        for l in open('/home/user/FableDecomp/rebuild/oracles/auto-re-candidates.tsv'):
            s = l.split('\t')
            if len(s) >= 3 and s[0] != 'address': L.append((int(s[0], 16), int(s[2]), s[1]))
        L.sort(); _C = L
    return _C
def owner(va):
    L = cat(); i = bisect.bisect_right([x[0] for x in L], va) - 1
    if i >= 0 and L[i][0] <= va < L[i][0] + L[i][1]: return '%08x %s' % (L[i][0], L[i][2])
    return None
def cstr(va, n=80):
    b = rd(va, n); return b.split(b'\0')[0].decode('latin-1') if b else None
