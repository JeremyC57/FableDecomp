// CInputProcessQuickAccessMenu::ProcessInput(const CInputEvent& event, CProcessedInput& processed) -- while the live hero's quick-access menu (component 0xF6) is open and not closing, translate press/release of actions 0x25..0x28 into front-end UI events 0..3, and close the menu on a press of action 2; handled input is marked
// Leaf: ?ProcessInput@CInputProcessQuickAccessMenu@@UAEXABVCInputEvent@@AAVCProcessedInput@@@Z
// NF478 (r467b, NAME_FIXES_QUEUE nf477, 2026-11-26): was the non-virtual ?ProcessInput@CInputProcessQuickAccessMenu@@QAE... on a local class
//   model; respelt onto engine/CInputProcessQuickAccessMenu.h FABLE_RETAIL_INPUTPROCESS_VTABLE (Ego_r.pdb / FableWin.pdb public virtual, retail
//   vftable slot 4), so the family vftables 0x00488D10 emits name this row. The base queries come from the retail view of
//   engine/CInputProcessGameBase.h (the same PDB spellings). No body change.
// NF64 (round 7, q10652): the local IsEventButtonPressed / Held / Released redeclarations on the derived class are gone; the calls bind
//   the Ego_r.pdb + FableWin.pdb protected const CInputProcessGameBase members (overlay opt-in FABLE_RETAIL_CINPUTPROCESSGAMEBASE_ISEVENTBUTTON).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00690FD0, length 325 bytes, `ret 8`.
// Callees: CPlayer::GetPControlledCreature @ 0x00487DC0 (MyPlayer +0x18), the component map's LowerBound @ 0x0040F020, the
//   menu component's IsActive() @ 0x00625930 (+0x0C), IsQuitting() @ 0x00625940 (+0x1C) and Deactivate() @ 0x00625920
//   (manifest labels "GetCanItemBeQuickAssigned" / "CTCCreditsUI::Deactivate" are propagated; names as in the landed
//   CTCInventory::AddCloseInventoryEvent 0x00584A73), CInputProcessGameBase::IsEventButtonPressed @ 0x00687A90,
//   IsEventButtonHeld @ 0x00687B30, IsEventGameAction @ 0x00687CF0, NUISystem::CFrontEndManager::GetInstance
//   @ 0x0041E5F2 + slot 0, CProcessedInput::SetAsPerformedEvent @ 0x00A0D390.
// Layout: `this` is engine/CInputProcessGameBase.h (MyPlayer +0x18); CThing dead flag bit 0 of +0x91, component bitset
//   +0x20 (kind 0xF6 = word 7 bit 22) and map +0x44 as in the landed CInputProcessBetting::ProcessInput 0x00691560.
// Shape: the presence test precedes the lookup, so the component pick is the plain end/hit ternary (no preload); the
//   release arm's 0x25/0x26 sends are cross-jumped into from the press arm.
#include "fable_compiler.h"
#include "engine/EGameAction.h"
#define FABLE_RETAIL_INPUTPROCESS
#define FABLE_RETAIL_CAINPUTPROCESS_ADDTOINTERFACE
#define FABLE_RETAIL_CAINPUTPROCESS_INIT
#define FABLE_RETAIL_INPUTPROCESS_VTABLE
#define FABLE_RETAIL_INPUTPROCESS_ZTARGET
#define FABLE_RETAIL_INPUTPROCESS_MOVEMENT
#include "engine/CInputProcessGameBase.h"
class CInputEvent;
class CProcessedInput {
public:
    void SetAsPerformedEvent();                             // 0x00A0D390 (Ego_r 0x00B74CA0 + FableWin 0x0318F830 ?SetAsPerformedEvent@CProcessedInput@@QAEXXZ; NF33, was SetAsPerformedEvent)
};
namespace NUISystem {
class CManager {
public:
    virtual void SendEvent(long event);            // slot 0
    static CManager* __fastcall GetInstance();   // 0x0041E5F2
};
}
// NF59 (round 7, queue q10497/q10498): the menu component is engine/CTCQuickAccessMenu.h (ISACTIVE, ISQUITTING_ACTIVATE, DEACTIVATE:
//   Ego_r.pdb / FableWin.pdb IsActive 0x00625930, IsQuitting 0x00625940, Deactivate 0x00625920) and the component map engine/CVectorMap.h
//   (as the sibling 0x0068FF50); the local CTCQuickAccessMenu stand-in (IsClosing / RequestClose) and CVectorMap specialization are gone.
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCQUICKACCESSMENU_ISACTIVE
#define FABLE_RETAIL_CTCQUICKACCESSMENU_ISQUITTING_ACTIVATE
#define FABLE_RETAIL_CTCQUICKACCESSMENU_DEACTIVATE
#include "engine/ETCInterfaceType.h"
#include "engine/CCountedPointer.h"
#include "engine/CVectorMap.h"
#include "engine/CTCQuickAccessMenu.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*> CTCMap;      // engine/CVectorMap.h: the thing's component map at +0x44
class CThing { public:
    unsigned char _pad00[0x20];
    unsigned long m_tcMask[8];                     // +0x20: which component kinds the thing holds
    unsigned char _pad40[4];
    CTCMap        m_tcs;                           // +0x44
    unsigned char _pad54[0x91 - 0x54];               // the map is the 16-byte CVectorMap (vector + Compare + Dirty)
    unsigned char m_dead : 1;                      // +0x91 bit 0
    FABLE_FORCEINLINE unsigned long HasTC(long kind) const { return m_tcMask[kind >> 5] & (1UL << (kind & 31)); }
    FABLE_FORCEINLINE CTCQuickAccessMenu* GetTC(long kind)
    {
        CTCMap* map = &m_tcs;
        CTCMap::value_type* it = map->LowerBound((ETCInterfaceType)kind);
        return (CTCQuickAccessMenu*)((it == map->end() || kind < it->first) ? map->end() : it)->second;
    }
};
class CThingPlayerCreature : public CThing { };
class CPlayer {
public:
    CThingPlayerCreature* GetPControlledCreature() const;                     // 0x00487DC0
};
#include "engine/CInputProcessQuickAccessMenu.h"

void CInputProcessQuickAccessMenu::ProcessInput(const CInputEvent& event, CProcessedInput& processed)
{
    CThing* hero = MyPlayer->GetPControlledCreature();
    if (!hero || hero->m_dead || !hero->HasTC(0xf6))
        return;
    CTCQuickAccessMenu* menu = hero->GetTC(0xf6);
    if (!menu->IsActive() || menu->IsQuitting())
        return;
    if (IsEventButtonPressed(event)) {
        if (IsEventGameAction(GAME_ACTION_INVENTORY_UP, event))
            NUISystem::CManager::GetInstance()->SendEvent(0);
        else if (IsEventGameAction(GAME_ACTION_INVENTORY_DOWN, event))
            NUISystem::CManager::GetInstance()->SendEvent(1);
        else if (IsEventGameAction(GAME_ACTION_INVENTORY_LEFT, event))
            NUISystem::CManager::GetInstance()->SendEvent(2);
        else if (IsEventGameAction(GAME_ACTION_INVENTORY_RIGHT, event))
            NUISystem::CManager::GetInstance()->SendEvent(3);
        else if (IsEventGameAction(GAME_ACTION_OPEN_INVENTORY, event)) {
            menu->Deactivate();
            processed.SetAsPerformedEvent();
            return;
        } else
            return;
    } else if (IsEventButtonHeld(event)) {
        if (IsEventGameAction(GAME_ACTION_INVENTORY_UP, event))
            NUISystem::CManager::GetInstance()->SendEvent(0);
        else if (IsEventGameAction(GAME_ACTION_INVENTORY_DOWN, event))
            NUISystem::CManager::GetInstance()->SendEvent(1);
        else
            return;
    } else
        return;
    processed.SetAsPerformedEvent();
}