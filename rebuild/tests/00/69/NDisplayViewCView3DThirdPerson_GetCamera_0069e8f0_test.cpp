#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <new>
#ifndef _MSC_VER
#define __cdecl
#define __stdcall
#define __fastcall
#define __fastcall
#define __forceinline inline
#define __int64 long long
#endif
static int g_ok = 1;
#define CHECK(c) do { if (!(c)) { g_ok = 0; std::printf("check failed line %d\n", __LINE__); } } while (0)
// ---- the candidate model (copied from src/0069e8f0.cpp) ----
// NDisplayView::CView3DThirdPerson::GetCamera(const CInterpolationInfo& info) -- vtable slot 4: the third-person view's camera is whatever the display-side camera source (slot 5) returns
// Leaf: ?GetCamera@CView3DThirdPerson@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0069E8F0, 29 bytes.
// Name: FableWin.pdb ?GetCamera@CView3DThirdPerson@NDisplayView@@UAE?AVCCamera@@ABVCInterpolationInfo@@@Z; slot 4 of
// the vtable 0x0125C7F8 (rtti-vtables.tsv).
// Callees: CViewBase::GetDisplayEngine() @ 0x00640010 (landed: returns the main component's +0x24 object as void*),
// that object's vtable slot 5 (`call [edx+14h]`, a CCamera by hidden pointer taking the interpolation info).
// Shape: the CCamera result is returned straight through the caller's hidden pointer (esi, reused as
// the callee's hidden pointer); `info` is reloaded after the call. The object behind GetDisplayEngine
// is modelled as a descriptive camera-source interface (its class is not recovered). `ret 8`.
class C3DVector { public: float X, Y, Z; C3DVector() {} C3DVector(const C3DVector&); };
class CRightHandedSet { public: C3DVector Up, Forward; CRightHandedSet() {} CRightHandedSet(const CRightHandedSet&); };
class CInterpolationInfo;
class CCamera { public:
    C3DVector WorldPosition; CRightHandedSet RHSet; bool HeightLocked, ViewVecZLocked;
    unsigned char _pad26[2]; unsigned long FOVFlags; float HorizontalFOV, VerticalFOV, Zoom;
    CCamera() {} CCamera(const CCamera&);
};

struct CCameraSource {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual CCamera GetCamera(const CInterpolationInfo& info);   // slot 5
};

struct CViewBase {
    virtual void v00();
    void* GetDisplayEngine();                                      // 0x00640010
};

namespace NDisplayView {

struct CView3DThirdPerson : CViewBase {
    CCamera GetCamera(const CInterpolationInfo& info);
};

CCamera CView3DThirdPerson::GetCamera(const CInterpolationInfo& info)
{
    return static_cast<CCameraSource*>(GetDisplayEngine())->GetCamera(info);
}

} // namespace NDisplayView

// ---- stubs for the external callees ----

C3DVector::C3DVector(const C3DVector& v) : X(v.X), Y(v.Y), Z(v.Z) {}
CRightHandedSet::CRightHandedSet(const CRightHandedSet& o) : Up(o.Up), Forward(o.Forward) {}
CCamera::CCamera(const CCamera& o)
    : WorldPosition(o.WorldPosition), RHSet(o.RHSet), HeightLocked(o.HeightLocked), ViewVecZLocked(o.ViewVecZLocked),
      FOVFlags(o.FOVFlags), HorizontalFOV(o.HorizontalFOV), VerticalFOV(o.VerticalFOV), Zoom(o.Zoom) {}
void CCameraSource::v00() {} void CCameraSource::v01() {} void CCameraSource::v02() {} void CCameraSource::v03() {} void CCameraSource::v04() {}
CCamera CCameraSource::GetCamera(const CInterpolationInfo&) { CCamera c; c.Zoom = 0.0f; return c; }
static const CInterpolationInfo* g_info = 0;
struct STestSource : CCameraSource {
    virtual CCamera GetCamera(const CInterpolationInfo& info)
    {
        g_info = &info;
        CCamera c;
        c.WorldPosition.X = 1.0f; c.WorldPosition.Y = 2.0f; c.WorldPosition.Z = 3.0f;
        c.HeightLocked = true;
        c.Zoom = 3.0f;
        c.HorizontalFOV = 0.5f;
        return c;
    }
};
static STestSource g_source;
void CViewBase::v00() {}
void* CViewBase::GetDisplayEngine() { return static_cast<CCameraSource*>(&g_source); }

static void run()
{

    int infoStorage = 0;
    const CInterpolationInfo& info = *reinterpret_cast<const CInterpolationInfo*>(&infoStorage);
    NDisplayView::CView3DThirdPerson view;
    CCamera camera = view.GetCamera(info);
    CHECK(g_info == &info);
    CHECK(camera.Zoom == 3.0f && camera.HorizontalFOV == 0.5f && camera.HeightLocked);
    CHECK(camera.WorldPosition.X == 1.0f && camera.WorldPosition.Y == 2.0f && camera.WorldPosition.Z == 3.0f);
}

int main()
{
    run();
    if (!g_ok) { std::printf("0069e8f0_TEST FAIL\n"); return 1; }
    std::printf("0069e8f0_TEST PASS\n");
    return 0;
}