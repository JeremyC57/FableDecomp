// Behaviour test for 0x004963A0 CFrontendGameComponent::Input: the row's body over a model with a scripted event stream.
#include <cstdio>
#include <cstring>
enum EInputEventType { EInputEventType_Pad = 0x7fffffff };
enum EInputKey { EInputKey_Pad = 0x7fffffff };
enum EGameMode { EGameMode_Pad = 0x7fffffff };
namespace NGameComponentHelpers { enum ECursorMovement { ECursorMovement_Pad = 0x7fffffff }; }
class C2DVector { public: float X, Y; };
class CInputEvent { public:
    int type; long key; long button; long device; float y;
    EInputEventType GetType() const { return (EInputEventType)type; }
    EInputKey GetKey() const { return (EInputKey)key; }
    long GetJoystickButton() const { return button; }
    long GetDeviceNumber() const { return device; }
    C2DVector GetJoystickPosition() const { C2DVector v; v.X = 0.0f; v.Y = y; return v; }
};
class CInputEventScanInfo { public: bool Started; long Index; CInputEventScanInfo() : Started(false) { } };
class CInputManager { public:
    CInputEvent events[8]; int count; int updates; int ends; int scanned;
    void Update() { ++updates; }
    bool EventScanStart(CInputEventScanInfo& info, CInputEvent& e) { info.Started = true; info.Index = 0; if (count == 0) return false; e = events[0]; ++scanned; return true; }
    bool EventScanNext(CInputEventScanInfo& info, CInputEvent& e) { if (++info.Index >= count) return false; e = events[info.Index]; ++scanned; return true; }
    void EventScanEnd(CInputEventScanInfo&) { ++ends; }
};
static CInputManager gManager;
static CInputManager* PInputManager = &gManager;
static long g_KeyToNumber[256];
template <class T, class TMin, class TMax> void GFWrapLimit(T& value, const TMin& minimum, const TMax& maximum)
{
    if (value < minimum) value = maximum;
    else if (value > maximum) value = minimum;
}
struct CFrontendGameComponent {
    bool CreateGame; long CreateGameJoystickDeviceNumber; bool CreateEditor; bool CreateReleaseFrontEnd;
    EGameMode Mode; long CursorHighlight; char String[128]; long StringPos;
    bool allowMove; long maxWorld, maxSave, maxProfile; int quits, worldFetches, saveModeCalls, moreWorlds, moreSaves, moreProfiles;
    long chosenWorld, chosenSave, chosenProfile;
    void Reset(int mode, long highlight)
    {
        CreateGame = false; CreateGameJoystickDeviceNumber = -1; CreateEditor = true; CreateReleaseFrontEnd = false;
        Mode = (EGameMode)mode; CursorHighlight = highlight; std::memset(String, 'x', sizeof(String)); StringPos = 5;
        allowMove = true; maxWorld = 3; maxSave = 2; maxProfile = 4; quits = worldFetches = saveModeCalls = 0;
        moreWorlds = moreSaves = moreProfiles = 0; chosenWorld = chosenSave = chosenProfile = 0;
    }
    bool ChangeSelection(NGameComponentHelpers::ECursorMovement) { return allowMove; }
    long GetMaxVisibleWorldNumber() const { return maxWorld; }
    long GetMaxVisibleSaveNumber() const { return maxSave; }
    long GetMaxVisibleProfileNumber() const { return maxProfile; }
    void ShowMoreWorlds() { ++moreWorlds; }
    void ShowMoreSaves() { ++moreSaves; }
    void ShowMoreProfiles() { ++moreProfiles; }
    void ChooseWorldNumber(long n) { chosenWorld = n; }
    void ChooseSaveNumber(long n) { chosenSave = n; }
    void ChooseProfileNumber(long n) { chosenProfile = n; }
    void SetQuit() { ++quits; }
    void SetMode(EGameMode mode)
    {
        Mode = mode; CursorHighlight = 1; std::memset(String, 0, sizeof(String)); StringPos = 0;
        if (Mode == 10) { ++worldFetches; return; }
        if (mode == 11) ++saveModeCalls;
    }
    void Input();
};

void CFrontendGameComponent::Input()
{
    CInputEventScanInfo scanInfo;
    CInputEvent event;
    CInputEvent unusedEvent;
    PInputManager->Update();
    bool handled = false;
    if (PInputManager->EventScanStart(scanInfo, event) == true) do {
        if (handled == true) break;
        switch (Mode) {
        case 1:
            if (event.GetType() == (EInputEventType)0x11) {
                C2DVector position = event.GetJoystickPosition();
                if (position.Y > 0.5f) {
                    if (ChangeSelection((NGameComponentHelpers::ECursorMovement)2)) {
                        ++CursorHighlight;
                        if (CursorHighlight == 4 || CursorHighlight == 5)
                            CursorHighlight = 6;
                        GFWrapLimit<long, int, int>(CursorHighlight, 1, 7);
                    }
                } else if (position.Y < -0.5f) {
                    if (ChangeSelection((NGameComponentHelpers::ECursorMovement)1)) {
                        --CursorHighlight;
                        if (CursorHighlight == 4 || CursorHighlight == 5)
                            CursorHighlight = 3;
                        GFWrapLimit<long, int, int>(CursorHighlight, 1, 7);
                    }
                }
            } else if (event.GetType() == (EInputEventType)0x15) {
                long button = event.GetJoystickButton();
                if (button == 1 || button == 2 || button == 4 || button == 5) {
                    switch (CursorHighlight) {
                    case 1:
                        CreateGame = true;
                        CreateGameJoystickDeviceNumber = event.GetDeviceNumber();
                        handled = true;
                        SetQuit();
                        break;
                    case 2:
                        CreateGame = true;
                        CreateGameJoystickDeviceNumber = event.GetDeviceNumber();
                        SetMode((EGameMode)10);
                        handled = true;
                        break;
                    case 3:
                        CreateGame = true;
                        CreateGameJoystickDeviceNumber = event.GetDeviceNumber();
                        SetMode((EGameMode)11);
                        handled = true;
                        break;
                    case 6:
                        SetQuit();
                        handled = true;
                        break;
                    case 7:
                        CreateReleaseFrontEnd = true;
                        SetQuit();
                        handled = true;
                        break;
                    }
                }
            } else if (event.GetType() == (EInputEventType)3) {
                if (event.GetKey() == (EInputKey)2 || event.GetKey() == (EInputKey)0x4f) {
                    CreateGame = true;
                    handled = true;
                    SetQuit();
                }
                if (event.GetKey() == (EInputKey)3 || event.GetKey() == (EInputKey)0x50) {
                    CreateGame = true;
                    SetMode((EGameMode)10);
                    handled = true;
                }
                if (event.GetKey() == (EInputKey)4 || event.GetKey() == (EInputKey)0x51) {
                    CreateGame = true;
                    SetMode((EGameMode)11);
                    handled = true;
                }
                if (event.GetKey() == (EInputKey)7 || event.GetKey() == (EInputKey)0x4d) {
                    SetQuit();
                    handled = true;
                }
                if (event.GetKey() == (EInputKey)8 || event.GetKey() == (EInputKey)0x47) {
                    CreateReleaseFrontEnd = true;
                    SetQuit();
                    handled = true;
                }
            }
            break;
        case 10:
            if (event.GetType() == (EInputEventType)0x11) {
                C2DVector position = event.GetJoystickPosition();
                if (position.Y > 0.5f) {
                    if (ChangeSelection((NGameComponentHelpers::ECursorMovement)2)) {
                        long highlight = ++CursorHighlight;
                        if (highlight > 9)
                            CursorHighlight = 0;
                        else if (highlight > GetMaxVisibleWorldNumber())
                            CursorHighlight = 9;
                    }
                } else if (position.Y < -0.5f) {
                    if (ChangeSelection((NGameComponentHelpers::ECursorMovement)1)) {
                        if (--CursorHighlight < 0)
                            CursorHighlight = 9;
                        long highlight = CursorHighlight;
                        if (highlight < 9 && highlight > GetMaxVisibleWorldNumber())
                            CursorHighlight = GetMaxVisibleWorldNumber();
                    }
                }
            } else if (event.GetType() == (EInputEventType)0x15) {
                long button = event.GetJoystickButton();
                if (button == 1 || button == 2 || button == 4 || button == 5) {
                    long highlight = CursorHighlight;
                    if (highlight >= 1 && highlight <= GetMaxVisibleWorldNumber()) {
                        ChooseWorldNumber(highlight);
                        handled = true;
                    } else if (highlight == 0) {
                        ShowMoreWorlds();
                        handled = true;
                    } else if (highlight == 9) {
                        CreateEditor = false;
                        CreateGame = false;
                        SetQuit();
                        handled = true;
                    }
                }
            } else if (event.GetType() == (EInputEventType)3) {
                long number = g_KeyToNumber[event.GetKey()];
                if (number >= 1 && number <= 8) {
                    ChooseWorldNumber(number);
                    handled = true;
                } else if (event.GetKey() == (EInputKey)0xb || event.GetKey() == (EInputKey)0x52) {
                    ShowMoreWorlds();
                    handled = true;
                } else if (event.GetKey() == (EInputKey)0xa || event.GetKey() == (EInputKey)0x49) {
                    CreateEditor = false;
                    CreateGame = false;
                    SetQuit();
                    handled = true;
                }
            }
            break;
        case 11:
            if (event.GetType() == (EInputEventType)0x11) {
                C2DVector position = event.GetJoystickPosition();
                if (position.Y > 0.5f) {
                    if (ChangeSelection((NGameComponentHelpers::ECursorMovement)2)) {
                        long highlight = ++CursorHighlight;
                        if (highlight > 9)
                            CursorHighlight = 0;
                        else if (highlight > GetMaxVisibleSaveNumber())
                            CursorHighlight = 9;
                    }
                } else if (position.Y < -0.5f) {
                    if (ChangeSelection((NGameComponentHelpers::ECursorMovement)1)) {
                        if (--CursorHighlight < 0)
                            CursorHighlight = 9;
                        long highlight = CursorHighlight;
                        if (highlight < 9 && highlight > GetMaxVisibleSaveNumber())
                            CursorHighlight = GetMaxVisibleSaveNumber();
                    }
                }
            } else if (event.GetType() == (EInputEventType)0x15) {
                long button = event.GetJoystickButton();
                if (button == 1 || button == 2 || button == 4 || button == 5) {
                    long highlight = CursorHighlight;
                    if (highlight >= 1 && highlight <= GetMaxVisibleSaveNumber()) {
                        ChooseSaveNumber(highlight);
                        handled = true;
                    } else if (highlight == 0) {
                        ShowMoreSaves();
                        handled = true;
                    } else if (highlight == 9) {
                        CreateEditor = false;
                        CreateGame = false;
                        SetQuit();
                        handled = true;
                    }
                }
            } else if (event.GetType() == (EInputEventType)3) {
                long number = g_KeyToNumber[event.GetKey()];
                if (number >= 1 && number <= 8) {
                    ChooseSaveNumber(number);
                    handled = true;
                } else if (event.GetKey() == (EInputKey)0xb || event.GetKey() == (EInputKey)0x52) {
                    ShowMoreSaves();
                    handled = true;
                } else if (event.GetKey() == (EInputKey)0xa || event.GetKey() == (EInputKey)0x49) {
                    CreateEditor = false;
                    CreateGame = false;
                    SetQuit();
                    handled = true;
                }
            }
            break;
        case 12:
            if (event.GetType() == (EInputEventType)0x11) {
                C2DVector position = event.GetJoystickPosition();
                if (position.Y > 0.5f) {
                    if (ChangeSelection((NGameComponentHelpers::ECursorMovement)2)) {
                        long highlight = ++CursorHighlight;
                        if (highlight > 12)
                            CursorHighlight = 1;
                        else if (highlight > GetMaxVisibleProfileNumber())
                            CursorHighlight = 11;
                    }
                } else if (position.Y < -0.5f) {
                    if (ChangeSelection((NGameComponentHelpers::ECursorMovement)1)) {
                        if (--CursorHighlight < 1)
                            CursorHighlight = 12;
                        long highlight = CursorHighlight;
                        if (highlight < 12 && highlight > GetMaxVisibleProfileNumber())
                            CursorHighlight = GetMaxVisibleProfileNumber();
                    }
                }
            } else if (event.GetType() == (EInputEventType)0x15) {
                long button = event.GetJoystickButton();
                if (button == 1 || button == 2 || button == 4 || button == 5) {
                    long highlight = CursorHighlight;
                    if (highlight >= 1 && highlight <= GetMaxVisibleProfileNumber()) {
                        ChooseProfileNumber(highlight);
                        handled = true;
                    } else if (highlight == 0) {
                        ShowMoreProfiles();
                        handled = true;
                    } else if (highlight == 9) {
                        CreateEditor = false;
                        CreateGame = false;
                        SetQuit();
                        handled = true;
                    }
                }
            } else if (event.GetType() == (EInputEventType)3) {
                long number = g_KeyToNumber[event.GetKey()];
                if (number >= 1 && number <= 8) {
                    ChooseProfileNumber(number);
                    handled = true;
                } else if (event.GetKey() == (EInputKey)0xb || event.GetKey() == (EInputKey)0x52) {
                    ShowMoreProfiles();
                    handled = true;
                } else if (event.GetKey() == (EInputKey)0xa || event.GetKey() == (EInputKey)0x49) {
                    CreateEditor = false;
                    CreateGame = false;
                    SetQuit();
                    handled = true;
                } else if (event.GetKey() == (EInputKey)0x1e) {
                    ChooseProfileNumber(12);
                    handled = true;
                } else if (event.GetKey() == (EInputKey)0x30) {
                    ChooseProfileNumber(13);
                    handled = true;
                }
            }
            break;
        }
    } while (PInputManager->EventScanNext(scanInfo, event) == true);
    PInputManager->EventScanEnd(scanInfo);
}

static CInputEvent Stick(float y) { CInputEvent e; e.type = 0x11; e.key = 0; e.button = 0; e.device = 0; e.y = y; return e; }
static CInputEvent Button(long b, long device) { CInputEvent e; e.type = 0x15; e.key = 0; e.button = b; e.device = device; e.y = 0.0f; return e; }
static CInputEvent Key(long k) { CInputEvent e; e.type = 3; e.key = k; e.button = 0; e.device = 0; e.y = 0.0f; return e; }
static int fails = 0;
static void Check(bool ok, int line) { if (!ok) { std::printf("fail at %d\n", line); ++fails; } }
static CFrontendGameComponent c;
int main()
{
    // Mode 1: stick up from 3 skips 4/5 to 6, from 7 wraps to 1; stick down from 6 lands on 3, from 1 wraps to 7.
    c.Reset(1, 3); gManager.count = 1; gManager.events[0] = Stick(1.0f); c.Input(); Check(c.CursorHighlight == 6, __LINE__);
    c.Reset(1, 7); c.Input(); Check(c.CursorHighlight == 1, __LINE__);
    c.Reset(1, 6); gManager.events[0] = Stick(-1.0f); c.Input(); Check(c.CursorHighlight == 3, __LINE__);
    c.Reset(1, 1); c.Input(); Check(c.CursorHighlight == 7, __LINE__);
    c.Reset(1, 1); gManager.events[0] = Stick(0.2f); c.Input(); Check(c.CursorHighlight == 1, __LINE__);
    c.Reset(1, 3); c.allowMove = false; gManager.events[0] = Stick(1.0f); c.Input(); Check(c.CursorHighlight == 3, __LINE__);
    // Mode 1: a pad button on entry 2 starts a game on that pad and opens the world list; the next event is fetched but not handled.
    c.Reset(1, 2); gManager.count = 2; gManager.events[0] = Button(4, 3); gManager.events[1] = Stick(1.0f); gManager.scanned = 0;
    gManager.updates = gManager.ends = 0; c.Input();
    Check(c.CreateGame && c.CreateGameJoystickDeviceNumber == 3 && c.Mode == 10 && c.worldFetches == 1 && c.CursorHighlight == 1
          && c.String[0] == 0 && c.StringPos == 0 && gManager.scanned == 2 && gManager.updates == 1 && gManager.ends == 1, __LINE__);
    c.Reset(1, 3); gManager.count = 1; gManager.events[0] = Button(1, 2); c.Input(); Check(c.Mode == 11 && c.saveModeCalls == 1 && c.CreateGame, __LINE__);
    c.Reset(1, 7); gManager.events[0] = Button(2, 0); c.Input(); Check(c.CreateReleaseFrontEnd && c.quits == 1, __LINE__);
    c.Reset(1, 6); gManager.events[0] = Button(5, 0); c.Input(); Check(!c.CreateGame && c.quits == 1, __LINE__);
    c.Reset(1, 2); gManager.events[0] = Button(3, 0); c.Input(); Check(c.Mode == 1 && !c.CreateGame, __LINE__);
    // Mode 1 keys: '1' / numpad 1 start, '7' releases the front end.
    c.Reset(1, 1); gManager.events[0] = Key(0x4f); c.Input(); Check(c.CreateGame && c.quits == 1 && c.CreateGameJoystickDeviceNumber == -1, __LINE__);
    c.Reset(1, 1); gManager.events[0] = Key(8); c.Input(); Check(c.CreateReleaseFrontEnd && c.quits == 1 && !c.CreateGame, __LINE__);
    // Mode 10: stick up past 9 wraps to 0, past the visible count jumps to 9; number keys choose, 0 shows more, 9 backs out.
    c.Reset(10, 9); gManager.events[0] = Stick(1.0f); c.Input(); Check(c.CursorHighlight == 0, __LINE__);
    c.Reset(10, 3); c.Input(); Check(c.CursorHighlight == 9, __LINE__);
    c.Reset(10, 0); gManager.events[0] = Stick(-1.0f); c.Input(); Check(c.CursorHighlight == 9, __LINE__);
    c.Reset(10, 9); c.Input(); Check(c.CursorHighlight == 3, __LINE__);
    g_KeyToNumber[5] = 4; g_KeyToNumber[0x4c] = 0;
    c.Reset(10, 1); gManager.events[0] = Key(5); c.Input(); Check(c.chosenWorld == 4, __LINE__);
    c.Reset(10, 1); gManager.events[0] = Key(0x52); c.Input(); Check(c.moreWorlds == 1, __LINE__);
    c.Reset(10, 1); gManager.events[0] = Key(0x49); c.Input(); Check(c.quits == 1 && !c.CreateEditor && !c.CreateGame, __LINE__);
    c.Reset(10, 2); gManager.events[0] = Button(1, 0); c.Input(); Check(c.chosenWorld == 2, __LINE__);
    c.Reset(10, 0); c.Input(); Check(c.moreWorlds == 1, __LINE__);
    // Mode 11: a pad button on 9 backs out; on an entry past the visible count nothing happens and scanning continues.
    c.Reset(11, 9); gManager.events[0] = Button(2, 0); c.Input(); Check(c.quits == 1 && !c.CreateEditor, __LINE__);
    c.Reset(11, 5); gManager.count = 2; gManager.events[0] = Button(2, 0); gManager.events[1] = Key(0xb); gManager.scanned = 0;
    c.Input(); Check(c.quits == 0 && c.chosenSave == 0 && c.moreSaves == 1 && gManager.scanned == 2, __LINE__);
    // Mode 12: wraps between 1 and 12, the A / B keys choose profiles 12 and 13.
    gManager.count = 1;
    c.Reset(12, 12); gManager.events[0] = Stick(1.0f); c.Input(); Check(c.CursorHighlight == 1, __LINE__);
    c.Reset(12, 1); gManager.events[0] = Stick(-1.0f); c.Input(); Check(c.CursorHighlight == 12, __LINE__);
    c.Reset(12, 6); c.Input(); Check(c.CursorHighlight == 4, __LINE__);
    c.Reset(12, 1); gManager.events[0] = Key(0x30); c.Input(); Check(c.chosenProfile == 13, __LINE__);
    c.Reset(12, 1); gManager.events[0] = Key(0x1e); c.Input(); Check(c.chosenProfile == 12, __LINE__);
    // Other modes and no events: nothing happens, the scan still ends.
    c.Reset(5, 4); gManager.events[0] = Key(0x30); c.Input(); Check(c.chosenProfile == 0 && c.CursorHighlight == 4, __LINE__);
    gManager.count = 0; gManager.ends = 0; c.Reset(1, 1); c.Input(); Check(gManager.ends == 1, __LINE__);
    if (fails) { std::printf("004963a0_TEST FAIL\n"); return 1; }
    std::printf("004963a0_TEST PASS\n");
    return 0;
}