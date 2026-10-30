// Behaviour test for CDSoundAccelerationQuery::OpenDeviceRegistryKey (0x00C5DC60): the "\\?\" interface path's first
// three '#' fields become the hardware-profile Enum key; bad prefixes, short paths and a failed open give -1.
#include <cstdio>
#include <cstring>
static char g_opened[512]; static long g_result = 0;
static long RegOpen(const char* path, long* key) { std::strcpy(g_opened, path); *key = 0x1234; return g_result; }
static long OpenDeviceRegistryKey(const char* m_interfacePath, const char* interfacePath)
{
    char keyPath[260];
    std::strcpy(keyPath, "SYSTEM\\CurrentControlSet\\Hardware Profiles\\Current\\System\\CurrentControlSet\\Enum");
    if (std::strncmp(interfacePath, "\\\\?\\", 4)) return -1;
    int start = 4, fields = 0;
    for (const char* p = m_interfacePath + 4; fields < 3; ++p) {
        if (*p == 0) return -1;
        if (*p == '#') {
            std::strcat(keyPath, "\\");
            std::strncat(keyPath, m_interfacePath + start, (p - m_interfacePath) - start);
            start = (p - m_interfacePath) + 1;
            ++fields;
        }
    }
    long key;
    if (RegOpen(keyPath, &key) != 0) return -1;
    return key;
}
int main()
{
    const char* path = "\\\\?\\pci#ven_1102&dev_0002#4&2b5bd3a&0&48f0#{6994ad04-93ef-11d0-a3cc-00a0c9223196}\\wave";
    bool ok = OpenDeviceRegistryKey(path, path) == 0x1234
        && std::strcmp(g_opened, "SYSTEM\\CurrentControlSet\\Hardware Profiles\\Current\\System\\CurrentControlSet\\Enum"
                                 "\\pci\\ven_1102&dev_0002\\4&2b5bd3a&0&48f0") == 0;
    ok = ok && OpenDeviceRegistryKey(path, "C:\\x") == -1;
    ok = ok && OpenDeviceRegistryKey("\\\\?\\pci#only", "\\\\?\\pci#only") == -1;
    g_result = 2;
    ok = ok && OpenDeviceRegistryKey(path, path) == -1;
    if (!ok) { std::printf("00c5dc60_TEST FAIL\n"); return 1; }
    std::printf("00c5dc60_TEST PASS\n");
    return 0;
}
