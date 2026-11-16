// Leaf: ??0CTCPhysicsBase@@QAE@AAVCThing@@@Z
// Ported 2026-11-16 onto the shared engine/CTCBase.h: the local base model CTCBase (which bound ??0CTCBase@@QAE@PAVCThing@@@Z to 0x00686800) is gone;
//   the base call is ??0CTCBase@@QAE@AAVCThing@@@Z and this constructor takes CThing& as the PDBs spell it (??0CTCPhysicsBase@@QAE@AAVCThing@@@Z).
// CTCPhysicsBase::CTCPhysicsBase(CThing&) -- base ctor, then zero position/old position/velocity, Radius = -1, null def pointer, clear the two moving bits
// Plan phase 3 (2026-11-04), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
#define FABLE_RETAIL_CTCBASE
#include "engine/CTCBase.h"                     // CTCBase(CThing&) 0x00686800 ??0CTCBase@@QAE@AAVCThing@@@Z
class CThing;
class CDefPointeeBase;
#include "engine/C3DVector.h"
template <class T> class CDefPointer { public:
    T* Object;
    CDefPointer() : Object(0) { }
};
class CPhysicsDef;
class CTCPhysicsBase : public CTCBase { public:
    C3DVector                       Position;                  // +0x0c
    C3DVector                       OldPosition;               // +0x18
    long                            OldPositionLastFrameSet;   // +0x24
    C3DVector                       Velocity;                  // +0x28
    float                           Radius;                    // +0x34
    CDefPointer<const CPhysicsDef>  PhysicsDef;                // +0x38
    bool                            Moving : 1;                // +0x3c bit 0
    bool                            OldMoving : 1;             // +0x3c bit 1
    CTCPhysicsBase(CThing& thing);
    virtual ~CTCPhysicsBase();
};

CTCPhysicsBase::CTCPhysicsBase(CThing& thing)
    : CTCBase(thing),
      Position(0.0f, 0.0f, 0.0f),
      OldPosition(0.0f, 0.0f, 0.0f),
      OldPositionLastFrameSet(0),
      Velocity(0.0f, 0.0f, 0.0f),
      Radius(-1.0f)
{
    Moving = false;
    OldMoving = false;
}
