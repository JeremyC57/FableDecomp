# fixlab24.py: post-fixes LABELS.tsv from mklabels24.py.
#  - CThingSearchToolsMethods (the landed view, decision D6) -> the PDB class CThingSearchTools; one address, one name.
#  - 00c2d9c0 carries two spellings (Reader::Find / CriteriaReader::FindSegment): neither is labelled until decision D5.
import os
W = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
p = os.path.join(W, 'LABELS.tsv'); R = [l.rstrip('\n').split('\t') for l in open(p)]
out = [R[0]]; seen = set()
for r in R[1:]:
    if r[0] == '00c2d9c0': continue
    if r[1].startswith('CThingSearchToolsMethods::'):
        r[1] = 'CThingSearchTools::' + r[1][len('CThingSearchToolsMethods::'):]
        r[2] += '; PDB class CThingSearchTools (Ego_r.pdb ??$...@CThingSearchTools@@QBE_N...), the row still models it through the CThingSearchToolsMethods view'
    if r[0] in seen: continue
    seen.add(r[0]); out.append(r)
out.append(('00686a80', 'CTCBase::PeekWorld', 'naming wave 24 (misc, 007066a0): Ego_r.pdb ?PeekWorld@CTCBase@@IBEABVCWorld@@XZ at 0x006B1E70, 16 bytes after ?GetWorld@CTCBase@@IBEAAVCWorld@@XZ 0x006B1E60 (FableWin.pdb 0x02190D40 / 0x02190D60), as retail GetWorld 0x00686A70 precedes 0x00686A80; 0x004BC750 already binds the PeekWorld spelling; replaces the descriptive GetGlobal_013b8a1c_deref0x24; rechecked RELOCATION_MATCH + PASS'))
open(p, 'w').write(''.join('\t'.join(r) + '\n' for r in out)); print(len(out) - 1, 'labels after fix')
