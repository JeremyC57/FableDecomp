#ifndef FABLE_ENGINE_CTCTARGETINGAI_H
#define FABLE_ENGINE_CTCTARGETINGAI_H
#ifdef FABLE_RETAIL_CTCTARGETINGAI_ATTACK
// Original complete PDB records in both Ego_r and FableWin; native007f6200 layout/calls.
#include "engine/CTCTargetingBase.h"
#include "engine/CIntelligentPointer.h"
#include "stlport/fable_containers.h"
class CTCTargetingAI : public CTCTargetingBase {
public:
 static CTCBase * FABLE_FASTCALL Alloc(CThing & p0);
 static CCharString FABLE_FASTCALL StatGetClassTypeName();
 virtual CCharString GetClassTypeName() const;
 CTCTargetingAI(CTCTargetingAI const & p0);
 CTCTargetingAI(CThing & p0);
 virtual void FrameUpdate();
 virtual void BulletTimeFrameUpdate();
 virtual bool HasFrameUpdate() const;
 virtual CThingCreatureBase * GetPBestEnemyCreatureForAttack() const;
 virtual CThingCreatureBase * GetPBestCreatureForInteractionTypeWithinRangeLimit(ECreatureInteractionType p0, float p1, float p2) const;
 virtual CThingCreatureBase * GetPBestCreatureForInteractionType(ECreatureInteractionType p0) const;
 void SetScriptTargetOverride(CThingCreatureBase * p0);
 CThingCreatureBase const * GetScriptTargetOverride() const;
 bool HasScriptTargetOverride() const;
 void AddThingToNotAttack(CThing & p0);
 void ClearAllThingsToNotAttack();
 bool IsThingValidTarget(CThing const & p0) const;
 bool HasThingsToNotAttack() const;
 void UpdatePBestEnemyCreatureForAttack(bool p0);
private:
 CIntelligentPointer<CThingCreatureBase> PBestEnemyCreatureForAttack;
 CIntelligentPointer<CThingCreatureBase> PScriptTargetOverride;
 std::list<CIntelligentPointer<CThing>,std::allocator<CIntelligentPointer<CThing> > > ThingsToNotAttack;
public:
 virtual ~CTCTargetingAI();
 CTCTargetingAI & operator=(CTCTargetingAI const & p0);
};
FABLE_STATIC_ASSERT(sizeof(CTCTargetingAI)==76);
#else
// CTCTargetingAI (new, chunk 765): RTTI .?AVCTCTargetingAI@@, single base CTCTargetingBase at +0 (ghidra_out/rtti_bases.tsv; engine/
// CTCTargetingBase.h, 56 bytes). Ego_r.pdb members (struct_layouts_egor.tsv, 84 bytes): PBestEnemyCreatureForAttack +0x38,
// PScriptTargetOverride +0x40 (CIntelligentPointer<CThingCreatureBase>), ThingsToNotAttack (list<CIntelligentPointer<CThing> >,
// PDB 12 bytes, retail STLport 4) +0x48: retail ClearAllThingsToNotAttack 0x007F6160 clears +0x48; sizeof 0x4C.
// Name set: ?ClearAllThingsToNotAttack@CTCTargetingAI@@QAEXXZ. No virtual overrides are declared.
#ifndef FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCBASE
#endif
#include "engine/CTCTargetingBase.h"
#include "engine/CIntelligentPointer.h"
#include "stlport/fable_containers.h"
class CThing;
class CTCTargetingAI : public CTCTargetingBase {
public:
    void ClearAllThingsToNotAttack();                                    // 0x007F6160
#ifdef FABLE_RETAIL_CTCTARGETINGAI_SETSCRIPTTARGETOVERRIDE
    // Opt-in (chunk 2017, row 0x00974110 CAIStateGroup_JackOfBladesBase::Summon): Ego_r.pdb / FableWin.pdb
    // ?SetScriptTargetOverride@CTCTargetingAI@@QAEXPAVCThingCreatureBase@@@Z, expanded in retail (0x00974564..0x009745A3: the
    // CIntelligentPointer<CThingCreatureBase> temporary built from the argument, SetPItem on +0x40, the temporary destroyed). Needs
    // FABLE_RETAIL_CINTELLIGENTPOINTER_ASSIGN.
    void SetScriptTargetOverride(CThingCreatureBase* target) { PScriptTargetOverride = target; }
#endif
protected:
    CIntelligentPointer<CThingCreatureBase> PBestEnemyCreatureForAttack; // +0x38
    CIntelligentPointer<CThingCreatureBase> PScriptTargetOverride;       // +0x40
    std::list<CIntelligentPointer<CThing> > ThingsToNotAttack;           // +0x48
};
FABLE_STATIC_ASSERT(sizeof(CTCTargetingAI) == 0x4c);

#endif
#endif
