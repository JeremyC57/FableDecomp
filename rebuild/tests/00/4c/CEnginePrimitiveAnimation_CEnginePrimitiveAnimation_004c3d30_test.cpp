// Behaviour test for retail 0x004c3d30: the candidate source below is embedded verbatim (from src/004c3d30.cpp),
// followed by dependency stubs/spies and a main that asserts on the observable effects.
// ===== candidate source =====
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
class C3DVector { public:                                      // the engine vector has a user-written member-wise copy
    float X, Y, Z;
    C3DVector(const C3DVector& v) : X(v.X), Y(v.Y), Z(v.Z) {}
};
class CRightHandedSet { public: C3DVector Up; C3DVector Forward; };   // 24 bytes
struct CCountedSharedData { long _header; long RefCount; };
template<class T> class CIVCountedPointer { public:
    T* P;
    CIVCountedPointer(const CIVCountedPointer& o) : P(o.P) { if (P) ++P->RefCount; }
};
class C3DAnimationBlendState { public:                         // 8 bytes
    class CSharedData : public CCountedSharedData { public:};
    virtual ~C3DAnimationBlendState();
    CIVCountedPointer<CSharedData> Data;                // +0x04
};
class C3DAnimationInterpolator { public:                       // 20 bytes
    virtual ~C3DAnimationInterpolator();
    C3DAnimationBlendState StartState;                  // +0x04
    C3DAnimationBlendState EndState;                    // +0x0c
};
class C3DPositionInterpolator { public:                        // 28 bytes
    virtual ~C3DPositionInterpolator();
    C3DVector StartPosition;                            // +0x04
    C3DVector EndPosition;                              // +0x10
};
class C3DOrientationInterpolator { public:                     // 52 bytes
    virtual ~C3DOrientationInterpolator();
    CRightHandedSet StartOrientation;                   // +0x04
    CRightHandedSet EndOrientation;                     // +0x1c
};
class CEnginePrimitiveAnimation { public:
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

// ===== harness =====
#include <stdio.h>
#include <string.h>
#include <new>
static int g_bs, g_ai, g_pi, g_oi;
C3DAnimationBlendState::~C3DAnimationBlendState() { ++g_bs; }
C3DAnimationInterpolator::~C3DAnimationInterpolator() { ++g_ai; }
C3DPositionInterpolator::~C3DPositionInterpolator() { ++g_pi; }
C3DOrientationInterpolator::~C3DOrientationInterpolator() { ++g_oi; }
static unsigned char a_buf[sizeof(C3DAnimationInterpolator)], p_buf[sizeof(C3DPositionInterpolator)], o_buf[sizeof(C3DOrientationInterpolator)];
static unsigned char d_buf[sizeof(CEnginePrimitiveAnimation)];
int main() {
    C3DAnimationBlendState::CSharedData s1, s2; s1.RefCount = 1; s2.RefCount = 5;
    memset(a_buf, 0, sizeof a_buf); memset(p_buf, 0, sizeof p_buf); memset(o_buf, 0, sizeof o_buf); memset(d_buf, 0xcd, sizeof d_buf);
    C3DAnimationInterpolator& a = *(C3DAnimationInterpolator*)a_buf;      /* raw sources: their vptrs stay null */
    C3DPositionInterpolator& p = *(C3DPositionInterpolator*)p_buf;
    C3DOrientationInterpolator& o = *(C3DOrientationInterpolator*)o_buf;
    a.StartState.Data.P = &s1; a.EndState.Data.P = &s2;
    p.StartPosition.X = 1.0f; p.EndPosition.Z = 6.0f;
    o.StartOrientation.Up.Y = 2.0f; o.EndOrientation.Forward.Z = 9.0f;
    CEnginePrimitiveAnimation* d = new (d_buf) CEnginePrimitiveAnimation(a, p, o);
    int ok = sizeof(CEnginePrimitiveAnimation) == 0x64 && (unsigned char*)&d->WorldPos - d_buf == 0x14 && (unsigned char*)&d->Orientation - d_buf == 0x30;
    ok = ok && s1.RefCount == 2 && s2.RefCount == 6 && d->Anim.StartState.Data.P == &s1 && d->Anim.EndState.Data.P == &s2;
    ok = ok && d->WorldPos.StartPosition.X == 1.0f && d->WorldPos.EndPosition.Z == 6.0f;
    ok = ok && d->Orientation.StartOrientation.Up.Y == 2.0f && d->Orientation.EndOrientation.Forward.Z == 9.0f;
    /* every member got its own class vtable: destroy each through a base pointer (virtual dispatch) */
    C3DAnimationInterpolator* pa = &d->Anim; C3DPositionInterpolator* pp = &d->WorldPos; C3DOrientationInterpolator* po = &d->Orientation;
    C3DAnimationBlendState* pb = &d->Anim.EndState;
    pb->~C3DAnimationBlendState(); pa->~C3DAnimationInterpolator(); pp->~C3DPositionInterpolator(); po->~C3DOrientationInterpolator();
    ok = ok && g_ai == 1 && g_pi == 1 && g_oi == 1 && g_bs == 3;
    printf(ok ? "004c3d30_TEST PASS\n" : "004c3d30_TEST FAIL\n"); return ok ? 0 : 1;
}