// CAudioDevice::GetElapsedTicks() -- performance-counter ticks elapsed since the stored start count (+0x190): QueryPerformanceCounter now, minus the start, as a 64-bit result in edx:eax
// Leaf: ?GetElapsedTicks@CAudioDevice@@QAE_JXZ
// Naming (sweep1, 2026-10-25): was landed under the placeholder class CAudioTimer_00c3cfe0; FINDINGS chunk 438 ("0xC3CFE0's class
//   is the audio device"). The same body is inlined at 0x00C3D504 in the audio system's frame update on `[system+4]` --
//   the device object that agent438's CAudioDevice BeginFrame 0x00C3E760 / UpdateMeters 0x00C3E800 rows work on (start
//   count +0x190/+0x194). Descriptive (no PDB covers libpk).
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C3CFE0, 46 bytes, __fastcall, plain `ret`, __int64 result.
// Library: libpk audio (no PDB/RTTI); descriptive names (no caller or vtable reference to this body survives in the
//   image). The manifest's "GetElapsedPerformanceTime" is propagated.
// Callee: KERNEL32 QueryPerformanceCounter through the import slot 0x0143FE00 (`call [__imp_]`).
// Shape: the counter is an 8-byte frame local; the subtraction is `sub ; sbb` on the two halves.
extern "C" __declspec(dllimport) int __stdcall QueryPerformanceCounter(__int64* count);   // [0x0143FE00]

struct CAudioDevice {
    unsigned char _pad_0x000[0x190];
    __int64 m_startCount;                       // +0x190
    __int64 GetElapsedTicks();
};

__int64 CAudioDevice::GetElapsedTicks()
{
    __int64 now;
    QueryPerformanceCounter(&now);
    return now - m_startCount;
}