// CTCTavernGameCardSorting::SelectCard() -- after the card-base selection, highlight every face-up card lower than the selected one (a sorting mistake: the round ends, a timed game is lost); with no mistake, a face-up highlighted card plays the "correct" sound
// Leaf: ?SelectCard@CTCTavernGameCardSorting@@UAE_NXZ
// Plan phase 2 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end().
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007D5890, 366 bytes (plain `ret`); CTCTavernGameCardSorting vtable 0x0126F5B4
// slot 59 (SelectCard, as CardPairs 0x007D5FB0, landed).
// Callees: CTCTavernGameCardBase::SelectCard() 0x008F09C0 (non-virtual base call); CTCTavernGameCardBase::GetCardDef(
//          CDefPointer<const CTavernGameCardBaseDef>*) 0x008F0B20; CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound
//          0x0040F020 (interface 0x5B = word 2 bit 27); NGraphicAppearance::CTCGraphicAppearance::SetAsHighlighted(float,
//          float, const CRGBColour&, bool, bool, EAppearanceMeshEffectPriority, void*) 0x004C04C0; CCharString(const char*,
//          long) 0x0099EBF0 ("TAVERN_GAME_CORRECT" 0x0126F414); CTCTavernGame::PlaySound(const CCharString&) 0x005EA950;
//          ~CCharString 0x0099EAE0; the def's slot 1 on release.
// Layout (retail, as the landed CardBase/CardPairs sources): GameWon +0x21, GameFinished +0x28, Cards +0x128,
//          RoundFinished +0x158, GameType +0x300, SelectedCard +0x308, HighlightedCard +0x30c; CCard Thing +0x00,
//          Value +0x08, Turning +0x24, FaceDown +0x25; CTavernGameCardBaseDef highlight time +0x50, colour +0x58.
// Shape: the appearance pointer is declared (unset) outside the loop, so VC keeps it in edi across iterations and the
//        absent-interface arm reuses it (its home is the lookup key's frame word); the game-type test is a one-case
//        `switch` (`dec eax ; jne`).
// Shared headers (NF96 q11339): engine/card_sorting_types.h (the Ego_r.pdb CTCTavernGameCardBase / CTCTavernGameCardSorting / CCard, recipe of
//   the landed CardsMainGame 0x007D5070), CTavernGameCardBaseDef.h (HighlightWidth +0x50 and HighlightColourBad +0x58, the PDB names of the old
//   model's HighlightTime / HighlightColour; overlay opt-in FABLE_RETAIL_CTAVERNGAMECARDBASEDEF_HIGHLIGHT_COLOUR_BAD types the colour),
//   NGraphicAppearance_CTCGraphicAppearance.h (SetAsHighlighted, FABLE_RETAIL_CTCGRAPHICAPPEARANCE_HIGHLIGHT + _EFFECT_ENTRY_PASS23 for the enumerators; priority 3 =
//   MESH_EFFECT_PRIORITY_TARGETING) and CThing.h GetTC (GETTC_EXPANDED, as before), with CDefPointer<const CTavernGameCardBaseDef>, so
//   GetCardDef 0x008F0B20 binds its PDB spelling (was a local CCardDefPointer struct and local CRGBColour / CTCGraphicAppearance / CCard /
//   vector / def / CTCTavernGameCardBase models).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCTAVERNGAME
#define FABLE_RETAIL_CTCTAVERNGAME_GETDEF_OUTOFLINE
#define FABLE_RETAIL_CTAVERNGAMECARDBASEDEF
#define FABLE_RETAIL_CTCTAVERNGAMECARDBASE_RHSET
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CGAMECOMPONENT
#define FABLE_RETAIL_CCLASSFACTORY
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_SCOPED_PTR
#define FABLE_RETAIL_CTAVERNGAMECARDBASEDEF_HIGHLIGHT_COLOUR_BAD
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CTCGRAPHICAPPEARANCE_HIGHLIGHT
#define FABLE_RETAIL_CTCGRAPHICAPPEARANCE_EFFECT_ENTRY_PASS23
#include "stlport/fable_containers.h"
#include "fable_game.h"
#include "engine/CThingObject.h"
#include "engine/CRightHandedSet.h"
#include "engine/CThing.h"
#include "engine/CVectorMap.h"
#include "engine/CRGBColour.h"
#include "engine/NGraphicAppearance_CTCGraphicAppearance.h"
#include "engine/CTavernGameDef.h"
#include "engine/CTavernGameCardBaseDef.h"
#include "engine/card_sorting_types.h"
bool CTCTavernGameCardSorting::SelectCard()
{
    CTCTavernGameCardBase::SelectCard();
    if (!SelectedCard)
        return true;
    bool mistake = false;
    CDefPointer<const CTavernGameCardBaseDef> def(NULL_DEF);
    GetCardDef(&def);
    NGraphicAppearance::CTCGraphicAppearance* appearance;
    for (std::vector<CCard*>::iterator it = Cards.begin(); it != Cards.end(); ++it) {
        CCard* card = *it;
        if (!card->FaceDown && card->Value < SelectedCard->Value) {
            card->Thing->GetTC(TCI_GRAPHIC_APPEARANCE_NEW, &appearance);
            appearance->SetAsHighlighted(def->HighlightWidth, 0.0f, def->HighlightColourBad, false, false,
                                         NGraphicAppearance::MESH_EFFECT_PRIORITY_TARGETING, this);
            mistake = true;
        }
    }
    if (mistake) {
        switch (GameType) {
        case ECGT_ROUND_LIMIT:
            GameWon = false;
            break;
        }
        RoundFinished = true;
        GameFinished = true;
        return false;
    }
    if (HighlightedCard && !HighlightedCard->FaceDown && !HighlightedCard->Turning) {
        CCharString sound("TAVERN_GAME_CORRECT", -1);
        PlaySoundW(sound);
    }
    return true;
}
