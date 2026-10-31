// Behaviour test for CDSoundAccelerationQuery::OpenDeviceRegistryKey (0x00C5DC60): the device interface path
// "\\?\a#b#c#..." becomes the key path "...\Enum\a\b\c" (first three '#'-terminated fields); a path without the
// "\\?\" prefix or with fewer than three fields gives -1.
#include <stdio.h>
#include <string.h>
static const char* path;
static char opened[300];
static long Open(const char* interfacePath)
{
    char keyPath[260];
    strcpy(keyPath, "ENUM");
    if (strncmp("\\\\?\\", interfacePath, 4)) return -1;
    int i = 4; int start = i; int fields = 0;
    for (; fields < 3; ++i) {
        const char* p = &path[i];
        if (*p == 0) return -1;
        if (*p == '#') { strcat(keyPath, "\\"); strncat(keyPath, path + start, i - start); start = i + 1; ++fields; }
    }
    strcpy(opened, keyPath); return 1;
}
int main()
{
    path = "\\\\?\\PCI#VEN_1#3&2#{guid}";
    if (Open(path) != 1 || strcmp(opened, "ENUM\\PCI\\VEN_1\\3&2") != 0) { printf("00c5dc60_TEST FAIL\n"); return 1; }
    path = "\\\\?\\PCI#VEN"; if (Open(path) != -1) { printf("00c5dc60_TEST FAIL\n"); return 1; }
    path = "C:\\x#y#z#"; if (Open(path) != -1) { printf("00c5dc60_TEST FAIL\n"); return 1; }
    printf("00c5dc60_TEST PASS\n"); return 0;
}