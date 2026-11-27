import sys,re
X=sys.argv[1]; M=X+"/inc_merge/"
def hunks(txt):
    L=txt.split("\n"); out=[]; i=0
    while i<len(L):
        if L[i].startswith("<<<<<<< "):
            j=L.index("=======",i); k=next(n for n in range(j,len(L)) if L[n].startswith(">>>>>>> "))
            out.append(("C",L[i+1:j],L[j+1:k])); i=k+1
        else: out.append(("L",L[i])); i+=1
    return out
def tail_to_endif(H,idx):
    t=[]
    for h in H[idx+1:]:
        if h[0]!="L": break
        t.append(h[1])
        if h[1].startswith("#endif"): return t
    raise SystemExit("no endif tail")
def generic(f):
    H=hunks(open(M+f+".CONFLICT").read()); out=[]
    for n,h in enumerate(H):
        if h[0]=="L": out.append(h[1]); continue
        ours,theirs=h[1],h[2]; tail=tail_to_endif(H,n)
        if not theirs: out+= tail+ours
        else: out+= ours+tail+theirs
    open(M+f,"w").write("\n".join(out)); print("resolved",f)
for f in ["engine/CQuestManager.h","engine/CTCHeroOpinionDeedLog.h","engine/CWorld.h","engine/CAIStateGroup_MinionReactToPlayerInteraction.h"]: generic(f)
# CEnginePrimitiveHandle: one #if head for both declaration-only Clear opt-ins
f="engine/CEnginePrimitiveHandle.h"; H=hunks(open(M+f+".CONFLICT").read()); out=[]
for h in H:
    if h[0]=="L": out.append(h[1]); continue
    ours=h[1]; assert ours[0].startswith("#if defined(FABLE_RETAIL_CENGINEPRIMITIVEHANDLE_CLEAR_DECL_ONLY)")
    out.append("#if defined(FABLE_RETAIL_CENGINEPRIMITIVEHANDLE_CLEAR_DECL_ONLY) || defined(FABLE_RETAIL_ICONTEXT_CLEAR_OUTOFLINE_PASS29)")
    out+= ours[1:-1]+["    // Codex pass 29 (FABLE_RETAIL_ICONTEXT_CLEAR_OUTOFLINE_PASS29, CIconText rows): the same declaration-only Clear."]+[ours[-1]]
open(M+f,"w").write("\n".join(out)); print("resolved",f)
# CTCCreatureGenerator: keep both; Codex's AddCreatureTriggerer is guarded so a TU with both opt-ins declares it once
f="engine/CTCCreatureGenerator.h"; H=hunks(open(M+f+".CONFLICT").read()); out=[]
for n,h in enumerate(H):
    if h[0]=="L": out.append(h[1]); continue
    ours,theirs=h[1],h[2]; tail=tail_to_endif(H,n)
    assert theirs[1].strip()=="void AddCreatureTriggerer(CThingCreatureBase&);"
    out+= ours+tail+[theirs[0],"#if !defined(FABLE_RETAIL_CTCCREATUREGENERATOR_SCRIPTTRIGGERERS)",theirs[1],"#endif"]+theirs[2:]
open(M+f,"w").write("\n".join(out)); print("resolved",f)
# MinionListenToStory: Codex's minimal view becomes an #elif of the retail view chain; the CTOR #error check is ours
f="engine/CAIStateGroup_MinionListenToStory.h"; H=hunks(open(M+f+".CONFLICT").read()); out=[]; c=0
for h in H:
    if h[0]=="L": out.append(h[1]); continue
    c+=1; ours,theirs=h[1],h[2]
    if c==1:
        assert theirs[0]=="#ifdef FABLE_RETAIL_MINION_LISTEN_STORY_PASS29"
        out+= ours+["#elif defined(FABLE_RETAIL_MINION_LISTEN_STORY_PASS29)","// Codex pass 29 view (row 0x009101C0 Update): only the members that row needs."]+theirs[1:]
    else: out+= ours
open(M+f,"w").write("\n".join(out)); print("resolved",f)
