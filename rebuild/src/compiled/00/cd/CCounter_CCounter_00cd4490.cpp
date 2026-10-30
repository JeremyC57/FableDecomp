// CCounter::CCounter() -- a frame counter: records the thing manager's current world frame as its reference frame
// Leaf: ??0CCounter
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00CD4490, length 23 bytes.
// Callees: the thing manager global @ 0x0143E8F8, vtable slot 653 (+0xa34, long: the current world frame;
// the slot the landed CCounter::operator> 0x00D90670 and CCounter::GetCounter 0x00EFD630 read).
// Label: the catalog name "CTimer::CTimer" is propagated (a byte-identical shape with another slot); the
// world-frame slot makes this the CCounter constructor. PDB CCounter = { long RelevantWorldFrame } (4 bytes).
// Shape: `this` kept in esi across the virtual call, the frame stored from eax, `this` returned.
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
extern CGameScriptInterface* g_pGameScriptInterface;   // 0x0143E8F8

class CCounter { public:
    long RelevantWorldFrame;                       // +0x00
    CCounter();
};

CCounter::CCounter()
    : RelevantWorldFrame(g_pGameScriptInterface->GetWorldFrame())
{
}