#ifndef FABLE_ENGINE_CTCDEXPERIENCEORB_H
#define FABLE_ENGINE_CTCDEXPERIENCEORB_H
// Layout and names: both original PDBs; donor026fd4f0 and native007c1000.
#include "engine/CTCDriverBase.h"
#include "engine/CDefPointer.h"
#ifdef FABLE_RETAIL_CTCDEXPERIENCEORB_PASS22_INITIALACTIVATE
#include "engine/CExperienceDef.h"
#endif
class CExperienceDef;
class CTCDExperienceOrb:public CTCDriverBase {public:
 static ETCInterfaceType FABLE_FASTCALL StatGetInterfaceType(){return TCI_EXPERIENCE_ORB;}
 long GetExperiencePoints()const{return ExperiencePoints;}void OnCollected(CThing&);
#ifdef FABLE_RETAIL_CTCDEXPERIENCEORB_STATGETDRIVERTYPE
 // Opt-in (NF107, round 10, row 0x0077ADF0 GetAllTCDsInRoughArea<CTCDExperienceOrb, CFilter_Bind2<CThingFilter_IsNot, CThingFilter_In2DRange>,
 // std::vector<CThing*> >): FableWin.pdb ?StatGetDriverType@CTCDExperienceOrb@@SIKXZ (0x01BA96D3; static fastcall, inline in retail: the
 // instance passes 26 to CTCDriverBase::IsDriverType, `push 1Ah ; call [edx+70h]`), as CTCDNavigationSeed (21) / CTCDRegionExit (25).
 static unsigned long __fastcall StatGetDriverType(){return 26;}
#endif
#ifdef FABLE_RETAIL_CTCDEXPERIENCEORB_PASS22_INITIALACTIVATE
// Retail opt-in (Codex pass22): both PDBs OnInitialActivate virtual and private const GetDef; native vtable slot6.
public: virtual void OnInitialActivate();
private: const CExperienceDef& GetDef() const { return *PDef; }
#endif
private:CDefPointer<const CExperienceDef>PDef;long ExperiencePoints,CombinationImmunityTimer,LifeTimer;bool DecayOverTime,PickedUp;
#ifdef FABLE_RETAIL_CTCDEXPERIENCEORB_DRIVERSLOTS
// agentFAM13 (2026-11-12): CTCDriverBase virtuals this driver overrides but this declaration lacks (TLC vtable slot 28 IsDriverType,
// slot 29 CreateAdditionalTCs, as engine/CTCDriverBase.h); spellings from the Ego_r / FableWin name sets. Declaration only.
public:
    virtual bool IsDriverType(long type) const;
#endif // FABLE_RETAIL_CTCDEXPERIENCEORB_DRIVERSLOTS
};
FABLE_STATIC_ASSERT_AT(18, sizeof(CTCDExperienceOrb)==32);
#endif
