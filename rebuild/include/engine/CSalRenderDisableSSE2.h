#ifndef FABLE_CSALRENDERDISABLESSE2_H
#define FABLE_CSALRENDERDISABLESSE2_H
// GameCODA plain-gain renderer, SSE2 variant (NF452; vtable 0x012B8A8C: Process 0x00C6EEF0 over the xmm kernels 0x00C6ED70 / 0x00C6ECD0; SetGain / SetPosition inherited
// from CSalRenderDisable 0x00C6EC50 / 0x00C6EC80). Names: GameCODA 2.10 SDK CODAApiDbg.lib Debug/SalRenderDisableSSE2.obj CodeView
// (CSalRenderDisableSSE2 : public CSalRenderDisable, no own members) + the retail tie of CSalSw3DVoice::CreateRenderer 0x00C55270 (NF441
// CLASS_TABLE / NAME_FIXES: the SDK Release CreateRenderer calls ??0CSalRenderDisableSSE2 where retail calls 0x00C550B0, same branch, same order).
// The constructor is the inline empty one; CreateRenderer calls its out-of-line COMDAT copy 0x00C550B0 (the SDK Release SalSw3DVoice.obj does the
// same: a COMDAT of that object, called, not expanded), an option S thin row of unit 0x00C55270. The destructor is implicit (retail 0x00C55540
// is `jmp 0x00C6E780`; the deleting destructor 0x00C55520 calls it).
#include "engine/CSalRenderDisable.h"
class CSalRenderDisableSSE2:public CSalRenderDisable {
public:
 CSalRenderDisableSSE2(){}                                                  // 0x00C550B0 (out-of-line COMDAT copy)
 virtual void Process(const short*,int*(&)[4]);                           // slot 2, 0x00C6EEF0
};
#endif
