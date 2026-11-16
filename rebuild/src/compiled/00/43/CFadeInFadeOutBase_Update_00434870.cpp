// CFadeInFadeOutBase::Update(float timeDelta) -- advance the fade: opening counts OpenTimer up to FadeInTime, closing counts CloseTimer down to zero and then deactivates
// Leaf: ?Update@CFadeInFadeOutBase@@QAEXM@Z (Ego_r.pdb; declared in engine/CFadeInFadeOutBase.h under FABLE_RETAIL_CFADEINFADEOUTBASE_DECLS)
// Retail: Fable.exe @ 0x00434870, 95 bytes, thiscall `ret 4`; 0.0f is the float at 0x0122DEDC; edx = 0 serves false and 0.0f.
// Body: Opening: OpenTimer += dt, Opening = false once OpenTimer >= FadeInTime. Else Closing: CloseTimer -= dt; when
// CloseTimer <= 0 clamp a negative timer to 0 and clear Active, Closing, Opening.
// Layout: engine/CFadeInFadeOutBase.h (Ego_r layout, 0x1c bytes).
// Names: Ego_r.pdb member names.
// Shape: else-if chain; the clamp nested inside the <= 0 test (fcom/fst then fcomp).
// Opt: /O2.
#define FABLE_RETAIL_CFADEINFADEOUTBASE_DECLS
#include "engine/CRGBColour.h"
#include "engine/CFadeInFadeOutBase.h"
void CFadeInFadeOutBase::Update(float timeDelta)
{
    if (Opening) {
        OpenTimer += timeDelta;
        if (OpenTimer >= FadeInTime) Opening = false;
    } else if (Closing) {
        CloseTimer -= timeDelta;
        if (CloseTimer <= 0.0f) {
            if (CloseTimer < 0.0f) CloseTimer = 0.0f;
            Active = false;
            Closing = false;
            Opening = false;
        }
    }
}