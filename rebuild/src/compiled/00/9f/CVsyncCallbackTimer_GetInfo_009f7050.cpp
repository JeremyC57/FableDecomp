// CVsyncCallbackTimer::GetInfo() -- snapshot the vsync timer: CPU frame count and time stamps from the timer globals, GPU time stamps and frame count from the current GPU record (read under its interlocked busy counter), returned by value
// Leaf: ?GetInfo@CVsyncCallbackTimer@@QAE?AUCVsyncCallbackTimerInfo@@XZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009F7050, length 138 bytes (+ int3 padding), `ret 4` (hidden result pointer).
// PDB: ?GetInfo@CVsyncCallbackTimer@@QAE?AUCVsyncCallbackTimerInfo@@XZ (Ego_r.pdb name set + reloc_bindings.tsv; NOT const --
//   the earlier attempt's `const` mangled QBE). CVsyncCallbackTimerInfo is a struct (U) in the mangling.
// Callees: InterlockedIncrement (IAT 0x0143FD34) / InterlockedDecrement (IAT 0x0143FD38) on the record's busy word;
//   CVsyncCallbackTimerInfo::CVsyncCallbackTimerInfo @ 0x009F7020 (landed; optimised out-of-line inline ctor).
// Data: the GPU record array 0x013CB4B0 (0x18 bytes each: time stamp double +0, previous time stamp double +8, frame count
//   +0x10, interlocked busy counter +0x14) indexed by 0x013CB4F4; CPU time stamps (doubles) 0x013CB4E0 / 0x013CB4E8 and the
//   CPU frame count 0x013CB4F0. These statics have no PDB names in our sets: descriptive names.
// Layout: CVsyncCallbackTimer (Ego_r.pdb names): Device +0, FreeQueoryObjects +4, PendingQueories +8 (4-byte STLport lists),
//   PendingFrame +0xc, FinishedFrame +0x10; `this` is homed but unused. No engine header exists for the class.
// Shape: the vsync TU is unoptimised (`#pragma optimize("", off)`, like its landed siblings 0x009F73A0 / 0x009F7250): every
//   value goes through memory and the 48-byte result is copied out with `rep movsd`. The /Od frame (info at ebp-0x30, record
//   pointer at ebp-0x34, `this` ebp-0x38, `sub esp,38h`) needs BOTH locals at function scope (an inner `{ }` around `info`
//   moves the pointer to ebp-4 and pads the frame to 0x3C) AND a local name whose symbol-table order puts it below `info`:
//   VC7.1 /Od lays out locals by NAME (hash order), not declaration order -- `pRecord`, `r`, `gpu` give retail's frame,
//   `record`, `rec`, `timeRecord` give the 0x3C frame. The retail spelling is unknown; `pRecord` is descriptive.
extern "C" __declspec(dllimport) long __stdcall InterlockedIncrement(long* addend);   // IAT 0x0143FD34
extern "C" __declspec(dllimport) long __stdcall InterlockedDecrement(long* addend);   // IAT 0x0143FD38

struct CVsyncCallbackTimerInfo {                   // 48 bytes (Ego_r.pdb)
    double        CPUTimeStamp;                    // +0x00
    double        CPUPrevTimeStamp;                // +0x08
    long          CPUFrameCount;                   // +0x10
    double        GPUTimeStamp;                    // +0x18
    double        GPUPrevTimeStamp;                // +0x20
    long          GPUFrameCount;                   // +0x28
    CVsyncCallbackTimerInfo();                     // 0x009F7020
};

struct SVsyncGPUTimeRecord {                       // 0x18 bytes (descriptive)
    double        TimeStamp;                       // +0x00
    double        PrevTimeStamp;                   // +0x08
    long          FrameCount;                      // +0x10
    long          Busy;                            // +0x14 interlocked reader count
};
extern SVsyncGPUTimeRecord g_VsyncGPUTimeRecords[];   // 0x013CB4B0
extern long                g_VsyncGPUTimeRecordIndex; // 0x013CB4F4
extern double              g_VsyncCPUTimeStamp;       // 0x013CB4E0
extern double              g_VsyncCPUPrevTimeStamp;   // 0x013CB4E8
extern long                g_VsyncCPUFrameCount;      // 0x013CB4F0

struct IDirect3DDevice9;
class CVsyncCallbackTimer {
public:
    IDirect3DDevice9* Device;                      // +0x00
    void*             FreeQueoryObjects;           // +0x04 STLport list<CPendingQuery>
    void*             PendingQueories;             // +0x08 STLport list<CPendingQuery>
    long              PendingFrame;                // +0x0c
    long              FinishedFrame;               // +0x10
    CVsyncCallbackTimerInfo GetInfo();
};

#pragma optimize("", off)
CVsyncCallbackTimerInfo CVsyncCallbackTimer::GetInfo()
{
    SVsyncGPUTimeRecord* pRecord = &g_VsyncGPUTimeRecords[g_VsyncGPUTimeRecordIndex];
    InterlockedIncrement(&pRecord->Busy);
    CVsyncCallbackTimerInfo info;
    info.CPUFrameCount = g_VsyncCPUFrameCount;
    info.GPUFrameCount = pRecord->FrameCount;
    info.CPUTimeStamp = g_VsyncCPUTimeStamp;
    info.CPUPrevTimeStamp = g_VsyncCPUPrevTimeStamp;
    info.GPUTimeStamp = pRecord->TimeStamp;
    info.GPUPrevTimeStamp = pRecord->PrevTimeStamp;
    InterlockedDecrement(&pRecord->Busy);
    return info;
}
#pragma optimize("", on)