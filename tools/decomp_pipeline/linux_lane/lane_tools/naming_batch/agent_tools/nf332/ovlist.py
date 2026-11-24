# ovlist.py <Class...>: per class, the retail vftable's override slots (labels not in the RTTI base chain) with the full RTTI-port label,
# the target address and whether the label is in the Ego_r / FableWin PDB name sets. No bytes. (NF323)
import sys, os, re, subprocess
R = '/home/user/FableDecomp'
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import importlib.util
EG = set(l.rstrip('\n').split('\t')[-1] for l in open(R + '/ghidra_out/egor_pdb_names.tsv', errors='replace'))
FW = set(l.rstrip('\n').split('\t')[-1] for l in open(R + '/ghidra_out/fablewin_pdb_names.tsv', errors='replace'))
out = subprocess.run(['python3', os.path.dirname(os.path.abspath(__file__)) + '/clonetri.py'] + sys.argv[1:], capture_output=True, text=True).stdout
L = {}
for f in ('labels_rtti_port.tsv', 'labels_rtti_port_dtors.tsv'):
    for l in open(R + '/ghidra_out/' + f):
        p = l.rstrip('\n').split('\t')
        if len(p) > 1: L.setdefault(p[0].lower(), p[1])
import struct
d = open(os.environ['FABLE_EXE'], 'rb').read()
pe = struct.unpack_from('<I', d, 0x3C)[0]; ns = struct.unpack_from('<H', d, pe + 6)[0]; osz = struct.unpack_from('<H', d, pe + 20)[0]
base = struct.unpack_from('<I', d, pe + 52)[0]; so = pe + 24 + osz; S_ = []
for i in range(ns):
    vsz, vaddr = struct.unpack_from('<II', d, so + i * 40 + 8); raw = struct.unpack_from('<I', d, so + i * 40 + 20)[0]; S_.append((base + vaddr, vsz, raw))
def rd(x):
    for s0, n0, r in S_:
        if s0 <= x < s0 + n0: return struct.unpack_from('<I', d, r + x - s0)[0]
for line in out.splitlines():
    p = line.split('\t'); c = p[0]; va = int(p[1][3:], 16)
    print('%s %s %s' % (c, p[1], p[2]))
    for it in (p[4].split() if len(p) > 4 else []):
        k = int(it.split(':')[0]); t = rd(va + 4 * k); lab = L.get('%08x' % t, '-')
        print('   slot %2d %08X %s eg=%d fw=%d' % (k, t, lab, lab in EG, lab in FW))
