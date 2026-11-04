// CTCShop::FrameUpdate() -- per frame: flag the crime display while the live controlled hero stands inside the shop building, switch the alarm off once the shop's village no longer considers the hero a criminal, and on a new day (live hero and shopkeeper) clear, restock and re-display the shop
// Leaf: ?FrameUpdate@CTCShop@@UAEXXZ
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x00775B40, 274 bytes (plain `ret`). CTCShop vtable 0x0126A94C slot 9 (CTCBase::FrameUpdate).
// Callees: CTCBase::PeekComponent() const @ 0x00686D60, CPlayerManager::GetMainPlayer() const @ 0x00449960 (on the inline
//   CMainGameComponent::PeekPlayerManager), CPlayer::PeekPControlledCreature() const @ 0x00487DD0, CThing::GetPos() const
//   @ 0x004C73D0, CThingBuilding::IsPointInside(const C3DVector&) const @ 0x0082D000, CPlayerGui::SetCrimeAsDisplaying()
//   @ 0x004376B0 (on CTBaseSingletonNoDefaultAlloc<CPlayerGui>::Get()), CVectorMap lower_bound @ 0x0040F020 (GetTC<CTCVillageMember>),
//   CTCVillageMember::GetPVillage() const @ 0x007EE540, CTCVillage::IsHeroCriminal() @ 0x0065BFCE, CTCShop::SetAlarm(bool)
//   @ 0x00773C90, CBaseIntelligentPointer::GetPItem @ 0x00A01B50, CTCBase::GetWorld() const @ 0x00686A70,
//   CTCShop::RemoveStockItemsFromShop @ 0x00773E10, CTCShop::UpdateStock @ 0x00775350, CTCShop::IsPermanentlyStocked() const
//   @ 0x007721A0, IS_THING_ALIVE<CIntelligentPointer<CThingCreatureBase> > (out of line, `__fastcall`) @ 0x006961F0,
//   CTCShop::StockDisplay @ 0x00774C60.
// Layout: the shared CTCShop (PShopKeeper +0x10, DayLastChecked +0x48, Alarm +0x54); CWorld PGameTimeManager +0x1C,
//   CGameTimeManager DayCount +0x14 (Ego_r.pdb; inline GetDayCount).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCBASE_COMPONENT
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGBUILDING
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGPLAYERCREATURE
#define FABLE_RETAIL_CPLAYERMANAGER
#define FABLE_RETAIL_PLAYER
#define FABLE_RETAIL_CPLAYERGUI
#define FABLE_RETAIL_CPLAYERGUI_CRIME
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_CMAINGAMECOMPONENT_PLAYERMANAGER
#define FABLE_RETAIL_CTCVILLAGE
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CTCSHOP
#include "fable_string.h"
#include "engine/CThing.h"
#include "engine/CTCBase.h"
#include "engine/CIntelligentPointer.h"
#include "engine/CThingBuilding.h"
#include "engine/CThingCreatureBase.h"
#include "engine/CThingPlayerCreature.h"
#include "engine/CPlayerManager.h"
#include "engine/CPlayer.h"
#include "engine/CPlayerGui.h"
#include "engine/CMainGameComponent.h"
#include "engine/CWorld.h"
#include "engine/CTCVillage.h"
#include "engine/CTCVillageMember.h"
#include "engine/CTCShop.h"

class CGameTimeManager {                                            // no shared header (Ego_r.pdb layout)
public:
    long GetDayCount() const { return DayCount; }                   // ?GetDayCount@CGameTimeManager@@QBEJXZ (inline)
private:
    CWorld*             World;                                      // +0x00
    CMainGameComponent* GameComponent;                              // +0x04
    float               Time;                                       // +0x08
    float               PrevTime;                                   // +0x0c
    float               TimeStep;                                   // +0x10
    long                DayCount;                                   // +0x14
};
// CClass::Type 3 = building (the type byte the draft and the landed CThingBuilding rows test).
inline bool IS_THING_ALIVE(const CThing* thing) { return thing != 0 && !thing->IsDead(); }
template<class T> bool __fastcall IS_THING_ALIVE(const T& pointer);   // 0x006961F0 (out of line)

void CTCShop::FrameUpdate()
{
    const CThing* hero = PeekComponent().PeekPlayerManager().GetMainPlayer().PeekPControlledCreature();
    if (IS_THING_ALIVE(hero)) {
        CThing* parent = ParentThing;
        if (parent->GetType() == 3 && static_cast<CThingBuilding*>(parent)->IsPointInside(hero->GetPos()))
            CTBaseSingletonNoDefaultAlloc<CPlayerGui>::Get().SetCrimeAsDisplaying();
    }
    CTCVillageMember* member;
    if (Alarm && ParentThing->GetTC(&member)) {
        CTCVillage* village = member->GetPVillage();
        if (village && !village->IsHeroCriminal())
            SetAlarm(false);
    }
    if (IS_THING_ALIVE(hero) && PShopKeeper.Get() && !PShopKeeper.Get()->IsDead()
        && DayLastChecked != GetWorld().GetGameTimeManager().GetDayCount()) {
        RemoveStockItemsFromShop();
        UpdateStock();
        if (IsPermanentlyStocked() && IS_THING_ALIVE(PShopKeeper))
            StockDisplay();
    }
}