// Behaviour test for NUISystem::CTextSlider::GetValueFromProfile (0x00548790): the slider's selected, previously
// selected and start children all take the profile option its def action names (bools as 0/1, the 0x53 option from
// the active profile, the 0x139 byte as "non-zero"), the display-mode actions also set the child count, and action 0
// or an unknown action leaves the slider alone -- through a copy of the switch over a stand-in profile.
#include <cstdio>

struct Profile {
    bool UseSecondProfile, Profile1Option17, Profile2Option17;
    unsigned char Option78;
    bool flags[12];
    int longs[4];
    int maxResolution, maxAntialiasing, maxRefreshRate;
    bool IsCameraXYInverted() const { return flags[0]; }
    bool IsCameraYZInverted() const { return flags[1]; }
    bool IsVibrationEnabled() const { return flags[2]; }
    bool IsCameraResetting() const { return flags[3]; }
    bool GetOption48DB35() const { return flags[4]; }
    bool GetOption548650() const { return flags[5]; }
    bool IsGuildMasterEnabled() const { return flags[6]; }
    bool IsHUDTooltipEnabled() const { return flags[7]; }
    bool IsVerticalSyncEnabled() const { return flags[8]; }
    bool GetOption548690() const { return flags[9]; }
    bool IsCameraPCBowYZInverted() const { return flags[10]; }
    bool GetOption548740() const { return flags[11]; }
    int GetOption5486B0() const { return longs[0]; }
    int GetOption548670() const { return longs[1]; }
    int GetOption5486F0() const { return longs[2]; }
    int GetOption5486D0() const { return longs[3]; }
    int GetMaxResolution() const { return maxResolution; }
    int GetMaxAntialiasing() const { return maxAntialiasing; }
    int GetMaxRefreshRate() const { return maxRefreshRate; }
    bool GetOption17() const { return UseSecondProfile ? Profile2Option17 : Profile1Option17; }
};

static Profile g_profile;
static Profile* Get() { return &g_profile; }

struct Slider {
    unsigned int SelectedChild, PreviouslySelectedChild, StartSelectedChild;
    int PDefAction;
    unsigned int MaxChild;
    void GetValueFromProfile()
    {
        if (PDefAction == 0)
            return;
        switch (PDefAction) {
        case 0x2b: SelectedChild = PreviouslySelectedChild = StartSelectedChild = Get()->IsCameraXYInverted(); break;
        case 0x2c: SelectedChild = PreviouslySelectedChild = StartSelectedChild = Get()->IsCameraYZInverted(); break;
        case 0x2d: SelectedChild = PreviouslySelectedChild = StartSelectedChild = Get()->IsVibrationEnabled(); break;
        case 0x2e: SelectedChild = PreviouslySelectedChild = StartSelectedChild = Get()->IsCameraResetting(); break;
        case 0x2f: SelectedChild = PreviouslySelectedChild = StartSelectedChild = Get()->GetOption48DB35(); break;
        case 0x53: SelectedChild = PreviouslySelectedChild = StartSelectedChild = Get()->GetOption17(); break;
        case 0x5d: SelectedChild = PreviouslySelectedChild = StartSelectedChild = Get()->IsGuildMasterEnabled(); break;
        case 0x5e: SelectedChild = PreviouslySelectedChild = StartSelectedChild = Get()->IsHUDTooltipEnabled(); break;
        case 0x54: SelectedChild = PreviouslySelectedChild = StartSelectedChild = Get()->GetOption548650(); break;
        case 0x139: SelectedChild = PreviouslySelectedChild = StartSelectedChild = Get()->Option78 != 0; break;
        case 0x102:
            SelectedChild = PreviouslySelectedChild = StartSelectedChild = Get()->GetOption5486B0();
            MaxChild = Get()->GetMaxResolution();
            break;
        case 0x13d:
            SelectedChild = PreviouslySelectedChild = StartSelectedChild = Get()->GetOption5486D0();
            MaxChild = Get()->GetMaxRefreshRate();
            break;
        case 0x109:
            SelectedChild = PreviouslySelectedChild = StartSelectedChild = Get()->GetOption548670();
            MaxChild = Get()->GetMaxAntialiasing();
            break;
        case 0x12c: SelectedChild = PreviouslySelectedChild = StartSelectedChild = Get()->GetOption5486F0(); break;
        case 0x106: SelectedChild = PreviouslySelectedChild = StartSelectedChild = Get()->IsVerticalSyncEnabled(); break;
        case 0x140: SelectedChild = PreviouslySelectedChild = StartSelectedChild = Get()->GetOption548690(); break;
        case 0x13f: SelectedChild = PreviouslySelectedChild = StartSelectedChild = Get()->IsCameraPCBowYZInverted(); break;
        case 0x143: SelectedChild = PreviouslySelectedChild = StartSelectedChild = Get()->GetOption548740(); break;
        }
    }
};

static bool Check(int action, unsigned int expected, unsigned int expectedMax)
{
    Slider slider = { 9, 9, 9, action, 99 };
    slider.GetValueFromProfile();
    return slider.SelectedChild == expected && slider.PreviouslySelectedChild == expected &&
           slider.StartSelectedChild == expected && slider.MaxChild == expectedMax;
}

int main()
{
    for (int i = 0; i < 12; ++i)
        g_profile.flags[i] = (i % 2) == 0;
    g_profile.longs[0] = 3; g_profile.longs[1] = 2; g_profile.longs[2] = 5; g_profile.longs[3] = 1;
    g_profile.maxResolution = 7; g_profile.maxAntialiasing = 4; g_profile.maxRefreshRate = 6;
    g_profile.UseSecondProfile = true; g_profile.Profile1Option17 = false; g_profile.Profile2Option17 = true;
    g_profile.Option78 = 0x40;
    bool ok = Check(0, 9, 99) && Check(0x30, 9, 99) && Check(0x2b, 1, 99) && Check(0x2c, 0, 99) && Check(0x53, 1, 99) &&
              Check(0x139, 1, 99) && Check(0x102, 3, 7) && Check(0x109, 2, 4) && Check(0x13d, 1, 6) &&
              Check(0x12c, 5, 99) && Check(0x143, 0, 99) && Check(0x13f, 1, 99);
    g_profile.UseSecondProfile = false;
    g_profile.Option78 = 0;
    ok = ok && Check(0x53, 0, 99) && Check(0x139, 0, 99);
    if (!ok) {
        std::printf("00548790_TEST FAIL\n");
        return 1;
    }
    std::printf("00548790_TEST PASS\n");
    return 0;
}