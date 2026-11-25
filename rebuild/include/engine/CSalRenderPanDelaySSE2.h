#ifndef FABLE_CSALRENDERPANDELAYSSE2_H
#define FABLE_CSALRENDERPANDELAYSSE2_H
// GameCODA HRTF-delay panner, SSE2 variant (NF447; vtable 0x012B8A64: ClearFilterHistory 0x00C6D560, Process 0x00C6D630, SetGain /
// SetPosition inherited from CSalRenderPanDelay 0x00C6D120 / 0x00C6D150). Names: GameCODA 2.10 SDK CODAApiDbg.lib
// Debug/SalRenderPanDelaySSE2.obj CodeView (CSalRenderPanDelaySSE2 : public CSalRenderPanDelay, no own members) + the retail tie of
// CSalSw3DVoice::CreateRenderer 0x00C55270 (SSE2 branch, C_HRTF_DELAY: `call 0x00C6CA20 ; mov [esi],012B8A64h`; NF441 CLASS_TABLE).
// The constructor is the inline empty one CreateRenderer expands; that translation unit also keeps its out-of-line COMDAT copy 0x00C55070 (no
// caller; an option S thin row of unit 0x00C55270), as the SDK Release SalSw3DVoice.obj does. The destructor is implicit: retail 0x00C554E0 is
// `jmp 0x00C6CAC0` and the deleting destructor 0x00C554C0 calls it.
#include "engine/CSalRenderPanDelay.h"
class CSalRenderPanDelaySSE2:public CSalRenderPanDelay {
public:
 CSalRenderPanDelaySSE2(){}                                                  // 0x00C55070 (out-of-line COMDAT copy)
 virtual void ClearFilterHistory();                                       // slot 1, 0x00C6D560
 virtual void Process(const short*,int*(&)[4]);                           // slot 2, 0x00C6D630
};
#endif
