// CInputProcessQuestCompletionUI::ProcessInput(const CInputEvent& event, CProcessedInput& processed) -- route input to the front end while the hero's quest-completion screen (component 0xFC) is up: pressed / held / released confirm, back, page-left and page-right actions become front-end events, action 0x48 closes the screen, mouse moves update the cursor and the wheel scrolls the menu
// Leaf: ProcessInput
// Retail: Fable.exe (TLC, VC7.1) @ 0x00691130, 755 bytes, `ret 8`.
// Shape: the action-0x47 arm copies the event's mouse position (a C2DVector, floats, inside UInputEventData at +0x0c)
//   into the front-end manager's MousePosition (+0xB0) through an 8-byte frame temporary that lives across the
//   GetInstance call (retail `sub esp,8`, stores to [esp+0xc]/[esp+0x10] before the call, reloads after). VC7.1
//   scalar-replaces a plain `T position = event.Position;` local into edi/esi (push ecx frame) whatever the member
//   type or constructors are; the memory home comes from returning the NON-POD C2DVector BY VALUE from an inline
//   getter (`GetMousePosition(event)` -> hidden return buffer = the local) and handing the local BY REFERENCE to an
//   inline manager setter (`SetMousePosition(CMousePosition&)`), the same form as the landed sibling
//   CInputProcessPhotojournalCapture::ProcessInput 0x00691AE0. Float members are required: with longs the same form
//   stays in registers.
#include "engine/CInputProcessGameBase.h"
struct CMousePosition {
    float X, Y;
    CMousePosition() {}
    CMousePosition(const CMousePosition& other) : X(other.X), Y(other.Y) {}
};
class CInputEvent {
public:
    unsigned char  _pad00[0x0c];
    CMousePosition Position;                       // +0x0C
    float GetWheelDelta() const;                   // 0x00A03BB0
};
class CProcessedInput {
public:
    void SetHandled();                             // 0x00A0D390
};
float __cdecl floor(float value);                  // 0x00689370 (float overload of MSVCR71 floor; first landed as ceil)
namespace NUISystem {
class CManager {
public:
    virtual void SendEvent(long event);            // slot 0
    unsigned char  _pad04[0xac];
    CMousePosition MousePosition;                  // +0xB0
    void SetMousePosition(CMousePosition& position) { MousePosition = position; }
    void ScrollMenu(long amount);                  // 0x0041DA2D
    static CManager* __fastcall GetInstance();   // 0x0041E5F2
};
}
class CTCQuestCompletionUI { public:
    unsigned char _pad00[0x0c];
    bool          Active;                          // +0x0C
    unsigned char _pad0d[0x0f];
    bool          Closing;                         // +0x1C
    unsigned char _pad1d[3];
    long          State;                           // +0x20
    void Deactivate();                             // 0x005E3DF0
};
class CTCBase;
enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
namespace std { template <class T1, class T2> struct pair { T1 first; T2 second; }; }   // STLport 4.6 <utility>
template <class K, class V> class CKeyPairCompareLess;
template <class K, class V, class TCompare> class CVectorMap;
typedef std::pair<ETCInterfaceType, CTCBase*> STCEntry;
template <> class CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > { public:                                    // the thing's component map at +0x44
    STCEntry* m_pFirst;
    STCEntry* m_pLast;
    STCEntry* m_pEnd;
    STCEntry* LowerBound(const ETCInterfaceType& kind);   // 0x0040F020
    __forceinline CTCQuestCompletionUI* Find(long kind)
    {
        STCEntry* it = LowerBound((ETCInterfaceType)kind);
        return ((CTCQuestCompletionUI*)((it == m_pLast || kind < it->first) ? m_pLast : it)->second);
    }
};
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CTCMap;
class CThing { public:
    unsigned char _pad00[0x20];
    unsigned long m_tcMask[9];                     // +0x20: which component kinds the thing holds
    CTCMap        m_tcs;                           // +0x44
    __forceinline unsigned long HasTC(long kind) const { return m_tcMask[kind >> 5] & (1UL << (kind & 31)); }
    __forceinline CTCQuestCompletionUI* GetTC(long kind) { return m_tcs.Find(kind); }
};
class CThingPlayerCreature : public CThing { };
class CPlayer {
public:
    CThingPlayerCreature* GetPControlledCreature() const;                     // 0x00487DC0
};
class CInputProcessQuestCompletionUI : public CInputProcessGameBase {
public:
    bool IsEventButtonPressed(const CInputEvent& event);              // 0x00687A90
    bool IsEventButtonReleased(const CInputEvent& event);             // 0x00687B30
    bool IsEventButtonHeld(const CInputEvent& event);                 // 0x00687BD0
    bool IsEventGameAction(long action, const CInputEvent& event);    // 0x00687CF0
    CMousePosition GetMousePosition(const CInputEvent& event) const { return event.Position; }
    void ProcessInput(const CInputEvent& event, CProcessedInput& processed);
};

void CInputProcessQuestCompletionUI::ProcessInput(const CInputEvent& event, CProcessedInput& processed)
{
    CThing* hero = MyPlayer->GetPControlledCreature();
    if (!hero->HasTC(0xfc))
        return;
    CTCQuestCompletionUI* ui = hero->GetTC(0xfc);
    if (!ui->Active || ui->Closing)
        return;
    if (IsEventButtonPressed(event)) {
        if (IsEventGameAction(0x19, event)) {
            NUISystem::CManager::GetInstance()->SendEvent(0x1a);
            processed.SetHandled();
            return;
        }
        if (IsEventGameAction(0x4c, event)) {
            NUISystem::CManager::GetInstance()->SendEvent(5);
            processed.SetHandled();
            return;
        }
        if (IsEventGameAction(0x58, event)) {
            NUISystem::CManager::GetInstance()->SendEvent(6);
            processed.SetHandled();
            return;
        }
        if (IsEventGameAction(0x59, event)) {
            NUISystem::CManager::GetInstance()->SendEvent(7);
            processed.SetHandled();
            return;
        }
        if (IsEventGameAction(0x48, event)) {
            if (ui->State == 0 || ui->State == 1)
                ui->Deactivate();
            processed.SetHandled();
        }
        return;
    }
    if (IsEventButtonHeld(event)) {
        if (IsEventGameAction(0x19, event)) {
            NUISystem::CManager::GetInstance()->SendEvent(0x1c);
            processed.SetHandled();
            return;
        }
        if (IsEventGameAction(0x4c, event)) {
            NUISystem::CManager::GetInstance()->SendEvent(0x1d);
            processed.SetHandled();
        }
        return;
    }
    if (IsEventButtonReleased(event)) {
        if (IsEventGameAction(0x19, event)) {
            NUISystem::CManager::GetInstance()->SendEvent(0x1f);
            processed.SetHandled();
            return;
        }
        if (IsEventGameAction(0x4c, event)) {
            NUISystem::CManager::GetInstance()->SendEvent(0x20);
            processed.SetHandled();
            return;
        }
        if (IsEventGameAction(0x58, event)) {
            NUISystem::CManager::GetInstance()->SendEvent(6);
            processed.SetHandled();
            return;
        }
        if (IsEventGameAction(0x59, event)) {
            NUISystem::CManager::GetInstance()->SendEvent(7);
            processed.SetHandled();
        }
        return;
    }
    if (IsEventGameAction(0x47, event)) {
        CMousePosition position = GetMousePosition(event);
        NUISystem::CManager::GetInstance()->SetMousePosition(position);
        NUISystem::CManager::GetInstance()->SendEvent(0x19);
        processed.SetHandled();
        return;
    }
    if (IsEventGameAction(0x65, event)) {
        NUISystem::CManager::GetInstance()->ScrollMenu((long)floor(event.GetWheelDelta() * 0.008333334f));
        processed.SetHandled();
    }
}