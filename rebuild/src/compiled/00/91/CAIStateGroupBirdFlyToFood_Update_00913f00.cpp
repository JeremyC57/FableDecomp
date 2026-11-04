// CAIStateGroup_BirdFlyToFood::Update() -- stamp the frame the food was found, fly a random pretty path to the food position, then Terminate()
// Leaf: ?Update@CAIStateGroup_BirdFlyToFood@@UAEXXZ
// Plan phase 3 (2026-11-04), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00913F00, length 35 bytes, tail jump (virtual, no arguments).
// Identity: slot 12 of vtable 0x0128961C (RTTI CAIStateGroup_BirdFlyToFood : CAIStateGroup_FlyingThingBase : CAIStateGroupBase :
//   CBaseClass), the only reference; Ego_r + FableWin ?Update@CAIStateGroup_BirdFlyToFood@@UAEXXZ (public virtual), listed beside
//   IsRunnable (= the next retail body 0x00913F30, slot 11).
// Callees: CAIStateGroupBase::PeekWorld() const @ 0x008FCFA0 (both PDBs ?PeekWorld@CAIStateGroupBase@@IBEABVCWorld@@XZ), the static
//   CWorld::GetFrame() @ 0x0049D870 (ESTABLISHED ?GetFrame@CWorld@@SIJXZ), CAIStateGroup_FlyingThingBase::AIAction_RandomFlyToPos
//   (const C3DVector&) @ 0x009130D0 (Ego_r donor order ctor / GetRandomFlightPos / AddRandomBezierCurveToPath / CalculatePrettyPath /
//   AIAction_RandomFlyToPos = retail 0x009126C0 / 0x00912700 / 0x00912840 / 0x00912BA0 / 0x009130D0; the landed BirdStartled::Update
//   0x00914600 spells it so), CAIStateGroupBase::Terminate() @ 0x008FCF40 (tail jump).
// Layout: Ego_r CAIStateGroup_BirdFlyToFood (0x34 bytes, no containers): FoodPos C3DVector +0x24, FrameLastFoundFood +0x30.
// Shape: the static GetFrame is reached through the PeekWorld() object expression, which is still evaluated (the call stays,
//   its result is dropped); the action's EAIActionReturn is ignored.
#include "engine/CBaseClass.h"

#include "engine/C3DVector.h"

class CWorld { public:
    static long __fastcall GetFrame();                       // 0x0049D870
};

enum EAIActionReturn { };

class CAIStateGroupBase : public CBaseClass { public:        // retail 0x14 bytes
    void Terminate();                                        // 0x008FCF40
protected:
    const CWorld& PeekWorld() const;                         // 0x008FCFA0
    unsigned char _body[0x14 - 4];
};

class CAIStateGroup_FlyingThingBase : public CAIStateGroupBase { public:   // 0x24 bytes
    float MinHeightToKeepAboveGround;                        // +0x14
    float MinHeightToKeepAboveGroundInFlight;                // +0x18
    float MaxHeightAboveGround;                              // +0x1c
    float FlightRange;                                       // +0x20
protected:
    EAIActionReturn AIAction_RandomFlyToPos(const C3DVector& pos);   // 0x009130D0
};

class CAIStateGroup_BirdFlyToFood : public CAIStateGroup_FlyingThingBase { public:   // vtable 0x0128961C
    virtual void Update();
    C3DVector     FoodPos;                                   // +0x24
    unsigned long FrameLastFoundFood;                        // +0x30
};

void CAIStateGroup_BirdFlyToFood::Update()
{
    FrameLastFoundFood = PeekWorld().GetFrame();
    AIAction_RandomFlyToPos(FoodPos);
    Terminate();
}