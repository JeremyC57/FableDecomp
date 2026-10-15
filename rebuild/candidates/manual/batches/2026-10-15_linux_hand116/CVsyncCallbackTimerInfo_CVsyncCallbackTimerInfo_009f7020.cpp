// CVsyncCallbackTimerInfo::CVsyncCallbackTimerInfo() -- zero the four double timings and the two counters of the vsync timer's info record
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009F7020, length 46 bytes (+ int3 padding). No callees.
// Leaf: ??0CVsyncCallbackTimerInfo
// Its one caller is CVsyncCallbackTimer::GetInfo (0x009F7050 -> call @ 0x009F707B), so the catalog name is plausible here.
// Members: the PDB names (CVsyncCallbackTimerInfo, 48 bytes). Layout from the stores: double +0x00, double +0x08, dword +0x10 (+0x14 is alignment padding), double +0x18,
// double +0x20, dword +0x28. Shape: each 0.0 double is `fld qword [0x0122ED70] ; fstp` (the 0.0 pool constant, not fldz),
// the two integer zeros share ecx and are scheduled between the x87 pairs -- a plain initialiser list in member order.
struct CVsyncCallbackTimerInfo {
    double        CPUTimeStamp;      // +0x00
    double        CPUPrevTimeStamp;        // +0x08
    unsigned long CPUFrameCount;         // +0x10
    double        GPUTimeStamp;     // +0x18
    double        GPUPrevTimeStamp;     // +0x20
    unsigned long GPUFrameCount;        // +0x28
    CVsyncCallbackTimerInfo();
};

CVsyncCallbackTimerInfo::CVsyncCallbackTimerInfo()
    : CPUTimeStamp(0.0), CPUPrevTimeStamp(0.0), CPUFrameCount(0), GPUTimeStamp(0.0), GPUPrevTimeStamp(0.0), GPUFrameCount(0)
{
}
