#ifndef FABLE_CSALRENDERDISABLEMMX_H
#define FABLE_CSALRENDERDISABLEMMX_H
// GameCODA plain-gain renderer, MMX variant (NF452; vtable 0x012B8AC8: Process 0x00C6FE10 over the mm kernels 0x00C6FC80 / 0x00C6FBB0; SetGain / SetPosition inherited
// from CSalRenderDisable 0x00C6EC50 / 0x00C6EC80). Names: GameCODA 2.10 SDK CODAApiDbg.lib Debug/SalRenderDisableMMX.obj CodeView
// (CSalRenderDisableMMX : public CSalRenderDisable, no own members) + the retail tie of CSalSw3DVoice::CreateRenderer 0x00C55270 (NF441
// CLASS_TABLE / NAME_FIXES: the SDK Release CreateRenderer calls ??0CSalRenderDisableMMX where retail calls 0x00C55110, same branch, same order).
// The constructor is the inline empty one; CreateRenderer calls its out-of-line COMDAT copy 0x00C55110 (the SDK Release SalSw3DVoice.obj does the
// same: a COMDAT of that object, called, not expanded), an option S thin row of unit 0x00C55270. The destructor is implicit (retail 0x00C555D0
// is `jmp 0x00C6E780`; the deleting destructor 0x00C555B0 calls it).
#include "engine/CSalRenderDisable.h"
class CSalRenderDisableMMX:public CSalRenderDisable {
public:
 CSalRenderDisableMMX(){}                                                  // 0x00C55110 (out-of-line COMDAT copy)
 virtual void Process(const short*,int*(&)[4]);                           // slot 2, 0x00C6FE10
};
#endif
