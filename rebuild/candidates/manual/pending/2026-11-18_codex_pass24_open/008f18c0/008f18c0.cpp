// 008f18c0 CTCTavernGameCardBase::UpdateHighlightedCard -- Projects the pointer and cards onto the table and updates the highlighted card.
// Grade: DIFFER(795v770) -- The region using the card-definition out-argument allocates different frame slots. The table-space basis and projected card bounds schedule differently.
// Retail: 0x008F18C0, 770 bytes, thiscall.
// Leaf: ?UpdateHighlightedCard@CTCTavernGameCardBase@@UAEXXZ
#define FABLE_RETAIL_C3DVECTOR_PACKED
#define FABLE_RETAIL_CTAVERNGAMECARDBASEDEF_HIGHLIGHT_COLOUR_PUBLIC_PASS24
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_CRIGHTHANDEDSET
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGOBJECT
#define FABLE_RETAIL_CTHINGOBJECT_CREATE
#define FABLE_RETAIL_CTCTAVERNGAME
#define FABLE_RETAIL_CTAVERNGAMECARDBASEDEF
#define FABLE_RETAIL_CTCGRAPHICAPPEARANCE_HIGHLIGHT
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/C3DVector.h"
#include "engine/CIntelligentPointer.h"
#include "engine/CRightHandedSet.h"
#include "engine/CTCTavernGameCardBase.h"
#include "engine/CCard.h"
#include "engine/CTavernGameCardBaseDef.h"
#include "engine/CThingObject.h"
#include "engine/NGraphicAppearance_CTCGraphicAppearance.h"

void CTCTavernGameCardBase::UpdateHighlightedCard()
{
    C3DVector relative_pos = Pointer->GetPos() - GamePositionPos;
    float pointer_x = relative_pos.Dot(GamePositionRHSet.GetRight());
    float pointer_y = relative_pos.Dot(GamePositionRHSet.GetForward());
    CDefPointer<const CTavernGameCardBaseDef> pdef;
    GetCardDef(&pdef);
    float half_card_width = pdef->CardWidth * 0.5f;
    float half_card_height = pdef->CardHeight * 0.5f;
    CCard* previously_highlighted_card = HighlightedCard;
    NGraphicAppearance::CTCGraphicAppearance* tc_graphic_appearance;
    if (previously_highlighted_card) {
        previously_highlighted_card->Thing->GetTC(TCI_GRAPHIC_APPEARANCE_NEW, &tc_graphic_appearance);
        tc_graphic_appearance->ClearMeshEffect(this);
        HighlightedCard = 0;
    }
    if (TavernGameState == ETGS_CARDS_MAIN_GAME) {
        for (std::vector<CCard*>::iterator card_it = Cards.begin(); card_it != Cards.end(); ++card_it) {
            C3DVector card_relative_pos = (*card_it)->Thing->GetPos() - GamePositionPos;
            float card_x = card_relative_pos.Dot(GamePositionRHSet.GetRight());
            float card_y = card_relative_pos.Dot(GamePositionRHSet.GetForward());
            if (card_x - half_card_width <= pointer_x && card_x + half_card_width >= pointer_x &&
                card_y - half_card_height <= pointer_y && card_y + half_card_height >= pointer_y) {
                HighlightedCard = *card_it;
                break;
            }
        }
        if (HighlightedCard) {
            if (HighlightedCard != previously_highlighted_card)
                PlaySoundW("SOUND_CARD_HIGHLIGHT");
            HighlightedCard->Thing->GetTC(TCI_GRAPHIC_APPEARANCE_NEW, &tc_graphic_appearance);
            tc_graphic_appearance->SetAsHighlighted(pdef->HighlightWidth, 0.0f, pdef->HighlightColourGood, false, false, (NGraphicAppearance::EAppearanceMeshEffectPriority)3, this);
        }
    }
}
