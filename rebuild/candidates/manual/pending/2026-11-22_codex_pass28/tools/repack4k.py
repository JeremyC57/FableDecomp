"""repack4k.py in.pdb out.pdb: rewrite an MSF 7.00 file with 1 KB blocks (multi-block block map) as 4 KB blocks for llvm-pdbutil."""
import struct, sys
src = open(sys.argv[1], 'rb').read()
MAGIC = b'Microsoft C/C++ MSF 7.00\r\n\x1aDS\0\0\0'
assert src[:32] == MAGIC
bs, fpm, nblk, ndir, _, bmap = struct.unpack_from('<6I', src, 32)
blk = lambda i: src[i * bs:(i + 1) * bs]
ndb = (ndir + bs - 1) // bs
nmap = (ndb * 4 + bs - 1) // bs
mapb = b''.join(blk(bmap + k) for k in range(nmap))
dirblocks = struct.unpack_from('<%dI' % ndb, mapb)
d = b''.join(blk(b) for b in dirblocks)[:ndir]
n = struct.unpack_from('<I', d)[0]
sizes = struct.unpack_from('<%dI' % n, d, 4)
off = 4 + 4 * n
streams = []
for s in sizes:
    if s == 0xFFFFFFFF:
        streams.append(None); continue
    k = (s + bs - 1) // bs
    bl = struct.unpack_from('<%dI' % k, d, off); off += 4 * k
    streams.append(b''.join(blk(b) for b in bl)[:s])
NB = 4096
out = {}
nxt = [3]
def alloc():
    while nxt[0] % NB in (1, 2): nxt[0] += 1
    i = nxt[0]; nxt[0] += 1; return i
def put(data):
    ids = []
    for p in range(0, len(data), NB):
        i = alloc(); out[i] = data[p:p + NB]; ids.append(i)
    return ids
lists = []
for s in streams:
    lists.append([] if s is None else put(s))
nd = struct.pack('<I', n) + b''.join(struct.pack('<I', 0xFFFFFFFF if s is None else len(s)) for s in streams)
nd += b''.join(struct.pack('<%dI' % len(l), *l) for l in lists)
dids = put(nd)
mids = put(struct.pack('<%dI' % len(dids), *dids))
assert len(mids) == 1, 'block map needs one 4 KB block'
total = nxt[0]
total += (-total) % 1
buf = bytearray(total * NB)
buf[:32] = MAGIC
struct.pack_into('<6I', buf, 32, NB, 1, total, len(nd), 0, mids[0])
for i, data in out.items():
    buf[i * NB:i * NB + len(data)] = data
# free page maps: every block used (bit 0)
for k in range(0, total, NB):
    for f in (1, 2):
        if k + f < total: buf[(k + f) * NB:(k + f + 1) * NB] = b'\0' * NB
open(sys.argv[2], 'wb').write(buf)
print('streams', n, 'blocks', total)
