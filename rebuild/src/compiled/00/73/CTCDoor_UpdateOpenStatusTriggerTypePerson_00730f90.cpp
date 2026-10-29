// CTCDoor::UpdateOpenStatusTriggerTypePerson() -- a person-triggered door is open while a creature stands within the trigger radius (and within one unit of height); it closes one second after the last creature was seen; open/close the door when the state changed
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00730F90, 292 bytes (+ int3 padding), plain `ret`.
// Callees: C2DBox::ToInteger @ 0x00A55A30 (manifest "CEditWorldMap::GetPenArea" is propagated), CTCBase::PeekThingSearchTools
//   @ 0x00686D90, CThingSearchTools::GetPFirstThingInRoughArea<CFilter_Bind2<CThingFilter_InArea, CThingFilter_IsWithinZRange>,
//   CThingCreatureBase> const @ 0x007313B0, CTCBase::PeekWorld const @ 0x00686A80, CWorld::GetFrame @ 0x0049D870 (static __fastcall),
//   CTCDoor::Open(bool) @ 0x00730C00, CTCDoor::Close @ 0x00730DD0.
// Globals: g_framesPerSecond @ 0x01375550.
// Layout: engine/CTCDoor.h (retail offsets): CurrentOpen +0x10, OldOpen +0x11, LastCreatureFoundWF +0x14, PDef +0x20;
//   CDoorDef TriggerRadius +0x2c; the parent's physics TC +0x60 with its position at +0x0c.
// Shape: the 2D centre is a named vector and the box is built from its X/Y and the radius (keeps X and Y live on the
//   x87 stack: `fld st0 ; fadd st3`); C2DBox has a user copy ctor (minY copied at once, maxY stored from the FPU); both
//   filters are argument temporaries of the Bind2 filter; the out pointer lives in an inner block, so the else arm reuses
//   its slot for the elapsed-frames temp; the world pointer is read before the frames-per-second copy.
extern long g_framesPerSecond;                  // 0x01375550
class C3DVector { public: float X, Y, Z; };
class C2DVector { public:
    float X, Y;
    C2DVector(const C3DVector& p) : X(p.X), Y(p.Y) { }
    C2DVector(const C2DVector& v) : X(v.X), Y(v.Y) { }
};
class C2DBoxI { public: long minX, minY, maxX, maxY; };
struct C2DBox {
    float minX, minY, maxX, maxY;
    C2DBox(float x, float y, float radius) { minX = x - radius; minY = y - radius; maxX = x + radius; maxY = y + radius; }
    C2DBox(const C2DBox& b) : minX(b.minX), minY(b.minY), maxX(b.maxX), maxY(b.maxY) { }
    C2DBoxI ToInteger() const;                  // 0x00A55A30
};
class CThingFilter_InArea { public:
    C2DBox Area;
    CThingFilter_InArea(const C2DBox& area) : Area(area) { }
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
class CThing { public:
    unsigned char _pad00[0x60];
    CTCPhysicsBase* PhysicsTC;                  // +0x60
};
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
    const C3DVector& pos = ParentThing->PhysicsTC->Position;
    C2DVector centre(pos);
    C2DBox box(centre.X, centre.Y, PDef->TriggerRadius);
    CThingCreatureBase* found;
    {
        CThingCreatureBase* creature = 0;
        PeekThingSearchTools().GetPFirstThingInRoughArea(box.ToInteger(), CFilter_Bind2<CThingFilter_InArea, CThingFilter_IsWithinZRange>(CThingFilter_InArea(box), CThingFilter_IsWithinZRange(pos.Z, 1.0f)), &creature);
        found = creature;
    }
    if (found) {
        CurrentOpen = true;
        LastCreatureFoundWF = PeekWorld().GetFrame();
    } else {
        const CWorld* world = &PeekWorld();
        long framesPerSecond = g_framesPerSecond;
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