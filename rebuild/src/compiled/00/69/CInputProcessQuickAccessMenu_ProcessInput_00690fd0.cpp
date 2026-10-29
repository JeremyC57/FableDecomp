// CInputProcessQuickAccessMenu::ProcessInput(const CInputEvent& event, CProcessedInput& processed) -- while the live hero's quick-access menu (component 0xF6) is open and not closing, translate press/release of actions 0x25..0x28 into front-end UI events 0..3, and close the menu on a press of action 2; handled input is marked
// Leaf: ProcessInput
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00690FD0, length 325 bytes, `ret 8`.
// Callees: CPlayer::GetPControlledCreature @ 0x00487DC0 (MyPlayer +0x18), the component map's LowerBound @ 0x0040F020, the
//   menu component's IsActive() @ 0x00625930 (+0x0C), IsClosing() @ 0x00625940 (+0x1C) and RequestClose() @ 0x00625920
//   (manifest labels "GetCanItemBeQuickAssigned" / "CTCCreditsUI::Deactivate" are propagated; names as in the landed
//   CTCInventory::AddCloseInventoryEvent 0x00584A73), CInputProcessGameBase::IsEventButtonPressed @ 0x00687A90,
//   IsEventButtonReleased @ 0x00687B30, IsEventGameAction @ 0x00687CF0, NUISystem::CFrontEndManager::GetInstance
//   @ 0x0041E5F2 + slot 0, CProcessedInput::SetHandled @ 0x00A0D390.
// Layout: `this` is engine/CInputProcessGameBase.h (MyPlayer +0x18); CThing dead flag bit 0 of +0x91, component bitset
//   +0x20 (kind 0xF6 = word 7 bit 22) and map +0x44 as in the landed CInputProcessBetting::ProcessInput 0x00691560.
// Shape: the presence test precedes the lookup, so the component pick is the plain end/hit ternary (no preload); the
//   release arm's 0x25/0x26 sends are cross-jumped into from the press arm.
#include "engine/CInputProcessGameBase.h"
class CInputEvent;
class CProcessedInput {
public:
    void SetHandled();                             // 0x00A0D390
};
namespace NUISystem {
class CManager {
public:
    virtual void SendEvent(long event);            // slot 0
    static CManager* __fastcall GetInstance();   // 0x0041E5F2
};
}
class CTCQuickAccessMenu { public:
    bool IsActive() const;                         // 0x00625930
    bool IsClosing() const;                        // 0x00625940
    void RequestClose();                           // 0x00625920
};
struct STCEntry {                                  // one CVectorMap<ETCInterfaceType, CTCBase*> element
    long                m_kind;
    CTCQuickAccessMenu* m_pTC;
};
struct CTCMap {                                    // the thing's component map at +0x44
    STCEntry* m_pFirst;
    STCEntry* m_pLast;
    STCEntry* m_pEnd;
    STCEntry* LowerBound(const unsigned long& kind);   // 0x0040F020
    __forceinline CTCQuickAccessMenu* Find(long kind)
    {
        STCEntry* it = LowerBound((unsigned long)kind);
        return ((it == m_pLast || kind < it->m_kind) ? m_pLast : it)->m_pTC;
    }
};
class CThing { public:
    unsigned char _pad00[0x20];
    unsigned long m_tcMask[8];                     // +0x20: which component kinds the thing holds
    unsigned char _pad40[4];
    CTCMap        m_tcs;                           // +0x44
    unsigned char _pad50[0x91 - 0x50];
    unsigned char m_dead : 1;                      // +0x91 bit 0
    __forceinline unsigned long HasTC(long kind) const { return m_tcMask[kind >> 5] & (1UL << (kind & 31)); }
    __forceinline CTCQuickAccessMenu* GetTC(long kind) { return m_tcs.Find(kind); }
};
class CThingPlayerCreature : public CThing { };
class CPlayer {
public:
    CThingPlayerCreature* GetPControlledCreature() const;                     // 0x00487DC0
};
class CInputProcessQuickAccessMenu : public CInputProcessGameBase {
public:
    bool IsEventButtonPressed(const CInputEvent& event);              // 0x00687A90
    bool IsEventButtonReleased(const CInputEvent& event);             // 0x00687B30
    bool IsEventGameAction(long action, const CInputEvent& event);    // 0x00687CF0
    void ProcessInput(const CInputEvent& event, CProcessedInput& processed);
};

void CInputProcessQuickAccessMenu::ProcessInput(const CInputEvent& event, CProcessedInput& processed)
{
    CThing* hero = MyPlayer->GetPControlledCreature();
    if (!hero || hero->m_dead || !hero->HasTC(0xf6))
        return;
    CTCQuickAccessMenu* menu = hero->GetTC(0xf6);
    if (!menu->IsActive() || menu->IsClosing())
        return;
    if (IsEventButtonPressed(event)) {
        if (IsEventGameAction(0x25, event))
            NUISystem::CManager::GetInstance()->SendEvent(0);
        else if (IsEventGameAction(0x26, event))
            NUISystem::CManager::GetInstance()->SendEvent(1);
        else if (IsEventGameAction(0x27, event))
            NUISystem::CManager::GetInstance()->SendEvent(2);
        else if (IsEventGameAction(0x28, event))
            NUISystem::CManager::GetInstance()->SendEvent(3);
        else if (IsEventGameAction(2, event)) {
            menu->RequestClose();
            processed.SetHandled();
            return;
        } else
            return;
    } else if (IsEventButtonReleased(event)) {
        if (IsEventGameAction(0x25, event))
            NUISystem::CManager::GetInstance()->SendEvent(0);
        else if (IsEventGameAction(0x26, event))
            NUISystem::CManager::GetInstance()->SendEvent(1);
        else
            return;
    } else
        return;
    processed.SetHandled();
}