// CTCInventoryAbilities::GetScreenType() const -- the screen's type, NHeroInformationScreens::HERO_ABILITIES_SCREEN (3)
// Leaf: ?GetScreenType@CTCInventoryAbilities@@UBE?AW4EScreenType@NHeroInformationScreens@@XZ
// Identity: slot 2 of the NHeroInformationScreens::CBase secondary vtable 0x01256AC0 of CTCInventoryAbilities (+0x130; engine/NHeroInformationScreens_CBase.h declares
//   that base's four virtuals in this order: GetScreenName, GetPlayerMode, GetScreenType, IsInventoryScreen; the Xbox 360 Test PDB agrees);
//   Ego_r/FableWin name set: ?GetScreenType@CTCInventoryAbilities@@UBE?AW4EScreenType@NHeroInformationScreens@@XZ; the body returns the value the enum names for this screen.
// Header: prelude as in the landed row CTCInventoryAbilities_HasFrameUpdate_005fa3a2.cpp.
#pragma optimize("s",on)
#define FABLE_RETAIL_CTCINVENTORYABILITIES_HISCREEN
#define FABLE_RETAIL_CTCINVENTORYABILITIES_VSLOTS
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCINVENTORYBASE
#define FABLE_RETAIL_CTCINVENTORYBASE_VTABLE
#define FABLE_RETAIL_CTCINVENTORYABILITIES
#define FABLE_RETAIL_CTCINVENTORYABILITIES_PRIORITISED
#define FABLE_RETAIL_STLP_VECTOR_PUSHBACK_OUTOFLINE
#include "engine/CTCInventoryAbilities.h"
NHeroInformationScreens::EScreenType CTCInventoryAbilities::GetScreenType() const
{
    return NHeroInformationScreens::HERO_ABILITIES_SCREEN;
}
