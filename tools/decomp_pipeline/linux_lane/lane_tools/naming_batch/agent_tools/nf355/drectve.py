# drectve.py <obj...>: print each object's full .drectve token list (NF355; for NF318 Q1 evidence)
import sys; sys.path.insert(0, '/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad/w22/agentNF355/tools'); import nf
for o in sys.argv[1:]:
    secs, _ = nf.coff(o)
    for s in secs:
        if s['name'] == '.drectve': print(o.split('/')[-1], s['data'].decode('latin1').split())
