#include <cstddef>

class CCharString;

// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"

typedef void (CGameScriptInterface::*GiveHeroWeaponSlot_t)(const CCharString& weaponDefName, int slot, bool equipNow) const;

struct CGameScriptInterface_vftable
{
    unsigned char pad0[0x1E4];
    void* giveHeroWeaponSlot;
};

void CGameScriptInterface::GiveHeroWeapon(const CCharString& weaponDefName, bool equipNow) const
{
    const CGameScriptInterface_vftable* vft = *reinterpret_cast<CGameScriptInterface_vftable* const*>(this);
    void* raw = vft->giveHeroWeaponSlot;
    GiveHeroWeaponSlot_t fn;
    *reinterpret_cast<void**>(&fn) = raw;
    (this->*fn)(weaponDefName, -1, equipNow);
}