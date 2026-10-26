// CInputProcessInventoryBase::ProcessInput(const CInputEvent& event, CProcessedInput& processed) -- for an analogue event, move the inventory screen's selection up/down (ChangeSelection 1/2) or page it left/right from the stick position past +-0.6 / +-0.9, else clear the current movement; on a button press or release, route game actions 0x25..0x28 to the screen's previous/next item and tab handlers and mark the input handled
// Leaf: ProcessInput
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006894C0, length 474 bytes (`ret 8`).
// Callees: CInputEvent::GetType @ 0x00A03B40, CInputEvent::GetAnaloguePosition @ 0x00A043A0 (C2DVector by hidden pointer, as the
//   landed CTCAssassinRush::UpdateDirectionOverride 0x007A9960), CInputProcessInventoryBase::ChangeSelection @ 0x006893D0,
//   CInputProcessGameBase::IsEventButtonPressed @ 0x00687A90 / IsEventButtonReleased @ 0x00687B30 / IsEventGameAction
//   @ 0x00687CF0 (manifest "_Cons_val" label is propagated), CProcessedInput::SetHandled @ 0x00A0D390.
// Layout: rebuild/include/engine/CInputProcessInventoryBase.h (sizeof 0x34): CurrentMovement +0x28 (ECurrentMovement). Modelled
//   locally because the slot-9 screen accessor (pure in the base vtable 0x012376CC, overridden per screen at 0x006896A0..) and
//   the screen's slots 38..41 / 62..65 need real virtuals. Stick thresholds: 0x0125BEC8 = 0.6f, 0x0125BED4 = -0.6f,
//   0x0125BED0 = -0.9f, 0x0125BECC = 0.9f.
class CInputEvent {
public:
    struct C2DVector { float X, Y; C2DVector() {} };
    long GetType() const;                          // 0x00A03B40
    C2DVector GetAnaloguePosition() const;         // 0x00A043A0
};
class CProcessedInput {
public:
    void SetHandled();                             // 0x00A0D390
};
class CInventoryScreen {
public:
    virtual void v00();
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
    virtual void v06();
    virtual void v07();
    virtual void v08();
    virtual void v09();
    virtual void v10();
    virtual void v11();
    virtual void v12();
    virtual void v13();
    virtual void v14();
    virtual void v15();
    virtual void v16();
    virtual void v17();
    virtual void v18();
    virtual void v19();
    virtual void v20();
    virtual void v21();
    virtual void v22();
    virtual void v23();
    virtual void v24();
    virtual void v25();
    virtual void v26();
    virtual void v27();
    virtual void v28();
    virtual void v29();
    virtual void v30();
    virtual void v31();
    virtual void v32();
    virtual void v33();
    virtual void v34();
    virtual void v35();
    virtual void v36();
    virtual void v37();
    virtual void OnStickDown();   // +0x98
    virtual void OnStickUp();   // +0x9c
    virtual void OnStickLeft();   // +0xa0
    virtual void OnStickRight();   // +0xa4
    virtual void v42();
    virtual void v43();
    virtual void v44();
    virtual void v45();
    virtual void v46();
    virtual void v47();
    virtual void v48();
    virtual void v49();
    virtual void v50();
    virtual void v51();
    virtual void v52();
    virtual void v53();
    virtual void v54();
    virtual void v55();
    virtual void v56();
    virtual void v57();
    virtual void v58();
    virtual void v59();
    virtual void v60();
    virtual void v61();
    virtual void OnPreviousPage();   // +0xf8
    virtual void OnNextPage();   // +0xfc
    virtual void OnPreviousTab();   // +0x100
    virtual void OnNextTab();   // +0x104
};
enum ECurrentMovement { MOVEMENT_NONE = 0, MOVEMENT_UP = 1, MOVEMENT_DOWN = 2 };
class CWorld; class CPlayer; class CGameDefinitionManager; class CMainGameComponent; class CGamePlayerInterface;
class CInputProcessInventoryBase {
public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03();
    virtual void ProcessInput(const CInputEvent& event, CProcessedInput& processed);   // slot 4
    virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08();
    virtual CInventoryScreen* GetScreen() = 0;     // slot 9
    unsigned char           _pad04[0x10];
    CWorld*                 World;                 // +0x14
    CPlayer*                MyPlayer;              // +0x18
    CGameDefinitionManager* DefinitionManager;     // +0x1c
    CMainGameComponent*     Component;             // +0x20
    CGamePlayerInterface*   PGamePlayerInterface;  // +0x24
    ECurrentMovement        CurrentMovement;       // +0x28
    unsigned long           FrameSinceLastMovement;   // +0x2c
    bool                    StartedRepeatingMovement; // +0x30
    bool IsEventButtonPressed(const CInputEvent& event);              // 0x00687A90
    bool IsEventButtonReleased(const CInputEvent& event);             // 0x00687B30
    bool IsEventGameAction(long action, const CInputEvent& event);    // 0x00687CF0
    bool ChangeSelection(ECurrentMovement movement);                  // 0x006893D0
};

void CInputProcessInventoryBase::ProcessInput(const CInputEvent& event, CProcessedInput& processed)
{
    CInventoryScreen* screen = GetScreen();
    if (event.GetType() == 0x11) {
        CInputEvent::C2DVector stick = event.GetAnaloguePosition();
        if (stick.Y > 0.6f) {
            if (ChangeSelection(MOVEMENT_UP))
                screen->OnStickUp();
            processed.SetHandled();
        } else if (stick.Y < -0.6f) {
            if (ChangeSelection(MOVEMENT_DOWN))
                screen->OnStickDown();
            processed.SetHandled();
        } else if (stick.X < -0.9f) {
            screen->OnStickLeft();
            processed.SetHandled();
        } else if (stick.X > 0.9f) {
            screen->OnStickRight();
            processed.SetHandled();
        } else
            CurrentMovement = MOVEMENT_NONE;
    }
    if (IsEventButtonPressed(event) || IsEventButtonReleased(event)) {
        if (IsEventGameAction(0x25, event)) {
            if (ChangeSelection(MOVEMENT_UP))
                GetScreen()->OnPreviousPage();
            processed.SetHandled();
        } else if (IsEventGameAction(0x26, event)) {
            if (ChangeSelection(MOVEMENT_UP))
                GetScreen()->OnNextPage();
            processed.SetHandled();
        } else if (IsEventGameAction(0x27, event)) {
            GetScreen()->OnPreviousTab();
            processed.SetHandled();
        } else if (IsEventGameAction(0x28, event)) {
            GetScreen()->OnNextTab();
            processed.SetHandled();
        }
    }
}