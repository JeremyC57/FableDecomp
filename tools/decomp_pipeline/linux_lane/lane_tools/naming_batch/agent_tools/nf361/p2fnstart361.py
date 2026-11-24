# p2fnstart361.py <site...>: heuristic function start for a retail site: the nearest 16-aligned address below the site whose previous
# byte is int3 padding (0xCC) or a ret (C3 / C2 xx xx), and which no landed / catalog row covers. Prints start and catalog / label hits.
import sys, struct
sys.path.insert(0, '/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad/w22/agentNF361/tools'); import nf
secs = nf.exe()
for s in sys.argv[1:]:
    s = int(s, 16)
    for nm, va, d, ch in secs:
        if va <= s < va + len(d):
            p = s & ~0xF
            while p > va:
                o = p - va
                if d[o - 1] == 0xCC or d[o - 1] == 0xC3 or (o >= 3 and d[o - 3] == 0xC2 and d[o - 1] == 0): break
                p -= 16
            print('%08x start~%08x' % (s, p))
