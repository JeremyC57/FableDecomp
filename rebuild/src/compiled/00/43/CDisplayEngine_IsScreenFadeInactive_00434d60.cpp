// CDisplayEngine::IsScreenFadeInactive() const -- true when the screen fade-out (the CFadeInFadeOutBase at +0xbc) is not active
// Retail: Fable.exe @ 0x00434D60, 12 bytes. Catalog label C3DAnimation2::CanRelocateData is wrong (Ego_r.pdb C3DAnimation2 is
// only 136 bytes). Evidence: both callers (CSubtitleRenderer::IsInInteractiveState 0x00490B9F, CWorld::
// EAOpenHeroInformationScreen 0x0062B0FC) call it on g_pMainGameComponent (0x013B86A0) +0x28, the PDB
// CMainGameComponent::PDisplayEngine; engine/CDisplayEngine.h has ScreenFadeOutInfo (CFadeInFadeOutBase) at +0xbc,
// whose first member is the bool Active. Same TU as 0x00434A00 / 0x004349F0.
// Shape: `Active ? false : true` gives `test al,al ; sete al` (`!Active` widens through a zeroed eax).
#include "engine/CDisplayEngine.h"
#include "engine/CFadeInFadeOutBase.h"

struct CDisplayEngine_Methods : CDisplayEngine {
    bool IsScreenFadeInactive() const;
};

bool CDisplayEngine_Methods::IsScreenFadeInactive() const
{
    return ((const CFadeInFadeOutBase*)ScreenFadeOutInfo)->Active ? false : true;
}