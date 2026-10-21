// Grade: RELOCATION_MATCH (Fable landing 2026-10-19 after the Codex review hold: test exercises a hand-written stand-in, not the candidate)
// CEnginePrimitiveAnimation::CEnginePrimitiveAnimation(const C3DAnimationInterpolator&, const C3DPositionInterpolator&,
//   const C3DOrientationInterpolator&) -- copy-construct the three interpolator members (the blend states' counted
//   shared data gains a reference each)
// Leaf: ??0CEnginePrimitiveAnimation
// Retail: Fable.exe (TLC, VC7.1) @ 0x004C3D30, 185 bytes; called from CTCGraphicAppearance::RefreshPrimitiveAnimationAndPos
//   (landed 0x004BCFE0). The four immediates are the implicit copy constructors' vtable stores:
//   0x01239840 C3DAnimationInterpolator, 0x01239838 C3DAnimationBlendState, 0x0123981C C3DPositionInterpolator,
//   0x01239824 C3DOrientationInterpolator (rtti-vtables.tsv; slot 0 of each is the deleting destructor).
// Layout: PDB (Ego_r) CEnginePrimitiveAnimation (0x64): Anim +0x00, WorldPos +0x14, Orientation +0x30; member types and
//   names from struct_layouts_egor.tsv. CIVCountedPointer's target keeps its reference count at +0x04.
//   Local model: no engine/ header exists for these classes.
struct C3DVector {                                      // the engine vector has a user-written member-wise copy
    float X, Y, Z;
    C3DVector(const C3DVector& v) : X(v.X), Y(v.Y), Z(v.Z) {}
};
struct CRightHandedSet { C3DVector Up; C3DVector Forward; };   // 24 bytes
struct CCountedSharedData { long _header; long RefCount; };
template<class T> struct CIVCountedPointer {
    T* P;
    CIVCountedPointer(const CIVCountedPointer& o) : P(o.P) { if (P) ++P->RefCount; }
};
struct C3DAnimationBlendState {                         // 8 bytes
    struct CSharedData : CCountedSharedData {};
    virtual ~C3DAnimationBlendState();
    CIVCountedPointer<CSharedData> Data;                // +0x04
};
struct C3DAnimationInterpolator {                       // 20 bytes
    virtual ~C3DAnimationInterpolator();
    C3DAnimationBlendState StartState;                  // +0x04
    C3DAnimationBlendState EndState;                    // +0x0c
};
struct C3DPositionInterpolator {                        // 28 bytes
    virtual ~C3DPositionInterpolator();
    C3DVector StartPosition;                            // +0x04
    C3DVector EndPosition;                              // +0x10
};
struct C3DOrientationInterpolator {                     // 52 bytes
    virtual ~C3DOrientationInterpolator();
    CRightHandedSet StartOrientation;                   // +0x04
    CRightHandedSet EndOrientation;                     // +0x1c
};
struct CEnginePrimitiveAnimation {
    C3DAnimationInterpolator   Anim;                    // +0x00
    C3DPositionInterpolator    WorldPos;                // +0x14
    C3DOrientationInterpolator Orientation;             // +0x30
    CEnginePrimitiveAnimation(const C3DAnimationInterpolator& anim, const C3DPositionInterpolator& world_pos,
                              const C3DOrientationInterpolator& orientation);
};
CEnginePrimitiveAnimation::CEnginePrimitiveAnimation(const C3DAnimationInterpolator& anim,
                                                     const C3DPositionInterpolator& world_pos,
                                                     const C3DOrientationInterpolator& orientation)
    : Anim(anim), WorldPos(world_pos), Orientation(orientation)
{
}