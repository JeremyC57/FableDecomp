# b1fix.py: NF309 batch B1 -- T1 "other" respells in each binder's own local model (declaration + call sites), line counts kept.
# Each pair is (old, new, expected count). Old names are kept in a trailing comment on the declaration line ("was <old>").
import re
A='/tmp/claude-0/-home-user-FableDecomp/5221bbec-fb18-575f-8649-a32d96bb1c2d/scratchpad/w22/agentNF309/rows/'
def edit(a,pairs):
    p=A+a+'.cpp'; t=open(p,encoding='utf-8').read(); n0=t.count('\n')
    for o,n,k in pairs:
        c=t.count(o); assert c==k,(a,o[:70],c,k); t=t.replace(o,n)
    assert t.count('\n')==n0,(a,t.count('\n'),n0); open(p,'w',encoding='utf-8').write(t); print('ok',a)
E={
'00a99610':[('    CCountedPointer<const C3DAnimFileSequenceChunk> GetFirstSequence() const;   // 0x00AB03D0',
             '    CCountedPointer<const C3DAnimFileSequenceChunk> FindFirstSequence() const;   // 0x00AB03D0 (Ego_r + FableWin; NF309, was GetFirstSequence)',1),
            ('object->GetFirstSequence();','object->FindFirstSequence();',1)],
'00a9f2f0':[('    void CalcLocalBoneTransformationsRelative(C3DKeyframe* out, unsigned long count, float time, const C3DMesh2& mesh) const;   // 0x00A4D770',
             '    void CalcLocalTransformationsRelative(C3DKeyframe* out, unsigned long count, float time, const C3DMesh2& mesh) const;   // 0x00A4D770 (PDB; NF309, was CalcLocalBoneTransformationsRelative)',1),
            ('    void CalcLocalBoneTransformations(C3DKeyframe* out, unsigned long count, float time) const;   // 0x00A4CEC0',
             '    void CalcLocalTransformations(C3DKeyframe* out, unsigned long count, float time) const;   // 0x00A4CEC0 (PDB; NF309, was CalcLocalBoneTransformations)',1),
            ('animation->CalcLocalBoneTransformationsRelative(out, count, time, mesh);','animation->CalcLocalTransformationsRelative(out, count, time, mesh);',1),
            ('animation->CalcLocalBoneTransformations(out, count, time);','animation->CalcLocalTransformations(out, count, time);',1)],
'007864c0':[('    void ApplyParameters();                             // 0x00A45330','    void NotifyParamUpdate();                           // 0x00A45330 (Ego_r + FableWin; NF309, was ApplyParameters)',1),
            ('PSkeletalMorph->ApplyParameters();','PSkeletalMorph->NotifyParamUpdate();',1)],
'00786700':[('    void ResetCombinedMorph();                      // 0x00A45330','    void NotifyParamUpdate();                       // 0x00A45330 (Ego_r + FableWin; NF309, was ResetCombinedMorph)',1),
            ('PSkeletalMorph->ResetCombinedMorph();','PSkeletalMorph->NotifyParamUpdate();',1)],
'00833ba0':[('class CAIBrain { public:\n    void OnLevelUnload();                           // 0x0088B6F0',
             'class CAIBrain { public:\n    void OnKill();                                  // 0x0088B6F0 (Ego_r + FableWin; NF309, was OnLevelUnload)',1),
            ('PBrain_Data->OnLevelUnload();','PBrain_Data->OnKill();',1)],
'0088bac0':[('    bool IsEnabledAndRunnable();                         // 0x008FCF10','    bool IsRunnableMain();                               // 0x008FCF10 (Ego_r + FableWin; NF309, was IsEnabledAndRunnable)',1),
            ('group->IsEnabledAndRunnable())','group->IsRunnableMain())',1)],
'0073e9d0':[('    void SetGroups(const CCharString& groupA, const CCharString& groupB);   // 0x0073E040',
             '    void SetCurrentGroupNames(const CCharString& groupA, const CCharString& groupB);   // 0x0073E040 (FableWin; NF309, was SetGroups)',1),
            ('    SetGroups(groupA, groupB);','    SetCurrentGroupNames(groupA, groupB);',1)],
'0073ea70':[('{ SetGroups(group1, group2); }','{ SetCurrentGroupNames(group1, group2); }',1),
            ('    void SetGroups(const CCharString& group1, const CCharString& group2);   // 0x0073E040',
             '    void SetCurrentGroupNames(const CCharString& group1, const CCharString& group2);   // 0x0073E040 (FableWin; NF309, was SetGroups)',1)],
'0073fdb0':[('        SetGroups(groupA, groupB);','        SetCurrentGroupNames(groupA, groupB);',1),
            ('    void SetGroups(const CCharString& groupA, const CCharString& groupB);   // 0x0073E040',
             '    void SetCurrentGroupNames(const CCharString& groupA, const CCharString& groupB);   // 0x0073E040 (FableWin; NF309, was SetGroups)',1)],
'008d89c0':[('    void SetGroups(const CCharString& group1, const CCharString& group2);   // 0x0073E040',
             '    void SetCurrentGroupNames(const CCharString& group1, const CCharString& group2);   // 0x0073E040 (FableWin; NF309, was SetGroups)',1),
            ('Filter.SetGroups("CROUCH",','Filter.SetCurrentGroupNames("CROUCH",',1)],
'007306c0':[('    void Reset();                               // 0x0070EC30','    void ClearAllAnims();                       // 0x0070EC30 (Ego_r + FableWin; NF309, was Reset)',1),
            ('    void SetMainAnim(const CAnimInit& init);    // 0x0070D920','    void Init(const CAnimInit& init);           // 0x0070D920 (Ego_r + FableWin; NF309, was SetMainAnim)',1),
            ('class CTCSwitchableNavigation { public: void SetBlocking(bool blocking); };   // 0x00787580',
             'class CTCSwitchableNavigation { public: void SetAsBlockingNavigation(bool blocking); };   // 0x00787580 (Ego_r + FableWin; NF309, was SetBlocking)',1),
            ('    animate.Reset();','    animate.ClearAllAnims();',1),('    animate.SetMainAnim(init);','    animate.Init(init);',1),
            ('navigation->SetBlocking(false);','navigation->SetAsBlockingNavigation(false);',1),('navigation->SetBlocking(true);','navigation->SetAsBlockingNavigation(true);',1)],
'007502d0':[('    void Reset();                                              // 0x0070EC30','    void ClearAllAnims();                                      // 0x0070EC30 (Ego_r + FableWin; NF309, was Reset)',1),
            ('    animator.Reset();','    animator.ClearAllAnims();',1)],
'00730c00':[('class CTCSwitchableNavigation { public: void SetBlocking(bool blocking); };   // 0x00787580',
             'class CTCSwitchableNavigation { public: void SetAsBlockingNavigation(bool blocking); };   // 0x00787580 (Ego_r + FableWin; NF309, was SetBlocking)',1),
            ('navigation->SetBlocking(false);','navigation->SetAsBlockingNavigation(false);',1)],
'00730dd0':[('class CTCSwitchableNavigation { public: void SetBlocking(bool blocking); };   // 0x00787580',
             'class CTCSwitchableNavigation { public: void SetAsBlockingNavigation(bool blocking); };   // 0x00787580 (Ego_r + FableWin; NF309, was SetBlocking)',1),
            ('navigation->SetBlocking(true);','navigation->SetAsBlockingNavigation(true);',1)],
'009cb960':[('    CWideString GetRetailBankPath(const CCharString& bankHandle);   // 0x009A7D40',
             '    CWideString GetRetailBankFilePath(const CCharString& bankHandle);   // 0x009A7D40 (Ego_r + FableWin; NF309, was GetRetailBankPath)',1),
            ('    void RegisterRetailBank(const CCharString& bankHandle, CBankFile* bank);   // 0x009A8090',
             '    void RegisterBank(const CCharString& bankHandle, CBankFile* bank);   // 0x009A8090 (Ego_r + FableWin; NF309, was RegisterRetailBank)',1),
            ('g_bankFileManager.GetRetailBankPath(bankHandle);','g_bankFileManager.GetRetailBankFilePath(bankHandle);',1),
            ('g_bankFileManager.RegisterRetailBank(bankHandle, this);','g_bankFileManager.RegisterBank(bankHandle, this);',1)],
'005c024a':[('    void SetOwner(CThing* owner);                                 // 0x005BD183','    void SetCategoryThingAssociation(CThing* owner);              // 0x005BD183 (Ego_r + FableWin; NF309, was SetOwner)',1),
            ('Categories.back().SetOwner(owner);','Categories.back().SetCategoryThingAssociation(owner);',1)],
}
for a in ('0073fea0','009380e0','009381b0'):
    E[a]=[('CAnimFilterOf2Groups(const CCharString& a,const CCharString& b){SetGroups(a,b);}','CAnimFilterOf2Groups(const CCharString& a,const CCharString& b){SetCurrentGroupNames(a,b);}',1),
          ('void SetGroups(const CCharString&,const CCharString&);','void SetCurrentGroupNames(const CCharString&,const CCharString&); // 0x0073E040 (FableWin; NF309, was SetGroups)',1)]
for a,p in E.items(): edit(a,p)
