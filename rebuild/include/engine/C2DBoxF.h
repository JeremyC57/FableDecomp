#ifndef FABLE_ENGINE_C2DBOXF_H
#define FABLE_ENGINE_C2DBOXF_H
// Original public fields and user default constructor from Ego_r TPI.
class C2DBoxF { public: float TLX,TLY,BRX,BRY; C2DBoxF() {}
#ifdef FABLE_RETAIL_C2DBOXF
 // Retail opt-in (movie row 0x006286F0): FableWin.pdb ??0C2DBoxF@@QAE@MMMM@Z (debug only: inline in release; 0x006286F0
 // builds C2DBoxF(0, 0, VIRTUAL_SCREEN_WIDTH, VIRTUAL_SCREEN_HEIGHT) on the stack, four stores, no call).
 C2DBoxF(float tlx,float tly,float brx,float bry):TLX(tlx),TLY(tly),BRX(brx),BRY(bry){}
#endif
};
#endif
