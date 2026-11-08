// CDSoundAccelerationQuery::GetAccelerationLevel(const _GUID* deviceGuid) -- read the DirectSound hardware-acceleration setting of a render device from its driver's registry key: unknown (3) on Windows 9x, when the device's interface path cannot be found or the key cannot be opened or read; 2 for "standard" acceleration (8); for "full" acceleration (15) 1 when the driver reports a present VxD or WDM device, else 0; 0 for any other setting
// Leaf: ?GetAccelerationLevel@CDSoundAccelerationQuery@@QAEJPBU_GUID@@@Z
// Flags: /EHsc
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C5DD70, 604 bytes (the oracle row is cut at 728: see NOTES.md), thiscall `ret 4`, 8-byte aligned frame, EH-framed (the three
//   CRegistryKey locals, states 0..2).
// Identity (descriptive; libpk / GameCODA platform code, no PDB / RTTI): the caller of the landed CDSoundAccelerationQuery rows
//   FindDeviceInterface 0x00C5DB40 and OpenDeviceRegistryKey 0x00C5DC60 (their notes name this "the DirectSound acceleration
//   probe"); the strings "DirectSound\Mixer Defaults" / "Acceleration" (0x012B90B8 / 0x012B90A8) and "DirectSound\Device
//   Presence" / "VxD" / "WDM" (0x012B908C / 0x012B9088 / 0x012B9084). Its caller is the audio device set-up 0x00C3FB5D. The
//   catalog's "Audio_InitializeDriver" is a propagated description.
// Callees: the version probe guard EnsureWindowsVersionDetected_00c73770 0x00C73770 (landed spelling; run on a local), IsWindows9x
//   0x00C5DA70 (expanded at the first test, called at the second), the Windows 95 flag 0x0143CCC2 and the other 9x flags,
//   CRegistryKey 0x00C73790 / ~CRegistryKey 0x00C739A0 / Create 0x00C737D0 / Attach 0x00C73820 / QueryDWORD 0x00C73830.
#include <stddef.h>
struct _GUID;
typedef struct HKEY__* HKEY;

extern bool g_isWindows95;                              // 0x0143CCC2
extern bool g_isWindows98;                              // 0x0143CCC3
extern bool g_isWindows98SE;                            // 0x0143CCC4
extern bool g_isWindowsMe;                              // 0x0143CCC5
void* __fastcall EnsureWindowsVersionDetected_00c73770(void* self);   // 0x00C73770

bool IsWindows9x()                                      // 0x00C5DA70 (landed)
{
    return g_isWindows95 || g_isWindows98 || g_isWindows98SE || g_isWindowsMe;
}

class CRegistryKey {                                    // vtable 0x012BEFB0
public:
    CRegistryKey();                                     // 0x00C73790
    virtual ~CRegistryKey();                            // 0x00C739A0
    int Create(HKEY parent, const char* subKey, unsigned long access);   // 0x00C737D0
    void Attach(HKEY key);                              // 0x00C73820
    int QueryDWORD(const char* name, unsigned long* value);   // 0x00C73830
    HKEY m_hKey;                                        // +0x04
};

class CDSoundAccelerationQuery {
public:
    char m_interfacePath[0x104];                        // +0x000 the device's driver interface path, empty when unknown
    void FindDeviceInterface(const _GUID* deviceGuid);  // 0x00C5DB40
    HKEY OpenDeviceRegistryKey(const char* interfacePath);   // 0x00C5DC60
    long GetAccelerationLevel(const _GUID* deviceGuid);
};

long CDSoundAccelerationQuery::GetAccelerationLevel(const _GUID* deviceGuid)
{
    long versionProbe;
    EnsureWindowsVersionDetected_00c73770(&versionProbe);
    if (IsWindows9x())
        return 3;
    FindDeviceInterface(deviceGuid);
    if (!m_interfacePath[0] || g_isWindows95 || IsWindows9x())
        return 3;
    HKEY deviceKey = OpenDeviceRegistryKey(m_interfacePath);
    if (deviceKey == (HKEY)-1)
        return 3;
    CRegistryKey device;
    device.Attach(deviceKey);
    CRegistryKey mixerDefaults;
    unsigned long acceleration;
    if (!mixerDefaults.Create(deviceKey, "DirectSound\\Mixer Defaults", 0xF003F)
        || !mixerDefaults.QueryDWORD("Acceleration", &acceleration) || acceleration == 0)
        return 3;
    switch (acceleration) {
    case 8:
        return 2;
    case 15: {
        CRegistryKey presence;
        unsigned long present;
        if (!presence.Create(deviceKey, "DirectSound\\Device Presence", 0xF003F))
            return 0;
        if ((!presence.QueryDWORD("VxD", &present) || present == 0) && !presence.QueryDWORD("WDM", &present))
            return 0;
        if (present != 1)
            return 0;
        return 1;
    }
    }
    return 0;
}