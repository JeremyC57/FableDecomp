// NCameraModes::CBase::UpdateCameraStretchAroundHero(C3DVector& position, C3DVector& lookAt) const
// Grade: RELOCATION_MATCH (542 bytes). No-code steering constructs in the final block set VC7.1's 81-tuple scheduling region cut:
//   the parentheses ((d * d), exp(...), (curR / nomR), (height / nomH), (lerp * dz)) AND the `(double)` cast on the exp argument
//   (exp's intrinsic takes a double anyway, so the cast emits no code). Together they give retail's order: the d pop (fstp st) after
//   the blend store, and the lookAt Y/Z copy `mov eax,[38] ; mov ecx,[3c] ; mov [edi+4],eax ; mov [edi+8],ecx` after the last
//   `fstp [3c]`. Without the cast no paren combination of a 256-way sweep matched (NOTES attempt 2).
// Leaf: ?UpdateCameraStretchAroundHero@CBase@NCameraModes@@IBEXAAVC3DVector@@0@Z
// When the mode stretches around the hero (vslot 8 IsStretchAroundHero) and the observed thing is the main player's creature,
//   pull the camera towards the hero: the horizontal offset from the hero is scaled by lerp(1, currentRadius/nominalRadius, blend)
//   and the height by lerp(1, GetObservedThingCurrentMeshHeight()/nominalHeight, blend), blend = exp(-0.01 * d^2) of the horizontal
//   distance beyond the current mesh radius (clamped at 0); the look-at point keeps its offset from the camera.
// Retail: Fable.exe (TLC, VC7.1) @ 0x0084D910, 542 bytes, __fastcall, `ret 8`.
// Callees: CBaseIntelligentPointer::GetPItem 0x00A01B50 (through the inline CBase/CGameCameraManager::GetObservedThing, PObservedThing +0x100),
//   CThing::PeekPlayer 0x004C7A10, CPlayer::IsMainPlayer 0x00487520, CThing::GetPos 0x004C73D0,
//   CThingCreatureBase::GetNominalMeshRadius 0x006625C0 / GetCurrentMeshRadius 0x00664BA0 / GetNominalMeshHeight 0x006625E0,
//   virtual IsStretchAroundHero (+0x20) and GetObservedThingCurrentMeshHeight (+0x3C). exp / sqrt are x87 intrinsics.
// Shape: the whole new position is ONE expression (delta * lerp + heroPos + C3DVector(0,0,height lerp * dz)) -- any named scale/height
//   local lets VC7.1 hoist the vector sum above the virtual call. delta is C3DVector(dx,dy,0) (Z folds to scale*0.0f), dz a separate float.
#include <math.h>
#pragma intrinsic(exp, sqrt)
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_PEEKPLAYER
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGCREATUREBASE_MESHSIZE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_GET_ORIGINAL_CHAIN
#define FABLE_RETAIL_PLAYER
#define FABLE_RETAIL_PLAYER_ISMAINPLAYER_OUTOFLINE
#define FABLE_RETAIL_CCOUNTEDPOINTER
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_NCAMERAMODES_CBASE_CYLINDER
#define FABLE_RETAIL_CGAMECAMERAMANAGER_LOOKAROUND
#define FABLE_RETAIL_CGAMECAMERAMANAGER_GETOBSERVEDTHING
#include "engine/C3DVector.h"
#include "engine/GFGetInterpolatedValue.h"
#include "engine/CThingCreatureBase.h"
#include "engine/CPlayer.h"
#include "engine/NCameraModes_CBase.h"
#include "engine/CGameCameraManager.h"

namespace NCameraModes {
// Same body as the landed out-of-line row 0x0084F1B0 (rebuild/src/compiled/00/84/NCameraModesCBase_GetObservedThing_0084f1b0.cpp:
//   `mov ecx,[ecx+40h] ; add ecx,100h ; jmp GetPItem`). Defined inline here because this row never calls 0x0084F1B0: retail
//   expands it in place as `lea ecx,[eax+100h] ; call 0x00A01B50` at 0x0084D92C, 0x0084D944, 0x0084D96B and 0x0084D99F.
inline const CThingGameObject* CBase::GetObservedThing() const
{
    return CameraManager.GetObservedThing();
}

void CBase::UpdateCameraStretchAroundHero(C3DVector& position, C3DVector& lookAt) const
{
    if (!IsStretchAroundHero()) return;
    if (GetObservedThing()->GetType() != 2) return;
    if (!GetObservedThing()->PeekPlayer().IsMainPlayer()) return;
    const CThingCreatureBase* hero = static_cast<const CThingCreatureBase*>(GetObservedThing());
    C3DVector offset = lookAt - position;
    C3DVector heroPos = GetObservedThing()->GetPos();
    C3DVector delta(position.X - heroPos.X, position.Y - heroPos.Y, 0.0f);
    float dz = position.Z - heroPos.Z;
    float nomR = hero->GetNominalMeshRadius();
    float curR = hero->GetCurrentMeshRadius();
    float d = (float)sqrt(delta.X * delta.X + delta.Y * delta.Y) - curR;
    if (d < 0.0f) d = 0.0f;
    float blend = (float)(exp((double)((d * d) * -0.01f)));
    float nomH = hero->GetNominalMeshHeight();
    position = delta * GFGetInterpolatedValue(1.0f, (curR / nomR), blend) + heroPos + C3DVector(0.0f, 0.0f, (GFGetInterpolatedValue(1.0f, (GetObservedThingCurrentMeshHeight() / nomH), blend) * dz));
    lookAt = offset + position;
}
}