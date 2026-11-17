// CCreatureActionBase::GetFrameUpdateTime() const -- the creature's animation speed multiplier times one frame (1 / frame rate)
// Leaf: ?GetFrameUpdateTime@CCreatureActionBase@@MBEMXZ
// Naming wave 18: the global stand-in g_frameRate (0x01375550) is the protected static long CMainGameComponent::ConstantFPS, read through the
//   inline static accessor GetConstantFPS() as retail code outside the class does (name-only: the bytes are unchanged).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00692BC0, length 36 bytes. Vtable slot 62 of the CCreatureActionBase family
// (rtti-vtables.tsv lists it through CCreatureAction_StartSneaking, 0x01258DFC).
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (PCreature at +8), CThingCreatureBase::GetAnimationSpeedMultiplier
// @ 0x006655A0. Globals: the long frame rate @ 0x01375550 (`fild`), the float 1.0f @ 0x0122DED8 (`fdivr`: 1.0f / rate).
// Shape: the frame time is a named float local (`fstp [esp]` into the one-dword frame before the call -- a named float
// local for a value that must survive a call, GOTCHAS), then the call result is multiplied by it (`fmul [esp]`).
// NF59 (round 7, queue q10501): Ego_r.pdb 0x00519550 + FableWin.pdb 0x01DB94C0 ?GetFrameUpdateTime@CCreatureActionBase@@MBEMXZ, the protected
//   virtual (slot 62); re-authored on the retail engine/CCreatureActionBase.h (it declares `protected: virtual float GetFrameUpdateTime() const`,
//   PCreature +0x08; the include block of the landed PeekWorld 0x00692950) over the retail CThingCreatureBase (UPDATEANIMATION declares
//   GetAnimationSpeedMultiplier). The landed row defined a public non-virtual member (QBE) of a local CCreatureActionBase /
//   CBaseIntelligentPointer / CIntelligentPointer model.
class CMainGameComponent {
protected:
    static long ConstantFPS;                             // 0x01375550 ?ConstantFPS@CMainGameComponent@@1JA (protected static)
public:
    static long __fastcall GetConstantFPS() { return ConstantFPS; }   // Ego_r / FableWin ?GetConstantFPS@CMainGameComponent@@SIJXZ (inline)
};
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGCREATUREBASE_UPDATEANIMATION
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CCREATUREACTIONBASE
#define FABLE_RETAIL_CCREATUREACTIONBASE_PCREATURE
#include "engine/CThingCreatureBase.h"
#include "engine/CIntelligentPointer.h"
#include "engine/CCreatureActionBase.h"

float CCreatureActionBase::GetFrameUpdateTime() const
{
    float frameTime = 1.0f / (float)CMainGameComponent::GetConstantFPS();
    return static_cast<const CThingCreatureBase*>(PCreature.GetPItem())->GetAnimationSpeedMultiplier() * frameTime;
}
