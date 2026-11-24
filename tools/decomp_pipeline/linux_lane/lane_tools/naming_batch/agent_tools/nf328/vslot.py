# vslot.py <vtable va hex> <slot...>: read vtable slots from Fable.exe and print the RTTI-port label of each target (no bytes printed)
import sys, struct, os
sys.path.insert(0, '/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad/w22/agentNF328/tools')
d = open(os.environ['FABLE_EXE'], 'rb').read()
pe = struct.unpack_from('<I', d, 0x3C)[0]; ns = struct.unpack_from('<H', d, pe + 6)[0]; osz = struct.unpack_from('<H', d, pe + 20)[0]
base = struct.unpack_from('<I', d, pe + 52)[0]; so = pe + 24 + osz; secs = []
for i in range(ns):
    va, vs, raw = struct.unpack_from('<I', d, so + i * 40 + 12)[0], struct.unpack_from('<I', d, so + i * 40 + 8)[0], struct.unpack_from('<I', d, so + i * 40 + 20)[0]
    secs.append((base + va, vs, raw))
def rd(va):
    for s, n, r in secs:
        if s <= va < s + n: return struct.unpack_from('<I', d, r + va - s)[0]
L = {}
for f in ('labels_rtti_port.tsv', 'labels_rtti_port_dtors.tsv'):
    for l in open('/home/user/FableDecomp/ghidra_out/' + f):
        p = l.rstrip('\n').split('\t')
        if len(p) > 1: L.setdefault(p[0].lower(), p[1])
vt = int(sys.argv[1], 16)
for s in sys.argv[2:]:
    t = rd(vt + 4 * int(s)); print('slot', s, '%08X' % t, L.get('%08x' % t, '-'))
