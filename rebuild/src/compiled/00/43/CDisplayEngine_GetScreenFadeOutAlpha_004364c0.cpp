// CDisplayEngine::GetScreenFadeOutAlpha() const -- the screen fade's current alpha as a 0..255 byte
// Leaf: GetScreenFadeOutAlpha@CDisplayEngine_Methods
// Retail: Fable.exe @ 0x004364C0, 22 bytes: `add ecx,0BCh ; call 0x004348D0 ; fmul [255.0f] ; jmp __ftol2`. Catalog label
// "std::_Dest_val<...CMusicManager...>" is wrong. Ego_r.pdb ?GetScreenFadeOutAlpha@CDisplayEngine@@QBEEXZ (returns unsigned
// char); CDisplayEngine::ScreenFadeOutInfo is the CFadeInFadeOutBase at +0xbc; callee 0x004348D0 is
// CFadeInFadeOutBase::GetFadeInOutAlpha (as in 0x00434A00). Only caller CDrawScreenFade::Draw 0x00649715 stores al into
// the fade colour's alpha byte. 0x01230014 is 255.0f.
#include "engine/CDisplayEngine.h"
#include "engine/CFadeInFadeOutBase.h"

struct CFadeInFadeOutBase_Methods : CFadeInFadeOutBase {
    float GetFadeInOutAlpha() const;                     // 0x004348D0
};
struct CDisplayEngine_Methods : CDisplayEngine {
    unsigned char GetScreenFadeOutAlpha() const;
};

unsigned char CDisplayEngine_Methods::GetScreenFadeOutAlpha() const
{
    return (unsigned char)(((const CFadeInFadeOutBase_Methods*)ScreenFadeOutInfo)->GetFadeInOutAlpha() * 255.0f);
}