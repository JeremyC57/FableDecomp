#!/usr/bin/env python3
"""idxscan.py <obj...>: per object, every external symbol that is an operator[] (??A) returning / taking the scripted-thing map types
(names CScriptGameResourceObjectScriptedThingBase), DEF or UND, plus any symbol naming a CCutscene*Map stand-in. Agent NF354 census
helper (read-only; used over NF350's HEAD object cache)."""
import sys, re
sys.path.insert(0, '/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad/w22/agentNF354/tools')
sys.dont_write_bytecode = True
import nf
REAL = '??A?$map@VCCharString@@VCScriptGameResourceObjectScriptedThingBase@@U?$less@VCCharString@@@std@@V?$allocator@U?$pair@$$CBVCCharString@@VCScriptGameResourceObjectScriptedThingBase@@@std@@@4@@std@@QAEAAVCScriptGameResourceObjectScriptedThingBase@@ABVCCharString@@@Z'
for o in sys.argv[1:]:
    hits = []
    for s in sorted(nf.ext_syms(o)):
        k, n = s.split(' ', 1)
        if (n.startswith('??A') and 'CScriptGameResourceObjectScriptedThingBase' in n) or re.search(r'CCutscene(Cast|Arg|Flag)Map', n):
            hits.append('%s:%s' % (k, 'REAL4' if n == REAL else n))
    print('%s\t%s' % (o.rsplit('/', 1)[-1], ' '.join(hits) or 'none'))
