#ifndef FABLE_CSALRENDERPANSSE2_H
#define FABLE_CSALRENDERPANSSE2_H
// GameCODA volume panner, SSE2 variant (NF452; vtable 0x012B8A78: Process 0x00C6E410 over the xmm kernels 0x00C6E290 / 0x00C6E1F0; SetGain / SetPosition inherited
// from CSalRenderPan 0x00C6E020 / 0x00C6E050). Names: GameCODA 2.10 SDK CODAApiDbg.lib Debug/SalRenderPanSSE2.obj CodeView
// (CSalRenderPanSSE2 : public CSalRenderPan, no own members) + the retail tie of CSalSw3DVoice::CreateRenderer 0x00C55270 (NF441
// CLASS_TABLE / NAME_FIXES: the SDK Release CreateRenderer calls ??0CSalRenderPanSSE2 where retail calls 0x00C55090, same branch, same order).
// The constructor is the inline empty one; CreateRenderer calls its out-of-line COMDAT copy 0x00C55090 (the SDK Release SalSw3DVoice.obj does the
// same: a COMDAT of that object, called, not expanded), an option S thin row of unit 0x00C55270. The destructor is implicit (retail 0x00C55510
// is `jmp 0x00C6DA30`; the deleting destructor 0x00C554F0 calls it).
#include "engine/CSalRenderPan.h"
class CSalRenderPanSSE2:public CSalRenderPan {
public:
 CSalRenderPanSSE2(){}                                                      // 0x00C55090 (out-of-line COMDAT copy)
 virtual void Process(const short*,int*(&)[4]);                           // slot 2, 0x00C6E410
};
#endif
