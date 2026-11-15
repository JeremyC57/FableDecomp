// CGameDefinitionManager::InitAndCompile() -- build the game's definition search paths (bank headers, def headers, templates and
//   .def files), register the 138 game definition classes with their Alloc functions, register the animation-entry and physical-
//   primitive stat classes, store the path lists, compile every definition and cache the global / engine / environment / hero-stats defs
// Leaf: ?InitAndCompile@CGameDefinitionManager@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0044C72B, 8995 bytes (vtable 0x01232C24 slot 2; thiscall, plain `ret`).
// Identity (proven): Ego_r.pdb + FableWin.pdb ?InitAndCompile@CGameDefinitionManager@@UAEXXZ; vtable 0x01232C24 slot 2 (only xref).
// Callees: CWideString(const unsigned short*) 0x0099B6B0, ~CWideString 0x0099B510, NBankFileManager::GetHeaderDir 0x009A76A0,
//   NResourceDirectoryNames::GetDefDir 0x0041A080, operator+(const CWideString&, const unsigned short*) 0x0099BF30 / (CWideString,
//   CWideString) 0x0099BE70, vector<CWideString>::push_back 0x004128A0 / operator= 0x00433DF0 / ~vector 0x00412130, CCharString
//   0x0099EBF0 / copy 0x0099EC30 / dtor 0x0099EAE0, CDefinitionManager::AddDefClass 0x009B0AC0 (138x, CDefClassInit inline ctor
//   expanded), CAnimationEntry::StatRegisterClass 0x005DD670, CPhysicalPrimitiveInit::StatRegisterClass 0x0067F9B0,
//   NProgressDisplay::DisplayProgress 0x009E9F40, CDefinitionManager::Compile 0x009B08C0, GetDef<CGlobalDef> 0x0043FEB0,
//   <CEngineDef> 0x00430096, <CEnvironmentDef> 0x00451354, <CHeroStatsDef> 0x004513B9.
// TU: `optimize("s", on)` + `("y", off)` (same-TU ctor 0x0044C6C2, sibling CStartupDefinitionManager 0x004336BC); no /EHsc.
// Alloc functions: only their addresses are taken; each is ?Alloc@<Class>@@SIPAVCDefClassBase@@XZ (Ego_r.pdb / FableWin.pdb).
// Models: the 85 registered classes that have a shared header come from engine/<Class>.h (NUISystem_<Class>.h for three UI defs), each
//   declaring its static Alloc: 16 headers already do (8 by default, 8 under existing opt-ins enabled below; CTHINGBUILDINGDEF selects
//   the retail branch, and the 3 CCombatAbility*Data opt-ins satisfy the combat-ability headers' layout asserts), 69 under a new additive FABLE_RETAIL_<CLASS>_ALLOC opt-in
//   (wfE11 overlay, hdr/README.md). The 52 registered classes with no header anywhere keep the registration-interface-only
//   declaration of the landed CTCBase::RegisterTCs 0x004EE23F. Extern symbol set identical to the all-local model (162 symbols).
// Spellings: no FITTED item and no device; include order is free (alphabetical and registration order both grade RELOCATION_MATCH).
#pragma optimize("s", on)
#pragma optimize("y", off)
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CDEFINITIONMANAGER_ADDDEFCLASS
#define FABLE_RETAIL_CDEFINITIONMANAGER_COMPILE
#define FABLE_RETAIL_CDEFINITIONMANAGER_PATHLISTS
#define FABLE_RETAIL_STLP_VECTOR_ASSIGN
#define FABLE_RETAIL_CGAMEDEFINITIONMANAGER
#define FABLE_RETAIL_CGAMEDEFINITIONMANAGER_DEFPOINTERS
// Definition-class headers: the opt-ins that declare each registered class's static Alloc (existing ones, then wfE11 *_ALLOC additions).
#define FABLE_RETAIL_CTHINGBUILDINGDEF
#define FABLE_RETAIL_CTHINGOBJECTDEF_VALIDATE
#define FABLE_RETAIL_CDEGRADABLEDEF_ORIGINAL_FUNCTORS
#define FABLE_RETAIL_SKYBEAM_ORIGINAL
#define FABLE_RETAIL_CSIMBUILDINGDEF
#define FABLE_RETAIL_CENVIRONMENTDEF_PARENT
#define FABLE_RETAIL_CENVIRONMENTTHEMEDAYSETDEF
#define FABLE_RETAIL_CSPECIALABILITIESFIREBALLSPELLDEF_MEMBERS
#define FABLE_RETAIL_CJACKDRAGONDEF
#define FABLE_RETAIL_CCOMBATABILITYDATA
#define FABLE_RETAIL_CCOMBATABILITYBLOCKINGDATA
#define FABLE_RETAIL_CCOMBATABILITYPROJECTILEATTACKDATA
#define FABLE_RETAIL_CACTIONUSEDEF_ALLOC
#define FABLE_RETAIL_CAPPEARANCEDEF_ALLOC
#define FABLE_RETAIL_CAPPEARANCEMODIFIERDEF_ALLOC
#define FABLE_RETAIL_CARMOURDEF_ALLOC
#define FABLE_RETAIL_CAUGMENTATIONDEF_ALLOC
#define FABLE_RETAIL_CBONUSITEMDEF_ALLOC
#define FABLE_RETAIL_CBRIARROSEDEF_ALLOC
#define FABLE_RETAIL_CCAMERAMANAGERSETDEF_ALLOC
#define FABLE_RETAIL_CCARRYSLOTDEF_ALLOC
#define FABLE_RETAIL_CCOMBATABILITYBLOCKCOUNTERATTACKDEF_ALLOC
#define FABLE_RETAIL_CCOMBATABILITYBLOCKHEAVYWEAPONATTACKDEF_ALLOC
#define FABLE_RETAIL_CCOMBATABILITYBLOCKLIGHTWEAPONATTACKDEF_ALLOC
#define FABLE_RETAIL_CCOMBATABILITYBLOCKPROJECTILEWEAPONATTACKDEF_ALLOC
#define FABLE_RETAIL_CCOMBATABILITYBLOCKUNARMEDATTACKDEF_ALLOC
#define FABLE_RETAIL_CCOMBATABILITYFLOURISHCOUNTERATTACKDEF_ALLOC
#define FABLE_RETAIL_CCOMBATABILITYGETHITCOUNTERATTACKDEF_ALLOC
#define FABLE_RETAIL_CCOMBATABILITYSTRAFEDEF_ALLOC
#define FABLE_RETAIL_CCOMBATABILITYUSEPROJECTILEWEAPONDEF_ALLOC
#define FABLE_RETAIL_CCOMBATTYPEDEF_ALLOC
#define FABLE_RETAIL_CCONTEXTSENSITIVEITEMDEF_ALLOC
#define FABLE_RETAIL_CCREATUREDEF_ALLOC
#define FABLE_RETAIL_CCREATUREGENERATIONFAMILYDEF_ALLOC
#define FABLE_RETAIL_CCREATURESTATSDEF_ALLOC
#define FABLE_RETAIL_CDRAGONACTIONSWOOPDEF_ALLOC
#define FABLE_RETAIL_CENGINEDEF_ALLOC
#define FABLE_RETAIL_CENGINEVIDEOOPTIONSDEF_ALLOC
#define FABLE_RETAIL_CENTITYSOUNDDEF_ALLOC
#define FABLE_RETAIL_CEXPLOSIVETRAILDEF_ALLOC
#define FABLE_RETAIL_CHAIRCARDDEF_ALLOC
#define FABLE_RETAIL_CHEROABILITYDEF_ALLOC
#define FABLE_RETAIL_CHEROCOMBATDEF_ALLOC
#define FABLE_RETAIL_CHEROMARRIAGEDEF_ALLOC
#define FABLE_RETAIL_CHEROSTATSDEF_ALLOC
#define FABLE_RETAIL_CHEROTITLEDEF_ALLOC
#define FABLE_RETAIL_CINVENTORYDEF_ALLOC
#define FABLE_RETAIL_CINVENTORYITEMDEF_ALLOC
#define FABLE_RETAIL_CLIGHTNINGDEF_ALLOC
#define FABLE_RETAIL_CMATERIALDEF_ALLOC
#define FABLE_RETAIL_CMESSAGEEVENTDEF_ALLOC
#define FABLE_RETAIL_CPHYSICSDEF_ALLOC
#define FABLE_RETAIL_CPLAYERGUIDEF_ALLOC
#define FABLE_RETAIL_CPLAYERMOVEMENTDEF_ALLOC
#define FABLE_RETAIL_CREGIONDEF_ALLOC
#define FABLE_RETAIL_CSKYDEF_ALLOC
#define FABLE_RETAIL_CSMASHABLEDEF_ALLOC
#define FABLE_RETAIL_CSNOWTROLLDEF_ALLOC
#define FABLE_RETAIL_CSOUNDDEF_ALLOC
#define FABLE_RETAIL_CSPECIALABILITIESBERSERKDEF_ALLOC
#define FABLE_RETAIL_CSPECIALABILITIESBULLETTIMEDEF_ALLOC
#define FABLE_RETAIL_CSPECIALABILITIESDIVINEWRATHDEF_ALLOC
#define FABLE_RETAIL_CSPECIALABILITIESENFLAMEDEF_ALLOC
#define FABLE_RETAIL_CSPECIALABILITIESFORCEPUSHDEF_ALLOC
#define FABLE_RETAIL_CSPECIALABILITIESMULTISTRIKEDEF_ALLOC
#define FABLE_RETAIL_CSPECIALABILITIESPHYSICALSHIELDDEF_ALLOC
#define FABLE_RETAIL_CSPECIALABILITIESTURNCOATSPELLDEF_ALLOC
#define FABLE_RETAIL_CSPECIALABILITIESUNHOLYPOWERDEF_ALLOC
#define FABLE_RETAIL_CSUMMONERDEF_ALLOC
#define FABLE_RETAIL_CTARGETINGDEF_ALLOC
#define FABLE_RETAIL_CTATTOODEF_ALLOC
#define FABLE_RETAIL_CTHINGBASEDEF_ALLOC
#define FABLE_RETAIL_CTHINGBUILDINGDEF_ALLOC
#define FABLE_RETAIL_CTHINGCREATUREDEF_ALLOC
#define FABLE_RETAIL_CTHINGHOLYSITEDEF_ALLOC
#define FABLE_RETAIL_CTHINGMARKERDEF_ALLOC
#define FABLE_RETAIL_CTHINGMULTIARROWSHOTDEF_ALLOC
#define FABLE_RETAIL_CTHINGPHYSICALSWITCHDEF_ALLOC
#define FABLE_RETAIL_CTHINGSHOTDEF_ALLOC
#define FABLE_RETAIL_CTHINGVILLAGEDEF_ALLOC
#define FABLE_RETAIL_CUIDEF_ALLOC
#include "fable_string.h"
#include "stlport/fable_containers.h"
#include "engine/CGameDefinitionManager.h"
#include "engine/CDefClassInit.h"
#include "engine/CAnimationEntry.h"
#include "engine/CPhysicalPrimitiveInit.h"
#include "engine/NProgressDisplay.h"
// The registered definition classes that have a shared header, in alphabetical order (each declares its static Alloc under the opt-ins above).
#include "engine/CActionUseDef.h"
#include "engine/CAppearanceDef.h"
#include "engine/CAppearanceModifierDef.h"
#include "engine/CArmourDef.h"
#include "engine/CAugmentationDef.h"
#include "engine/CBonusItemDef.h"
#include "engine/CBriarRoseDef.h"
#include "engine/CCameraManagerSetDef.h"
#include "engine/CCarrySlotDef.h"
#include "engine/CCombatAbilityBlockCounterAttackDef.h"
#include "engine/CCombatAbilityBlockHeavyWeaponAttackDef.h"
#include "engine/CCombatAbilityBlockLightWeaponAttackDef.h"
#include "engine/CCombatAbilityBlockProjectileWeaponAttackDef.h"
#include "engine/CCombatAbilityBlockUnarmedAttackDef.h"
#include "engine/CCombatAbilityFlourishCounterAttackDef.h"
#include "engine/CCombatAbilityGetHitCounterAttackDef.h"
#include "engine/CCombatAbilityStrafeDef.h"
#include "engine/CCombatAbilityUseProjectileWeaponDef.h"
#include "engine/CCombatDialogueDef.h"
#include "engine/CCombatTypeDef.h"
#include "engine/CContextSensitiveItemDef.h"
#include "engine/CCreatureDef.h"
#include "engine/CCreatureGenerationFamilyDef.h"
#include "engine/CCreatureStatsDef.h"
#include "engine/CDegradableDef.h"
#include "engine/CDragonActionSwoopDef.h"
#include "engine/CEngineDef.h"
#include "engine/CEngineVideoOptionsDef.h"
#include "engine/CEntitySoundDef.h"
#include "engine/CEnvironmentDef.h"
#include "engine/CEnvironmentThemeDaySetDef.h"
#include "engine/CExplosiveTrailDef.h"
#include "engine/CExpressionDef.h"
#include "engine/CGlobalDef.h"
#include "engine/CHairCardDef.h"
#include "engine/CHeroAbilityDef.h"
#include "engine/CHeroCombatDef.h"
#include "engine/CHeroMarriageDef.h"
#include "engine/CHeroStatsDef.h"
#include "engine/CHeroTitleDef.h"
#include "engine/CHitLocationDef.h"
#include "engine/CInventoryDef.h"
#include "engine/CInventoryItemDef.h"
#include "engine/CJackDragonDef.h"
#include "engine/CLightningDef.h"
#include "engine/CMaterialDef.h"
#include "engine/CMessageEventDef.h"
#include "engine/COpinionDeedEffectsDef.h"
#include "engine/CPhysicsDef.h"
#include "engine/CPlayerGuiDef.h"
#include "engine/CPlayerMovementDef.h"
#include "engine/CRegionDef.h"
#include "engine/CSimBuildingDef.h"
#include "engine/CSkyDef.h"
#include "engine/CSmashableDef.h"
#include "engine/CSnowTrollDef.h"
#include "engine/CSoundDef.h"
#include "engine/CSpecialAbilitiesBerserkDef.h"
#include "engine/CSpecialAbilitiesBulletTimeDef.h"
#include "engine/CSpecialAbilitiesDivineWrathDef.h"
#include "engine/CSpecialAbilitiesEnflameDef.h"
#include "engine/CSpecialAbilitiesFireballSpellDef.h"
#include "engine/CSpecialAbilitiesForcePushDef.h"
#include "engine/CSpecialAbilitiesMultiStrikeDef.h"
#include "engine/CSpecialAbilitiesPhysicalShieldDef.h"
#include "engine/CSpecialAbilitiesThunderLightningStormDef.h"
#include "engine/CSpecialAbilitiesTurncoatSpellDef.h"
#include "engine/CSpecialAbilitiesUnholyPowerDef.h"
#include "engine/CSpecialEffectsDef.h"
#include "engine/CSummonerDef.h"
#include "engine/CTargetingDef.h"
#include "engine/CTattooDef.h"
#include "engine/CThingBaseDef.h"
#include "engine/CThingBuildingDef.h"
#include "engine/CThingCreatureDef.h"
#include "engine/CThingHolySiteDef.h"
#include "engine/CThingMarkerDef.h"
#include "engine/CThingMultiArrowShotDef.h"
#include "engine/CThingObjectDef.h"
#include "engine/CThingPhysicalSwitchDef.h"
#include "engine/CThingShotDef.h"
#include "engine/CThingVillageDef.h"
#include "engine/NUISystem_CUIDef.h"
#include "engine/NUISystem_CUILocaleGraphicsDef.h"
#include "engine/NUISystem_CUIMiscThingsDef.h"

namespace NBankFileManager { CWideString __fastcall GetHeaderDir(); }               // 0x009A76A0
namespace NResourceDirectoryNames { CWideString __fastcall GetDefDir(); }           // 0x0041A080

class CDefClassBase;
#define DEF_CLASS_REGISTRATION_INTERFACE(DefClass) \
    static CDefClassBase* __fastcall Alloc();

class CHeroPostcardGeneratorDef { public: DEF_CLASS_REGISTRATION_INTERFACE(CHeroPostcardGeneratorDef) };   // Alloc 0x0045d613
class CTextureReplacementDef { public: DEF_CLASS_REGISTRATION_INTERFACE(CTextureReplacementDef) };   // Alloc 0x00456397
class CCreatureAbilityDef { public: DEF_CLASS_REGISTRATION_INTERFACE(CCreatureAbilityDef) };   // Alloc 0x00462827
class CThingSwitchDef { public: DEF_CLASS_REGISTRATION_INTERFACE(CThingSwitchDef) };   // Alloc 0x0045948d
class CPlayerDef { public: DEF_CLASS_REGISTRATION_INTERFACE(CPlayerDef) };   // Alloc 0x004628a7
class CThingGroupDef { public: DEF_CLASS_REGISTRATION_INTERFACE(CThingGroupDef) };   // Alloc 0x00462864
class CEngineThemeGroupDef { public: DEF_CLASS_REGISTRATION_INTERFACE(CEngineThemeGroupDef) };   // Alloc 0x004629d2
class CEngineThemeDef { public: DEF_CLASS_REGISTRATION_INTERFACE(CEngineThemeDef) };   // Alloc 0x00462ae9
class CBrainDef { public: DEF_CLASS_REGISTRATION_INTERFACE(CBrainDef) };   // Alloc 0x00462e12
class CPlayerInventoryDef { public: DEF_CLASS_REGISTRATION_INTERFACE(CPlayerInventoryDef) };   // Alloc 0x00462f7d
class CInventoryCategoryDef { public: DEF_CLASS_REGISTRATION_INTERFACE(CInventoryCategoryDef) };   // Alloc 0x0045bd8d
class CEngineLocalDetailGeneratorDef { public: DEF_CLASS_REGISTRATION_INTERFACE(CEngineLocalDetailGeneratorDef) };   // Alloc 0x0045c0f7
class CSimVoicesDef { public: DEF_CLASS_REGISTRATION_INTERFACE(CSimVoicesDef) };   // Alloc 0x0045c50d
class CSoundThemeDef { public: DEF_CLASS_REGISTRATION_INTERFACE(CSoundThemeDef) };   // Alloc 0x0045c5ab
class CThingNoiseDef { public: DEF_CLASS_REGISTRATION_INTERFACE(CThingNoiseDef) };   // Alloc 0x0045c5c1
class CObjectFamilyDef { public: DEF_CLASS_REGISTRATION_INTERFACE(CObjectFamilyDef) };   // Alloc 0x00461329
class CFactionDef { public: DEF_CLASS_REGISTRATION_INTERFACE(CFactionDef) };   // Alloc 0x0045d329
class CControlsDef { public: DEF_CLASS_REGISTRATION_INTERFACE(CControlsDef) };   // Alloc 0x00432180
class CCameraModeDef { public: DEF_CLASS_REGISTRATION_INTERFACE(CCameraModeDef) };   // Alloc 0x0045d4d9
class CCameraManagerDef { public: DEF_CLASS_REGISTRATION_INTERFACE(CCameraManagerDef) };   // Alloc 0x0045d562
class COpinionPersonalityDef { public: DEF_CLASS_REGISTRATION_INTERFACE(COpinionPersonalityDef) };   // Alloc 0x0045e7b9
class COpinionReactionManagerDef { public: DEF_CLASS_REGISTRATION_INTERFACE(COpinionReactionManagerDef) };   // Alloc 0x0044c2e8
class COpinionReactionMaskDef { public: DEF_CLASS_REGISTRATION_INTERFACE(COpinionReactionMaskDef) };   // Alloc 0x0045de51
class COpinionDeedMaskDef { public: DEF_CLASS_REGISTRATION_INTERFACE(COpinionDeedMaskDef) };   // Alloc 0x0045e2d3
class COpinionSourceDef { public: DEF_CLASS_REGISTRATION_INTERFACE(COpinionSourceDef) };   // Alloc 0x0045f15f
class CSpecialAbilitiesAssassinRushDef { public: DEF_CLASS_REGISTRATION_INTERFACE(CSpecialAbilitiesAssassinRushDef) };   // Alloc 0x0045f617
class CSpecialAbilitiesBattleChargeDef { public: DEF_CLASS_REGISTRATION_INTERFACE(CSpecialAbilitiesBattleChargeDef) };   // Alloc 0x0045f6f8
class CSpecialAbilitiesSummonSpellDef { public: DEF_CLASS_REGISTRATION_INTERFACE(CSpecialAbilitiesSummonSpellDef) };   // Alloc 0x004604b4
class CSpecialAbilitiesDrainLifeDef { public: DEF_CLASS_REGISTRATION_INTERFACE(CSpecialAbilitiesDrainLifeDef) };   // Alloc 0x00460116
class CSpecialAbilitiesHealLifeDef { public: DEF_CLASS_REGISTRATION_INTERFACE(CSpecialAbilitiesHealLifeDef) };   // Alloc 0x0046016a
class CSpecialAbilitiesGhostSwordDef { public: DEF_CLASS_REGISTRATION_INTERFACE(CSpecialAbilitiesGhostSwordDef) };   // Alloc 0x004601c1
class CSpecialAbilitiesLightningSpellDef { public: DEF_CLASS_REGISTRATION_INTERFACE(CSpecialAbilitiesLightningSpellDef) };   // Alloc 0x00460439
class CSpecialAbilitiesMultiArrowDef { public: DEF_CLASS_REGISTRATION_INTERFACE(CSpecialAbilitiesMultiArrowDef) };   // Alloc 0x0045f836
class CSpecialAbilitiesBurntEffectDef { public: DEF_CLASS_REGISTRATION_INTERFACE(CSpecialAbilitiesBurntEffectDef) };   // Alloc 0x00460502
class CSpecialAbilitiesCreatureTintDef { public: DEF_CLASS_REGISTRATION_INTERFACE(CSpecialAbilitiesCreatureTintDef) };   // Alloc 0x00460a46
class CSpecialAbilitiesDrunkennessDef { public: DEF_CLASS_REGISTRATION_INTERFACE(CSpecialAbilitiesDrunkennessDef) };   // Alloc 0x004605bc
class CSpecialAbilitiesElectrocutedEffectDef { public: DEF_CLASS_REGISTRATION_INTERFACE(CSpecialAbilitiesElectrocutedEffectDef) };   // Alloc 0x0046056e
class CVillagePeopleDef { public: DEF_CLASS_REGISTRATION_INTERFACE(CVillagePeopleDef) };   // Alloc 0x00453ae0
class CVillagerInteractionsDef { public: DEF_CLASS_REGISTRATION_INTERFACE(CVillagerInteractionsDef) };   // Alloc 0x0045e645
class CMeleeCombatAbilityDef { public: DEF_CLASS_REGISTRATION_INTERFACE(CMeleeCombatAbilityDef) };   // Alloc 0x004625e2
class CAttackPatternDef { public: DEF_CLASS_REGISTRATION_INTERFACE(CAttackPatternDef) };   // Alloc 0x004611c0
class CHitLocationsDef { public: DEF_CLASS_REGISTRATION_INTERFACE(CHitLocationsDef) };   // Alloc 0x00453b92
class CCoopSpiritDef { public: DEF_CLASS_REGISTRATION_INTERFACE(CCoopSpiritDef) };   // Alloc 0x0044f931
class CMeleeCombatKnockdownEffects { public: DEF_CLASS_REGISTRATION_INTERFACE(CMeleeCombatKnockdownEffects) };   // Alloc 0x00462654
class CAbilityDef { public: DEF_CLASS_REGISTRATION_INTERFACE(CAbilityDef) };   // Alloc 0x0044ff53
class CCombatSequenceDef { public: DEF_CLASS_REGISTRATION_INTERFACE(CCombatSequenceDef) };   // Alloc 0x00461ebd
class CDragonActionNapalmDef { public: DEF_CLASS_REGISTRATION_INTERFACE(CDragonActionNapalmDef) };   // Alloc 0x0044fe81
class CDragonActionHoverDef { public: DEF_CLASS_REGISTRATION_INTERFACE(CDragonActionHoverDef) };   // Alloc 0x0044fec7
namespace NUISystem { class CUIIconsDef { public: DEF_CLASS_REGISTRATION_INTERFACE(CUIIconsDef) }; }   // Alloc 0x00432e00
class CGuildMasterDef { public: DEF_CLASS_REGISTRATION_INTERFACE(CGuildMasterDef) };   // Alloc 0x0045006b
class CSpecialAbilitiesDrainLifeDataDef { public: DEF_CLASS_REGISTRATION_INTERFACE(CSpecialAbilitiesDrainLifeDataDef) };   // Alloc 0x0044f85f
class CSpecialAbilitiesForcePushDataDef { public: DEF_CLASS_REGISTRATION_INTERFACE(CSpecialAbilitiesForcePushDataDef) };   // Alloc 0x0044f8a5
class CConfigOptionsDefaultsDef { public: DEF_CLASS_REGISTRATION_INTERFACE(CConfigOptionsDefaultsDef) };   // Alloc 0x00434660

void CGameDefinitionManager::InitAndCompile()
{
    std::vector<CWideString> headers;
    std::vector<CWideString> defs;
    CWideString platform(L"pc\\");
    headers.push_back(NBankFileManager::GetHeaderDir() + L"*.h");
    headers.push_back(NBankFileManager::GetHeaderDir() + platform + L"*.h");
    headers.push_back(NResourceDirectoryNames::GetDefDir() + L"*.h");
    headers.push_back(NResourceDirectoryNames::GetDefDir() + L"*.tpl");
    headers.push_back(NResourceDirectoryNames::GetDefDir() + L"*.def");
    defs.push_back(NResourceDirectoryNames::GetDefDir() + L"*.tpl");
    defs.push_back(NResourceDirectoryNames::GetDefDir() + L"*.def");

    AddDefClass(CDefClassInit("CHeroPostcardGeneratorDef", CHeroPostcardGeneratorDef::Alloc));
    AddDefClass(CDefClassInit("GLOBAL", CGlobalDef::Alloc));
    AddDefClass(CDefClassInit("CREATURE", CThingCreatureDef::Alloc));
    AddDefClass(CDefClassInit("CCreatureDef", CCreatureDef::Alloc));
    AddDefClass(CDefClassInit("CPhysicsDef", CPhysicsDef::Alloc));
    AddDefClass(CDefClassInit("CSmashableDef", CSmashableDef::Alloc));
    AddDefClass(CDefClassInit("CDegradableDef", CDegradableDef::Alloc));
    AddDefClass(CDefClassInit("CSpecialEffectsDef", CSpecialEffectsDef::Alloc));
    AddDefClass(CDefClassInit("CTextureReplacementDef", CTextureReplacementDef::Alloc));
    AddDefClass(CDefClassInit("CTargetingDef", CTargetingDef::Alloc));
    AddDefClass(CDefClassInit("CActionUseDef", CActionUseDef::Alloc));
    AddDefClass(CDefClassInit("CAppearanceDef", CAppearanceDef::Alloc));
    AddDefClass(CDefClassInit("SHOT", CThingShotDef::Alloc));
    AddDefClass(CDefClassInit("CThingMultiArrowShotDef", CThingMultiArrowShotDef::Alloc));
    AddDefClass(CDefClassInit("OBJECT", CThingObjectDef::Alloc));
    AddDefClass(CDefClassInit("BUILDING", CThingBuildingDef::Alloc));
    AddDefClass(CDefClassInit("VILLAGE", CThingVillageDef::Alloc));
    AddDefClass(CDefClassInit("CREATURE_ABILITY", CCreatureAbilityDef::Alloc));
    AddDefClass(CDefClassInit("SWITCH", CThingSwitchDef::Alloc));
    AddDefClass(CDefClassInit("MARKER", CThingMarkerDef::Alloc));
    AddDefClass(CDefClassInit("PHYSICAL_SWITCH", CThingPhysicalSwitchDef::Alloc));
    AddDefClass(CDefClassInit("HOLY_SITE", CThingHolySiteDef::Alloc));
    AddDefClass(CDefClassInit("PLAYER", CPlayerDef::Alloc));
    AddDefClass(CDefClassInit("PLAYER_MOVEMENT", CPlayerMovementDef::Alloc));
    AddDefClass(CDefClassInit("SIM_BUILDING", CSimBuildingDef::Alloc));
    AddDefClass(CDefClassInit("THING_GROUP", CThingGroupDef::Alloc));
    AddDefClass(CDefClassInit("ENGINE_THEME_GROUP", CEngineThemeGroupDef::Alloc));
    AddDefClass(CDefClassInit("ENGINE_THEME", CEngineThemeDef::Alloc));
    AddDefClass(CDefClassInit("BRAIN", CBrainDef::Alloc));
    AddDefClass(CDefClassInit("PLAYER_INVENTORY", CPlayerInventoryDef::Alloc));
    AddDefClass(CDefClassInit("PLAYER_GUI", CPlayerGuiDef::Alloc));
    AddDefClass(CDefClassInit("INVENTORY_CATEGORY", CInventoryCategoryDef::Alloc));
    AddDefClass(CDefClassInit("INVENTORY_ITEM", CInventoryItemDef::Alloc));
    AddDefClass(CDefClassInit("INVENTORY_TYPE", CInventoryDef::Alloc));
    AddDefClass(CDefClassInit("MESSAGE_EVENT", CMessageEventDef::Alloc));
    AddDefClass(CDefClassInit("SKY", CSkyDef::Alloc));
    AddDefClass(CDefClassInit("ENVIRONMENT", CEnvironmentDef::Alloc));
    AddDefClass(CDefClassInit("ENVIRONMENT_THEME_DAY", CEnvironmentThemeDaySetDef::Alloc));
    AddDefClass(CDefClassInit("LOCAL_DETAIL_GENERATOR", CEngineLocalDetailGeneratorDef::Alloc));
    AddDefClass(CDefClassInit("CARRY_SLOT", CCarrySlotDef::Alloc));
    AddDefClass(CDefClassInit("COMBAT_TYPE", CCombatTypeDef::Alloc));
    AddDefClass(CDefClassInit("MATERIAL", CMaterialDef::Alloc));
    AddDefClass(CDefClassInit("ENGINE", CEngineDef::Alloc));
    AddDefClass(CDefClassInit("ENGINE_VIDEO_OPTIONS", CEngineVideoOptionsDef::Alloc));
    AddDefClass(CDefClassInit("SIM_VOICES", CSimVoicesDef::Alloc));
    AddDefClass(CDefClassInit("SOUND_THEME", CSoundThemeDef::Alloc));
    AddDefClass(CDefClassInit("NOISE", CThingNoiseDef::Alloc));
    AddDefClass(CDefClassInit("THING", CThingBaseDef::Alloc));
    AddDefClass(CDefClassInit("CREATURE_GENERATION_FAMILY", CCreatureGenerationFamilyDef::Alloc));
    AddDefClass(CDefClassInit("OBJECT_FAMILY", CObjectFamilyDef::Alloc));
    AddDefClass(CDefClassInit("HERO_ABILITY", CHeroAbilityDef::Alloc));
    AddDefClass(CDefClassInit("HERO_STATS", CHeroStatsDef::Alloc));
    AddDefClass(CDefClassInit("REGION", CRegionDef::Alloc));
    AddDefClass(CDefClassInit("FACTION", CFactionDef::Alloc));
    AddDefClass(CDefClassInit("CONTROL_SCHEME", CControlsDef::Alloc));
    AddDefClass(CDefClassInit("CAppearanceModifierDef", CAppearanceModifierDef::Alloc));
    AddDefClass(CDefClassInit("CEntitySoundDef", CEntitySoundDef::Alloc));
    AddDefClass(CDefClassInit("CAMERA_MODE", CCameraModeDef::Alloc));
    AddDefClass(CDefClassInit("CAMERA_MANAGER", CCameraManagerDef::Alloc));
    AddDefClass(CDefClassInit("CAMERA_MANAGER_SET", CCameraManagerSetDef::Alloc));
    AddDefClass(CDefClassInit("EXPRESSION", CExpressionDef::Alloc));
    AddDefClass(CDefClassInit("OPINION_DEED_EFFECTS", COpinionDeedEffectsDef::Alloc));
    AddDefClass(CDefClassInit("OPINION_PERSONALITY", COpinionPersonalityDef::Alloc));
    AddDefClass(CDefClassInit("OPINION_REACTION_MANAGER", COpinionReactionManagerDef::Alloc));
    AddDefClass(CDefClassInit("OPINION_REACTION_MASK", COpinionReactionMaskDef::Alloc));
    AddDefClass(CDefClassInit("OPINION_DEED_MASK", COpinionDeedMaskDef::Alloc));
    AddDefClass(CDefClassInit("OPINION_SOURCE", COpinionSourceDef::Alloc));
    AddDefClass(CDefClassInit("SPECIAL_ABILITIES_ASSASSIN_RUSH_DEF", CSpecialAbilitiesAssassinRushDef::Alloc));
    AddDefClass(CDefClassInit("SPECIAL_ABILITIES_BATTLE_CHARGE_DEF", CSpecialAbilitiesBattleChargeDef::Alloc));
    AddDefClass(CDefClassInit("SPECIAL_ABILITIES_BERSERK_DEF", CSpecialAbilitiesBerserkDef::Alloc));
    AddDefClass(CDefClassInit("SPECIAL_ABILITIES_BULLET_TIME_DEF", CSpecialAbilitiesBulletTimeDef::Alloc));
    AddDefClass(CDefClassInit("SPECIAL_ABILITIES_FORCE_PUSH_DEF", CSpecialAbilitiesForcePushDef::Alloc));
    AddDefClass(CDefClassInit("SPECIAL_ABILITIES_SUMMON_SPELL_DEF", CSpecialAbilitiesSummonSpellDef::Alloc));
    AddDefClass(CDefClassInit("SPECIAL_ABILITIES_ENFLAME_DEF", CSpecialAbilitiesEnflameDef::Alloc));
    AddDefClass(CDefClassInit("SPECIAL_ABILITIES_PHYSICAL_SHIELD_DEF", CSpecialAbilitiesPhysicalShieldDef::Alloc));
    AddDefClass(CDefClassInit("SPECIAL_ABILITIES_DRAIN_LIFE_DEF", CSpecialAbilitiesDrainLifeDef::Alloc));
    AddDefClass(CDefClassInit("SPECIAL_ABILITIES_HEAL_LIFE_DEF", CSpecialAbilitiesHealLifeDef::Alloc));
    AddDefClass(CDefClassInit("SPECIAL_ABILITIES_GHOST_SWORD_DEF", CSpecialAbilitiesGhostSwordDef::Alloc));
    AddDefClass(CDefClassInit("SPECIAL_ABILITIES_TURNCOAT_SPELL_DEF", CSpecialAbilitiesTurncoatSpellDef::Alloc));
    AddDefClass(CDefClassInit("SPECIAL_ABILITIES_FIREBALL_SPELL_DEF", CSpecialAbilitiesFireballSpellDef::Alloc));
    AddDefClass(CDefClassInit("SPECIAL_ABILITIES_LIGHTNING_SPELL_DEF", CSpecialAbilitiesLightningSpellDef::Alloc));
    AddDefClass(CDefClassInit("SPECIAL_ABILITIES_THUNDER_LIGHTNING_STORM_DEF", CSpecialAbilitiesThunderLightningStormDef::Alloc));
    AddDefClass(CDefClassInit("SPECIAL_ABILITIES_MULTI_ARROW_DEF", CSpecialAbilitiesMultiArrowDef::Alloc));
    AddDefClass(CDefClassInit("SPECIAL_ABILITIES_MULTI_STRIKE_DEF", CSpecialAbilitiesMultiStrikeDef::Alloc));
    AddDefClass(CDefClassInit("SPECIAL_ABILITIES_BURNT_EFFECT_DEF", CSpecialAbilitiesBurntEffectDef::Alloc));
    AddDefClass(CDefClassInit("SPECIAL_ABILITIES_CREATURE_TINT_DEF", CSpecialAbilitiesCreatureTintDef::Alloc));
    AddDefClass(CDefClassInit("SPECIAL_ABILITIES_DRUNKENNESS_DEF", CSpecialAbilitiesDrunkennessDef::Alloc));
    AddDefClass(CDefClassInit("SPECIAL_ABILITIES_ELECTROCUTED_EFFECT_DEF", CSpecialAbilitiesElectrocutedEffectDef::Alloc));
    AddDefClass(CDefClassInit("SPECIAL_ABILITIES_DIVINE_WRATH_DEF", CSpecialAbilitiesDivineWrathDef::Alloc));
    AddDefClass(CDefClassInit("SPECIAL_ABILITIES_UNHOLY_POWER_DEF", CSpecialAbilitiesUnholyPowerDef::Alloc));
    AddDefClass(CDefClassInit("COMBAT_DIALOGUE_DEF", CCombatDialogueDef::Alloc));
    AddDefClass(CDefClassInit("SOUND_SETUP", CSoundDef::Alloc));
    AddDefClass(CDefClassInit("CVillagePeopleDef", CVillagePeopleDef::Alloc));
    AddDefClass(CDefClassInit("VILLAGER_INTERACTION", CVillagerInteractionsDef::Alloc));
    AddDefClass(CDefClassInit("HERO_COMBAT", CHeroCombatDef::Alloc));
    AddDefClass(CDefClassInit("HERO_MELEE_COMBAT_ABILITY", CMeleeCombatAbilityDef::Alloc));
    AddDefClass(CDefClassInit("ATTACK_PATTERN", CAttackPatternDef::Alloc));
    AddDefClass(CDefClassInit("CCreatureStatsDef", CCreatureStatsDef::Alloc));
    AddDefClass(CDefClassInit("ARMOUR", CArmourDef::Alloc));
    AddDefClass(CDefClassInit("HIT_LOCATION", CHitLocationDef::Alloc));
    AddDefClass(CDefClassInit("CHitLocationsDef", CHitLocationsDef::Alloc));
    AddDefClass(CDefClassInit("CCoopSpiritDef", CCoopSpiritDef::Alloc));
    AddDefClass(CDefClassInit("CContextSensitiveItemDef", CContextSensitiveItemDef::Alloc));
    AddDefClass(CDefClassInit("MELEE_COMBAT_KNOCKDOWN_EFFECTS", CMeleeCombatKnockdownEffects::Alloc));
    AddDefClass(CDefClassInit("CAbilityDef", CAbilityDef::Alloc));
    AddDefClass(CDefClassInit("CBonusItemDef", CBonusItemDef::Alloc));
    AddDefClass(CDefClassInit("CHeroMarriageDef", CHeroMarriageDef::Alloc));
    AddDefClass(CDefClassInit("CHeroTitleDef", CHeroTitleDef::Alloc));
    AddDefClass(CDefClassInit("CTattooDef", CTattooDef::Alloc));
    AddDefClass(CDefClassInit("CHairCardDef", CHairCardDef::Alloc));
    AddDefClass(CDefClassInit("UI", NUISystem::CUIDef::Alloc));
    AddDefClass(CDefClassInit("CCombatAbilityBlockUnarmedAttackDef", CCombatAbilityBlockUnarmedAttackDef::Alloc));
    AddDefClass(CDefClassInit("CCombatAbilityBlockLightWeaponAttackDef", CCombatAbilityBlockLightWeaponAttackDef::Alloc));
    AddDefClass(CDefClassInit("CCombatAbilityBlockHeavyWeaponAttackDef", CCombatAbilityBlockHeavyWeaponAttackDef::Alloc));
    AddDefClass(CDefClassInit("CCombatAbilityBlockProjectileWeaponAttackDef", CCombatAbilityBlockProjectileWeaponAttackDef::Alloc));
    AddDefClass(CDefClassInit("CCombatAbilityStrafeDef", CCombatAbilityStrafeDef::Alloc));
    AddDefClass(CDefClassInit("CCombatAbilityBlockCounterAttackDef", CCombatAbilityBlockCounterAttackDef::Alloc));
    AddDefClass(CDefClassInit("CCombatAbilityFlourishCounterAttackDef", CCombatAbilityFlourishCounterAttackDef::Alloc));
    AddDefClass(CDefClassInit("CCombatAbilityUseProjectileWeaponDef", CCombatAbilityUseProjectileWeaponDef::Alloc));
    AddDefClass(CDefClassInit("CCombatAbilityGetHitCounterAttackDef", CCombatAbilityGetHitCounterAttackDef::Alloc));
    AddDefClass(CDefClassInit("COMBAT_SEQUENCE", CCombatSequenceDef::Alloc));
    AddDefClass(CDefClassInit("UI_MISC_THINGS_DEF", NUISystem::CUIMiscThingsDef::Alloc));
    AddDefClass(CDefClassInit("CDragonActionSwoopDef", CDragonActionSwoopDef::Alloc));
    AddDefClass(CDefClassInit("CDragonActionNapalmDef", CDragonActionNapalmDef::Alloc));
    AddDefClass(CDefClassInit("CDragonActionHoverDef", CDragonActionHoverDef::Alloc));
    AddDefClass(CDefClassInit("UI_LOCALE_GRAPHICS_DEF", NUISystem::CUILocaleGraphicsDef::Alloc));
    AddDefClass(CDefClassInit("UI_ICONS_DEF", NUISystem::CUIIconsDef::Alloc));
    AddDefClass(CDefClassInit("LIGHTNING", CLightningDef::Alloc));
    AddDefClass(CDefClassInit("CAugmentationDef", CAugmentationDef::Alloc));
    AddDefClass(CDefClassInit("CSummonerDef", CSummonerDef::Alloc));
    AddDefClass(CDefClassInit("CSnowTrollDef", CSnowTrollDef::Alloc));
    AddDefClass(CDefClassInit("CGuildMasterDef", CGuildMasterDef::Alloc));
    AddDefClass(CDefClassInit("CJackDragonDef", CJackDragonDef::Alloc));
    AddDefClass(CDefClassInit("CBriarRoseDef", CBriarRoseDef::Alloc));
    AddDefClass(CDefClassInit("CExplosiveTrailDef", CExplosiveTrailDef::Alloc));
    AddDefClass(CDefClassInit("CSpecialAbilitiesDrainLifeDataDef", CSpecialAbilitiesDrainLifeDataDef::Alloc));
    AddDefClass(CDefClassInit("CSpecialAbilitiesForcePushDataDef", CSpecialAbilitiesForcePushDataDef::Alloc));
    AddDefClass(CDefClassInit("CONFIG_OPTIONS_DEFAULTS_DEF", CConfigOptionsDefaultsDef::Alloc));
    CAnimationEntry::StatRegisterClass();
    CPhysicalPrimitiveInit::StatRegisterClass();
    SymbolPathList = headers;
    CompilePathList = defs;
    NProgressDisplay::DisplayProgress("Game Definition Manager: Compile", -1.0f, false, false);
    Compile();
    NProgressDisplay::DisplayProgress("Game Definition Manager: GetDefs", -1.0f, false, false);
    GetDef("GLOBAL", 1, &PGlobalDef);
    GetDef("ENGINE", 1, &PEngineDef);
    GetDef("ENVIRONMENT", 1, &PEnvironmentDef);
    GetDef("HERO_STATS", 1, &PHeroStatsDef);
}
