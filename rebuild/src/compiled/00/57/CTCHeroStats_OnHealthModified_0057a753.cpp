// CTCHeroStats::OnHealthModified() -- restart the health-refill delay: FramesUntilHealthRefills = ConstantFPS * the hero-stats def's NumberOfSecondsAfterChangeHealthStartsRefilling
// Leaf: ?OnHealthModified@CTCHeroStats@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x0057A753.
// Identity: Ego_r.pdb + FableWin.pdb ?OnHealthModified@CTCHeroStats@@QAEXXZ (public). Evidence: Ego_r donor order GetMoralityMax 0x0056DA4B /
// OnHealthModified 0x0056DB3E / IsDrunk 0x0056DE4A = retail 0x0057A67E / 0x0057A753 / 0x0057A874; CThingPhysical::InternalModifyHealth
// 0x006A62E0 calls it on interface 4 (CTCHeroStats) after a health change; the body writes FramesUntilHealthRefills +0x64 from
// NumberOfSecondsAfterChangeHealthStartsRefilling +0x44 (the stamina twin ForceStaminaTo 0x0057B26F writes FramesUntilStaminaRefills
// +0x60 from NumberOfSecondsAfterUseStaminaStartsRefilling +0x40). The catalog's CTCHeroExperience::AddSuccessfulBlockParry is
// propagated, and 0x006A62E0's ?OnHealthChanged@CTCHeroHealthObserver@@QAEXXZ is invented (NAME_FIXES.tsv).
// Callees: CTCBase::GetDefinitionManager 0x006869D0; CGameDefinitionManager::PeekHeroStatsDef (inline, +0xDC);
// CMainGameComponent::ConstantFPS 0x01375550 (protected static, through the inline GetConstantFPS).
// Name fix (agentNF8 2026-11-11): was the free `void __fastcall CTCHeroExperience_AddSuccessfulBlockParry(CTCHeroExperience*)` with
// an invented `GetBigObj` callee; now the PDB member on the shared headers. See RENAMES.tsv.
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CTCHEROSTATS
#define FABLE_RETAIL_CTCHEROSTATS_SERIALISE
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_STLP_VECTOR
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_CGAMEDEFINITIONMANAGER_PEEKHEROSTATSDEF
#define FABLE_RETAIL_CGAMEDEFINITIONMANAGER_HEROSTATSDEF
#define FABLE_RETAIL_CGAMEDEFINITIONMANAGER
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCHEROSTATS_ONHEALTHMODIFIED
#include "engine/CMainGameComponent.h"
#include "stlport/fable_containers.h"
#include "stlport/fable_map.h"
#include "fable_string.h"
#include "engine/CTCHeroStats.h"
#include "engine/CHeroStatsDef.h"
#include "engine/CGameDefinitionManager.h"

void CTCHeroStats::OnHealthModified()
{
    FramesUntilHealthRefills = (long)((float)CMainGameComponent::GetConstantFPS() * GetDefinitionManager().PeekHeroStatsDef().NumberOfSecondsAfterChangeHealthStartsRefilling);
}
