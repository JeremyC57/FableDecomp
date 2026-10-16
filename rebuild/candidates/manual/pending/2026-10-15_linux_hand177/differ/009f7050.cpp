// CVsyncCallbackTimer::GetInfo() const -- snapshot the vsync timer: CPU frame count and time stamps from the timer globals, GPU time stamps and frame count from the current GPU record (read under its interlocked busy counter), returned by value
// Grade: DIFFER(138v138) -- /Od frame layout only: retail allocates the info local above the record pointer (info at ebp-0x30, record ebp-0x34, this ebp-0x38, `sub esp,38h`), ours puts the record first (ebp-4) and pads 4 bytes before the 8-aligned info (`sub esp,3Ch`); every instruction otherwise matches; tried pointer local, reference local, info in a nested block
// Leaf: ?GetInfo@CVsyncCallbackTimer@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009F7050, length 136 bytes (padded to 144), `ret 4` (hidden result).
// Callees: InterlockedIncrement (IAT 0x0143FD34) / InterlockedDecrement (IAT 0x0143FD38) on the record's busy word,
// CVsyncCallbackTimerInfo::CVsyncCallbackTimerInfo @ 0x009F7020 (landed).
// Data: the GPU record array 0x013CB4B0 (0x18 bytes each: GPUTimeStamp double +0, GPUPrevTimeStamp double +8, frame count +0x10,
// busy counter +0x14) indexed by 0x013CB4F4; the CPU time stamps (doubles) 0x013CB4E0 / 0x013CB4E8 and frame count 0x013CB4F0.
// Graphics layer (D3D9 vsync timing, to be replaced): CVsyncCallbackTimer = {IDirect3DDevice9* Device +0, the query lists,
// PendingFrame, FinishedFrame} (Ego_r.pdb, 36 bytes); `this` is saved but unused.
// Shape: an unoptimised body (frame-pointer locals, every value through memory, the 48-byte result copied out with
// `rep movsd`): `#pragma optimize("", off)` around this function; the record pointer is a named local.
extern "C" __declspec(dllimport) long __stdcall InterlockedIncrement(long volatile* addend);   // IAT 0x0143FD34
extern "C" __declspec(dllimport) long __stdcall InterlockedDecrement(long volatile* addend);   // IAT 0x0143FD38
struct CVsyncCallbackTimerInfo {                   // 48 bytes
    double        CPUTimeStamp;                    // +0x00
    double        CPUPrevTimeStamp;                // +0x08
    unsigned long CPUFrameCount;                   // +0x10
    double        GPUTimeStamp;                    // +0x18
    double        GPUPrevTimeStamp;                // +0x20
    unsigned long GPUFrameCount;                   // +0x28
    CVsyncCallbackTimerInfo();                     // 0x009F7020
};
struct SVsyncGPURecord {                           // 0x18 bytes
    double        TimeStamp;                       // +0x00
    double        PrevTimeStamp;                   // +0x08
    unsigned long FrameCount;                      // +0x10
    long          Busy;                            // +0x14 interlocked reader count
};
extern SVsyncGPURecord g_VsyncGPURecords[];        // 0x013CB4B0
extern long            g_VsyncGPURecordIndex;      // 0x013CB4F4
extern double          g_VsyncCPUTimeStamp;        // 0x013CB4E0
extern double          g_VsyncCPUPrevTimeStamp;    // 0x013CB4E8
extern unsigned long   g_VsyncCPUFrameCount;       // 0x013CB4F0
struct IDirect3DDevice9;
struct CVsyncCallbackTimer {
    IDirect3DDevice9* Device;                      // +0x00
    void*             FreeQueoryObjects;           // +0x04 (STLport list)
    void*             PendingQueories;             // +0x08 (STLport list)
    long              PendingFrame;                // +0x0c
    long              FinishedFrame;               // +0x10
    CVsyncCallbackTimerInfo GetInfo() const;
};
#pragma optimize("", off)
CVsyncCallbackTimerInfo CVsyncCallbackTimer::GetInfo() const
{
    SVsyncGPURecord* record = &g_VsyncGPURecords[g_VsyncGPURecordIndex];
    InterlockedIncrement(&record->Busy);
    {
        CVsyncCallbackTimerInfo info;
        info.CPUFrameCount = g_VsyncCPUFrameCount;
        info.GPUFrameCount = record->FrameCount;
        info.CPUTimeStamp = g_VsyncCPUTimeStamp;
        info.CPUPrevTimeStamp = g_VsyncCPUPrevTimeStamp;
        info.GPUTimeStamp = record->TimeStamp;
        info.GPUPrevTimeStamp = record->PrevTimeStamp;
        InterlockedDecrement(&record->Busy);
        return info;
    }
}
#pragma optimize("", on)
