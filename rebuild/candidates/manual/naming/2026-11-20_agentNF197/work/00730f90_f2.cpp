// CTCDoor::UpdateOpenStatusTriggerTypePerson() -- a person-triggered door is open while a creature stands within the trigger radius (and within one unit of height); it closes one second after the last creature was seen; open/close the door when the state changed
// Leaf: ?UpdateOpenStatusTriggerTypePerson@CTCDoor@@QAEXXZ
// Plan phase 3 (2026-11-04), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
// Naming wave 18: the global stand-in g_framesPerSecond (0x01375550) is the protected static long CMainGameComponent::ConstantFPS, read through the
//   inline static accessor GetConstantFPS() as retail code outside the class does (name-only: the bytes are unchanged).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00730F90, 292 bytes (+ int3 padding), plain `ret`.
// Callees: C2DBoxF::ToC2DBoxI @ 0x00A55A30 (manifest "CEditWorldMap::GetPenArea" is propagated), CTCBase::PeekThingSearchTools
//   @ 0x00686D90, CThingSearchTools::GetPFirstThingInRoughArea<CFilter_Bind2<CThingFilter_InArea, CThingFilter_IsWithinZRange>,
//   CThingCreatureBase> const @ 0x007313B0, CTCBase::PeekWorld const @ 0x00686A80, CWorld::GetFrame @ 0x0049D870 (static __fastcall),
//   CTCDoor::Open(bool) @ 0x00730C00, CTCDoor::Close @ 0x00730DD0.
// Globals: CMainGameComponent::GetConstantFPS() @ 0x01375550.
// Layout: engine/CTCDoor.h (retail offsets): CurrentOpen +0x10, OldOpen +0x11, LastCreatureFoundWF +0x14, PDef +0x20;
//   CDoorDef TriggerRadius +0x2c; the parent's physics TC +0x60 with its position at +0x0c.
// Shape: the 2D centre is a named vector and the box is built from its X/Y and the radius (keeps X and Y live on the
//   x87 stack: `fld st0 ; fadd st3`); C2DBox has a user copy ctor (minY copied at once, maxY stored from the FPU); both
//   filters are argument temporaries of the Bind2 filter; the out pointer lives in an inner block, so the else arm reuses
//   its slot for the elapsed-frames temp; the world pointer is read before the frames-per-second copy.
class CMainGameComponent {
protected:
    static long ConstantFPS;                             // 0x01375550 ?ConstantFPS@CMainGameComponent@@1JA (protected static)
public:
    static long __fastcall GetConstantFPS() { return ConstantFPS; }   // Ego_r / FableWin ?GetConstantFPS@CMainGameComponent@@SIJXZ (inline)
};
#include "engine/C3DVector.h"
// NF197 (2026-11-20): the local C2DVector / C2DBox stand-ins replaced by the shared engine/C2DVector.h (FROM3D) and
// engine/C2DBoxF.h (CENTRE: the inline FableWin ??0C2DBoxF@@QAE@ABVC2DVector@@M@Z and ?ToC2DBoxI@C2DBoxF@@QBE?AVC2DBoxI@@XZ at
// 0x00A55A30, Ego_r.pdb + FableWin.pdb; COPY: the authored member-wise copy constructor).
#define FABLE_RETAIL_C2DVECTOR
#define FABLE_RETAIL_C2DVECTOR_FROM3D
#define FABLE_RETAIL_C2DBOXF
#define FABLE_RETAIL_C2DBOXF_CENTRE
#define FABLE_RETAIL_C2DBOXF_COPY
#include "engine/C2DVector.h"
#include "engine/C2DBoxF.h"
class C2DBoxI { public: long minX, minY, maxX, maxY; };
class CThingFilter_InArea { public:
    C2DBoxF Area;
    CThingFilter_InArea(const C2DBoxF& area) : Area(area) { }
};
class CThingFilter_IsWithinZRange { public:
    float Z;
    float Range;
    CThingFilter_IsWithinZRange(float z, float range) : Z(z), Range(range) { }
};
template <class TFilter1, class TFilter2> class CFilter_Bind2 { public:
    const TFilter1* First;
    const TFilter2* Second;
    CFilter_Bind2(const TFilter1& a, const TFilter2& b) : First(&a), Second(&b) { }
};
class CThingCreatureBase;
class CThingSearchTools { public:
    template <class TFilter, class TThing>
    bool GetPFirstThingInRoughArea(const C2DBoxI& box, const TFilter& filter, TThing** out) const;   // 0x007313B0 (Ego_r.pdb instance)
};
class CTCPhysicsBase { public:
    unsigned char _pad00[0x0c];
    C3DVector Position;                         // +0x0c
};
#include "engine/CThing.h"
class CWorld { public:
    static long __fastcall GetFrame();          // 0x0049D870 ?GetFrame@CWorld@@SIJXZ
};
class CDoorDef { public:
    unsigned char _pad00[0x2c];
    float TriggerRadius;                        // +0x2c
};
class CTCBase { public:
    virtual ~CTCBase();                                           // slot 0 (0x00686830)
protected:
    const CWorld& PeekWorld() const;   // 0x00686A80 (Ego_r.pdb CTCBase, protected)
    const CThingSearchTools& PeekThingSearchTools() const;   // 0x00686D90 (Ego_r.pdb CTCBase, protected)
};
class CTCDoor : public CTCBase { public:
    CThing* ParentThing;                        // +0x04 (CTCBase)
    unsigned char _pad08[0x10 - 0x08];
    bool CurrentOpen;                           // +0x10
    bool OldOpen;                               // +0x11
    bool Opening;                               // +0x12
    bool Closing;                               // +0x13
    long LastCreatureFoundWF;                   // +0x14
    long UpdateID;                              // +0x18
    long TriggerType;                           // +0x1c
    const CDoorDef* PDef;                       // +0x20
    void Open(bool instant);                    // 0x00730C00
    void Close();                               // 0x00730DD0
    void UpdateOpenStatusTriggerTypePerson();
};

void CTCDoor::UpdateOpenStatusTriggerTypePerson()
{
    const C3DVector& pos = ParentThing->GetPhysicsTC()->Position;

    C2DBoxF box(C2DVector(pos), PDef->TriggerRadius);
    CThingCreatureBase* found;
    {
        CThingCreatureBase* creature = 0;
        PeekThingSearchTools().GetPFirstThingInRoughArea(box.ToC2DBoxI(), CFilter_Bind2<CThingFilter_InArea, CThingFilter_IsWithinZRange>(CThingFilter_InArea(box), CThingFilter_IsWithinZRange(pos.Z, 1.0f)), &creature);
        found = creature;
    }
    if (found) {
        CurrentOpen = true;
        LastCreatureFoundWF = PeekWorld().GetFrame();
    } else {
        const CWorld* world = &PeekWorld();
        long framesPerSecond = CMainGameComponent::GetConstantFPS();
        long elapsed = world->GetFrame() - LastCreatureFoundWF;
        if ((float)elapsed > (float)framesPerSecond)
            CurrentOpen = false;
    }
    if (CurrentOpen != OldOpen) {
        if (CurrentOpen)
            Open(true);
        else
            Close();
        OldOpen = CurrentOpen;
    }
}