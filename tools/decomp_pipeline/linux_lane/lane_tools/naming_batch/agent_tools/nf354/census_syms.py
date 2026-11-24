#!/usr/bin/env python3
"""census_syms.py <obj...>: per object, (1) the leaf relocations whose retail target is 0x00CD3D2E (nf.py xr: symbol, count) and
(2) every external DEF / UND symbol that names a cut-scene map stand-in (CCutsceneCastMap / CCutsceneArgMap / CCutsceneFlagMap) or
RunCutsceneMacro / RunScriptedCutscene. Object name = <tag>_<addr>.obj. Agent NF354 census helper (read-only)."""
import sys, re, io, contextlib
sys.path.insert(0, '/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad/w22/agentNF354/tools')
sys.dont_write_bytecode = True
import nf
pat = re.compile(r'CCutscene(Cast|Arg|Flag)Map|RunCutsceneMacro|RunScriptedCutscene')
for o in sys.argv[1:]:
    a = re.search(r'([0-9a-f]{8})\.obj$', o).group(1)
    buf = io.StringIO()
    with contextlib.redirect_stdout(buf): nf.cmd_xr(a, o)
    for l in buf.getvalue().splitlines():
        t, sym, k = l.split('\t')
        if t == '00cd3d2e': print('%s\tXR\t%s\t%s' % (a, k, sym))
    for s in sorted(nf.ext_syms(o)):
        if pat.search(s): print('%s\t%s' % (a, s.replace(' ', '\t', 1)))
