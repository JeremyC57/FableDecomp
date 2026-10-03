// GUIDs that uuid.lib defines on Windows (the headers only declare them).
#include <windows.h>

extern "C" {
extern const GUID GUID_NULL;
extern const IID IID_IUnknown;
const GUID GUID_NULL = {0x00000000, 0x0000, 0x0000, {0, 0, 0, 0, 0, 0, 0, 0}};
const IID IID_IUnknown = {0x00000000, 0x0000, 0x0000, {0xC0, 0, 0, 0, 0, 0, 0, 0x46}};
}
