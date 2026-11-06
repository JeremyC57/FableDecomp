// NScript::CV_HiddenBootyScript::CM_HiddenBooty::Main() -- the hidden booty digging spot: with the alive predicate, note whether the hero holds any / all six treasure clues, then until the quest is complete re-check the clues whenever the hero receives an item (until he has them all), add the fake booty to the spot once any clue is held, swap it for the real booty once all are held, and complete the quest when the hero is rewarded with items from this spot
// Leaf: ?Main@CM_HiddenBooty@CV_HiddenBootyScript@NScript@@EAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00E93120, 2354 bytes, thiscall, plain `ret`, no EH frame, ends in its `ret` at 0x00E93A51.
// Identity (proven): slot 1 (Main) of the entity-script vtable 0x012ECAE8 (its only reference, .rdata 0x012ECAEC), which the landed
//   CM_HiddenBooty::Alloc 0x00E92F70 stores; Ego_r.pdb / FableWin.pdb name set ?Main@CM_HiddenBooty@CV_HiddenBootyScript@NScript@@EAEXXZ
//   (private virtual, as engine/CV_HiddenBootyScript.h declares it).
// Callees: CEntityScriptBase::SetPredicate 0x00F35B10 / IsActiveThreadTerminating 0x00F35B30, CScriptThing copy 0x004ABE90 (inside the
//   inline CIsThingAlive ctor), ~CIsThingAlive 0x00E241B0, CCharString(const char*, long) 0x0099EBF0 / ~ 0x0099EAE0, CDefString::operator
//   CCharString() const 0x00415D70 (Ego_r / FableWin ??BCDefString@@QBE?AVCCharString@@XZ, out of line: FABLE_RETAIL_CDEFSTRING_OUTOFLINE_
//   CONVERSION), CBasicString<char>::Compare 0x00411570 (inline CCharString ==); pGlobalScriptDef 0x0143E90C (HiddenBootyTreasureFake
//   +0x684, HiddenBootyTreasureReal +0x688); CGameScriptInterfaceBase slots 7 NewScriptFrame, 57 MsgOnHeroRewardedWithItemsFrom, 70 GetHero,
//   184 IsObjectInThingsPossession, 585 AddItemToContainer, 586 RemoveItemFromContainer; CScriptThing slots 1 GetName, 36
//   MsgReceivedInventoryItem. The 24 string literals were paired with retail by relocation (strchk).
// Layout: engine/CV_HiddenBootyScript.h (unchanged): CM_HiddenBooty HasHeroGotAllClues +0x1c, HasHeroGotAnyClues +0x1d,
//   ThingFromWhichHeroMightBeRewardedFrom +0x20; parent QuestComplete +0x49, FakeBootyAddedToDiggingSpot +0x4a, RealBootyAddedToDiggingSpot
//   +0x4b.
// Shape: generated entity-script code. The two clue tests are one || chain and one && chain of by-value CCharString temporaries, so
//   VC7.1 keeps one conditional-destruction flag word (ebx) for the whole function; the trailing `if (IsActiveThreadTerminating())
//   return;` after the loop is the bare call at 0x00E93A43.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CDEFSTRING_OUTOFLINE_CONVERSION
#include "fable_string.h"
#include "engine/CCountedPointer.h"
#include "engine/CScriptThing.h"
#include "engine/CGameScriptInterfaceBase.h"
#include "engine/CQ_SunnyvaleMasterData.h"
#include "engine/CV_HiddenBootyScript.h"
#include "engine/CIsThingAlive.h"
#include "engine/CScriptDef.h"
#include "engine/CDefPointer.h"
extern CDefPointer<const CScriptDef> pGlobalScriptDef;   // 0x0143E90C
inline NScript::CIsThingAlive::CIsThingAlive(const CScriptThing& thing):Thing(thing) {}

void NScript::CV_HiddenBootyScript::CM_HiddenBooty::Main()
{
    SetPredicate(CIsThingAlive(Me));
    HasHeroGotAnyClues = Game->IsObjectInThingsPossession("OBJECT_TREASURE_CLUE_1", Game->GetHero())
        || Game->IsObjectInThingsPossession("OBJECT_TREASURE_CLUE_2", Game->GetHero())
        || Game->IsObjectInThingsPossession("OBJECT_TREASURE_CLUE_3", Game->GetHero())
        || Game->IsObjectInThingsPossession("OBJECT_TREASURE_CLUE_4", Game->GetHero())
        || Game->IsObjectInThingsPossession("OBJECT_TREASURE_CLUE_5", Game->GetHero())
        || Game->IsObjectInThingsPossession("OBJECT_TREASURE_CLUE_6", Game->GetHero());
    HasHeroGotAllClues = Game->IsObjectInThingsPossession("OBJECT_TREASURE_CLUE_1", Game->GetHero())
        && Game->IsObjectInThingsPossession("OBJECT_TREASURE_CLUE_2", Game->GetHero())
        && Game->IsObjectInThingsPossession("OBJECT_TREASURE_CLUE_3", Game->GetHero())
        && Game->IsObjectInThingsPossession("OBJECT_TREASURE_CLUE_4", Game->GetHero())
        && Game->IsObjectInThingsPossession("OBJECT_TREASURE_CLUE_5", Game->GetHero())
        && Game->IsObjectInThingsPossession("OBJECT_TREASURE_CLUE_6", Game->GetHero());
    while (!ParentClass.QuestComplete) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating()) return;
        if (!HasHeroGotAllClues && Game->GetHero().MsgReceivedInventoryItem()) {
            if (IsActiveThreadTerminating()) return;
            HasHeroGotAnyClues = Game->IsObjectInThingsPossession("OBJECT_TREASURE_CLUE_1", Game->GetHero())
                || Game->IsObjectInThingsPossession("OBJECT_TREASURE_CLUE_2", Game->GetHero())
                || Game->IsObjectInThingsPossession("OBJECT_TREASURE_CLUE_3", Game->GetHero())
                || Game->IsObjectInThingsPossession("OBJECT_TREASURE_CLUE_4", Game->GetHero())
                || Game->IsObjectInThingsPossession("OBJECT_TREASURE_CLUE_5", Game->GetHero())
                || Game->IsObjectInThingsPossession("OBJECT_TREASURE_CLUE_6", Game->GetHero());
            HasHeroGotAllClues = Game->IsObjectInThingsPossession("OBJECT_TREASURE_CLUE_1", Game->GetHero())
                && Game->IsObjectInThingsPossession("OBJECT_TREASURE_CLUE_2", Game->GetHero())
                && Game->IsObjectInThingsPossession("OBJECT_TREASURE_CLUE_3", Game->GetHero())
                && Game->IsObjectInThingsPossession("OBJECT_TREASURE_CLUE_4", Game->GetHero())
                && Game->IsObjectInThingsPossession("OBJECT_TREASURE_CLUE_5", Game->GetHero())
                && Game->IsObjectInThingsPossession("OBJECT_TREASURE_CLUE_6", Game->GetHero());
        }
        if (HasHeroGotAnyClues && !ParentClass.FakeBootyAddedToDiggingSpot) {
            if (IsActiveThreadTerminating()) return;
            Game->AddItemToContainer(Me, pGlobalScriptDef->HiddenBootyTreasureFake);
            ParentClass.FakeBootyAddedToDiggingSpot = true;
        }
        if (HasHeroGotAllClues && !ParentClass.RealBootyAddedToDiggingSpot) {
            if (IsActiveThreadTerminating()) return;
            if (ParentClass.FakeBootyAddedToDiggingSpot) {
                if (IsActiveThreadTerminating()) return;
                Game->RemoveItemFromContainer(Me, pGlobalScriptDef->HiddenBootyTreasureFake);
            }
            Game->AddItemToContainer(Me, pGlobalScriptDef->HiddenBootyTreasureReal);
            ParentClass.RealBootyAddedToDiggingSpot = true;
        }
        if (Game->MsgOnHeroRewardedWithItemsFrom(ThingFromWhichHeroMightBeRewardedFrom)) {
            if (IsActiveThreadTerminating()) return;
            if (ThingFromWhichHeroMightBeRewardedFrom == Me.GetName()) {
                if (IsActiveThreadTerminating()) return;
                ParentClass.QuestComplete = true;
            }
        }
    }
    if (IsActiveThreadTerminating()) return;
}