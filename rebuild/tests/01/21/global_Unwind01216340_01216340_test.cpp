// Behaviour test for CDSoundAccelerationQuery::GetAccelerationLevel (0x00C5DD70): 3 on Windows 9x / no interface path / no key /
// unreadable or zero "Acceleration"; 2 for 8; for 15, 1 when "VxD" (non-zero) or else "WDM" reads 1, otherwise 0; 0 for other
// settings. Every opened key is closed.
#include <cstdio>
#include <cstring>
static bool g_isWindows95, g_isWindows98, g_isWindows98SE, g_isWindowsMe;
static bool IsWindows9x() { return g_isWindows95 || g_isWindows98 || g_isWindows98SE || g_isWindowsMe; }
struct SValue { const char* key; const char* name; unsigned long value; };
static SValue g_values[8]; static int g_nValues = 0; static bool g_haveKey = true; static int g_open = 0;
typedef int HKEY;
struct CRegistryKey {
    const char* m_path; bool m_open;
    CRegistryKey() : m_path(0), m_open(false) {}
    ~CRegistryKey() { if (m_open) --g_open; }
    int Create(HKEY, const char* subKey, unsigned long) { m_path = subKey; m_open = true; ++g_open; return 1; }
    void Attach(HKEY) { m_path = "device"; m_open = true; ++g_open; }
    int QueryDWORD(const char* name, unsigned long* value)
    {
        for (int i = 0; i < g_nValues; ++i)
            if (!std::strcmp(g_values[i].key, m_path) && !std::strcmp(g_values[i].name, name)) { *value = g_values[i].value; return 1; }
        return 0;
    }
};
struct CQuery {
    char m_interfacePath[0x104];
    void FindDeviceInterface(const void*) {}
    HKEY OpenDeviceRegistryKey(const char*) { return g_haveKey ? 5 : -1; }
    long GetAccelerationLevel(const void* guid)
    {
        if (IsWindows9x()) return 3;
        FindDeviceInterface(guid);
        if (!m_interfacePath[0] || g_isWindows95 || IsWindows9x()) return 3;
        HKEY deviceKey = OpenDeviceRegistryKey(m_interfacePath);
        if (deviceKey == -1) return 3;
        CRegistryKey device; device.Attach(deviceKey);
        CRegistryKey mixerDefaults; unsigned long acceleration;
        if (!mixerDefaults.Create(deviceKey, "DirectSound\\Mixer Defaults", 0xF003F)
            || !mixerDefaults.QueryDWORD("Acceleration", &acceleration) || acceleration == 0) return 3;
        switch (acceleration) {
        case 8: return 2;
        case 15: {
            CRegistryKey presence; unsigned long present;
            if (!presence.Create(deviceKey, "DirectSound\\Device Presence", 0xF003F)) return 0;
            if ((!presence.QueryDWORD("VxD", &present) || present == 0) && !presence.QueryDWORD("WDM", &present)) return 0;
            if (present != 1) return 0;
            return 1;
        }
        }
        return 0;
    }
};
static void Set(int n, const SValue* v) { g_nValues = n; for (int i = 0; i < n; ++i) g_values[i] = v[i]; }
int main()
{
    CQuery q; std::strcpy(q.m_interfacePath, "\\\\?\\path");
    bool ok = true;
    SValue none[1] = { { "x", "x", 0 } };
    Set(0, none); ok = ok && q.GetAccelerationLevel(0) == 3;
    SValue std8[1] = { { "DirectSound\\Mixer Defaults", "Acceleration", 8 } };
    Set(1, std8); ok = ok && q.GetAccelerationLevel(0) == 2;
    SValue full[2] = { { "DirectSound\\Mixer Defaults", "Acceleration", 15 }, { "DirectSound\\Device Presence", "VxD", 1 } };
    Set(2, full); ok = ok && q.GetAccelerationLevel(0) == 1;
    SValue fullWdm[3] = { { "DirectSound\\Mixer Defaults", "Acceleration", 15 }, { "DirectSound\\Device Presence", "VxD", 0 },
                          { "DirectSound\\Device Presence", "WDM", 1 } };
    Set(3, fullWdm); ok = ok && q.GetAccelerationLevel(0) == 1;
    fullWdm[2].value = 2; Set(3, fullWdm); ok = ok && q.GetAccelerationLevel(0) == 0;
    Set(1, full); ok = ok && q.GetAccelerationLevel(0) == 0;
    SValue other[1] = { { "DirectSound\\Mixer Defaults", "Acceleration", 3 } };
    Set(1, other); ok = ok && q.GetAccelerationLevel(0) == 0;
    g_haveKey = false; ok = ok && q.GetAccelerationLevel(0) == 3; g_haveKey = true;
    g_isWindows98 = true; Set(1, std8); ok = ok && q.GetAccelerationLevel(0) == 3; g_isWindows98 = false;
    q.m_interfacePath[0] = 0; ok = ok && q.GetAccelerationLevel(0) == 3;
    ok = ok && g_open == 0;
    if (!ok) { std::printf("00c5dd70_TEST FAIL\n"); return 1; }
    std::printf("01216340_TEST PASS\n");
    return 0;
}