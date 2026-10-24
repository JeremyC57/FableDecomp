// CDisplayEngine::GetLetterBoxAlpha() const -- forwards to the letter-box fade's CFadeInFadeOutBase::GetFadeInOutAlpha (the CLetterBoxModeInfo member at +0x70)
// Leaf: CEntry>_const&>
// Retail: Fable.exe @ 0x00434A00, 9 bytes: `add ecx,70h ; call 0x004348D0 ; ret` (catalog label "_Cons_val<...>" is wrong).
// Evidence: the only caller, CDrawLetterBoxBlackBars::Draw (0x00644677), calls the sibling 0x004349F0
// (`fld [ecx+8Ch]`, the letter box Ratio) and this function on the same CDisplayEngine; 0x004349D0 (EndLetterBox)
// tests the same +0x70 Active byte. PDB: CDisplayEngine::LetterBox is a CLetterBoxModeInfo (32 bytes) at +0x70,
// a CFadeInFadeOutBase (28 bytes) followed by float Ratio. Callee 0x004348D0 CFadeInFadeOutBase::GetFadeInOutAlpha.
// Shape: the result goes through a named float local: VC7.1 then emits `call ; ret` (returning the call
// expression directly gives a tail `jmp`).
#include "engine/CDisplayEngine.h"
#include "engine/CFadeInFadeOutBase.h"

struct CFadeInFadeOutBase_Methods : CFadeInFadeOutBase {
    float GetFadeInOutAlpha() const;                     // 0x004348D0
};
struct CLetterBoxModeInfo : CFadeInFadeOutBase_Methods {
    float Ratio;                                         // +0x1c
};
struct CDisplayEngine_Methods : CDisplayEngine {
    float GetLetterBoxAlpha() const;
};

float CDisplayEngine_Methods::GetLetterBoxAlpha() const
{
    float alpha = ((const CLetterBoxModeInfo*)LetterBox)->GetFadeInOutAlpha();
    return alpha;
}