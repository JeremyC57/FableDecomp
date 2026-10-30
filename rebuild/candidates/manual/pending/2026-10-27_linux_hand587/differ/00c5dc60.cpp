// CDSoundAccelerationQuery::OpenDeviceRegistryKey(const char* interfacePath) -- turn the device interface path found by FindDeviceInterface ("\\?\<bus>#<device>#<instance>#{class}") into its hardware-profile Enum key (SYSTEM\CurrentControlSet\Hardware Profiles\Current\System\CurrentControlSet\Enum\<bus>\<device>\<instance>) and open it; returns the key, or -1
// Grade: DIFFER(265v267) -- loop induction variables: retail spills `this` to [esp+8] at entry, keeps the offset in ebx, the pointer in esi, start in eax and the field count in ebp, and rebuilds start after strncat as p + (1 - this) (a spilled invariant); ours keeps this in ebp and the count in memory; the -1 exit block sits inline (retail) vs at the end (ours)
// Leaf: ?OpenDeviceRegistryKey@CDSoundAccelerationQuery@@
// Flags: /GS
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C5DC60, 267 bytes, thiscall, `ret 4`; /GS frame (cookie 0x0139C8A8, check 0x00BFE9F9).
// Caller: the DirectSound acceleration probe 0x00C5DD70 (call at 0x00C5DDFE, `push esi ; mov ecx,esi`: the object's own
//   path is the argument), after FindDeviceInterface 0x00C5DB40 (landed) found a path; it gives up when -1 comes back, else
//   reads the key through the registry wrapper 0x00C73790 / 0x00C73820.
// Callees: strncmp 0x00BFEAF8, strncat 0x00F3FCFA (MSVCR71 import thunk), RegOpenKeyExA (IAT 0x0143FC08); strcpy and the
//   "\\" strcat are the VC7.1 intrinsics (rep movsd of the 81-byte literal 0x012B9030; find-end + word store of 0x0124C530).
// Name: descriptive (libpk / GameCODA: no PDB or RTTI); the class and its m_interfacePath (+0, 0x104 chars) are the landed
//   FindDeviceInterface model.
// Shape: `if (strncmp(interfacePath, "\\\\?\\", 4)) return -1;` (its own early /GS epilogue), then the first three
//   '#'-separated fields of m_interfacePath after the "\\?\" prefix are appended, each behind a "\\", into the
//   MAX_PATH key buffer; a NUL before the third '#' and a failed RegOpenKeyExA(HKEY_LOCAL_MACHINE, ..., KEY_ALL_ACCESS)
//   share the -1 exit. The loop reads the member (the saved `this`), the prefix test reads the argument.
extern "C" int __cdecl strncmp(const char* a, const char* b, unsigned int count);    // 0x00BFEAF8
extern "C" char* __cdecl strncat(char* dest, const char* src, unsigned int count);   // 0x00F3FCFA
extern "C" char* __cdecl strcpy(char* dest, const char* src);
extern "C" char* __cdecl strcat(char* dest, const char* src);
#pragma intrinsic(strcpy, strcat)

struct HKEY__ { int unused; };
typedef HKEY__* HKEY;
#define HKEY_LOCAL_MACHINE ((HKEY)(unsigned long)0x80000002)
#define KEY_ALL_ACCESS 0xF003F
#define ERROR_SUCCESS 0L
#define MAX_PATH 260
extern "C" __declspec(dllimport) long __stdcall RegOpenKeyExA(HKEY key, const char* subKey, unsigned long options,
                                                              unsigned long desired, HKEY* result);   // IAT 0x0143FC08

class CDSoundAccelerationQuery {
public:
    char m_interfacePath[0x104];                         // +0x000 the device's driver interface path, empty when unknown
    HKEY OpenDeviceRegistryKey(const char* interfacePath);
};

HKEY CDSoundAccelerationQuery::OpenDeviceRegistryKey(const char* interfacePath)
{
    char keyPath[MAX_PATH];
    strcpy(keyPath, "SYSTEM\\CurrentControlSet\\Hardware Profiles\\Current\\System\\CurrentControlSet\\Enum");
    if (strncmp(interfacePath, "\\\\?\\", 4))
        return (HKEY)-1;
    int start = 4;
    int fields = 0;
    for (const char* p = m_interfacePath + 4; fields < 3; ++p) {
        if (*p == 0)
            return (HKEY)-1;
        if (*p == '#') {
            strcat(keyPath, "\\");
            strncat(keyPath, m_interfacePath + start, (p - m_interfacePath) - start);
            start = (p - m_interfacePath) + 1;
            ++fields;
        }
    }
    HKEY key;
    if (RegOpenKeyExA(HKEY_LOCAL_MACHINE, keyPath, 0, KEY_ALL_ACCESS, &key) != ERROR_SUCCESS)
        return (HKEY)-1;
    return key;
}
