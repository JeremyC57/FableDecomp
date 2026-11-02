# h31_hos.py: naming wave 31, family hos (c702 queue: ?ClearScoreboard@CTCHeroOnlineScoreboard@@AAEXXZ 0x00562E10, private in Ego_r.pdb /
#  FableWin.pdb; chunk 702 landed the own row with it). The three CTCHeroOnlineScoreboard members that call it (0x00562E70, 0x00563130,
#  0x00563410) declared it public (QAE); they are members of the class, so the private declaration is reachable. Their bare Leafs become
#  the emitted symbols: 0x00563130 ?ShowScoreboard@CTCHeroOnlineScoreboard@@QAEXXZ (Ego_r.pdb / FableWin.pdb); 0x00562E70 / 0x00563410 keep
#  their descriptive names (0x00562E70 = Close is PROBABLE only, chunk 702: Ego_r block order + body, no second source).
import os, sys
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import he
from common import note
LEAF = {'00562e70': '?ResetScoreboard@CTCHeroOnlineScoreboard@@QAEXXZ', '00563130': '?ShowScoreboard@CTCHeroOnlineScoreboard@@QAEXXZ',
        '00563410': '?AbortWithMessage@CTCHeroOnlineScoreboard@@QAEXABVCCharString@@@Z'}
BARE = {'00562e70': 'ResetScoreboard', '00563130': 'ShowScoreboard', '00563410': 'AbortWithMessage'}
for a in sorted(LEAF):
    he.resub(a, 'src', r'\n(    )void ClearScoreboard\(\);( +)// 0x00562E10\n',
             r'\nprivate:\n\1void ClearScoreboard();\2// 0x00562E10 ?ClearScoreboard@CTCHeroOnlineScoreboard@@AAEXXZ (Ego_r.pdb / FableWin.pdb: private)\npublic:\n', 1)
    he.edit(a, 'src', '// Leaf: %s\n' % BARE[a], '// Leaf: %s\n' % LEAF[a])
    note(a, 'hos', 'ClearScoreboard 0x00562E10 is private in both PDBs (?ClearScoreboard@CTCHeroOnlineScoreboard@@AAEXXZ, the chunk 702 own\n'
         '//   row; was the public QAE spelling); the bare Leaf "%s" is the emitted symbol %s%s' % (BARE[a], LEAF[a],
         ' (Ego_r.pdb / FableWin.pdb)' if a == '00563130' else ' (descriptive name; %s)' % ('PROBABLE Close, chunk 702: one source' if a == '00562e70' else 'in neither PDB')))
