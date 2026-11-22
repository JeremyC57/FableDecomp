// Behaviour test for 0x00899DB0 CGameScriptInterface::IsPlayerCreatureFlourishEnabled (NF256, CODEXP27 L57), on the shared headers: the
// row's own define / include block; the three out-of-line members the row object imports (CPlayerManager::GetMainPlayer,
// CPlayer::GetPControlledCreature, CTCCombat::IsMeleeFlourishValid) are stubbed; the player creature is a zeroed byte image of the retail
// CThing (UsingComponents bitset +0x20, Components vector +0x44, Dead = bit 0 of +0x91) reached through the header types, and the row
// is called qualified (no constructor, no vtable). Every branch is checked. Keeps the catalog PassPattern CGameScriptInterface_00899db0_TEST.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGPLAYERCREATURE
#define FABLE_RETAIL_CPLAYERMANAGER
#define FABLE_RETAIL_PLAYER
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_IS_THING_ALIVE
#define FABLE_RETAIL_CTCCOMBAT
#define FABLE_RETAIL_CTCCOMBAT_WEAPONTRAIL
#include "stlport/fable_containers.h"
#include "fable_string.h"
#include "engine/ETCInterfaceType.h"
#include "engine/CThing.h"
#include "engine/CThingCreatureBase.h"
#include "engine/CThingPlayerCreature.h"
#include "engine/CPlayer.h"
#include "engine/CPlayerManager.h"
#include "engine/ECombatAbilityDataType.h"
#include "engine/CTCCombat.h"
#include "engine/ThingAlive.h"
#include "engine/CGameScriptInterface.h"
#include <cstdio>
#include <cstring>

static CPlayerManager* g_ObservedManager;
static unsigned char g_PlayerImage[0x40];
static CThingPlayerCreature* g_Creature;
static const CTCCombat* g_ObservedCombat;
static bool g_FlourishResult;
static unsigned long g_FlourishCalls;

CPlayer& CPlayerManager::GetMainPlayer() { g_ObservedManager = this; return *reinterpret_cast<CPlayer*>(g_PlayerImage); }
CThingPlayerCreature* CPlayer::GetPControlledCreature() const { return g_Creature; }
bool CTCCombat::IsMeleeFlourishValid() const { g_ObservedCombat = this; ++g_FlourishCalls; return g_FlourishResult; }

static int g_Failures;
static void Check(bool ok, const char* what) { if (!ok) { std::printf("FAIL: %s\n", what); ++g_Failures; } }

struct STCEntryImage { long Type; void* Component; };                    // pair<ETCInterfaceType, CTCBase*>
static unsigned long g_ThingImage[0x400 / 4];
static STCEntryImage g_Entries[3];
static unsigned long g_CombatImage[0x100 / 4];
static unsigned long g_OtherImage[0x100 / 4];
static unsigned long g_ManagerImage[0x40 / 4];
static unsigned long g_InterfaceImage[0x100 / 4];

static void SetThing(bool dead, bool hasCombat, long entries)
{
    unsigned char* bytes = reinterpret_cast<unsigned char*>(g_ThingImage);
    std::memset(g_ThingImage, 0, sizeof(g_ThingImage));
    if (hasCombat) bytes[0x26] |= 0x01;                                  // UsingComponents bit 48 (word +0x24, bit 16)
    if (dead) bytes[0x91] |= 0x01;                                       // Dead
    STCEntryImage* first = g_Entries;
    STCEntryImage* last = g_Entries + entries;
    std::memcpy(bytes + 0x44, &first, 4);                                // Components start
    std::memcpy(bytes + 0x48, &last, 4);                                 // Components finish
    std::memcpy(bytes + 0x4C, &last, 4);
    g_FlourishCalls = 0; g_ObservedCombat = 0; g_ObservedManager = 0;
}

int main()
{
    CPlayerManager* manager = reinterpret_cast<CPlayerManager*>(g_ManagerImage);
    unsigned char* iface = reinterpret_cast<unsigned char*>(g_InterfaceImage);
    std::memcpy(iface + 0x14, &manager, 4);                              // CGameScriptInterface::PlayerManager
    const CGameScriptInterface* gsi = reinterpret_cast<const CGameScriptInterface*>(g_InterfaceImage);
    const CTCCombat* combat = reinterpret_cast<const CTCCombat*>(g_CombatImage);
    g_Entries[0].Type = 0x10; g_Entries[0].Component = g_OtherImage;
    g_Entries[1].Type = 0x30; g_Entries[1].Component = g_CombatImage;    // TCI_COMBAT
    g_Entries[2].Type = 0x50; g_Entries[2].Component = g_OtherImage;
    CThingPlayerCreature* creature = reinterpret_cast<CThingPlayerCreature*>(g_ThingImage);

    g_Creature = 0; SetThing(false, true, 3); g_FlourishResult = true;
    Check(!gsi->CGameScriptInterface::IsPlayerCreatureFlourishEnabled() && g_FlourishCalls == 0 && g_ObservedManager == manager, "no creature");

    g_Creature = creature; SetThing(true, true, 3); g_FlourishResult = true;
    Check(!gsi->CGameScriptInterface::IsPlayerCreatureFlourishEnabled() && g_FlourishCalls == 0, "dead creature");

    SetThing(false, false, 3); g_FlourishResult = true;
    Check(!gsi->CGameScriptInterface::IsPlayerCreatureFlourishEnabled() && g_FlourishCalls == 0, "no combat component");

    SetThing(false, true, 3); g_FlourishResult = true;
    Check(gsi->CGameScriptInterface::IsPlayerCreatureFlourishEnabled() && g_FlourishCalls == 1 && g_ObservedCombat == combat, "flourish valid");

    SetThing(false, true, 3); g_FlourishResult = false;
    Check(!gsi->CGameScriptInterface::IsPlayerCreatureFlourishEnabled() && g_FlourishCalls == 1 && g_ObservedCombat == combat, "flourish not valid");

    if (g_Failures) { std::printf("CGameScriptInterface_00899db0_TEST FAIL (%d)\n", g_Failures); return 1; }
    std::printf("CGameScriptInterface_00899db0_TEST PASS\n");
    return 0;
}
