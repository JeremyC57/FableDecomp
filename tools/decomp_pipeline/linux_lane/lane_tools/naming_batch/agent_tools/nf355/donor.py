# donor.py (NF345): read-only donor-build evidence from ego_r.exe / FableWin.exe + their PDB publics (names and call relations only;
# never write donor bytes anywhere). usage:
#   donor.py <egor|fw> find <substring>          publics whose name contains substring, with VA
#   donor.py <egor|fw> calls <exact mangled>     direct call / jmp targets of that function (named), in order
import sys, re, struct, pickle, os, bisect, capstone
W = sys.argv[1]; P = '/tmp/claude-0/egod_pdb/'
pub = P + ('Ego_r.publics.txt' if W == 'egor' else 'FableWin.publics.txt'); exe = P + ('ego_r.exe' if W == 'egor' else 'FableWin.exe')
cache = '/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad/w22/agentNF345/work/donor_%s.pickle' % W
d = open(exe, 'rb').read(); pe = struct.unpack_from('<I', d, 0x3C)[0]; ns = struct.unpack_from('<H', d, pe + 6)[0]; osz = struct.unpack_from('<H', d, pe + 20)[0]
base = struct.unpack_from('<I', d, pe + 24 + 28)[0]; secs = []
for i in range(ns):
    o = pe + 24 + osz + 40 * i; vs, va, rs, rp = struct.unpack_from('<IIII', d, o + 8); secs.append((va, vs, rp, rs))
if os.path.exists(cache): names = pickle.load(open(cache, 'rb'))
else:
    names = []; cur = None
    for l in open(pub, encoding='utf-8', errors='replace'):
        m = re.search(r'S_PUB32 \[size = \d+\] `(.*)`', l)
        if m: cur = m.group(1); continue
        m = re.search(r'flags = function, addr = (\d{4}):(\d+)', l)
        if m and cur:
            s = int(m.group(1)); off = int(m.group(2)); names.append((base + secs[s - 1][0] + off, cur)); cur = None
    names.sort(); pickle.dump(names, open(cache, 'wb'))
addrs = [a for a, n in names]; byaddr = {}
for a, n in names: byaddr.setdefault(a, []).append(n)
def code(va, n):
    for sva, vs, rp, rs in secs:
        if base + sva <= va < base + sva + vs: o = rp + va - base - sva; return d[o:o + n]
    return b''
cmd = sys.argv[2]
if cmd == 'find':
    for a, n in names:
        if sys.argv[3] in n: print('%08x %s' % (a, n))
elif cmd == 'calls':
    tgt = [a for a, n in names if n == sys.argv[3]]
    for a in tgt:
        k = bisect.bisect_right(addrs, a); nxt = addrs[k] if k < len(addrs) else a + 0x400
        size = min(nxt - a, 0x4000); print('## %08x size<=%d %s' % (a, size, sys.argv[3]))
        md = capstone.Cs(capstone.CS_ARCH_X86, capstone.CS_MODE_32)
        for i in md.disasm(code(a, size), a):
            if i.mnemonic in ('call', 'jmp') and i.op_str.startswith('0x'):
                t = int(i.op_str, 16)
                if a <= t < a + size: continue
                nm = byaddr.get(t)
                if not nm:  # follow an incremental-link thunk (jmp rel32)
                    b = code(t, 5)
                    if b[:1] == b'\xe9': t2 = (t + 5 + struct.unpack_from('<i', b, 1)[0]) & 0xffffffff; nm = byaddr.get(t2, ['?%08x' % t2])
                print('  %08x %s %s' % (i.address, i.mnemonic, ' | '.join(nm or ['?%08x' % t])))
