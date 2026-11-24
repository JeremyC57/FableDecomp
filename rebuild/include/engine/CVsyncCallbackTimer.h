#ifndef FABLE_ENGINE_CVSYNCCALLBACKTIMER_H
#define FABLE_ENGINE_CVSYNCCALLBACKTIMER_H
// New header (hand3c2, row 0x009BF7E0 CDisplayManager::CreateDevice): CVsyncCallbackTimer, the object CDisplayManager owns through
// boost::scoped_ptr (VsyncCallbackTimer +0x294). Ego_d.pdb LF_CLASS 0xB3F7 / field list 0x60C01: private `Device` (IDirect3DDevice9*)
// at offset 0, then FreeQueoryObjects / PendingQueories (std::list<CPendingQuery>), PendingFrame, FinishedFrame; public
// ?SetDevice@CVsyncCallbackTimer@@QAEXPAUIDirect3DDevice9@@@Z, one 21-byte source line in Ego_d (the member store), inline in retail
// (0x009BFA0B `mov edx,[ebp+294h]` ... 0x009BFA23 `mov [edx],eax`). Only the first member is modelled: the lists are VC10 containers
// in Ego_d (16 bytes each there) and no retail row reads past offset 0 yet, so the size is not asserted and the class must not be
// constructed or embedded from this header.
struct IDirect3DDevice9;
#if defined(FABLE_RETAIL_CVSYNCCALLBACKTIMERINFO) || defined(FABLE_RETAIL_CVSYNCCALLBACKTIMER_GETINFO)
// RP1 (2026-11-20): CVsyncCallbackTimerInfo, the 48-byte record GetInfo returns by value (hidden result pointer). Members as Ego_r.pdb
// (struct_layouts_egor.tsv: doubles +0x00 / +0x08 / +0x18 / +0x20, longs +0x10 / +0x28, 0x30 bytes); retail agrees: the ctor 0x009F7020
// stores 0.0 / 0 at exactly those offsets and GetInfo 0x009F7050 fills them. A struct (U) in ?GetInfo@CVsyncCallbackTimer@@QAE?AU...
// The ctor is out of line (0x009F7020, Ego_r / FableWin ??0CVsyncCallbackTimerInfo@@QAE@XZ): declared only.
#include "rebuild_abi.h"
struct CVsyncCallbackTimerInfo {
    double CPUTimeStamp;                                                 // +0x00
    double CPUPrevTimeStamp;                                             // +0x08
    long   CPUFrameCount;                                                // +0x10
    double GPUTimeStamp;                                                 // +0x18
    double GPUPrevTimeStamp;                                             // +0x20
    long   GPUFrameCount;                                                // +0x28
    CVsyncCallbackTimerInfo();                                           // 0x009F7020
};
FABLE_STATIC_ASSERT(sizeof(CVsyncCallbackTimerInfo) == 0x30);
#endif
#ifdef FABLE_RETAIL_CVSYNCCALLBACKTIMER_PENDINGQUERY
struct IDirect3DQuery9;
#endif
class CVsyncCallbackTimer {
public:
#ifdef FABLE_RETAIL_CVSYNCCALLBACKTIMER_PENDINGQUERY
    // RP1 (2026-11-20): the list element of FreeQueoryObjects / PendingQueories (Ego_r.pdb, 0x10 bytes; a struct: every PDB instance
    // mangles UCPendingQuery@CVsyncCallbackTimer@@). Nested type only: no layout claim on CVsyncCallbackTimer.
    struct CPendingQuery {
        IDirect3DQuery9* Query;                                          // +0x00
        long             Frame;                                          // +0x04
        double           TimeStamp;                                      // +0x08
    };
#endif
#ifdef FABLE_RETAIL_CVSYNCCALLBACKTIMER_DTOR
    ~CVsyncCallbackTimer();                                              // 0x009F7330 ??1CVsyncCallbackTimer@@QAE@XZ (Ego_r, FableWin)
#endif
#ifdef FABLE_RETAIL_CVSYNCCALLBACKTIMER_GETINFO
    CVsyncCallbackTimerInfo GetInfo();                                   // 0x009F7050 ?GetInfo@CVsyncCallbackTimer@@QAE?AUCVsyncCallbackTimerInfo@@XZ
#endif
#ifdef FABLE_RETAIL_CVSYNCCALLBACKTIMER_ONPREDEVICERESET
    void OnPreDeviceReset();                                             // 0x009F73A0 ?OnPreDeviceReset@CVsyncCallbackTimer@@QAEXXZ
#endif
#ifdef FABLE_RETAIL_CVSYNCCALLBACKTIMER_ONPOSTSWAPSCREENS
    void OnPostSwapScreens();                                            // 0x009F73C0 ?OnPostSwapScreens@CVsyncCallbackTimer@@QAEXXZ
#endif
    void SetDevice(IDirect3DDevice9* device) { Device = device; }
private:
    IDirect3DDevice9* Device;                                            // +0x00
};
#endif
