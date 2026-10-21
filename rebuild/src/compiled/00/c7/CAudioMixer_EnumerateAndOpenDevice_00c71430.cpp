// CAudioMixer::EnumerateAndOpenDevice(const char* deviceName) -- find the mixer device whose caps name matches, open it, and bind the wave-out source line's volume control and its single-channel unsigned detail; any failing step returns false
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C71430, 402 bytes (416 with padding), `ret 4`. Built with /GS.
// Flags: /GS
// Callees (WINMM imports): mixerGetNumDevs 0x014404C0, mixerGetDevCapsA 0x01440480, mixerOpen 0x0144047C,
//   mixerGetLineInfoA 0x014404D0 (COMPONENTTYPE | OBJECTF_HMIXER), mixerGetLineControlsA 0x014404CC (ONEBYTYPE |
//   OBJECTF_HMIXER), mixerGetControlDetailsA 0x014404BC (OBJECTF_HMIXER); the /GS cookie 0x0139C8A8 and its check
//   0x00BFE9F9. The name compare is the strcmp intrinsic.
// Layout: no PDB class; the mixer handle +0, the volume MIXERCONTROL +4 (0x94 bytes, dwControlID +8) and the
//   MIXERCONTROLDETAILS_UNSIGNED value +0x98 follow from the stores.
#include <string.h>

typedef unsigned long DWORD_;
struct MIXERCAPSA_ {                             // 0x30
    unsigned short wMid, wPid;
    DWORD_ vDriverVersion;
    char szPname[32];
    DWORD_ fdwSupport;
    DWORD_ cDestinations;
};
struct MIXERLINEA_ {                             // 0xa8
    DWORD_ cbStruct, dwDestination, dwSource, dwLineID, fdwLine, dwUser, dwComponentType, cChannels, cConnections,
           cControls;
    char szShortName[16];
    char szName[64];
    struct { DWORD_ dwType, dwDeviceID; unsigned short wMid, wPid; DWORD_ vDriverVersion; char szPname[32]; } Target;
};
struct MIXERCONTROLA_ {                          // 0x94
    DWORD_ cbStruct, dwControlID, dwControlType, fdwControl, cMultipleItems;
    char szShortName[16];
    char szName[64];
    DWORD_ Bounds[6];
    DWORD_ Metrics[6];
};
struct MIXERLINECONTROLSA_ {                     // 0x18
    DWORD_ cbStruct, dwLineID, dwControlType, cControls, cbmxctrl;
    MIXERCONTROLA_* pamxctrl;
};
struct MIXERCONTROLDETAILS_ {                    // 0x18
    DWORD_ cbStruct, dwControlID, cChannels, cMultipleItems, cbDetails;
    void* paDetails;
};
extern "C" {
__declspec(dllimport) unsigned int __stdcall mixerGetNumDevs();
__declspec(dllimport) unsigned int __stdcall mixerGetDevCapsA(unsigned int id, MIXERCAPSA_* caps, unsigned int size);
__declspec(dllimport) unsigned int __stdcall mixerOpen(void** mixer, unsigned int id, DWORD_ callback, DWORD_ instance, DWORD_ flags);
__declspec(dllimport) unsigned int __stdcall mixerGetLineInfoA(void* mixer, MIXERLINEA_* line, DWORD_ flags);
__declspec(dllimport) unsigned int __stdcall mixerGetLineControlsA(void* mixer, MIXERLINECONTROLSA_* controls, DWORD_ flags);
__declspec(dllimport) unsigned int __stdcall mixerGetControlDetailsA(void* mixer, MIXERCONTROLDETAILS_* details, DWORD_ flags);
}

struct CAudioMixer {
    void*          Mixer;                        // +0x00 HMIXER
    MIXERCONTROLA_ VolumeControl;                // +0x04
    DWORD_         VolumeValue;                  // +0x98 MIXERCONTROLDETAILS_UNSIGNED
    bool EnumerateAndOpenDevice(const char* deviceName);
};

bool CAudioMixer::EnumerateAndOpenDevice(const char* deviceName)
{
    MIXERCAPSA_ caps;
    caps.szPname[0] = 0;
    unsigned int count = mixerGetNumDevs();
    unsigned int id;
    for (id = 0; id < count; ++id) {
        if (mixerGetDevCapsA(id, &caps, sizeof(caps)) != 0)
            return false;
        if (strcmp(caps.szPname, deviceName) == 0)
            break;
    }
    if (id == count)
        return false;
    if (mixerOpen(&Mixer, id, 0, 0, 0) != 0)
        return false;
    MIXERLINEA_ line;
    line.cbStruct = sizeof(line);
    line.dwComponentType = 0x1008;               // MIXERLINE_COMPONENTTYPE_SRC_WAVEOUT
    if (mixerGetLineInfoA(Mixer, &line, 0x80000003) != 0)
        return false;
    MIXERLINECONTROLSA_ controls;
    controls.cbStruct = sizeof(controls);
    controls.dwLineID = line.dwLineID;
    controls.dwControlType = 0x50030001;         // MIXERCONTROL_CONTROLTYPE_VOLUME
    controls.cControls = 1;
    controls.cbmxctrl = sizeof(MIXERCONTROLA_);
    controls.pamxctrl = &VolumeControl;
    if (mixerGetLineControlsA(Mixer, &controls, 0x80000002) == 0) {
        MIXERCONTROLDETAILS_ details;
        details.cbStruct = sizeof(details);
        details.dwControlID = VolumeControl.dwControlID;
        details.cChannels = 1;
        details.cMultipleItems = 0;
        details.cbDetails = sizeof(DWORD_);
        details.paDetails = &VolumeValue;
        bool ok = mixerGetControlDetailsA(Mixer, &details, 0x80000000) == 0;
        return ok;
    }
    return false;
}