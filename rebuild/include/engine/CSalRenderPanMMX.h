#ifndef FABLE_CSALRENDERPANMMX_H
#define FABLE_CSALRENDERPANMMX_H
// GameCODA volume panner, MMX variant (NF452; vtable 0x012B8AB4: Process 0x00C6F990 over the mm kernels 0x00C6F800 / 0x00C6F730; SetGain / SetPosition inherited
// from CSalRenderPan 0x00C6E020 / 0x00C6E050). Names: GameCODA 2.10 SDK CODAApiDbg.lib Debug/SalRenderPanMMX.obj CodeView
// (CSalRenderPanMMX : public CSalRenderPan, no own members) + the retail tie of CSalSw3DVoice::CreateRenderer 0x00C55270 (NF441
// CLASS_TABLE / NAME_FIXES: the SDK Release CreateRenderer calls ??0CSalRenderPanMMX where retail calls 0x00C550F0, same branch, same order).
// The constructor is the inline empty one; CreateRenderer calls its out-of-line COMDAT copy 0x00C550F0 (the SDK Release SalSw3DVoice.obj does the
// same: a COMDAT of that object, called, not expanded), an option S thin row of unit 0x00C55270. The destructor is implicit (retail 0x00C555A0
// is `jmp 0x00C6DA30`; the deleting destructor 0x00C55580 calls it).
#include "engine/CSalRenderPan.h"
class CSalRenderPanMMX:public CSalRenderPan {
public:
 CSalRenderPanMMX(){}                                                      // 0x00C550F0 (out-of-line COMDAT copy)
 virtual void Process(const short*,int*(&)[4]);                           // slot 2, 0x00C6F990
};
#endif
