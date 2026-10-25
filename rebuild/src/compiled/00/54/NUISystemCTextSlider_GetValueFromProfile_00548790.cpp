// NUISystem::CTextSlider::GetValueFromProfile() -- sets the slider's selected / previously selected / start child from the user-profile option its def action names (camera inversion, vibration, tooltips, resolution, refresh rate, antialiasing, ...), and for the display modes also the child count
// Leaf: ?GetValueFromProfile@CTextSlider@NUISystem@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00548790, 538 bytes of code (0x21A incl. `mov edi,edi` alignment) then the switch
//   tables (two dword jump tables and a byte index table, 852 bytes in all), thiscall `ret`. Virtual: RTTI vtable of
//   NUISystem::CTextSlider 0x01248A8C slot 143.
// Identity: Ego_r.pdb ?GetValueFromProfile@CTextSlider@NUISystem@@MAEXXZ (protected virtual, no arguments); the
//   catalog's "CEditControlCentre_DispatchPropertySetByEnum" is descriptive.
// Callees: CTBaseSingleton<CUserProfileManager>::Get 0x0040D2A0 (fetched again for every access) and the profile
//   accessors (catalog names; each reads the active profile at +0xc ? profile 2 : profile 1): 0x00548590 / 0x005485B0 /
//   0x00548570 / 0x00548630 / 0x0048DB35 / 0x005485F0 / 0x00548610 / 0x00548650 / 0x00548710 / 0x005485D0 / 0x00548690 /
//   0x00548740 (bool), 0x005486B0 / 0x00548670 / 0x005486F0 / 0x005486D0 (long), GetMaxResolution 0x00408900,
//   GetMaxAntialiasing 0x00409090, GetMaxRefreshRate 0x00406890.
// Layout: retail CTextSlider (Ego_r.pdb - 0x30 to the action vector, - 0x34 after it): SelectedChild +0x15c,
//   PreviouslySelectedChild +0x160, StartSelectedChild +0x164, PDefAction +0x188, MaxChild +0x19c. CUserProfileManager:
//   the active-profile flag +0xc, profile-1 / profile-2 bytes +0x17 / +0xd7 (action 0x53, read inline), the byte +0x78
//   (action 0x139).
// Shape: speed-optimised TU (base /O2; the neighbouring profile accessors are 16-byte aligned with int3 padding).
//   No profile action (0) returns before the switch (a `test eax,eax; je` that a `case 0:` would fold into the
//   default); the case bodies sit in the source order recovered from the retail block order (0x2B..0x2F, 0x53, 0x5D,
//   0x5E, 0x54, 0x139, 0x102, 0x13D, 0x109, 0x12C, 0x106, 0x140, 0x13F, 0x143); the bool options share one `movzx` +
//   three-store tail, the long options store their value and return (the display modes also set MaxChild). VC7.1 lowers
//   it to a 0x2B..0x2F table, the 0x53 compare and a byte-indexed table for 0x54..0x143.

class CUserProfileManager {
public:
    unsigned char _pad_0x00[0x0c];
    bool          UseSecondProfile;                       // +0x0c
    unsigned char _pad_0x0d[0x17 - 0x0d];
    bool          Profile1Option17;                       // +0x17
    unsigned char _pad_0x18[0x78 - 0x18];
    unsigned char Option78;                               // +0x78
    unsigned char _pad_0x79[0xd7 - 0x79];
    bool          Profile2Option17;                       // +0xd7
    bool IsVibrationEnabled() const;                      // 0x00548570
    bool IsCameraXYInverted() const;                      // 0x00548590
    bool IsCameraYZInverted() const;                      // 0x005485B0
    bool IsCameraPCBowYZInverted() const;                 // 0x005485D0
    bool IsGuildMasterEnabled() const;                    // 0x005485F0
    bool IsHUDTooltipEnabled() const;                     // 0x00548610
    bool IsCameraResetting() const;                       // 0x00548630
    bool GetOption548650() const;                         // 0x00548650
    long GetOption548670() const;                         // 0x00548670
    bool GetOption548690() const;                         // 0x00548690
    long GetOption5486B0() const;                         // 0x005486B0
    long GetOption5486D0() const;                         // 0x005486D0
    long GetOption5486F0() const;                         // 0x005486F0
    bool IsVerticalSyncEnabled() const;                   // 0x00548710
    bool GetOption548740() const;                         // 0x00548740
    bool GetOption48DB35() const;                         // 0x0048DB35
    long GetMaxResolution() const;                        // 0x00408900
    long GetMaxAntialiasing() const;                      // 0x00409090
    long GetMaxRefreshRate() const;                       // 0x00406890
    bool GetOption17() const { return UseSecondProfile ? Profile2Option17 : Profile1Option17; }
};

template <class T> class CTBaseSingleton {
public:
    static T* Get();                                      // 0x0040D2A0 (CUserProfileManager)
};

namespace NUISystem {
class CTextSlider {
public:
    unsigned char _pad_0x00[0x15c];
    unsigned long SelectedChild;                          // +0x15c
    unsigned long PreviouslySelectedChild;                // +0x160
    unsigned long StartSelectedChild;                     // +0x164
    unsigned char _pad_0x168[0x188 - 0x168];
    long          PDefAction;                             // +0x188 (EActionType)
    unsigned char _pad_0x18c[0x19c - 0x18c];
    unsigned long MaxChild;                               // +0x19c
    void GetValueFromProfile();
};

void CTextSlider::GetValueFromProfile()
{
    if (PDefAction == 0)
        return;
    switch (PDefAction) {
    case 0x2b:
        SelectedChild = PreviouslySelectedChild = StartSelectedChild =
            CTBaseSingleton<CUserProfileManager>::Get()->IsCameraXYInverted();
        break;
    case 0x2c:
        SelectedChild = PreviouslySelectedChild = StartSelectedChild =
            CTBaseSingleton<CUserProfileManager>::Get()->IsCameraYZInverted();
        break;
    case 0x2d:
        SelectedChild = PreviouslySelectedChild = StartSelectedChild =
            CTBaseSingleton<CUserProfileManager>::Get()->IsVibrationEnabled();
        break;
    case 0x2e:
        SelectedChild = PreviouslySelectedChild = StartSelectedChild =
            CTBaseSingleton<CUserProfileManager>::Get()->IsCameraResetting();
        break;
    case 0x2f:
        SelectedChild = PreviouslySelectedChild = StartSelectedChild =
            CTBaseSingleton<CUserProfileManager>::Get()->GetOption48DB35();
        break;
    case 0x53:
        SelectedChild = PreviouslySelectedChild = StartSelectedChild =
            CTBaseSingleton<CUserProfileManager>::Get()->GetOption17();
        break;
    case 0x5d:
        SelectedChild = PreviouslySelectedChild = StartSelectedChild =
            CTBaseSingleton<CUserProfileManager>::Get()->IsGuildMasterEnabled();
        break;
    case 0x5e:
        SelectedChild = PreviouslySelectedChild = StartSelectedChild =
            CTBaseSingleton<CUserProfileManager>::Get()->IsHUDTooltipEnabled();
        break;
    case 0x54:
        SelectedChild = PreviouslySelectedChild = StartSelectedChild =
            CTBaseSingleton<CUserProfileManager>::Get()->GetOption548650();
        break;
    case 0x139:
        SelectedChild = PreviouslySelectedChild = StartSelectedChild =
            CTBaseSingleton<CUserProfileManager>::Get()->Option78 != 0;
        break;
    case 0x102:
        SelectedChild = PreviouslySelectedChild = StartSelectedChild =
            CTBaseSingleton<CUserProfileManager>::Get()->GetOption5486B0();
        MaxChild = CTBaseSingleton<CUserProfileManager>::Get()->GetMaxResolution();
        break;
    case 0x13d:
        SelectedChild = PreviouslySelectedChild = StartSelectedChild =
            CTBaseSingleton<CUserProfileManager>::Get()->GetOption5486D0();
        MaxChild = CTBaseSingleton<CUserProfileManager>::Get()->GetMaxRefreshRate();
        break;
    case 0x109:
        SelectedChild = PreviouslySelectedChild = StartSelectedChild =
            CTBaseSingleton<CUserProfileManager>::Get()->GetOption548670();
        MaxChild = CTBaseSingleton<CUserProfileManager>::Get()->GetMaxAntialiasing();
        break;
    case 0x12c:
        SelectedChild = PreviouslySelectedChild = StartSelectedChild =
            CTBaseSingleton<CUserProfileManager>::Get()->GetOption5486F0();
        break;
    case 0x106:
        SelectedChild = PreviouslySelectedChild = StartSelectedChild =
            CTBaseSingleton<CUserProfileManager>::Get()->IsVerticalSyncEnabled();
        break;
    case 0x140:
        SelectedChild = PreviouslySelectedChild = StartSelectedChild =
            CTBaseSingleton<CUserProfileManager>::Get()->GetOption548690();
        break;
    case 0x13f:
        SelectedChild = PreviouslySelectedChild = StartSelectedChild =
            CTBaseSingleton<CUserProfileManager>::Get()->IsCameraPCBowYZInverted();
        break;
    case 0x143:
        SelectedChild = PreviouslySelectedChild = StartSelectedChild =
            CTBaseSingleton<CUserProfileManager>::Get()->GetOption548740();
        break;
    }
}
}