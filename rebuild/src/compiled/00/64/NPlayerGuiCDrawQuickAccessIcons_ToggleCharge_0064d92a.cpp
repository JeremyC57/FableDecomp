// NPlayerGui::CDrawQuickAccessIcons::ToggleCharge(long buttonIndex, bool start) -- start charging a quick-access button (validated against the hero's inventory unless -2), playing the charge-up sound when none is running, or stop charging (sound cleared, no button)
// Leaf: ?ToggleCharge@CDrawQuickAccessIcons@NPlayerGui@@QAEXJ_N@Z
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x0064D92A, 304 bytes (`ret 8`). Frame-pointer size TU: optimize("y",off) + ("s",on), as the
//   landed same-TU sibling ClearSoundEffects 0x0064B668.
// Callees: CPlayerManager::GetMainPlayer 0x00449970, CPlayer::GetPControlledCreature 0x00487DC0, IS_THING_ALIVE<CThingPlayerCreature*>
//   0x0043DBD0 (__fastcall), std::bitset<274>::test 0x00410DE0, CVectorMap LowerBound const 0x004365B0 (PeekTC<const CTCInventory>) /
//   non-const 0x0040F020 (GetTC<CTCSoundPlayer>), CTCInventory::GetQuickAccessItemDef(long) const 0x00581DDB, CCharString 0x0099EBF0 /
//   ~CCharString 0x0099EAE0, CTCSoundPlayer::PlayAttachedCriteria 0x0066BD00, CDrawQuickAccessIcons::ClearSoundEffects 0x0064B668.
// Layout (Ego_r.pdb names, retail offsets: CDrawBase's vector is 12 bytes): ChargingSoundID +0x28, ChargingButtonIndex +0x2C,
//   ChargingButtonTime +0x30, Charging +0x34, FullyCharged +0x35.
#pragma optimize("y", off)
#pragma optimize("s", on)
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_PLAYER
#define FABLE_RETAIL_CPLAYERMANAGER
#define FABLE_RETAIL_CTCSOUNDPLAYER
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CVECTORMAP_FORCEINLINE_FIND
#include "fable_string.h"
#include "engine/CThing.h"
#include "engine/CIntelligentPointer.h"
#include "engine/CDefPointer.h"
#include "engine/CDefClassBase.h"
#include "engine/CPlayerManager.h"
#include "engine/CPlayer.h"
#include "engine/CThingPlayerCreature.h"
#include "engine/CTCSoundPlayer.h"


class CInventoryItemDef : public CDefClassBase { };
class CTCInventory : public CTCBase { public:                   // RTTI .?AVCTCInventory@@ : CTCBase; no retail engine header
    static ETCInterfaceType __fastcall StatGetInterfaceType() { return TCI_INVENTORY; }
    CDefPointer<const CInventoryItemDef> GetQuickAccessItemDef(long buttonIndex) const;   // 0x00581DDB
};

// Same-TU body of IS_THING_ALIVE<CThingPlayerCreature*> 0x0043DBD0, as the landed same-TU siblings ClearSoundEffects 0x0064B668 and
// Update 0x0064D6A8: VC7.1 then knows the reference is not kept, so the hero stays in ebx and a copy is bound in the frame.
template <class T> __declspec(noinline) bool __fastcall IS_THING_ALIVE(const T& thing)
{
    return thing != 0 && !thing->IsDead();
}

class CGraphicDataBank; class CMainGameComponent; class CPlayerGui; class CWorld; class CEnginePrimitiveHandle;
namespace NPlayerGui {
class CDrawBase { public:                                       // Ego_r.pdb members, retail offsets
    virtual ~CDrawBase();
    CPlayerGui* PlayerGui;                                      // +0x04
    CPlayerManager* PlayerManager;                              // +0x08
    CWorld* World;                                              // +0x0C
    CMainGameComponent* Component;                              // +0x10
    CGraphicDataBank* Bank;                                     // +0x14
    bool Drawing;                                               // +0x18
    std::vector<CEnginePrimitiveHandle> PrimitiveHandles;       // +0x1C
};
class CDrawQuickAccessIcons : public CDrawBase { public:
    unsigned long ChargingSoundID;                              // +0x28
    long ChargingButtonIndex;                                   // +0x2C
    float ChargingButtonTime;                                   // +0x30
    bool Charging;                                              // +0x34
    bool FullyCharged;                                          // +0x35
    void ClearSoundEffects();                                   // 0x0064B668
    void ToggleCharge(long buttonIndex, bool start);
};

void CDrawQuickAccessIcons::ToggleCharge(long buttonIndex, bool start)
{
    if (start) {
        CThingPlayerCreature* hero = PlayerManager->GetMainPlayer().GetPControlledCreature();
        if (!IS_THING_ALIVE(hero))
            return;
        if (buttonIndex != -2) {
            const CTCInventory* inventory = 0;
            hero->PeekTC(&inventory);
            CDefPointer<const CInventoryItemDef> item = inventory->GetQuickAccessItemDef(buttonIndex);
            if (item.IsNull())
                return;
        }
        if (ChargingSoundID == 0) {
            CTCSoundPlayer* sound;
            if (hero->GetTC(&sound)) {
                CCharString name("GUILD_SEAL_CHARGEUP", -1);
                ChargingSoundID = sound->PlayAttachedCriteria(name, true, false, false, -1);
            }
        }
        ChargingButtonTime = 0.0f;
        ChargingButtonIndex = buttonIndex;
        Charging = true;
        FullyCharged = false;
    } else {
        ClearSoundEffects();
        ChargingButtonIndex = -1;
        Charging = false;
    }
}

}