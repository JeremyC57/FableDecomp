// Plan phase 4 (2026-11-05), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
class CThing;

// NF47 (round 7): 0x013B86A0 is ?PMainGameComponent@CMainGameComponent@@1PAV1@A (protected static, read through the inline
//   CMainGameComponent::Get(), engine/CMainGameComponent.h), its world (PWorld +0x24) through the inline PeekWorld() (FableWin.pdb ?PeekWorld@CMainGameComponent@@QBEABVCWorld@@XZ), handed to 0x007EA2F0 = Ego_r/FableWin-named NTargeting::GetTargetingPositionOnThing(const CWorld&, const CThing&, float) (engine/NTargeting.h, the spelling the landed 0x007F5950 binds; was the stand-in Inner(int, CThing*, float)); was the stand-in CGlob* g_glob.
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_CMAINGAMECOMPONENT_WORLD
#include "engine/CMainGameComponent.h"

#include "engine/C3DVector.h"

#include "engine/NTargeting.h"

C3DVector __fastcall GetCreatureFireAtPos(CThing* thing)
{
    return NTargeting::GetTargetingPositionOnThing(CMainGameComponent::Get()->PeekWorld(), *thing, 1.0f);
}