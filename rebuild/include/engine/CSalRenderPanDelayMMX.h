#ifndef FABLE_CSALRENDERPANDELAYMMX_H
#define FABLE_CSALRENDERPANDELAYMMX_H
// GameCODA HRTF-delay panner, MMX variant (NF447; vtable 0x012B8AA0: ClearFilterHistory 0x00C6F300, Process 0x00C6F3E0, SetGain /
// SetPosition inherited from CSalRenderPanDelay 0x00C6D120 / 0x00C6D150). Names: GameCODA 2.10 SDK CODAApiDbg.lib
// Debug/SalRenderPanDelayMMX.obj CodeView (CSalRenderPanDelayMMX : public CSalRenderPanDelay, no own members) + the retail tie of
// CSalSw3DVoice::CreateRenderer 0x00C55270 (MMX branch, C_HRTF_DELAY: `call 0x00C6CA20 ; mov [esi],012B8AA0h`; NF441 CLASS_TABLE).
// The constructor is the inline empty one CreateRenderer expands; that translation unit also keeps its out-of-line COMDAT copy 0x00C550D0 (no
// caller; an option S thin row of unit 0x00C55270), as the SDK Release SalSw3DVoice.obj does. The destructor is implicit: retail 0x00C55570 is
// `jmp 0x00C6CAC0` and the deleting destructor 0x00C55550 calls it.
#include "engine/CSalRenderPanDelay.h"
class CSalRenderPanDelayMMX:public CSalRenderPanDelay {
public:
 CSalRenderPanDelayMMX(){}                                                  // 0x00C550D0 (out-of-line COMDAT copy)
 virtual void ClearFilterHistory();                                       // slot 1, 0x00C6F300
 virtual void Process(const short*,int*(&)[4]);                           // slot 2, 0x00C6F3E0
};
#endif
