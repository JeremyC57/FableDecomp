// CInputProcessInventoryTradeScreen::ProcessInput(const CInputEvent& event, CProcessedInput& processed) -- refresh the process (own virtual slot 9), then route input to the front end and the hero's inventory map (component 0x1B): pressed / held / released menu actions become front-end events, held action 0x48 queues a close-inventory game event, released d-pad actions zoom (front-end slot 38) or pan the map, mouse moves update the cursor and the wheel scrolls the menu
// Leaf: ProcessInput
// Flags: /GS
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0068B7C0, length 1111 bytes, `ret 8`.
// Callees: CPlayer::GetHeroCreature @ 0x00487DC0, CVectorMap LowerBound @ 0x0040F020, CInputProcessGameBase::
//   IsEventButtonPressed @ 0x00687A90 / IsEventButtonHeld @ 0x00687BD0 / IsEventButtonReleased @ 0x00687B30 /
//   IsEventGameAction @ 0x00687CF0, NUISystem::CFrontEndManager::GetInstance @ 0x0041E5F2 (+ slot 0 SendEvent, slot 38
//   +0x98, ScrollMenu @ 0x0041DA2D), CPlayerManager::GetMainPlayerNumber @ 0x00449700, CMainGameComponent::AddGameEvent
//   @ 0x0041649C (on 0x013B86A0), CTCInventoryMap::PanMap @ 0x005FB140, CInputEvent::GetWheelDelta @ 0x00A03BB0,
//   ceil @ 0x00689370, CProcessedInput::SetHandled @ 0x00A0D390, __security_check_cookie @ 0x00BFE9F9.
// GS note: /GS -- the CGameEvent's 32-byte data array gives the cookie frame.
// Layout: engine/CInputProcessGameBase.h layout (MyPlayer +0x18), modelled locally because the process's own slot-9 virtual is
//   called (the header's vptr is a plain word), as the landed CInputProcessInventoryStatsScreen::ProcessInput 0x00689C40;
//   byte-twin of CInputProcessInventoryMapScreen::ProcessInput 0x0068AE30 apart from the Refresh call; the game event, mouse position and component-map
//   lookups as the landed siblings CInputProcessQuestCompletionUI::ProcessInput 0x00691130 and
//   CTCInventoryBase::AddCloseInventoryEvent 0x005BC527.
class CPlayer;
class CInputProcessGameBase {                      // engine/CInputProcessGameBase.h layout
public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08();
    virtual void Refresh();                        // slot 9 (+0x24)
    unsigned char _pad04[0x14];                    // +0x04 CAInputProcess .. World
    CPlayer*      MyPlayer;                        // +0x18
    unsigned char _pad1c[0x0c];                    // +0x1C DefinitionManager .. PGamePlayerInterface
};
struct CMousePosition {
    float X, Y;
    CMousePosition() {}
    CMousePosition(float x, float y) : X(x), Y(y) {}
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
float __cdecl ceil(float value);                   // 0x00689370
namespace NUISystem {
class CFrontEndManager {
public:
    virtual void SendEvent(long event);            // slot 0
    virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10();
    virtual void v11(); virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15();
    virtual void v16(); virtual void v17(); virtual void v18(); virtual void v19(); virtual void v20();
    virtual void v21(); virtual void v22(); virtual void v23(); virtual void v24(); virtual void v25();
    virtual void v26(); virtual void v27(); virtual void v28(); virtual void v29(); virtual void v30();
    virtual void v31(); virtual void v32(); virtual void v33(); virtual void v34(); virtual void v35();
    virtual void v36(); virtual void v37();
    virtual void SetZoomDirection(const CMousePosition& direction);   // slot 38 (+0x98)
    unsigned char  _pad04[0xac];
    CMousePosition MousePosition;                  // +0xB0
    void SetMousePosition(CMousePosition& position) { MousePosition = position; }
    void ScrollMenu(long amount);                  // 0x0041DA2D
    static CFrontEndManager* __fastcall GetInstance();   // 0x0041E5F2
};
}
struct CMemoryBuffer {
    void Copy(unsigned long length, const void* data);   // 0x009F1760
};
struct CGameEvent : CMemoryBuffer {
    long          Type;                           // +0x00
    signed char   Player;                         // +0x04
    unsigned char Data[32];                       // +0x05
    unsigned char EndPos;                         // +0x25
    bool          Valid;                          // +0x26
    bool          Replacement;                    // +0x27
    CGameEvent(long type) : Type(type), Player(-1), EndPos(0), Valid(true), Replacement(false) { }
};
struct CPlayerManager {
    long GetMainPlayerNumber();                   // 0x00449700
};
struct CMainGameComponent {
    unsigned char   _pad00[0x1c];
    CPlayerManager* PlayerManager;                // +0x1C
    void AddGameEvent(const CGameEvent& event);   // 0x0041649C
};
extern CMainGameComponent* g_pMainGameComponent;  // 0x013B86A0
struct CTCInventoryMap {
    void PanMap(CMousePosition direction);        // 0x005FB140
};
struct STCEntry {                                  // one CVectorMap<ETCInterfaceType, CTCBase*> element
    long             m_kind;
    CTCInventoryMap* m_pTC;
};
struct CTCMap {                                    // the thing's component map at +0x44
    STCEntry* m_pFirst;
    STCEntry* m_pLast;
    STCEntry* m_pEnd;
    STCEntry* LowerBound(const unsigned long& kind);   // 0x0040F020
    __forceinline CTCInventoryMap* Find(long kind)
    {
        STCEntry* it = LowerBound((unsigned long)kind);
        return ((it == m_pLast || kind < it->m_kind) ? m_pLast : it)->m_pTC;
    }
};
struct CThing {
    unsigned char _pad00[0x20];
    unsigned long m_tcMask[9];                     // +0x20
    CTCMap        m_tcs;                           // +0x44
    __forceinline unsigned long HasTC(long kind) const { return m_tcMask[kind >> 5] & (1UL << (kind & 31)); }
    __forceinline CTCInventoryMap* GetTC(long kind) { return m_tcs.Find(kind); }
};
class CPlayer {
public:
    CThing* GetHeroCreature();                     // 0x00487DC0
};
class CInputProcessInventoryTradeScreen : public CInputProcessGameBase {
public:
    bool IsEventButtonPressed(const CInputEvent& event);              // 0x00687A90
    bool IsEventButtonReleased(const CInputEvent& event);             // 0x00687B30
    bool IsEventButtonHeld(const CInputEvent& event);                 // 0x00687BD0
    bool IsEventGameAction(long action, const CInputEvent& event);    // 0x00687CF0
    CMousePosition GetMousePosition(const CInputEvent& event) const { return event.Position; }
    void ProcessInput(const CInputEvent& event, CProcessedInput& processed);
};

void CInputProcessInventoryTradeScreen::ProcessInput(const CInputEvent& event, CProcessedInput& processed)
{
    Refresh();
    CTCInventoryMap* map = 0;
    CThing* hero = MyPlayer->GetHeroCreature();
    if (hero->HasTC(0x1b))
        map = hero->GetTC(0x1b);
    if (IsEventButtonPressed(event)) {
        if (IsEventGameAction(0x19, event))
            NUISystem::CFrontEndManager::GetInstance()->SendEvent(0x1a);
        else if (IsEventGameAction(0x4c, event))
            NUISystem::CFrontEndManager::GetInstance()->SendEvent(5);
        else if (IsEventGameAction(0x58, event))
            NUISystem::CFrontEndManager::GetInstance()->SendEvent(6);
        else if (IsEventGameAction(0x59, event))
            NUISystem::CFrontEndManager::GetInstance()->SendEvent(7);
        else if (IsEventGameAction(0x25, event))
            NUISystem::CFrontEndManager::GetInstance()->SendEvent(0);
        else if (IsEventGameAction(0x26, event))
            NUISystem::CFrontEndManager::GetInstance()->SendEvent(1);
        else if (IsEventGameAction(0x27, event))
            NUISystem::CFrontEndManager::GetInstance()->SendEvent(5);
        else if (IsEventGameAction(0x28, event))
            NUISystem::CFrontEndManager::GetInstance()->SendEvent(4);
        else
            return;
    } else if (IsEventButtonHeld(event)) {
        if (IsEventGameAction(0x19, event))
            NUISystem::CFrontEndManager::GetInstance()->SendEvent(0x1c);
        else if (IsEventGameAction(0x4c, event))
            NUISystem::CFrontEndManager::GetInstance()->SendEvent(0x1d);
        else if (IsEventGameAction(0x48, event)) {
            CGameEvent closeEvent(0x1b);
            closeEvent.Player = (signed char)g_pMainGameComponent->PlayerManager->GetMainPlayerNumber();
            g_pMainGameComponent->AddGameEvent(closeEvent);
        } else
            return;
    } else if (IsEventButtonReleased(event)) {
        if (IsEventGameAction(0x19, event))
            NUISystem::CFrontEndManager::GetInstance()->SendEvent(0x1f);
        else if (IsEventGameAction(0x4c, event))
            NUISystem::CFrontEndManager::GetInstance()->SendEvent(0x20);
        else if (IsEventGameAction(0x58, event))
            NUISystem::CFrontEndManager::GetInstance()->SendEvent(6);
        else if (IsEventGameAction(0x59, event))
            NUISystem::CFrontEndManager::GetInstance()->SendEvent(7);
        else if (IsEventGameAction(0x67, event)) {
            CMousePosition zoom(1.0f, 1.0f);
            NUISystem::CFrontEndManager::GetInstance()->SetZoomDirection(zoom);
            NUISystem::CFrontEndManager::GetInstance()->SendEvent(0xc);
        } else if (IsEventGameAction(0x66, event)) {
            CMousePosition zoom(-1.0f, -1.0f);
            NUISystem::CFrontEndManager::GetInstance()->SetZoomDirection(zoom);
            NUISystem::CFrontEndManager::GetInstance()->SendEvent(0xd);
        } else if (IsEventGameAction(0x6b, event)) {
            if (map)
                map->PanMap(CMousePosition(0.0f, -1.0f));
        } else if (IsEventGameAction(0x6a, event)) {
            if (map)
                map->PanMap(CMousePosition(0.0f, 1.0f));
        } else if (IsEventGameAction(0x68, event)) {
            if (map)
                map->PanMap(CMousePosition(-1.0f, 0.0f));
        } else if (IsEventGameAction(0x69, event)) {
            if (map)
                map->PanMap(CMousePosition(1.0f, 0.0f));
        } else
            return;
    } else if (IsEventGameAction(0x47, event)) {
        CMousePosition position = GetMousePosition(event);
        NUISystem::CFrontEndManager::GetInstance()->SetMousePosition(position);
        NUISystem::CFrontEndManager::GetInstance()->SendEvent(0x19);
    } else if (IsEventGameAction(0x65, event))
        NUISystem::CFrontEndManager::GetInstance()->ScrollMenu((long)ceil(event.GetWheelDelta() * 0.008333334f));
    else
        return;
    processed.SetHandled();
}
