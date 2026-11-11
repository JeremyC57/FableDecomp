// CFrontendGameComponent::Input() -- scan this frame's input events and drive the debug front end by mode: on the start menu (mode 1) the stick moves the highlight over entries 1..7 (skipping 4 and 5) and a pad button or the 1/2/3/6/7 keys start a new game, open the world list, open the save list, quit or switch to the release front end; on the world (10), save (11) and profile (12) lists the stick moves the highlight, a pad button or a number key picks an entry, 0 shows more, 9 backs out; stop at the first handled event
// Leaf: ?Input@CFrontendGameComponent@@AAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x004963A0, 2384 bytes incl. the 64-byte switch tables after the `ret` (0x00496CB4: 5 case
//   pointers + the 12-byte mode index at 0x00496CC8; 0x00496CD4: the 7 highlight cases), aligned frame (`and esp,-8`, a double
//   is live in the TU's inline ChangeSelection callers).
// Identity (two sources): Ego_r.pdb / FableWin.pdb ?Input@CFrontendGameComponent@@AAEXXZ (private, void()), and the landed
//   CFrontendGameComponent::Run 0x00496CF0 calls 0x004963A0 then Output 0x00494900 once per updated frame.
// Callees (Ego_r.pdb / FableWin.pdb spellings): GPInputManager 0x013B8388 (?GPInputManager@@3PAVCInputManager@@A) Update (slot 2),
//   EventScanStart 0x009F4ED0 / EventScanNext 0x009F4F10 / EventScanEnd 0x009F4AC0; CInputEvent() 0x00A04410 / ~CInputEvent
//   0x00A03B30 / GetType 0x00A03B40 / GetDeviceNumber 0x00A03B60 / GetKey 0x00A03B70 / GetJoystickButton 0x00A03B90 /
//   GetJoystickPosition 0x00A043A0; the CFrontendGameComponent privates ChangeSelection 0x00494380, GetMaxVisibleProfileNumber
//   0x00494520, ShowMoreProfiles 0x00494560, GetMaxVisibleWorldNumber 0x00494680, ShowMoreWorlds 0x004946C0, ChooseWorldNumber
//   0x00494710, GetMaxVisibleSaveNumber 0x00494770, ShowMoreSaves 0x004947B0, ChooseSaveNumber 0x00494800, GetWorldNames 0x00495890,
//   SetMode 0x00495FE0 (called for mode 11, expanded for mode 10), ChooseProfileNumber 0x004962C0; SetQuit = vtable slot 4
//   (0x00494370); GFWrapLimit<long,int,int> 0x00496EB0 (??$GFWrapLimit@JHH@@YIXAAJABH1@Z); g_KeyToNumber 0x013B87B0 (established
//   spelling, no PDB data names). The 0.5f / -0.5f stick thresholds are this TU's literals (0x012384D8 / 0x01238798).
// Layout: Ego_r.pdb CFrontendGameComponent : CGameComponent with the retail offsets (each STLport vector 4 bytes smaller than
//   the Dinkumware one: ProfileNames +0x1E0C, SaveNames +0x1E1C, SaveToLoad +0x1E2C, WorldNames +0x1E30, WorldToLoad +0x1E40,
//   CursorHighlight +0x1E44, as the landed GetMaxVisibleWorldNumber / SetMode / Run read them). The enums (EGameMode,
//   ECursorMovement, EInputEventType, EInputKey) have no enumerator names in the repo's PDB extracts: written as numbers.
// Shape: `if (EventScanStart(...) == true) do { if (handled == true) break; switch (Mode) {...} } while (EventScanNext(...) == true);`
//   gives retail's loop head (`cmp bl,1 ; mov edi,7 ; je end`, the 7 for both wrap calls kept in edi); a for/while condition aligns
//   the loop top and jumps over the handled test. The same-TU bodies retail compiled before Input decide three shapes:
//   GetMaxVisible{Profile,World,Save}Number (kept out of line; VC7.1 then knows ecx survives the call, so no `mov ecx,esi` before the
//   following ChooseXNumber / second GetMaxVisible call), SetMode over ClearString (expanded at the two mode-10 sites only), and
//   GFWrapLimit<long,int,int> as a `__declspec(noinline)` body (retail's out-of-line instantiation in this TU, 0x00496EB0): with it
//   VC7.1 sees the 1 / 7 temporaries as uncaptured and packs both calls' pairs into the one 8-byte slot pair; declaration-only gives
//   frame 0xA0, a visible plain body expands the first call. `handled = true` precedes SetQuit() on the key-1 path (bl set before
//   the call) and every case writes its own `handled = true` (cross-jumped by VC7.1 into the shared tails at 0x00496A48 / 0x00496BA1).
#define FABLE_RETAIL_C2DVECTOR
#define FABLE_RETAIL_CINPUTEVENT_ACCESSORS
#define FABLE_RETAIL_CINPUTMANAGER_EVENTSCANEND
#include <string.h>
#include "fable_string.h"
#include "stlport/fable_containers.h"
#include "engine/C2DVector.h"
#include "engine/CGameComponent.h"
#include "engine/CInputEvent.h"
#include "engine/CInputEventScanInfo.h"
#include "engine/CInputManager.h"

namespace NGameComponentHelpers { enum ECursorMovement { }; }
enum EGameMode { };
extern CInputManager* GPInputManager;                   // 0x013B8388 ?GPInputManager@@3PAVCInputManager@@A
extern long g_KeyToNumber[];                                             // 0x013B87B0
// GFWrapLimit<long,int,int> 0x00496EB0: the header template, instantiated out of line in this TU (its body follows Run).
template <class T, class TMin, class TMax> __declspec(noinline) void __fastcall GFWrapLimit(T& value, const TMin& minimum, const TMax& maximum)
{
    if (value < minimum)
        value = maximum;
    else if (value > maximum)
        value = minimum;
}

class CFrontendGameComponent : public CGameComponent {
public:
    virtual ~CFrontendGameComponent();
    virtual bool Run(CGameComponent** nextComponent);                    // slot 2 0x00496CF0
    virtual void ChangeTextureColourDepth(long depth);                   // slot 3 0x00495610
    virtual void SetQuit();                                              // slot 4 0x00494370
private:
    bool ChangeSelection(NGameComponentHelpers::ECursorMovement movement);   // 0x00494380
    long GetMaxVisibleProfileNumber() const;                             // 0x00494520
    void ShowMoreProfiles();                                             // 0x00494560
    long GetMaxVisibleWorldNumber() const;                               // 0x00494680
    void ShowMoreWorlds();                                               // 0x004946C0
    void ChooseWorldNumber(long number);                                 // 0x00494710
    long GetMaxVisibleSaveNumber() const;                                // 0x00494770
    void ShowMoreSaves();                                                // 0x004947B0
    void ChooseSaveNumber(long number);                                  // 0x00494800
    static void __fastcall GetSaveNames(std::vector<CWideString>& names);    // 0x00495830
    void GetWorldNames();                                                // 0x00495890
    static void __fastcall GetProfileNames(std::vector<CWideString>& names); // 0x004959E0
    void SetMode(EGameMode mode);                                        // 0x00495FE0
    void ChooseProfileNumber(long number);                               // 0x004962C0
    void ClearString();                                                  // 0x004944C0
    void Input();

    bool          CreateGame;                                            // +0x10
    long          CreateGameJoystickDeviceNumber;                        // +0x14
    bool          CreateEditor;                                          // +0x18
    bool          CreateReleaseFrontEnd;                                 // +0x19
    EGameMode     Mode;                                                  // +0x1c
    long          NoConnections;                                         // +0x20
    long          NoSessions;                                            // +0x24
    unsigned char Connections[64];                                       // +0x28 CNetworkConnection[64]
    unsigned char Sessions[256];                                         // +0x68 CNetworkSession[256]
    char          String[128];                                           // +0x168
    long          StringPos;                                             // +0x1e8
    void*         CurrentConnection;                                     // +0x1ec CNetworkConnection*
    unsigned char TempBuffer[2048];                                      // +0x1f0
    char          SessionString[20][256];                                // +0x9f0
    long          FirstSessionString;                                    // +0x1df0
    long          LastSessionString;                                     // +0x1df4
    long          NoSessionStrings;                                      // +0x1df8
    unsigned char PTempGraphics[8];                                      // +0x1dfc CCountedPointer<CGraphicDataBank>
    unsigned char MainFont[8];                                           // +0x1e04 CCountedPointer<const CFontBank>
    std::vector<CWideString> ProfileNames;                               // +0x1e0c
    long          ProfileNamesOffset;                                    // +0x1e18
    std::vector<CWideString> SaveNames;                                  // +0x1e1c
    long          SaveNamesOffset;                                       // +0x1e28
    CWideString   SaveToLoad;                                            // +0x1e2c
    std::vector<CWideString> WorldNames;                                 // +0x1e30
    long          WorldNamesOffset;                                      // +0x1e3c
    CWideString   WorldToLoad;                                           // +0x1e40
    long          CursorHighlight;                                       // +0x1e44
};

// 0x004944C0 (expanded at every use in retail; no caller calls it).
void CFrontendGameComponent::ClearString()
{
    memset(String, 0, sizeof(String));
    StringPos = 0;
}

// The same retail TU (0x00494520 / 0x00494680 / 0x00494770): bodies visible before Input.
long CFrontendGameComponent::GetMaxVisibleProfileNumber() const
{
    if (ProfileNames.size() - ProfileNamesOffset <= 8)
        return ProfileNames.size() - ProfileNamesOffset;
    return 8;
}
long CFrontendGameComponent::GetMaxVisibleWorldNumber() const
{
    if (WorldNames.size() - WorldNamesOffset <= 8)
        return WorldNames.size() - WorldNamesOffset;
    return 8;
}
long CFrontendGameComponent::GetMaxVisibleSaveNumber() const
{
    if (SaveNames.size() - SaveNamesOffset <= 8)
        return SaveNames.size() - SaveNamesOffset;
    return 8;
}

// 0x00495FE0, the same TU as Input: VC7.1 expands it at the two mode-10 sites (the mode 11 / 12 arms fold away) and calls it for
// mode 11.
void CFrontendGameComponent::SetMode(EGameMode mode)
{
    Mode = mode;
    CursorHighlight = 1;
    ClearString();
    if (Mode == 10) {
        GetWorldNames();
        return;
    }
    if (mode == 11) {
        GetSaveNames(SaveNames);
        WorldToLoad = L"";
        SaveNamesOffset = 0;
        return;
    }
    if (mode == 12) {
        GetProfileNames(ProfileNames);
        ProfileNamesOffset = 0;
    }
}

void CFrontendGameComponent::Input()
{
    CInputEventScanInfo scanInfo;
    CInputEvent event;
    CInputEvent unusedEvent;
    GPInputManager->Update();
    bool handled = false;
    if (GPInputManager->EventScanStart(scanInfo, event) == true) do {
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
    } while (GPInputManager->EventScanNext(scanInfo, event) == true);
    GPInputManager->EventScanEnd(scanInfo);
}