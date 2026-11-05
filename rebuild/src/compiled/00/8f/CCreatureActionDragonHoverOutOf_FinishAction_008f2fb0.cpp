// CCreatureAction_DragonHoverOutOf::FinishAction() -- put the dragon back at its position raised by a fixed height, remove its interface-0x4d component, then the base FinishAction
// Leaf: ?FinishAction@CCreatureAction_DragonHoverOutOf@@UAEXXZ
// Plan phase 3 (2026-11-04), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008F2FB0, length 99 bytes, plain `ret`.
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (x3), CThing::GetPos @ 0x004C73D0,
//          CTCPhysicsBase::ResetPosition(const C3DVector&) (virtual slot 31, `call [eax+7Ch]`, rtti-vtables slot 31 =
//          0x006B01D0 ResetPosition) on the thing's physics TC at +0x60, CThing::RemoveTC(long) @ 0x004C9840,
//          CCombatActionBase::FinishAction @ 0x00859A20.
// Shape: a frame C3DVector built field by field from the position reference (X/Y through integer registers, Z
//   = current Z + the float constant 0x01288430 (31.148378f) on the FPU, `fld [eax+8] ; fadd`; copy-then-`+=` goes
//   through the frame), then passed by reference to the physics TC of a named creature local (loaded into ecx);
//   PCreature's address is taken once (esi); the base FinishAction is a direct call.
// Layout: CCombatActionBase (retail 0x114 bytes) with PCreature at +8; CThing's PhysicsTC at +0x60 (landed
//   CThingPhysical 0x006A4D90 model).
#include "engine/C3DVector.h"

class CTCPhysicsBase { public:                     // the thing's physics TC; only slot 31 is modelled
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17();
    virtual void v18(); virtual void v19(); virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23();
    virtual void v24(); virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28(); virtual void v29();
    virtual void v30();
    virtual void ResetPosition(const C3DVector& position);   // slot 31 (+0x7c)
};

#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_STATEACCESS
#include "engine/CThing.h"

class CBaseObject;
class CBaseObjectPointer;
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#include "engine/CIntelligentPointer.h"


class CCombatActionBase { public:                  // retail sizeof 0x114
    virtual void v00();
    unsigned char _pad04[4];                // +0x04
    CIntelligentPointer<CThing> PCreature;      // +0x08 CIntelligentPointer<CThingCreatureBase>
    unsigned char _pad10[0x114 - 0x10];
    protected: virtual void FinishAction(); public:            // 0x00859A20
};

class CCreatureAction_DragonHoverOutOf : public CCombatActionBase { public:
    virtual void FinishAction();
};

void CCreatureAction_DragonHoverOutOf::FinishAction()
{
    const C3DVector& current = PCreature.Get()->GetPos();
    C3DVector position;
    position.X = current.X;
    position.Y = current.Y;
    position.Z = current.Z + 31.148378f;    // 0x01288430
    CThing* creature = PCreature.Get();   // named: the physics TC then loads straight into ecx
    creature->GetPhysicsTC()->ResetPosition(position);
    PCreature.Get()->RemoveTC((ETCInterfaceType)(0x4d));
    CCombatActionBase::FinishAction();
}