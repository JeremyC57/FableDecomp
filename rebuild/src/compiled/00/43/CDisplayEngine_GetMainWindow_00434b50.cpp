// CDisplayEngine::GetMainWindow() const -- the main window box converted from virtual to actual screen coordinates, returned by value
// Leaf: ?GetMainWindow@CDisplayEngine@@QBE?AVC2DBoxF@@XZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00434B50, length 50 bytes, `ret 4` (__fastcall + hidden C2DBoxF result), returns it.
// Name: FableWin.pdb / Ego_r.pdb `?GetMainWindow@CDisplayEngine@@QBE?AVC2DBoxF@@XZ`; Ego_r CDisplayEngine has `C2DBoxF MainWindow`
// at +0x1C (the `add ecx,0x1c`); callers CDrawMiniMap::Draw 0x00648915 (on CMainGameComponent::PDisplayEngine +0x28), 0x006574D4,
// 0x0066FCAA; it sits in the CDisplayEngine TU (ConfirmFeedbackGameEventPackage 0x00434A60 before, StartLetterBoxMode 0x00434BA0
// after). The catalog's "GetDefaultVal<CEngineGraphic>" is a label.
// Callee: GFVBoxToABox(const C2DBoxF*, C2DBoxF*) @ 0x009E1E70 (__fastcall: the virtual box in ecx, the output in edx).
// Shape: /O2; `C2DBoxF box; GFVBoxToABox(&MainWindow, &box); return box;` -- the local at [esp], then the return copy into the hidden
// result through C2DBoxF's USER copy constructor (member init list): VC7.1 then moves the first float through the FPU (`fld [esp]
// ... fstp [eax]`) and the other three through integer registers. With the implicit (bitwise) copy all four are integer moves
// (graded DIFFER 52v50). The PDB name set confirms both a user copy constructor `??0C2DBoxF@@QAE@ABV0@@Z` and a default one.
// Layout: engine/CDisplayEngine.h flattens MainWindow into four floats (MainWindow_TLX..BRY), which cannot be passed as the C2DBoxF
// the converter takes nor carry the member function, so a local class with the same +0x1C member.
// Model: local -- engine/CDisplayEngine.h exists, but the generated header is data-only (pack(1), no member functions, no
// virtuals: the vptr and counted pointers are flattened into raw words); this body is a const member function of it, so the
// class is declared locally (MainWindow +0x1C, the header's offset).
class C2DBoxF {
public:
    C2DBoxF() {}
    C2DBoxF(const C2DBoxF& other) : TLX(other.TLX), TLY(other.TLY), BRX(other.BRX), BRY(other.BRY) {}
    float TLX;                                           // +0x00
    float TLY;                                           // +0x04
    float BRX;                                           // +0x08
    float BRY;                                           // +0x0c
};
void __fastcall GFVBoxToABox(const C2DBoxF* vbox, C2DBoxF* abox);   // 0x009E1E70

class CDisplayEngine {
public:
    unsigned char _pad00[0x1c];
    C2DBoxF       MainWindow;                            // +0x1C
    C2DBoxF GetMainWindow() const;
};

C2DBoxF CDisplayEngine::GetMainWindow() const
{
    C2DBoxF box;
    GFVBoxToABox(&MainWindow, &box);
    return box;
}