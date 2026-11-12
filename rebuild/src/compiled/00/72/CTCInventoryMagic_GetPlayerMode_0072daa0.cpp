// CTCInventoryMagic::GetPlayerMode() const -- the magic screen's value, PLAYER_MODE_NAVIGATE_INVENTORY_MAGIC_SCREEN (9)
// Leaf: ?GetPlayerMode@CTCInventoryMagic@@UBE?AW4EPlayerMode@@XZ
// Identity: slot 1 of the NHeroInformationScreens::CBase secondary vtable 0x012668D8 of CTCInventoryMagic (+0x130; engine/NHeroInformationScreens_CBase.h
//   declares that base's four virtuals in this order: GetScreenName, GetPlayerMode, GetScreenType, IsInventoryScreen; the Xbox 360 Test PDB agrees);
//   Ego_r/FableWin name set: ?GetPlayerMode@CTCInventoryMagic@@UBE?AW4EPlayerMode@@XZ; the body returns the value the enum names for this screen.
// Model: no engine/ header exists for CTCInventoryMagic; the class is declared with just this override (no layout is read).
#include "engine/NHeroInformationScreens_CBase.h"
class CTCInventoryMagic : public NHeroInformationScreens::CBase {
public:
    virtual EPlayerMode GetPlayerMode() const;
};
EPlayerMode CTCInventoryMagic::GetPlayerMode() const
{
    return PLAYER_MODE_NAVIGATE_INVENTORY_MAGIC_SCREEN;
}
