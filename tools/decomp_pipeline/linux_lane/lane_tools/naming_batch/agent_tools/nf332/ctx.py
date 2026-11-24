# ctx.py <addr> <line> [before] [after]: non-blank source lines of a landed row around a line (class-model triage)
import sys; sys.path.insert(0, '/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad/w22/agentNF332/tools'); import nf
a, n = sys.argv[1], int(sys.argv[2]); b = int(sys.argv[3]) if len(sys.argv) > 3 else 12; c = int(sys.argv[4]) if len(sys.argv) > 4 else 2
L = nf.src_of(a).read_text(encoding='utf-8', errors='replace').split('\n')
for i in range(max(1, n - b), min(len(L), n + c) + 1):
    if L[i - 1].strip(): print('%5d: %s' % (i, L[i - 1][:170]))
