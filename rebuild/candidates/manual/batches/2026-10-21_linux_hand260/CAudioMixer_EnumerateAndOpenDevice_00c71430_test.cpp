#include <cstdio>
#include <cstring>
// Behaviour model of CAudioMixer::EnumerateAndOpenDevice: match the device name, open it, then fetch the wave-out
// line, its volume control and the control details; any API failure returns false.
typedef unsigned long DW;
struct Caps { unsigned short wMid, wPid; DW ver; char szPname[32]; DW sup, dest; };
struct Line { DW cbStruct, dwDestination, dwSource, dwLineID, fdwLine, dwUser, dwComponentType, cChannels, cConnections, cControls; char s[16]; char n[64]; DW t[12]; };
struct Control { DW cbStruct, dwControlID, rest[35]; };
struct LineControls { DW cbStruct, dwLineID, dwControlType, cControls, cbmxctrl; Control* pamxctrl; };
struct Details { DW cbStruct, dwControlID, cChannels, cMultipleItems, cbDetails; void* paDetails; };
static const char* g_names[3] = { "Speakers", "SB Live", "Line" };
static int g_failStep = -1;
static unsigned int g_openedId = 99; static DW g_lineQueried = 0, g_ctlType = 0, g_detailCtl = 0;
static unsigned int mixerGetNumDevs() { return 3; }
static unsigned int mixerGetDevCapsA(unsigned int id, Caps* c, unsigned int size)
{ if (g_failStep == 0 || size != sizeof(Caps)) return 1; std::strcpy(c->szPname, g_names[id]); return 0; }
static unsigned int mixerOpen(void** m, unsigned int id, DW, DW, DW) { if (g_failStep == 1) return 1; *m = (void*)0x1234; g_openedId = id; return 0; }
static unsigned int mixerGetLineInfoA(void*, Line* l, DW f) { if (g_failStep == 2 || f != 0x80000003 || l->dwComponentType != 0x1008) return 1; l->dwLineID = 77; return 0; }
static unsigned int mixerGetLineControlsA(void*, LineControls* c, DW f)
{ if (g_failStep == 3 || f != 0x80000002) return 1; g_lineQueried = c->dwLineID; g_ctlType = c->dwControlType; c->pamxctrl->dwControlID = 555; return 0; }
static unsigned int mixerGetControlDetailsA(void*, Details* d, DW f)
{ if (g_failStep == 4 || f != 0x80000000) return 1; g_detailCtl = d->dwControlID; *(DW*)d->paDetails = 0xffff; return 0; }
struct CAudioMixer {
    void* Mixer; Control VolumeControl; DW VolumeValue;
    bool EnumerateAndOpenDevice(const char* deviceName)
    {
        Caps caps; caps.szPname[0] = 0;
        unsigned int count = mixerGetNumDevs(); unsigned int id;
        for (id = 0; id < count; ++id) {
            if (mixerGetDevCapsA(id, &caps, sizeof(caps)) != 0) return false;
            if (std::strcmp(caps.szPname, deviceName) == 0) break;
        }
        if (id == count) return false;
        if (mixerOpen(&Mixer, id, 0, 0, 0) != 0) return false;
        Line line; line.cbStruct = sizeof(line); line.dwComponentType = 0x1008;
        if (mixerGetLineInfoA(Mixer, &line, 0x80000003) != 0) return false;
        LineControls controls; controls.cbStruct = sizeof(controls); controls.dwLineID = line.dwLineID;
        controls.dwControlType = 0x50030001; controls.cControls = 1; controls.cbmxctrl = sizeof(Control); controls.pamxctrl = &VolumeControl;
        if (mixerGetLineControlsA(Mixer, &controls, 0x80000002) == 0) {
            Details details; details.cbStruct = sizeof(details); details.dwControlID = VolumeControl.dwControlID;
            details.cChannels = 1; details.cMultipleItems = 0; details.cbDetails = sizeof(DW); details.paDetails = &VolumeValue;
            bool ok = mixerGetControlDetailsA(Mixer, &details, 0x80000000) == 0;
            return ok;
        }
        return false;
    }
};
int main()
{
    CAudioMixer m;
    if (!m.EnumerateAndOpenDevice("SB Live") || g_openedId != 1 || g_lineQueried != 77 || g_ctlType != 0x50030001
        || g_detailCtl != 555 || m.VolumeValue != 0xffff) { std::printf("00c71430_TEST FAIL\n"); return 1; }
    if (m.EnumerateAndOpenDevice("Missing")) { std::printf("00c71430_TEST FAIL\n"); return 1; }
    for (int step = 0; step < 5; ++step) {
        g_failStep = step;
        if (m.EnumerateAndOpenDevice("Line")) { std::printf("00c71430_TEST FAIL\n"); return 1; }
    }
    std::printf("00c71430_TEST PASS\n");
    return 0;
}
