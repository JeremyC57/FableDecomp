// Behaviour test for CAudioMixer::SetVolume (0x00C71640): min + (DWORD)((max - min) * level) written as a 1-channel unsigned detail.
#include <cstdio>

typedef unsigned int DWORD_;
struct MIXERCONTROLA_ {
    DWORD_ cbStruct, dwControlID, dwControlType, fdwControl, cMultipleItems;
    char szShortName[16];
    char szName[64];
    DWORD_ Bounds[6];
    DWORD_ Metrics[6];
};
struct MIXERCONTROLDETAILS_ {
    DWORD_ cbStruct, dwControlID, cChannels, cMultipleItems, cbDetails;
    void* paDetails;
};

static void* g_mixer = 0;
static DWORD_ g_control = 0, g_value = 0, g_flags = 0, g_size = 0, g_channels = 0, g_cb = 0;
static unsigned int mixerSetControlDetails(void* mixer, MIXERCONTROLDETAILS_* details, DWORD_ flags)
{
    g_mixer = mixer; g_flags = flags; g_size = details->cbStruct; g_control = details->dwControlID;
    g_channels = details->cChannels; g_cb = details->cbDetails; g_value = *(DWORD_*)details->paDetails;
    return 0;
}

struct CAudioMixer {
    void*          Mixer;
    MIXERCONTROLA_ VolumeControl;
    DWORD_         VolumeValue;
    void SetVolume(float level)
    {
        DWORD_ minimum = VolumeControl.Bounds[0];
        DWORD_ value = minimum + (DWORD_)((float)(VolumeControl.Bounds[1] - minimum) * level);
        MIXERCONTROLDETAILS_ details;
        details.cbStruct = sizeof(details);
        details.dwControlID = VolumeControl.dwControlID;
        details.cChannels = 1;
        details.cMultipleItems = 0;
        details.cbDetails = sizeof(DWORD_);
        details.paDetails = &value;
        mixerSetControlDetails(Mixer, &details, 0x80000000);
    }
};

int main()
{
    static CAudioMixer m;
    m.Mixer = &m;
    m.VolumeControl.dwControlID = 77;
    m.VolumeControl.Bounds[0] = 1000;
    m.VolumeControl.Bounds[1] = 65535;
    m.SetVolume(1.0f);
    if (g_mixer != &m || g_flags != 0x80000000u || g_size != sizeof(MIXERCONTROLDETAILS_) || g_control != 77 || g_channels != 1 ||
        g_cb != 4 || g_value != 65535) { std::printf("00c71640_TEST FAIL\n"); return 1; }
    m.SetVolume(0.5f);
    if (g_value < 33266 || g_value > 33268) { std::printf("00c71640_TEST FAIL\n"); return 1; }
    m.SetVolume(0.0f);
    if (g_value != 1000) { std::printf("00c71640_TEST FAIL\n"); return 1; }
    std::printf("00c71640_TEST PASS\n");
    return 0;
}