# hb305.py: NF311 batch-1 in-place edits. For each row: HEAD source -> orig/<addr>.cpp, edited -> rows/<addr>.cpp; every
# replacement must hit exactly once and the line count must not change (prints OK / FAIL per row).
import subprocess,sys
A='/tmp/claude-0/-home-user-FableDecomp/5221bbec-fb18-575f-8649-a32d96bb1c2d/scratchpad/w22/agentNF311'
E={
'0094fe30':[('public:\n    EAIActionReturn AIAction_DoNothing(); public:','protected:\n    EAIActionReturn AIAction_DoNothing(); public:')],
'009189c0':[('    CThingGameObject* FindFreeGazePos();        // 0x00918880','    protected: CThingGameObject* FindFreeGazePos(); public:        // 0x00918880')],
'00872050':[('    const CTools& GetCamTools() const;','    protected: const CTools& GetCamTools() const;')],
'009a06b0':[('    void ExtendData(unsigned long newLength);','    protected: void ExtendData(unsigned long newLength); public:')],
'00bdda80':[('bool ClipSphere(const C3DVector& centre, float radius, unsigned long* flags);','bool ClipSphere(const C3DVector& centre, float radius, unsigned long* flags) const;')],
'00b8ff70':[('class CEngineSceneGridCell { public:\n    void InvalidateAll();','class CEngineSceneGridCell { friend class CEngineInternalPrimitiveBase; public:\n    private: void InvalidateAll(); public:')],
'00644179':[('    bool IsOpen();','    bool IsOpen() const;')],
'00892f40':[('    bool IsQuestActive(const CCharString& questName);','    bool IsQuestActive(const CCharString& questName) const;')],
'00730f90':[('    void Open(bool instant);','    protected: void Open(bool instant); public:')],
'006d8b50':[('    void UpdateAbilitiesForStat(EHeroTrainableStatType stat, bool b);','    private: void UpdateAbilitiesForStat(EHeroTrainableStatType stat, bool b);'),
            ('    void OnStatLevelChange(bool b);','    void OnStatLevelChange(bool b); public:')],
'006d7cb0':[('bool IsAbilityPotentiallyAvailable(EHeroAbility ability);','bool IsAbilityPotentiallyAvailable(EHeroAbility ability) const;')],
'0079e3a0':[('    void StopSound(unsigned long sound);','    protected: void StopSound(unsigned long sound); public:')],
'00904ee0':[('    bool IsActionListEmpty();','    bool IsActionListEmpty() const;')],
'00788555':[('    void Reset();  ','    protected: void Reset(); public:  ')],
'008e9080':[('const CAnimationEntry* GetAnimFromName(const CCharString& name, long index) const;','const CAnimationEntry& GetAnimFromName(const CCharString& name, long index) const;'),
            ('creature.GetAnimFromName(animName, 0)->BankIndex','creature.GetAnimFromName(animName, 0).BankIndex')],
'006e9d90':[('    CCombatTypeDef* GetCombatTypeDef() const;','    const CCombatTypeDef& GetCombatTypeDef() const;'),
            ('victim->GetCombatTypeDef()->RotatesCombatRings','victim->GetCombatTypeDef().RotatesCombatRings')],
'006ebee0':[('    CCombatTypeDef* GetCombatTypeDef();','    const CCombatTypeDef& GetCombatTypeDef() const;'),
            ('attacker->GetCombatTypeDef()->m_ringDistance','attacker->GetCombatTypeDef().m_ringDistance')],
}
for a in (sys.argv[1:] or sorted(E)):
    src=subprocess.run(['bash',A+'/tools/srcof.sh',a],capture_output=True,text=True).stdout.strip()
    t=open(src,encoding='utf-8').read(); n=t
    ok=True
    for o,r in E[a]:
        c=n.count(o)
        if c!=1: print('FAIL',a,'count',c,repr(o[:60])); ok=False; break
        n=n.replace(o,r)
    if ok and n.count('\n')!=t.count('\n'): print('FAIL',a,'line delta'); ok=False
    if ok:
        open(A+'/orig/%s.cpp'%a,'w',encoding='utf-8').write(t); open(A+'/rows/%s.cpp'%a,'w',encoding='utf-8').write(n); print('OK',a,src.split('/')[-1])
