// Leaf: ?IsRouteToAppearOnMinimap@CTCQuestCard@@QBE_NXZ
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCQUESTCARD
#define FABLE_RETAIL_CTCQUESTCARD_ACCESSORS
#define FABLE_RETAIL_MINIMAP_REGION_EXIT_MARKERS_PASS30
#define FABLE_RETAIL_CDEFPOINTER_ORIGINAL_DIRECT_DEREF_PASS26
#include "engine/CTCQuestCard.h"

bool CTCQuestCard::IsRouteToAppearOnMinimap() const
{
    return PDef->MakeVignetteRouteAppearOnMinimap || !PDef->IsVignette;
}

#include <stdio.h>
#include <string.h>

int main()
{
    union { unsigned long align; unsigned char bytes[sizeof(CTCQuestCard)]; } card_image;
    union { unsigned long align; unsigned char bytes[sizeof(CQuestCardDef)]; } definition_image;
    for (unsigned int bits = 0; bits < 16; ++bits)
    {
        memset(card_image.bytes, 0, sizeof(card_image.bytes));
        memset(definition_image.bytes, 0, sizeof(definition_image.bytes));
        CQuestCardDef* definition = reinterpret_cast<CQuestCardDef*>(definition_image.bytes);
        memcpy(card_image.bytes + 0x0c, &definition, sizeof(definition));
        const bool core = (bits & 1) != 0;
        const bool vignette = (bits & 2) != 0;
        const bool exclusive = (bits & 4) != 0;
        const bool route = (bits & 8) != 0;
        definition_image.bytes[0x58] = core;
        definition_image.bytes[0x59] = vignette;
        definition_image.bytes[0x5a] = exclusive;
        definition_image.bytes[0x5b] = route;
        const CTCQuestCard* card = reinterpret_cast<const CTCQuestCard*>(card_image.bytes);
        if (card->IsRouteToAppearOnMinimap() != (!vignette || route))
            return 1;
    }
    puts("007022f0_TEST PASS");
    return 0;
}
