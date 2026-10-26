// NGraphicAppearance::CTCGraphicAppearance::Draw(const CEnginePrimitivePrepareInfo& info) -- draw at the owner's render interpolators: build the position (render-height adjusted) and orientation interpolators of the parent thing and run the three-argument Draw
// Leaf: ?Draw@CTCGraphicAppearance@NGraphicAppearance@@QAEXABVCEnginePrimitivePrepareInfo@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004BE4E0, length 163 bytes, `ret 4` (thiscall: info).
// Ego_r.pdb: `?Draw@CTCGraphicAppearance@NGraphicAppearance@@QAEXABVCEnginePrimitivePrepareInfo@@@Z`.
// Callees: the file-static __fastcall GetRenderPositionInterpolateForThing(const CThing&) @ 0x004BC0E0 (landed),
// CBase::CBase @ 0x0099A2D0 / CBase::~CBase @ 0x0099A2E0, CTCPhysicsBase::GetOrientationInterpolator(C3DOrientationInterpolator&)
// const @ 0x006B0460, CTCGraphicAppearance::SetPrimitive(info, position, orientation) @ 0x004BD6E0. Globals: the latch byte
// 0x013B8A14 and the prepare-enabled byte 0x01375480; C3DOrientationInterpolator vtable 0x01239824.
// Label: the catalog "CTCGraphicAppearance_PrepareForDraw" is behavioural. The body inlines the three-argument
// `Draw(info, position, orientation)` whose out-of-line copy is the landed 0x004BE490 (landed under the descriptive name
// "PrepareForRenderIfNeeded"; Ego_r.pdb has no such member, the overload is
// `?Draw@CTCGraphicAppearance@NGraphicAppearance@@QAEXABVCEnginePrimitivePrepareInfo@@ABVC3DPositionInterpolator@@ABVC3DOrientationInterpolator@@@Z`).
// Layout: the landed 0x004BE490 model (retail CTCGraphicAppearance 0x5c bytes): ParentThing +0x04, UpdatePrimitiveReasons +0x58,
// the flag bits +0x59; retail CThing PhysicsTC +0x60.
// Shape: the physics component is read into a named local before the position call (edi); the position interpolator is
// returned by hidden pointer straight into its frame slot; the orientation interpolator is default-constructed (CBase ctor,
// vtable store sunk below the argument push); both destroyed through the implicit destructors (~CBase only).
class C3DVector {
public:
    float X, Y, Z;
};
class CRightHandedSet { public:
    C3DVector Up;
    C3DVector Forward;
};

namespace N_0099a2d0 { struct CBase {
    virtual ~CBase();                                       // 0x0099A2E0
    CBase();                                                // 0x0099A2D0
}; } using N_0099a2d0::CBase;

class C3DPositionInterpolator : public CBase { public:                    // vtable 0x0123981C (Ego_r.pdb 0x1c)
    C3DVector StartPosition;                                // +0x04
    C3DVector EndPosition;                                  // +0x10
};
class C3DOrientationInterpolator : public CBase { public:                 // vtable 0x01239824 (Ego_r.pdb 0x34)
    CRightHandedSet StartOrientation;                       // +0x04
    CRightHandedSet EndOrientation;                         // +0x1c
};

class CTCPhysicsBase { public:
    void GetOrientationInterpolator(C3DOrientationInterpolator& out) const;   // 0x006B0460
};

class CThing {
public:
    unsigned char   _pad00[0x60];
    CTCPhysicsBase* PhysicsTC;                              // +0x60
};

C3DPositionInterpolator __fastcall GetRenderPositionInterpolateForThing(const CThing& thing);   // 0x004BC0E0 (file-static)

class CEnginePrimitivePrepareInfo;

extern bool g_LatchForceResendPrimitives;                   // 0x013B8A14
extern bool g_PreparePrimitivesEnabled;                     // 0x01375480

namespace NGraphicAppearance {

class CTCGraphicAppearance { public:
    void*          __vftable;                               // +0x00 (CTCBase)
    CThing*        ParentThing;                             // +0x04
    unsigned char  _pad08[0x58 - 0x08];
    unsigned char  UpdatePrimitiveReasons;                  // +0x58
    unsigned char  SelfIlluminating : 1;                    // +0x59 bit 0
    unsigned char  ForceResendEveryRenderFrame : 1;         //        bit 1
    unsigned char  ResendPrimsEveryRenderFrame : 1;         //        bit 2
    unsigned char  DisableOcclusionTesting : 1;             //        bit 3
    unsigned char  DrawAfterWater : 1;                      //        bit 4
    unsigned char  CachedStaticFocalPos : 1;                //        bit 5
    unsigned char  RenderedLastFrame : 1;                   //        bit 6
    unsigned char  PrimitivesDrawn : 1;                     //        bit 7
    unsigned long GetPrimitiveUpdateReasons() const         // inline: a resend-every-frame appearance always has a reason
    {
        if (!UpdatePrimitiveReasons && ResendPrimsEveryRenderFrame)
            return 1;
        return UpdatePrimitiveReasons;
    }
    void SetPrimitive(const CEnginePrimitivePrepareInfo& info, const C3DPositionInterpolator& position,
                      const C3DOrientationInterpolator& orientation);           // 0x004BD6E0
    __forceinline void Draw(const CEnginePrimitivePrepareInfo& info, const C3DPositionInterpolator& position,
                            const C3DOrientationInterpolator& orientation)      // out of line: 0x004BE490
    {
        PrimitivesDrawn = 1;
        if (g_LatchForceResendPrimitives)
            ResendPrimsEveryRenderFrame = ForceResendEveryRenderFrame;
        if (g_PreparePrimitivesEnabled && GetPrimitiveUpdateReasons())
            SetPrimitive(info, position, orientation);
    }
    void Draw(const CEnginePrimitivePrepareInfo& info);
};

void CTCGraphicAppearance::Draw(const CEnginePrimitivePrepareInfo& info)
{
    const CTCPhysicsBase* physics = ParentThing->PhysicsTC;
    C3DPositionInterpolator position = GetRenderPositionInterpolateForThing(*ParentThing);
    C3DOrientationInterpolator orientation;
    physics->GetOrientationInterpolator(orientation);
    Draw(info, position, orientation);
}

}