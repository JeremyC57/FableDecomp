// CTCTavernGameCardPairs::GetRandomCard(long& index, long& value, long current_card, long total_cards) -- draw random cards from the def's 52-card pack until one fits: in the second half of the deal it must pair (same value, other name) with its first-half partner Cards[current_card - total_cards/2 - 1], in the first half it must share neither name nor value with any dealt card; return its name and value
// Grade: DIFFER(268v465) -- structure and callees right; CCardDef's inline default constructor (CResource ctor 0x009FBEC0 + field inits + vftable 0x01243A44) and implicit copy-assignment (member-wise with the 3 bitfield bits and CRetailVector<CSubDefInfo>::operator= 0x00431F90) are not expanded with today's def-class headers, and the destructor tail differs
// Leaf: ?GetRandomCard@CTCTavernGameCardPairs@@UAEXAAJ0JJ@Z
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
#define FABLE_RETAIL_CDEFPOINTER_RELEASE_COUNTREF
#define FABLE_RETAIL_CTAVERNGAMECARDBASEDEF_CARDPACK
#define FABLE_RETAIL_CTCBASE_WORLDSEED
#define FABLE_RETAIL_CTCTAVERNGAMECARDPAIRS_GETRANDOMCARD
#include <stddef.h>
#include "stlport/fable_containers.h"
#include "engine/C3DVector.h"
#include "engine/CRightHandedSet.h"
#include "engine/CCardDef.h"
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
    while (!quit) {
        card = pdef->CardPack[GFRandom(52, GetPWorldSeed())];
        quit = true;
        if (current_card > total_cards / 2) {
            long original_card = current_card - total_cards / 2 - 1;
            if (Cards[original_card]->Value != card.CardVal || Cards[original_card]->Name == card.CardName)
                quit = false;
        } else {
            for (std::vector<CCard*>::iterator card_it = Cards.begin(); card_it != Cards.end(); ++card_it) {
                if ((*card_it)->Name == card.CardName || (*card_it)->Value == card.CardVal) {
                    quit = false;
                    break;
                }
            }
        }
    }
    index = card.CardName;
    value = card.CardVal;
}
