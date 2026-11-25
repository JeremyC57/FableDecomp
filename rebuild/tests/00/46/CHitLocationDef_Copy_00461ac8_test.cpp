// Behaviour test for 0x00461AC8 CHitLocationDef::Copy (NF425): links the row object; CHitLocationDef::operator= is defined here and records its `this` and
// argument, so the test checks the copy goes from `other` into `this` exactly once (a swapped or doubled copy fails).
// Ported (NF427 round 2, 2026-11-25): no class body is defined here; the classes come from the shared headers (the row's
//   prelude below, verbatim, plus engine/CHitLocationDef.h). Removed local bodies: CHitLocationDef. The members the test supplies (??4CHitLocationDef@@QAEAAV0@ABV0@@Z) and the row's CHitLocationDef::Copy are
//   reached through extern "C" __fastcall stand-ins mapped by /alternatename (ecx = this, edx unused, stack arguments
//   popped by the callee: the __thiscall frame); sizes and offsets are the retail literals of the original test.
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
#include <stdio.h>

class CDefClassBase;

static const void* g_this = 0;
static const void* g_other = 0;
static int g_calls = 0;
extern "C" void* __fastcall nf427_assign(void* self, int edx_unused, const void* other) { g_this = self; g_other = other; ++g_calls; return self; }   // CHitLocationDef::operator=
#pragma comment(linker, "/alternatename:??4CHitLocationDef@@QAEAAV0@ABV0@@Z=@nf427_assign@12")

static long g_dst[32];
static long g_src[32];

extern "C" void __fastcall nf427_copy(void* self, int edx_unused, const void* other);   // the row's CHitLocationDef::Copy
#pragma comment(linker, "/alternatename:@nf427_copy@12=?Copy@CHitLocationDef@@UAEXPBVCDefClassBase@@@Z")

int main(void)
{
    void* dst = g_dst;
    const CDefClassBase* src = (const CDefClassBase*)g_src;
    nf427_copy(dst, 0, src);
    if (g_calls != 1 || g_this != (const void*)g_dst || g_other != (const void*)g_src) {
        printf("FAIL_00461ac8 calls=%d\n", g_calls);
        return 1;
    }
    printf("PASS_00461ac8\n");
    return 0;
}
