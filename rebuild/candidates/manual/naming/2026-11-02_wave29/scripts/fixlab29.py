# fixlab29.py: post-fixes LABELS.tsv from mklabels29.py: one row per address, plus explicit rows for targets this wave settled whose
#  label DB entry is a stand-in or absent (mklabels skips a target whose right name is anywhere in labels_*.tsv).
import os
W = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
p = os.path.join(W, 'LABELS.tsv'); R = [l.rstrip('\n').split('\t') for l in open(p)]
out = [R[0]]; seen = set()
for r in R[1:]:
    if r[0] in seen: continue
    seen.add(r[0]); out.append(r)
RK = 'rechecked RELOCATION_MATCH + PASS'
ADD = [
 ('0049d810', 'CWorld::GetWorldSeed', 'naming wave 29 (gws, DECISIONS_W28 D1): Ego_r.pdb / FableWin.pdb ?GetWorldSeed@CWorld@@QBEPAKXZ, now declared by '
  'engine/CWorld.h; binders 0x00914C60 / 0x008A7E00 respelled from GetPRandomSeed stand-ins; ' + RK),
 ('00c038d0', 'z_inflateEnd', 'naming wave 29 (zlib, c698 queue): both PDBs spell _z_inflateEnd (Z_PREFIX build); 0x00A3ABB0 respelled; ' + RK),
 ('00c00ae0', 'z_deflateEnd', 'naming wave 29 (zlib, c698 queue): both PDBs spell _z_deflateEnd (Z_PREFIX build); 0x00A3A930 respelled; ' + RK),
]
for t, n, e in ADD:
    if t not in seen: seen.add(t); out.append((t, n, e))
open(p, 'w').write(''.join('\t'.join(r) + '\n' for r in out)); print(len(out) - 1, 'labels after fix')
