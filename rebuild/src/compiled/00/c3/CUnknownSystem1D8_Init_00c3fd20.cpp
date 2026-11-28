// Flags: 
// CUnknownSystem1D8::Init(void*, const SSystemInitParams*) -- copy or default the init parameters, default the window to the foreground window, read the performance-counter frequency (failing with -24) and derive the ticks per 44.1 kHz sample, stamp the start time, create the 12-byte device object (dropping it if its init fails), run the two sub-system inits (clearing each one's global state when it is disabled or fails) and return 0, or -9 when nothing came up
// Leaf: ?Init@CUnknownSystem1D8@@QAEJPAXPBUSSystemInitParams@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C3FD20, length 581 bytes (`ret 8`); called from 0x00C4D3B5 right after the
//   owner is allocated (0x1D8 bytes, module operator new 0x00C3C360) and constructed (0x00C3E520). Not a quest script: the
//   manifest label "CQ_GuildTrainingSkillScript::InitializeRenderer" is wrong and the class has no name in any PDB dump.
// Callees: default parameters 0x00C3E260 (cdecl), GetForegroundWindow (USER32 IAT 0x01440378), QueryPerformanceFrequency /
//   QueryPerformanceCounter (KERNEL32 IAT 0x0143FE04 / 0x0143FE00), __alldiv 0x00F3FC50, module operator new 0x00C3C360
//   (device object, vtable 0x012B5BB0: slot 0 deleting dtor, slot 1 Init), sub-system inits 0x00C3EC30 and 0x00C3F9D0.
// Data: performance frequency 0x0139CA20; sub-system state globals 0x0139C988 .. 0x0139CA1C (descriptive names).
// Shape: each disabled/failed sub-system is wiped with intrinsic memsets of 0x18-byte slots (0x00C4D0B0 picks one slot
//   of each pair by pointer): every memset expansion materialises its own zero value (xor ecx / xor edx) apart from
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
#include "engine/CodaCaps.h"
extern "C" void* __cdecl memset(void* dest, int value, unsigned int count);
#pragma intrinsic(memset)
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
    if (!QueryPerformanceFrequency(reinterpret_cast<LARGE_INTEGER*>(&g_SalCaps.RefTimeTicksPerSec)))
        return -24;
    TicksPerSample = g_SalCaps.RefTimeTicksPerSec / 44100;
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
        g_SalCaps.nHWAllVoices = 0;
        g_SalCaps.nHWFreeVoices = 0;
        g_SalCaps.ffSystem &= ~0x10;
        memset(&g_SalCaps.HW2DVoice, 0, sizeof(g_SalCaps.HW2DVoice));
        memset(&g_SalCaps.HW3DVoice, 0, sizeof(g_SalCaps.HW3DVoice));
        g_SalCaps.nHWReverbs = 0;
    }
    ok = InitSubSystemB(owner, &p);
    if (!ok) {
        g_SalCaps.ffSystem &= ~0x20;
        g_SalCaps.nSWAllVoices = 0;
        g_SalCaps.nSWFreeVoices = 0;
        memset(&g_SalCaps.SW2DVoice, 0, sizeof(g_SalCaps.SW2DVoice));
        memset(&g_SalCaps.SW3DVoice, 0, sizeof(g_SalCaps.SW3DVoice));
        memset(&g_SalCaps.SWMixer, 0, sizeof(g_SalCaps.SWMixer));
    }
    if (Field1D4 != 0 || Field180 != 0)
        ok = true;
    return ok ? 0 : -9;
}