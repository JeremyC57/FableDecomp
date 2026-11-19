// CWorld::ConsoleSetFreeCameraLookVector(CWorld& owner, const std::vector<CGenericVar>& params) -- console command: point the main player's free camera along (params[0], params[1], params[2]), with the up vector the normalised cross product of the horizontal side vector (look.Y, -look.X, 0) -- (1, 0, 0) when that is shorter than DELTA_NOTIONAL_ZERO -- and the look vector
// Leaf: ?ConsoleSetFreeCameraLookVector@CWorld@@KIXAAV1@ABV?$vector@VCGenericVar@@V?$allocator@VCGenericVar@@@std@@@std@@@Z
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x0049ED80, 310 bytes, plain `ret` (protected static __fastcall: owner in ecx, params in edx).
// Shape: FableWin.pdb (FableWin.exe 0x01B527F0, world.cpp) and its locals owner, params, camera, side_vector, look_vect, up_vect:
//   camera = owner.PlayerManager->GetMainPlayer().GetCurrentFreeCamera(); look_vect from the three GetFloat()s; C3DVector side_vector(
//   look_vect.Y, -look_vect.X, 0.0f); `if (side_vector.GetMagnitude() < DELTA_NOTIONAL_ZERO) side_vector.Initialise(1.0f, 0.0f, 0.0f);`
//   up_vect = side_vector ^ look_vect; up_vect.Normalise(); camera.SetView(look_vect, up_vect). Retail expands every call but
//   GetMainPlayer / SetView (GetMagnitude's root is fsqrt, the constant Z*Z folds). FABLE_RETAIL_C3DVECTOR_ORIGINAL_GROUPED_CROSS_PASS22
//   (operator^ with the donor's parameter name `vector`, each component grouped) gives retail's x87 order in the cross product
//   (0x0049EE0E `fxch st(1) ; fmulp st(2)` ... `fstp st(0)`); the ungrouped / X / XY / named-result texts do not.
// New header opt-ins: FABLE_RETAIL_CWORLD_CONSOLESETFREECAMERALOOKVECTOR (the protected static declaration),
//   FABLE_RETAIL_CPLAYER_GETCURRENTFREECAMERA (FableWin's inline accessor). CGenericVar has no engine header: the Ego_r.pdb 28-byte
//   layout with FableWin's ?GetFloat@CGenericVar@@QBEMXZ, inline in retail (`fld [eax+34h]` = params[1].ValFloat).
// Callees: CPlayerManager::GetMainPlayer 0x00449970, CCamera::SetView 0x00A0BE30. Data: DELTA_NOTIONAL_ZERO 0x0129BA3C, the
//   GFFastOneOverSqrt mantissa table 0x013CE558, pool floats 1.0f 0x0122DED8 / 0.0f 0x0122DEDC / 3.0f 0x0122DED4 / 0.5f 0x0122F59C.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_PLAYER
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CCAMERA
#define FABLE_RETAIL_CCAMERA_SETVIEW
#define FABLE_RETAIL_CPLAYER_GETCURRENTFREECAMERA
#define FABLE_RETAIL_CWORLD_CONSOLESETFREECAMERALOOKVECTOR
#define FABLE_RETAIL_C2DVECTOR
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_C3DVECTOR_ORIGINAL_GROUPED_CROSS_PASS22
#include "fable_string.h"
#include "stlport/fable_containers.h"
#include "engine/C2DVector.h"
#include "engine/C3DVector.h"
#include "engine/CCamera.h"
#include "engine/CPlayer.h"
#include "engine/CPlayerManager.h"
#include "engine/CWorld.h"
enum EGenericVarType { };
class CGenericVar {                                      // Ego_r.pdb, 28 bytes (no engine header)
public:
    float GetFloat() const { return ValFloat; }          // FableWin ?GetFloat@CGenericVar@@QBEMXZ, inline in retail
private:
    int             _padding_;                           // +0x00
    EGenericVarType Type;                                // +0x04
    CCharString     ValString;                           // +0x08
    long            ValSLONG;                            // +0x0C
    unsigned long   ValULONG;                            // +0x10
    bool            ValBool;                             // +0x14
    float           ValFloat;                            // +0x18
};
void __fastcall CWorld::ConsoleSetFreeCameraLookVector(CWorld& owner, const std::vector<CGenericVar>& params)
{
    CCamera& camera = owner.PlayerManager->GetMainPlayer().GetCurrentFreeCamera();
    C3DVector look_vect(params[0].GetFloat(), params[1].GetFloat(), params[2].GetFloat());
    C3DVector side_vector(look_vect.Y, -look_vect.X, 0.0f);
    if (side_vector.GetMagnitude() < DELTA_NOTIONAL_ZERO)
        side_vector.Initialise(1.0f, 0.0f, 0.0f);
    C3DVector up_vect = side_vector ^ look_vect;
    up_vect.Normalise();
    camera.SetView(look_vect, up_vect);
}
