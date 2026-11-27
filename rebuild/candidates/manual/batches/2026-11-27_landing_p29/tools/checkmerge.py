import sys,re
sys.path.insert(0,sys.argv[1]+"/tools"); from ifeval import run
X=sys.argv[1]; HEAD="/home/user/FableDecomp/rebuild/include/"
def macros(p): return set(re.findall(r"\b(FABLE_\w+)\b",open(p).read()))
for f in ["engine/CQuestManager.h","engine/CTCHeroOpinionDeedLog.h","engine/CWorld.h","engine/CAIStateGroup_MinionReactToPlayerInteraction.h","engine/CEnginePrimitiveHandle.h","engine/CTCCreatureGenerator.h","engine/CAIStateGroup_MinionListenToStory.h"]:
    mo,mt,mm=HEAD+f,X+"/inc_base/"+f,X+"/inc_merge/"+f
    O,T=macros(mo),macros(mt); onlyT=T-O; onlyO=O-T; common={"FABLE_RETAIL"}  # common umbrella flags
    res=[]
    for name,D,ref in (("none",set(),None),("ours",O-onlyT,mo),("codex",T-onlyO,mt)):
        a=run(mm,D)
        if ref is None: ok = a==run(mo,D)==run(mt,D) or a==run(mo,D)
        else: ok = a==run(ref,D)
        res.append(f"{name}:{'OK' if ok else 'DIFF'}")
    print(f, " ".join(res))
