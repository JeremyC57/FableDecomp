# ed313.py <group>: the exact NF313 edits to rows/<addr>.cpp (copied from orig/ first when missing); each old string must match once.
import sys,os,shutil
A='/tmp/claude-0/-home-user-FableDecomp/5221bbec-fb18-575f-8649-a32d96bb1c2d/scratchpad/w22/agentNF313'
G={}
# entry 18 unit (NF302 Q5): owner 0x0097E330 IAE self fix + the four QAE binders (NF308's three respells + 0x009689C0)
G['e18']=[
 ('0097e330',"when the creature has that interface\n","when the creature has that interface\n// Leaf: ?SetPushableByHero@CAIStateGroupBase@@IAEX_N@Z\n",0),
 ('0097e330','    void SetPushableByHero(bool pushable);\n};','protected: void SetPushableByHero(bool pushable);      // NF313: IAE (Ego_r.pdb + FableWin.pdb)\n};',0),
 ('00967930','    void SetPushableByHero(bool pushable);                              protected: // 0x0097E330','    protected: void SetPushableByHero(bool pushable);                   protected: // 0x0097E330 (NF313: IAE, both PDBs)',0),
 ('00967b40','    void SetPushableByHero(bool pushable);      protected: // 0x0097E330','    protected: void SetPushableByHero(bool pushable);      protected: // 0x0097E330 (NF313: IAE, both PDBs)',0),
 ('00968bc0','    void SetPushableByHero(bool pushable);      // 0x0097E330','    protected: void SetPushableByHero(bool pushable); public:      // 0x0097E330 (NF313: IAE, both PDBs)',0),
 ('009689c0','    void SetPushableByHero(bool pushable);      // 0x0097E330','    protected: void SetPushableByHero(bool pushable);      // 0x0097E330 (NF313: IAE, both PDBs; the next line is protected: already)',0),
]

# batch A (pointer -> reference / const / unsigned, owner already defines the PDB form): binder-only signature fixes
N=' (NF313: the PDB parameter type, both PDBs)'
G['A']=[
 ('00b242a0','    void CalculateMatrix(CMatrix3x4* matrix) const;   // 0x00ADEBC0','    void CalculateMatrix(CMatrix3x4& matrix) const;   // 0x00ADEBC0'+N,0),
 ('00b242a0','    box.CalculateMatrix(&matrix);','    box.CalculateMatrix(matrix);',0),
 ('00bacbd0','    void CalculateMatrix(CMatrix3x4* matrix) const;   // 0x00ADEBC0','    void CalculateMatrix(CMatrix3x4& matrix) const;   // 0x00ADEBC0'+N,0),
 ('00bacbd0','        box.CalculateMatrix(&transform);','        box.CalculateMatrix(transform);',0),
 ('00bc5fc0','    void CalculateMatrix(CMatrix3x4* matrix) const;   // 0x00ADEBC0','    void CalculateMatrix(CMatrix3x4& matrix) const;   // 0x00ADEBC0'+N,0),
 ('00bc5fc0','        box.CalculateMatrix(&transform);','        box.CalculateMatrix(transform);',0),
 ('00861a80','    void SetMainAnim(CAnimInit* init, long blendTime, long delay);   // 0x0070D580','    void SetMainAnim(const CAnimInit& init, long blendTime, long delay);   // 0x0070D580'+N,0),
 ('00861a80','    animate.SetMainAnim(&init, blendTime, delay);','    animate.SetMainAnim(init, blendTime, delay);',0),
 ('00b3a910','    CEngineInternalPrimitiveBase(long type, long flags);   // 0x00B8FF00','    CEngineInternalPrimitiveBase(long type, unsigned long flags);   // 0x00B8FF00'+N,0),
 ('006cef20','    void SetText(CFontBank* font, const CCharString& text, unsigned long colour, float time, bool a, bool b);   // 0x00492ED1','    void SetText(const CFontBank& font, const CCharString& text, unsigned long colour, float time, bool a, bool b);   // 0x00492ED1'+N,0),
 ('006cef20','.m_subtitleRenderer.SetText(CMainGameComponent::Get()->m_pFontBank,','.m_subtitleRenderer.SetText(*CMainGameComponent::Get()->m_pFontBank,',0),
 ('00729b20','    void SetText(CFontBank* font, const CCharString& text, unsigned long colour, float time, bool a, bool b);   // 0x00492ED1','    void SetText(const CFontBank& font, const CCharString& text, unsigned long colour, float time, bool a, bool b);   // 0x00492ED1'+N,0),
 ('00729b20','ui->Subtitles.SetText(font, text, 4, 0.0f, false, true);','ui->Subtitles.SetText(*font, text, 4, 0.0f, false, true);',0),
 ('00729b20','.Subtitles.SetText(PeekComponent().GetFontBank(), text, 0x322,','.Subtitles.SetText(*PeekComponent().GetFontBank(), text, 0x322,',0),
 ('007800b0','    void SetText(CFontBank* font, const CCharString& text, unsigned long colour, float time, bool a, bool b);   // 0x00492ED1','    void SetText(const CFontBank& font, const CCharString& text, unsigned long colour, float time, bool a, bool b);   // 0x00492ED1'+N,0),
 ('007800b0','.Subtitles.SetText(component->PCutSceneMainFont_Data, text,','.Subtitles.SetText(*component->PCutSceneMainFont_Data, text,',0),
 ('008139c0','    void SetText(CFontBank* font, const CCharString& text, unsigned long colour, float time, bool a, bool b);   // 0x00492ED1','    void SetText(const CFontBank& font, const CCharString& text, unsigned long colour, float time, bool a, bool b);   // 0x00492ED1'+N,0),
 ('008139c0','.SubtitleRenderer.SetText(CMainGameComponent::Get()->Font, "You do not','.SubtitleRenderer.SetText(*CMainGameComponent::Get()->Font, "You do not',0),
 ('00849380','    void SetText(CFontBank* font, const CCharString& text, unsigned long colour, float time, bool a, bool b);   // 0x00492ED1','    void SetText(const CFontBank& font, const CCharString& text, unsigned long colour, float time, bool a, bool b);   // 0x00492ED1'+N,0),
 ('00849380','subtitles->SetText(component->FontBank, message,','subtitles->SetText(*component->FontBank, message,',0),
 ('007df4b0','    void SetText(CFontBank* font, const CCharString& text, const CCharString& a, const CCharString& b, const CCharString& c,\n                 unsigned long colour, float time, bool b1, bool b2);   // 0x00492FB4','    void SetText(const CFontBank& font, const CCharString& text, const CCharString& a, const CCharString& b, const CCharString& c,\n                 unsigned long colour, float time, bool b1, bool b2);   // 0x00492FB4'+N,0),
 ('007df4b0','.SubtitleRenderer.SetText(CMainGameComponent::Get()->Font, question,','.SubtitleRenderer.SetText(*CMainGameComponent::Get()->Font, question,',0),
 ('009d5a90','    unsigned long AddObject(CThreadedFileObjectDesc* desc);              // 0x0098F350','    unsigned long AddObject(const CThreadedFileObjectDesc& desc);        // 0x0098F350'+N,0),
 ('009d5a90','->ThreadedFile->AddObject(&data->LoadDesc);','->ThreadedFile->AddObject(data->LoadDesc);',0),
 ('0088e0b0','    long GetRegionNumberFromName(const CCharString* regionName);','    long GetRegionNumberFromName(const CCharString& regionName) const;   // 0x004FC210 (NF313: the PDB QBE const-reference form, both PDBs)',0),
 ('0088e0b0','->GetRegionNumberFromName(pName);','->GetRegionNumberFromName(*pName);',0),
 ('006ea450','    void PerformSequenceReset(CThingCreatureBase* creature);   // 0x006EA2C0','    private: void PerformSequenceReset(CThingCreatureBase& creature); public:   // 0x006EA2C0 (NF313: AAE, both PDBs)',0),
 ('006ea450','        PerformSequenceReset(attacker);','        PerformSequenceReset(*attacker);',0),
 ('006eabb0','    void PerformSequenceReset(CThingCreatureBase* creature);   // 0x006EA2C0','    private: void PerformSequenceReset(CThingCreatureBase& creature); public:   // 0x006EA2C0 (NF313: AAE, both PDBs)',0),
 ('006eabb0','        PerformSequenceReset(parent);','        PerformSequenceReset(*parent);',2),
]

# batch M (entry line 15, NF300 Q1 + R1): AIAction_MinionWander binders -> the PDB IAE signature (enum return, EMovementType from the
# shared engine/EMovementType.h, CThingBuilding*); the literal 3 -> AI_MOVEMENT_WALKING (= 3)
M='    protected: EAIActionReturn AIAction_MinionWander(EMovementType movementType, CThingBuilding* building); public:   // 0x0097F870 (NF313: IAE, both PDBs)'
G['M']=[
 ('0092a8a0','#include "fable_compiler.h"\n','#include "fable_compiler.h"\n#include "engine/EMovementType.h"   // NF313: the PDB parameter enum of AIAction_MinionWander\n',0),
 ('0092a8a0','enum EAIActionReturn { };\n','enum EAIActionReturn { };\nclass CThingBuilding;                          // NF313: the PDB parameter class of AIAction_MinionWander\n',0),
 ('0092a8a0','    long AIAction_MinionWander(long movementType, CThing* building);   // 0x0097F870',M,0),
 ('0092a8a0','AIAction_MinionWander(3, 0) != 2','AIAction_MinionWander(AI_MOVEMENT_WALKING, 0) != 2',0),
 ('0092b3e0','#include "fable_compiler.h"\n','#include "fable_compiler.h"\n#include "engine/EMovementType.h"   // NF313: the PDB parameter enum of AIAction_MinionWander\n',0),
 ('0092b3e0','enum EAIActionReturn { };\n','enum EAIActionReturn { };\nclass CThingBuilding;                          // NF313: the PDB parameter class of AIAction_MinionWander\n',0),
 ('0092b3e0','    long AIAction_MinionWander(long movementType, CThing* building);   // 0x0097F870',M,0),
 ('0092b3e0','AIAction_MinionWander(3, 0) != 2','AIAction_MinionWander(AI_MOVEMENT_WALKING, 0) != 2',0),
]
def run(g):
    for a,o,n,first in G[g]:
        p=A+'/rows/%s.cpp'%a
        if not os.path.exists(p): shutil.copy(A+'/orig/%s.cpp'%a,p)
        t=open(p,encoding='utf-8').read(); c=t.count(o)
        if first and t.startswith(o) and c>=1: t=n+t[len(o):]; open(p,'w',encoding='utf-8').write(t); print('OK',a,'(start)'); continue
        if first==2 and c>=1: open(p,'w',encoding='utf-8').write(t.replace(o,n)); print('OK',a,'x%d'%c); continue
        if c!=1: print('MISS',c,a,repr(o[:70])); continue
        open(p,'w',encoding='utf-8').write(t.replace(o,n)); print('OK',a)
if __name__=='__main__': run(sys.argv[1])
