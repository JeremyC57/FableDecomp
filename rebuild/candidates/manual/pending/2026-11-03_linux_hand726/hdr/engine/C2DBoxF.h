#ifndef FABLE_ENGINE_C2DBOXF_H
#define FABLE_ENGINE_C2DBOXF_H
// Original public fields and user default constructor from Ego_r TPI.
class C2DBoxF { public: float TLX,TLY,BRX,BRY; C2DBoxF() {}
#ifdef FABLE_RETAIL_C2DBOXF
 // Retail opt-in (movie row 0x006286F0): FableWin.pdb ??0C2DBoxF@@QAE@MMMM@Z (debug only: inline in release; 0x006286F0
 // builds C2DBoxF(0, 0, VIRTUAL_SCREEN_WIDTH, VIRTUAL_SCREEN_HEIGHT) on the stack, four stores, no call).
 C2DBoxF(float tlx,float tly,float brx,float bry):TLX(tlx),TLY(tly),BRX(brx),BRY(bry){}
 // chunk 726: ?ContainsPoint@C2DBoxF@@QBE_NABVC2DVector@@@Z, out of line at 0x00535CD0 (landed); called by
 // NUISystem::CList::ComputeChildrenVisibility 0x005379F5.
 // (the elaborated "class C2DVector" declares ::C2DVector, so the default path gains no forward declaration).
 bool ContainsPoint(const class C2DVector& point) const;
 // chunk 726: the PDB accessors (FableWin ?GetTLX@C2DBoxF@@QBEMXZ .. ?GetHeight@C2DBoxF@@QBEMXZ), inline in retail:
 // NUISystem::CScrollingViewport::Update 0x0053F1C0 copies Window.GetTLX() into a frame word before the GetUIScale call
 // and computes GetWidth() / GetHeight() (fld BRX ; fsub TLX) ahead of it.
 float GetTLX() const { return TLX; }
 float GetTLY() const { return TLY; }
 float GetBRX() const { return BRX; }
 float GetBRY() const { return BRY; }
 float GetWidth() const { return BRX-TLX; }
 float GetHeight() const { return BRY-TLY; }
#endif
};
#endif
