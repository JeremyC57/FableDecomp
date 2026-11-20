// NScript::CV_ChapelOfEvilScript::WatchForWeaponUnsheathed() -- track the hero drawing a weapon inside the Chapel; the first time (after the intro) the sound altar warns him with a one-line conversation
// Leaf: ?WatchForWeaponUnsheathed@CV_ChapelOfEvilScript@NScript@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00E6A340, 447 bytes (448 with padding), plain `ret`.
// Callees: CCharString(const char*, long) 0x0099EBF0 / ~CCharString 0x0099EAE0, CScriptThing::IsAlive 0x004AB130 (out of
//   line here), ~CBaseClass 0x0099A2E0, operator delete 0x00BFE9BC, ~CScriptThing 0x004AA840 (out of line),
//   CScriptBase::IsActiveThreadTerminating 0x00CB7940; through the interface (+0x40) NewScriptFrame (slot 7), GetHero
//   (slot 70), GetThingWithScriptName (slot 72, hidden result), IsPlayerWieldingWeapon (slot 195), AddNewConversation
//   (slot 364), AddPersonToConversation (slot 365), AddLineToConversation (slot 366).
// Strings: "M_SoundAltar" 0x012E8324, "TEXT_QST_019_WEAPON_01" 0x012E8450.
// Shape: an `if (!terminating) { do { ... } while (!terminating); }` watch loop; the altar handle's inline destructor is expanded once at the loop exit
//   (zero kept in ebx), the out-of-line ~CScriptThing called at the in-loop returns.
// Layout (retail = Ego_r.pdb - 0x14): WeaponWarningComplete +0x56, WeaponUnsheathed +0x57, ChapelIntroComplete +0x58,
//   InsideChapel +0x59; interface pointer +0x40.
// Naming (NF162, 2026-11-20, q6271): was a row-local global `struct CV_ChapelOfEvilScript` over a local CQuestScriptBase (leaf
//   ?WatchForWeaponUnsheathed@CV_ChapelOfEvilScript@@QAEXXZ) with local CCharString / CScriptThing models; now on the shared
//   engine/CV_ChapelOfEvilScript.h, engine/CScriptThing.h and fable_string.h. Identity: Ego_r.pdb
//   ?WatchForWeaponUnsheathed@CV_ChapelOfEvilScript@NScript@@QAEXXZ, the thread "WatchForWeaponUnsheathed" Main 0x00E693F0 spawns
//   (the landed Main binds this spelling; its only referencer).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_SUSPENDABLE_PROCESS
#define FABLE_RETAIL_BASE_OBJECT
#include "fable_string.h"
#include "engine/CCountedPointer.h"
#include "engine/CScriptThing.h"
#include "engine/CGameScriptInterfaceBase.h"
#include "engine/CV_ChapelOfEvilScript.h"
inline CScriptThing::~CScriptThing() { }

void NScript::CV_ChapelOfEvilScript::WatchForWeaponUnsheathed()
{
    CScriptThing altar = Game->GetThingWithScriptName("M_SoundAltar");
    if (!IsActiveThreadTerminating()) {
        do {
            if (InsideChapel && Game->IsPlayerWieldingWeapon() && !WeaponUnsheathed) {
                if (IsActiveThreadTerminating())
                    return;
                WeaponUnsheathed = true;
                if (!WeaponWarningComplete && ChapelIntroComplete) {
                    if (IsActiveThreadTerminating())
                        return;
                    if (altar.IsAlive()) {
                        if (IsActiveThreadTerminating())
                            return;
                        long conversation = Game->AddNewConversation(altar, false, false);
                        Game->AddPersonToConversation(conversation, Game->GetHero());
                        Game->AddLineToConversation(conversation, "TEXT_QST_019_WEAPON_01", false, altar, Game->GetHero());
                    }
                    WeaponWarningComplete = true;
                }
            } else if (!Game->IsPlayerWieldingWeapon() && WeaponUnsheathed) {
                if (IsActiveThreadTerminating())
                    return;
                WeaponUnsheathed = false;
            }
            Game->NewScriptFrame();
        } while (!IsActiveThreadTerminating());
    }
}