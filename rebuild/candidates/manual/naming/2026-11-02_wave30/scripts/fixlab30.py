# fixlab30.py: post-fixes LABELS.tsv from mklabels30.py: one row per address, plus explicit rows for targets this wave settled whose
#  label DB entry is a stand-in or absent (mklabels skips a target whose right name is anywhere in labels_*.tsv).
import os
W = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
p = os.path.join(W, 'LABELS.tsv'); R = [l.rstrip('\n').split('\t') for l in open(p)]
out = [R[0]]; seen = set()
DROP = {'00527d80', '00686af0', '006a1440', '0083d460'}   # stand-in spellings this wave respelled as row-local models (not retail names)
for r in R[1:]:
    if r[0] in seen or r[0] in DROP: continue
    seen.add(r[0]); out.append(r)
RK = 'rechecked RELOCATION_MATCH + PASS'
ADD = [
 ('00686af0', 'CTCBase::GetThingManager', 'naming wave 30 (c699, queue row 70): Ego_r.pdb / FableWin.pdb ?GetThingManager@CTCBase@@IBEABVCThingManager@@XZ (protected, const;'
  ' the IAE non-const overload is a second PDB member); the const CTCHeroStats callers 0x0057ABD6 / 0x0057AE53 reach PeekPThingWithUID through it'
  ' (the row-local owner class is not this family\'s); was the descriptive GetGlobal_013b8a1c_00686af0; ' + RK),
 ('00569940', 'std::vector<std::basic_string<char,std::char_traits<char>,std::allocator<char> >,std::allocator<std::basic_string<char,std::char_traits<char>,std::allocator<char> > > >::~vector',
  'naming wave 30 (lhn, c697 queue row 35): STLport ~vector<std::string>, the out-of-line COMDAT ~LHHttpHeaders 0x00569C30 tail-calls; own row respelled; ' + RK),
]
for t, n, e in ADD:
    if t not in seen: seen.add(t); out.append((t, n, e))
open(p, 'w').write(''.join('\t'.join(r) + '\n' for r in out)); print(len(out) - 1, 'labels after fix')
