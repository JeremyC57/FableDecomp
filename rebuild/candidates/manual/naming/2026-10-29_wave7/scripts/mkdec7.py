# DECISIONS_W7.tsv: QUEUE_562_563 finals + wave-7 conflict settlements + hand-pass finals (targets.py), checked against both PDB name lists
import csv, collections, os
os.chdir(os.path.dirname(os.path.abspath(__file__)))
from targets import FINAL
ROOT = '/home/user/FableDecomp'
def names(p):
    s = set()
    for l in open(p, encoding='utf-8', errors='replace'):
        q = l.rstrip('\n').split('\t')
        if len(q) >= 2: s.add(q[1])
    return s
EG = names(ROOT + '/ghidra_out/egor_pdb_names.tsv'); FW = names(ROOT + '/ghidra_out/fablewin_pdb_names.tsv')
FW |= names(ROOT + '/ghidra_out/_fablewin_api.tsv') if False else set()
bound = collections.defaultdict(set)
for r in csv.reader(open(ROOT + '/rebuild/oracles/reloc_bindings.tsv'), delimiter='\t'):
    if len(r) >= 2 and r[0] != 'symbol': bound[r[1]].add(r[0])
Q = {}
for l in open('QUEUE_562_563.tsv'):
    q = l.rstrip('\n').split('\t')
    if len(q) >= 4: Q[q[1]] = (q[0], q[3], q[4] if len(q) > 4 else '')
CONF = {
    '004fc180': 'RELOC_CONFLICT settlement (0064e019): ToggleCentering reaches 0x4FC180 through the world\'s slot 13 = PeekWorldMap (const) -> const GetRegion QBEABV; 0x4FB490 is the non-const GetRegion (GetWorldMap). 0064e019 now calls the const one and passes the const region to UpdateRegionExitNames 0x0064DD9A',
    '004fb490': 'see 004fc180',
    '00686a70': 'RELOC_CONFLICT settlement (007800b0 vs 006d53c0): Ego_r CTCBase run = retail + 0x2B33F0: 0x686A70 GetWorld() const -> CWorld& (IBEAAV), 0x686A80 PeekWorld() const -> const CWorld& (IBEABV)',
    '00686a80': 'see 00686a70',
    '004a9760': 'RELOC_CONFLICT settlement (CIntelligentPointer op=): template instance per pointee; 0x4A9760 is CIntelligentPointer<CThing>::operator= (rows 00788229, 007e00d0, 008597a0, 00954470 assign CThing pointers)',
    '00c16950': 'RELOC_CONFLICT settlement: 0x00C16950 is a separate operator= instance; pointee CThingCreatureBase from the catalog / caller 00c49100 only (one source): probable',
    '0048bd20': 'RELOC_CONFLICT settlement (CIntelligentPointerCreature op=): 0x48BD20 = CIntelligentPointer<CThingPlayerCreature> (00961550, Ego_r CPlayer::ControlledCreature type)',
    '0050f440': 'RELOC_CONFLICT settlement: 0x50F440 = CIntelligentPointer<CThingAICreature> (0065f01d)',
    '006d4d70': 'RELOC_CONFLICT settlement (IsAIFollower 006D4D70 vs 006D4DB0): Ego_r CTCFollowed order IsFollower(const CThing*) const, IsAIFollower(const CThingAICreature*) const = retail 0x6D4D70, 0x6D4DB0',
    '006d4db0': 'see 006d4d70',
    '013b8ab8': 'multi-name settlement: CMovie TU static (written by CMovie ctor 0x00547C90 from its CIEngine& arg); no PDB data symbol: descriptive',
    '013b8ac0': 'multi-name settlement: CMovie TU static texture (allocated by slot 69 AllocateTextureFromEngineResourcePools); no PDB data symbol: descriptive',
    '0049d9d0': 'convergence on the first binding (0095f9a0); no PDB symbol (inlined in Ego_r): descriptive',
    '013b8790': 'global player GUI pointer; spelling of the first binding, PDB global absent: descriptive',
    '006869d0': 'Ego_r CTCBase 0x006B1DC0 - 0x2B33F0 (GetDefinitionManager() const, IBEABV)',
}
DESCRIPTIVE = {'013b8ab8', '013b8ac0', '0049d9d0', '013b8790'}
PROBABLE = {'00c16950'}
out = ['\t'.join(['address', 'final_symbol', 'confidence', 'sources', 'chunks', 'wrong_spellings', 'bound_wrong_symbols', 'evidence'])]
for a in sorted(FINAL):
    s = FINAL[a]
    src = '+'.join(x for x, S in (('Ego_r.pdb', EG), ('FableWin.pdb', FW)) if s in S) or '-'
    if a in DESCRIPTIVE: conf = 'descriptive'
    elif a in PROBABLE: conf = 'probable'
    elif src == 'Ego_r.pdb+FableWin.pdb': conf = 'proven'
    elif src != '-': conf = 'proven-1pdb'
    else: conf = 'probable'
    q = Q.get(a); ev = []
    if q: ev.append(q[1])
    if a in CONF: ev.append(CONF[a])
    wrong = sorted(bound.get(a, set()) - {s})
    out.append('\t'.join([a, s, conf, src, q[0] if q else 'w7', q[2] if q else '', '|'.join(wrong), ' | '.join(ev)]))
open('DECISIONS_W7.tsv', 'w').write('\n'.join(out) + '\n')
print(collections.Counter(l.split('\t')[2] for l in out[1:]))
for l in out[1:]:
    q = l.split('\t')
    if q[2] != 'proven': print(q[0], q[1][:100], q[2], q[3])
