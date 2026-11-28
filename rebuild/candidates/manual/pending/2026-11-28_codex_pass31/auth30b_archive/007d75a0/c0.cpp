// 007d75a0
// Leaf: ?GetRandomCard@CTCTavernGameCardPairs@@UAEXAAJ0JJ@Z
// Flags: none
// Inline-budget: The native CCardDef constructor expands only the observed layers of base initialization. Four physical deleting destructors agree; the base assignment remains under review.
#define FABLE_RETAIL_CCARDDEF_CTOR_PASS29
#define FABLE_RETAIL_CDEFPOINTER_RELEASE_SELFDELETING_POINTEE
#define FABLE_RETAIL_CIVCOUNTEDPOINTEEBASE_RELEASE_ORIGINAL_SCOPES_PASS22
#define FABLE_RETAIL_STLP_VECTOR_ASSIGN
#define FABLE_RETAIL_DEF_CLASS_BASE_CTORS
#define FABLE_RETAIL_CVECTORMAP_CTOR
#define FABLE_PDCB_TYPED_MEMBERS
#define FABLE_RETAIL_CPARENTDEFCLASSBASE
#define FABLE_RETAIL_CDEFCLASSBASE_IMPLICIT_ASSIGN
#define FABLE_RETAIL_PDCB_IMPLICIT_ASSIGN
#define FABLE_RETAIL_TAVERN_PAIRS_RANDOM_PASS29
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTCTAVERNGAME
#define FABLE_RETAIL_CTCTAVERNGAME_GETDEF_OUTOFLINE
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C3DVECTOR_PACKED
#define FABLE_RETAIL_CRIGHTHANDEDSET
#define FABLE_RETAIL_CTAVERNGAMECARDBASEDEF
#define FABLE_RETAIL_CTHINGOBJECT
#define FABLE_RETAIL_CTAVERNGAMECARDBASEDEF_CARDPACK
#define FABLE_RETAIL_CTCBASE_WORLDSEED
#define FABLE_RETAIL_CTCTAVERNGAMECARDPAIRS_GETRANDOMCARD
#include <stddef.h>
#include "stlport/fable_containers.h"
#include "engine/C3DVector.h"
#include "engine/CRightHandedSet.h"
#include "engine/CDefString.h"
#include "engine/CVectorMap.h"
#include "engine/CParentDefClassBase.h"

#ifdef _MSC_VER
#pragma optimize("s",on)
#endif
#include "engine/CCardDef.h"

#ifdef _MSC_VER
#pragma inline_depth(2)
#endif
inline CCardDef::CCardDef() {}

#ifdef _MSC_VER
#pragma optimize("",on)
#endif

#ifdef _MSC_VER
#pragma inline_depth()
#endif

#include "engine/CTCTavernGame.h"
#include "engine/CTCTavernGameCardBase.h"
#include "engine/CTCTavernGameCardPairs.h"
#include "engine/CTavernGameDef.h"
#include "engine/CTavernGameCardBaseDef.h"
#include "engine/GFRandom.h"
void CTCTavernGameCardPairs::GetRandomCard(long& index, long& value, long current_card, long total_cards)
{
    CDefPointer<const CTavernGameCardBaseDef> pdef;
    GetCardDef(&pdef);
    CCardDef card;
    bool quit = false;
    do {
        card = pdef->CardPack[GFRandom(52, GetPWorldSeed())];
        quit = true;
        if (current_card > total_cards / 2) {
            long original_card = current_card - (total_cards / 2 + 1);
            if (Cards[original_card]->Value == card.CardVal && Cards[original_card]->Name != card.CardName) {
                index = card.CardName;
                value = card.CardVal;
                return;
            } else {
                quit = false;
            }
        } else {
            for (std::vector<CCard*>::iterator card_it = Cards.begin(); card_it != Cards.end(); ++card_it) {
                if ((*card_it)->Name == card.CardName) {
                    quit = false;
                    break;
                }
                if ((*card_it)->Value == card.CardVal) {
                    quit = false;
                    break;
                }
            }
        }
    } while (!quit);
    index = card.CardName;
    value = card.CardVal;
}
