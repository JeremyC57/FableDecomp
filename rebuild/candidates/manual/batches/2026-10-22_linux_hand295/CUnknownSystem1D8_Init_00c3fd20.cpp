// CUnknownSystem1D8::Init(void*, const SSystemInitParams*) -- copy or default the init parameters, default the window to the foreground window, read the performance-counter frequency (failing with -24) and derive the ticks per 44.1 kHz sample, stamp the start time, create the 12-byte device object (dropping it if its init fails), run the two sub-system inits (clearing each one's global state when it is disabled or fails) and return 0, or -9 when nothing came up
// Leaf: ?Init@CUnknownSystem1D8
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C3FD20, length 581 bytes (`ret 8`); called from 0x00C4D3B5 right after the
//   owner is allocated (0x1D8 bytes, module operator new 0x00C3C360) and constructed (0x00C3E520). Not a quest script: the
//   manifest label "CQ_GuildTrainingSkillScript::InitializeRenderer" is wrong and the class has no name in any PDB dump.
// Callees: default parameters 0x00C3E260 (cdecl), GetForegroundWindow (USER32 IAT 0x01440378), QueryPerformanceFrequency /
//   QueryPerformanceCounter (KERNEL32 IAT 0x0143FE04 / 0x0143FE00), __alldiv 0x00F3FC50, module operator new 0x00C3C360
//   (device object, vtable 0x012B5BB0: slot 0 deleting dtor, slot 1 Init), sub-system inits 0x00C3EC30 and 0x00C3F9D0.
// Data: performance frequency 0x0139CA20; sub-system state globals 0x0139C988 .. 0x0139CA1C (descriptive names).
// Shape: each disabled/failed sub-system is wiped with intrinsic memsets of 0x18-byte slots (0x00C4D0B0 picks one slot
//   of each pair by pointer): every memset expansion materialises its own zero register (xor ecx / xor edx) apart from
//   the function-wide int zero in ebx, and the scheduler interleaves the two expansions. Statement order (the int
//   stores before or after the memsets) fixes where the ebx stores land. The device's 0x80000000 field is stored
//   before its vfptr because it belongs to a non-polymorphic base (MSVC puts the vfptr first, the base at +4).
// Layout: owner +0x17C, +0x180, +0x188 device, +0x190 start counter, +0x198 last counter, +0x1B0 ticks per sample, +0x1D4.
typedef union _LARGE_INTEGER { struct { unsigned long LowPart; long HighPart; }; __int64 QuadPart; } LARGE_INTEGER;
extern "C" __declspec(dllimport) void* __stdcall GetForegroundWindow();
extern "C" __declspec(dllimport) int __stdcall QueryPerformanceFrequency(LARGE_INTEGER* frequency);
extern "C" __declspec(dllimport) int __stdcall QueryPerformanceCounter(LARGE_INTEGER* count);
struct SSystemInitParams {                              // 0x1c bytes
    long  Field00;
    long  Field04;
    long  Field08;
    long  DisableSubSystemA;                            // +0x0c
    long  DeviceParam;                                  // +0x10
    void* Window;                                       // +0x14
    long  Field18;
};
void __cdecl GetDefaultSystemInitParams(SSystemInitParams* params);   // 0x00C3E260
struct CSystemDeviceBase {                              // non-polymorphic base: lands at +4, after the vfptr
    long Field04;
    CSystemDeviceBase() : Field04((long)0x80000000) { }
};
struct CSystemDevice : CSystemDeviceBase {             // 12 bytes, vtable 0x012B5BB0
    long Field08;
    static void* operator new(unsigned int size);       // module operator new 0x00C3C360
    CSystemDevice() : Field08(0) { }
    virtual ~CSystemDevice();
    virtual bool Init(long param, void* window, long flags);
};
extern LARGE_INTEGER g_PerformanceFrequency;            // 0x0139CA20
extern "C" void* __cdecl memset(void* dest, int value, unsigned int count);
#pragma intrinsic(memset)
struct SSubSystemSlot { long Values[6]; };              // 0x18 bytes; 0x00C4D0B0 picks one of each pair by pointer
struct SSubSystemExtra { long Values[5]; };             // 0x14 bytes, up to the flags word
extern SSubSystemSlot  g_SubSystemASlot0;               // 0x0139C988
extern SSubSystemSlot  g_SubSystemASlot1;               // 0x0139C9A0
extern SSubSystemSlot  g_SubSystemBSlot0;               // 0x0139C9B8 (first dword read by 0x00C3E260)
extern SSubSystemSlot  g_SubSystemBSlot1;               // 0x0139C9D0 (first dword read by 0x00C3E260)
extern SSubSystemExtra g_SubSystemBExtra;               // 0x0139C9E8
extern unsigned long g_SubSystemFlags;                  // 0x0139C9FC
extern long g_SubSystemA0C;                             // 0x0139CA0C
extern long g_SubSystemA10;                             // 0x0139CA10
extern long g_SubSystemB14;                             // 0x0139CA14
extern long g_SubSystemB18;                             // 0x0139CA18
extern long g_SubSystemA1C;                             // 0x0139CA1C
class CUnknownSystem1D8 {
public:
    unsigned char _pad000[0x17c];
    long           Field17C;                            // +0x17c
    long           Field180;                            // +0x180
    long           _pad184;
    CSystemDevice* Device;                              // +0x188
    long           _pad18c;
    LARGE_INTEGER  StartCounter;                        // +0x190
    LARGE_INTEGER  LastCounter;                         // +0x198
    unsigned char  _pad1a0[0x1b0 - 0x1a0];
    __int64        TicksPerSample;                      // +0x1b0
    unsigned char  _pad1b8[0x1d4 - 0x1b8];
    long           Field1D4;                            // +0x1d4
    long Init(void* owner, const SSystemInitParams* params);
    bool InitSubSystemA(void* owner, SSystemInitParams* params);   // 0x00C3EC30
    bool InitSubSystemB(void* owner, SSystemInitParams* params);   // 0x00C3F9D0
};
long CUnknownSystem1D8::Init(void* owner, const SSystemInitParams* params)
{
    SSystemInitParams p;
    if (params == 0)
        GetDefaultSystemInitParams(&p);
    else
        p = *params;
    if (p.Window == 0 || p.Window == (void*)-1)
        p.Window = GetForegroundWindow();
    Field17C = 0;
    if (!QueryPerformanceFrequency(&g_PerformanceFrequency))
        return -24;
    TicksPerSample = g_PerformanceFrequency.QuadPart / 44100;
    QueryPerformanceCounter(&StartCounter);
    LastCounter = StartCounter;
    Device = new CSystemDevice;
    if (Device && !Device->Init(p.DeviceParam, p.Window, 0)) {
        delete Device;
        Device = 0;
    }
    bool ok = false;
    if (!p.DisableSubSystemA)
        ok = InitSubSystemA(owner, &p);
    if (p.DisableSubSystemA || !ok) {
        g_SubSystemA0C = 0;
        g_SubSystemA10 = 0;
        g_SubSystemFlags &= ~0x10;
        memset(&g_SubSystemASlot0, 0, sizeof(g_SubSystemASlot0));
        memset(&g_SubSystemASlot1, 0, sizeof(g_SubSystemASlot1));
        g_SubSystemA1C = 0;
    }
    ok = InitSubSystemB(owner, &p);
    if (!ok) {
        g_SubSystemFlags &= ~0x20;
        g_SubSystemB14 = 0;
        g_SubSystemB18 = 0;
        memset(&g_SubSystemBSlot0, 0, sizeof(g_SubSystemBSlot0));
        memset(&g_SubSystemBSlot1, 0, sizeof(g_SubSystemBSlot1));
        memset(&g_SubSystemBExtra, 0, sizeof(g_SubSystemBExtra));
    }
    if (Field1D4 != 0 || Field180 != 0)
        ok = true;
    return ok ? 0 : -9;
}
