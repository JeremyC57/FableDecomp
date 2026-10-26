// GetRenderPositionInterpolateForThing(const CThing&) -- the thing's physics position interpolator, with both end heights raised by the physics height offset for type-2 things
// Leaf: GetRenderPositionInterpolateForThing
//
// Retail 0x004BC0E0, 157 bytes (`__fastcall`: hidden result pointer in ecx, the thing in edx; plain
// `ret`). Ego_r.pdb `?GetRenderPositionInterpolateForThing@?A0xc76c19b4@@YI?AVC3DPositionInterpolator@@
// ABVCThing@@@Z` -- a file-static helper of the graphic-appearance TU (the catalog "CTCGraphicAppearance::
// Draw" is the neighbouring caller's name). It is declared at global scope here so the unreferenced
// helper is still emitted. Callees: CBase::CBase @ 0x0099A2D0, CTCPhysicsBase::GetPositionInterpolator
// (C3DPositionInterpolator&) const @ 0x006B0250, the physics component's virtual slot 79 (+0x13c, a
// float height offset at a parameter), CBase::~CBase @ 0x0099A2E0. Vtable 0x0123981C =
// C3DPositionInterpolator.
// Layout: C3DPositionInterpolator (Ego_r.pdb, 0x1c): StartPosition +0x4, EndPosition +0x10; retail CThing
// keeps its physics component at +0x60 and a type byte at +0x10.
// Shape: the local interpolator is default-constructed (CBase ctor + vtable store), filled out of line,
// its two Z members raised (`fadd ; fstp` into the frame), then copied into the result with the
// implicit copy constructor over C3DVector's memberwise user copy constructor (vtable store + six
// dword moves) and destroyed through ~CBase.
class C3DVector { public:
    float X, Y, Z;
    C3DVector() { }
    C3DVector(const C3DVector& v) : X(v.X), Y(v.Y), Z(v.Z) { }   // the engine's memberwise copy constructor
};

namespace N_0099a2d0 { struct CBase {
    virtual ~CBase();                                       // 0x0099A2E0
    CBase();                                                // 0x0099A2D0
}; } using N_0099a2d0::CBase;

class C3DPositionInterpolator : public CBase { public:                    // vtable 0x0123981C
    C3DVector StartPosition;                                // +0x04
    C3DVector EndPosition;                                  // +0x10
};

class CTCPhysicsBase { public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03();
    virtual void v04(); virtual void v05(); virtual void v06(); virtual void v07();
    virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15();
    virtual void v16(); virtual void v17(); virtual void v18(); virtual void v19();
    virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23();
    virtual void v24(); virtual void v25(); virtual void v26(); virtual void v27();
    virtual void v28(); virtual void v29(); virtual void v30(); virtual void v31();
    virtual void v32(); virtual void v33(); virtual void v34(); virtual void v35();
    virtual void v36(); virtual void v37(); virtual void v38(); virtual void v39();
    virtual void v40(); virtual void v41(); virtual void v42(); virtual void v43();
    virtual void v44(); virtual void v45(); virtual void v46(); virtual void v47();
    virtual void v48(); virtual void v49(); virtual void v50(); virtual void v51();
    virtual void v52(); virtual void v53(); virtual void v54(); virtual void v55();
    virtual void v56(); virtual void v57(); virtual void v58(); virtual void v59();
    virtual void v60(); virtual void v61(); virtual void v62(); virtual void v63();
    virtual void v64(); virtual void v65(); virtual void v66(); virtual void v67();
    virtual void v68(); virtual void v69(); virtual void v70(); virtual void v71();
    virtual void v72(); virtual void v73(); virtual void v74(); virtual void v75();
    virtual void v76(); virtual void v77(); virtual void v78();
    virtual float GetHeightOffset(float t);                 // slot 79 (+0x13c)
    void GetPositionInterpolator(C3DPositionInterpolator& out) const;   // 0x006B0250
};

class CThing { public:
    unsigned char   _pad00[0x10];
    unsigned char   Type;                                   // +0x10
    unsigned char   _pad11[0x60 - 0x11];
    CTCPhysicsBase* PPhysics;                               // +0x60
};

C3DPositionInterpolator __fastcall GetRenderPositionInterpolateForThing(const CThing& thing)
{
    CTCPhysicsBase* physics = thing.PPhysics;
    C3DPositionInterpolator interpolator;
    physics->GetPositionInterpolator(interpolator);
    if (thing.Type == 2)
    {
        interpolator.StartPosition.Z += physics->GetHeightOffset(0.0f);
        interpolator.EndPosition.Z += physics->GetHeightOffset(1.0f);
    }
    return interpolator;
}