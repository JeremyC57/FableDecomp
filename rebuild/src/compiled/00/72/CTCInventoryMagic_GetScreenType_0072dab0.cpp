// CTCInventoryMagic::GetScreenType() const -- the magic screen's value, NHeroInformationScreens::INVENTORY_MAGIC_SCREEN (10)
// Leaf: ?GetScreenType@CTCInventoryMagic@@UBE?AW4EScreenType@NHeroInformationScreens@@XZ
// Identity: slot 2 of the NHeroInformationScreens::CBase secondary vtable 0x012668D8 of CTCInventoryMagic (+0x130; engine/NHeroInformationScreens_CBase.h
//   declares that base's four virtuals in this order: GetScreenName, GetPlayerMode, GetScreenType, IsInventoryScreen; the Xbox 360 Test PDB agrees);
//   Ego_r/FableWin name set: ?GetScreenType@CTCInventoryMagic@@UBE?AW4EScreenType@NHeroInformationScreens@@XZ; the body returns the value the enum names for this screen.
// Model: no engine/ header exists for CTCInventoryMagic; the class is declared with just this override (no layout is read).
#include "engine/NHeroInformationScreens_CBase.h"
class CTCInventoryMagic : public NHeroInformationScreens::CBase {
public:
    virtual NHeroInformationScreens::EScreenType GetScreenType() const;
};
NHeroInformationScreens::EScreenType CTCInventoryMagic::GetScreenType() const
{
    return NHeroInformationScreens::INVENTORY_MAGIC_SCREEN;
}
