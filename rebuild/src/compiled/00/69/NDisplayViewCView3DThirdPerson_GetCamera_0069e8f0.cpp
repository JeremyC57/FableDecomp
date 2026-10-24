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
struct C3DVector { float X, Y, Z; C3DVector() {} C3DVector(const C3DVector&); };
struct CRightHandedSet { C3DVector Up, Forward; CRightHandedSet() {} CRightHandedSet(const CRightHandedSet&); };
struct CInterpolationInfo;
struct CCamera {
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