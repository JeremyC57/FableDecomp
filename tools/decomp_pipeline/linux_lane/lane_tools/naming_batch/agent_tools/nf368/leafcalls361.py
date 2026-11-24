# leafcalls361.py <addr> <obj...>: in each object, the leaf function's (row // Leaf: symbol) relocation count to the Reset / ReleaseData
# CCountedPointer<NUISystem::CComponent> symbols and the leaf size: shows whether a variant turned an inline expansion into a call.
import sys
sys.path.insert(0, '/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad/w22/agentNF368/tools'); import nf
RS = '?Reset@?$CCountedPointer@VCComponent@NUISystem@@@@QAEXPAVCComponent@NUISystem@@@Z'; RD = '?ReleaseData@?$CCountedPointer@VCComponent@NUISystem@@@@QAEXXZ'
a = sys.argv[1]; leaf = nf.leaf_of(nf.src_of(a).read_text(encoding='utf-8'), a)
for o in sys.argv[2:]:
    secs, sl = nf.coff(o)
    hit = [(n, sec) for i, n, v, sec, t, scl in sl if scl == 2 and sec > 0 and n.startswith(leaf)]
    if not hit: print(a, o.split('/')[-1], 'NOLEAF'); continue
    s = secs[hit[0][1] - 1]
    print(a, o.split('/')[-1], 'leaf_len=%d' % len(s['data']), 'calls_Reset=%d' % sum(1 for r in s['rel'] if r[1] == RS), 'calls_ReleaseData=%d' % sum(1 for r in s['rel'] if r[1] == RD))
