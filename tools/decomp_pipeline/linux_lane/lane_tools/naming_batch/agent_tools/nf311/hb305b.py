# hb305b.py: NF311 batch-2/3 in-place edits (sibling binders of the batch-1 stand-ins, three owner self fixes, 00788555's second
# stand-in). Base = rows/<addr>.cpp when it already exists (batch 1), else the HEAD source (saved to orig/). Each replacement hits
# exactly once; the line delta must equal the listed one (owners gain a full mangled // Leaf: line).
import subprocess,sys,os
A='/tmp/claude-0/-home-user-FableDecomp/5221bbec-fb18-575f-8649-a32d96bb1c2d/scratchpad/w22/agentNF311'
GAZE=('    CThingGameObject* FindFreeGazePos();','    protected: CThingGameObject* FindFreeGazePos(); public:')
E={
'00918900':(0,[GAZE]),'00918ac0':(0,[GAZE]),'00918b50':(0,[GAZE]),
'0064e019':(0,[('    bool IsOpen();','    bool IsOpen() const;')]),
'007887be':(0,[('    bool IsOpen();','    bool IsOpen() const;'),('    void Reset();','    protected: void Reset(); public:')]),
'00788555':(0,[('    bool IsOpen();','    bool IsOpen() const;')]),
'004b4490':(0,[('    bool IsQuestActive(const CCharString& name);','    bool IsQuestActive(const CCharString& name) const;')]),
'00904380':(0,[('    bool IsActionListEmpty();','    bool IsActionListEmpty() const;')]),
'00752640':(0,[('const CAnimationEntry* GetAnimFromName(const CCharString& name, long index) const;','const CAnimationEntry& GetAnimFromName(const CCharString& name, long index) const;'),
               ('CAnimEntryLite(*PParentCreature->GetAnimFromName(ST_HEAD_TURN_V, 0))','CAnimEntryLite(PParentCreature->GetAnimFromName(ST_HEAD_TURN_V, 0))'),
               ('CAnimEntryLite(*PParentCreature->GetAnimFromName(ST_HEAD_TURN_H, 0))','CAnimEntryLite(PParentCreature->GetAnimFromName(ST_HEAD_TURN_H, 0))')]),
'006ea2c0':(0,[('    CCombatTypeDef* GetCombatTypeDef() const;','    const CCombatTypeDef& GetCombatTypeDef() const;'),('creature.GetCombatTypeDef()->GetSubDef(&rotation)','creature.GetCombatTypeDef().GetSubDef(&rotation)')]),
'00858cc0':(0,[('    CCombatTypeDef* GetCombatTypeDef();','    const CCombatTypeDef& GetCombatTypeDef() const;'),('->GetCombatTypeDef()->BlockedResponse','->GetCombatTypeDef().BlockedResponse')]),
'0097ef60':(0,[('    CCombatTypeDef* GetCombatTypeDef();','    const CCombatTypeDef& GetCombatTypeDef() const;'),('GetCreature()->GetCombatTypeDef()->GetSubDef(&indicator)','GetCreature()->GetCombatTypeDef().GetSubDef(&indicator)')]),
'0097f3a0':(0,[('    CCombatTypeDef* GetCombatTypeDef();','    const CCombatTypeDef& GetCombatTypeDef() const;'),('GetCreature()->GetCombatTypeDef()->GetSubDef(&indicator)','GetCreature()->GetCombatTypeDef().GetSubDef(&indicator)')]),
'006d8ca0':(0,[('    void OnStatLevelChange(bool silent);','    private: void OnStatLevelChange(bool silent); public:')]),
# owner self fixes (NF302 Q5: access only, every landed binder of the old spelling moves in this landing)
'00918880':(1,[GAZE,('length 116 bytes (+ int3 padding).\n','length 116 bytes (+ int3 padding).\n// Leaf: ?FindFreeGazePos@CAIStateGroup_Gaze@@IAEPAVCThingGameObject@@XZ\n')]),
'006d8170':(0,[('    void UpdateAbilitiesForStat(EHeroTrainableStatType stat, bool silent);','    private: void UpdateAbilitiesForStat(EHeroTrainableStatType stat, bool silent); public:'),
               ('// Leaf: UpdateAbilitiesForStat\n','// Leaf: ?UpdateAbilitiesForStat@CTCHeroExperience@@AAEXW4EHeroTrainableStatType@@_N@Z\n')]),
'006d83b0':(1,[('    void OnStatLevelChange(bool checkExpressions);','    private: void OnStatLevelChange(bool checkExpressions); public:')]),
}
def leaf_after_line1(a,t):   # 006d83b0: insert the Leaf line after the first comment line
    i=t.index('\n')+1; return t[:i]+'// Leaf: ?OnStatLevelChange@CTCHeroExperience@@AAEX_N@Z\n'+t[i:]
for a in (sys.argv[1:] or sorted(E)):
    d,reps=E[a]
    src=subprocess.run(['bash',A+'/tools/srcof.sh',a],capture_output=True,text=True).stdout.strip()
    head=open(src,encoding='utf-8').read()
    base=open(A+'/rows/%s.cpp'%a,encoding='utf-8').read() if os.path.exists(A+'/rows/%s.cpp'%a) else head
    n=base; ok=True
    for o,r in reps:
        c=n.count(o)
        if c!=1: print('FAIL',a,'count',c,repr(o[:60])); ok=False; break
        n=n.replace(o,r)
    if ok and a=='006d83b0': n=leaf_after_line1(a,n)
    if ok and n.count('\n')-head.count('\n')!=d: print('FAIL',a,'line delta',n.count('\n')-head.count('\n')); ok=False
    if ok:
        if not os.path.exists(A+'/orig/%s.cpp'%a): open(A+'/orig/%s.cpp'%a,'w',encoding='utf-8').write(head)
        open(A+'/rows/%s.cpp'%a,'w',encoding='utf-8').write(n); print('OK',a,src.split('/')[-1])
