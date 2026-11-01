# fixlab25.py: post-fixes LABELS.tsv from mklabels25.py.
#  - 00c2d9c0 / 00c2da00 stay unlabelled until DECISIONS_W25 D2; 0139c8fc until D1.
#  - labels the label DB carries in a superseded spelling next to the right one (mklabels skips a target whose right name
#    is anywhere in labels_*.tsv): an explicit wave-25 row so the last-applied label is the PDB name.
#  - own rows re-authored this wave whose address carries no label at all.
import os
W = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
p = os.path.join(W, 'LABELS.tsv'); R = [l.rstrip('\n').split('\t') for l in open(p)]
out = [R[0]]; seen = set()
for r in R[1:]:
    if r[0] in ('00c2d9c0', '00c2da00', '0139c8fc'): continue
    if r[0] in seen: continue
    seen.add(r[0]); out.append(r)
ADD = [
 ('00f35b30', 'CEntityScriptBase::IsActiveThreadTerminating',
  'naming wave 25 (esb, DECISIONS_W24 item): Ego_r.pdb / FableWin.pdb ?IsActiveThreadTerminating@CEntityScriptBase@@QBE_NXZ; supersedes the '
  'label DB\'s IsDone / CEntityScriptBase::IsDone spellings (chunks 241 / 280 / 322); seven binders respelled over the empty-base global '
  'CEntityScriptBase (0x00CDF620 0x00D864B0 0x00D9C2B0 0x00E05360 0x00E76920 0x00EFB8C0 0x00EFE8D0); rechecked RELOCATION_MATCH + PASS'),
 ('0076c810', 'CTCEnemy::SetFaction',
  'naming wave 25 (peek): own row defines Ego_r.pdb / FableWin.pdb ?SetFaction@CTCEnemy@@QAEXABVCCharString@@@Z (Leaf line added; the '
  'body reads PeekWorld().PFactionManager and calls CFactionManager::GetPFaction 0x006B93D0); rechecked RELOCATION_MATCH + PASS'),
 ('0077db90', 'CTCHeroCentre::OnHeroWalkedThroughDoor',
  'naming wave 25 (peek): own row defines Ego_r.pdb / FableWin.pdb ?OnHeroWalkedThroughDoor@CTCHeroCentre@@QAEXW4EHeroCentreDoorType@@@Z '
  '(Leaf line added; calls CTCHeroCentre::OpenTrainingScreen 0x0077D4B0); rechecked RELOCATION_MATCH + PASS'),
 ('00aa7540', 'CChunkedFileLoader::~CChunkedFileLoader',
  'naming wave 25 (cfl): own row defines Ego_r.pdb / FableWin.pdb ??1CChunkedFileLoader@@UAE@XZ (virtual, the body re-installs the vtable '
  'and inlines nothing else); supersedes the label DB\'s CChunkedFileLoader::CChunkedFileLoader; rechecked RELOCATION_MATCH + PASS'),
]
for t, n, e in ADD:
    if t not in seen: seen.add(t); out.append((t, n, e))
open(p, 'w').write(''.join('\t'.join(r) + '\n' for r in out)); print(len(out) - 1, 'labels after fix')
