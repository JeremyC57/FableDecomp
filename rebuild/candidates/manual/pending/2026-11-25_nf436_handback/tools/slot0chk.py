#!/usr/bin/env python3
"""slot0chk.py <wd> <spec.tsv>: each row object's ??_G<C> (and ??_G?$CLevelScriptBase<T>) against retail slot 0 of the class (and base) vftable."""
import sys, subprocess, glob, struct, os
sys.path.insert(0, "/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad/w22/agentNF427/tools"); import pe
dw = lambda va: struct.unpack("<I", pe.read_va(pe.EXES["retail"], va, 4))[0]
tab = {}
for l in open("/home/user/FableDecomp/rebuild/oracles/reloc_bindings.tsv").read().splitlines()[1:]:
    p = l.split("\t"); tab.setdefault(p[0], []).append(int(p[1], 16))
T = "/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad/w22/agentNF436/tools/retcmp_coordinator.py"
for l in open(sys.argv[2]).read().splitlines():
    f = l.split("\t"); a, C, Tt = f[0], f[1], f[2]
    o = glob.glob(f"{sys.argv[1]}/**/{a}/{a}.obj", recursive=True)[0]
    args = []
    for vt, g in (("??_7%s@NScript@@6B@" % C, "??_G%s@NScript@@UAEPAXI@Z" % C),
                  ("??_7?$CLevelScriptBase@V%s@NScript@@@@6B@" % Tt, "??_G?$CLevelScriptBase@V%s@NScript@@@@UAEPAXI@Z" % Tt)):
        if vt in tab and len(tab[vt]) == 1: args.append("%s=%08x" % (g, dw(tab[vt][0])))
    r = subprocess.run([sys.executable, T, os.environ["FABLE_EXE"], o] + args, capture_output=True, text=True, cwd="/home/user/FableDecomp")
    for x in (r.stdout + r.stderr[-300:]).splitlines(): print(a + "\t" + x)
    if not args: print(a, "NOARGS")
