// NCameraModes::CBase::UpdateCameraOcclusion(C3DVector& cameraPosition) -- pull the camera in front of whatever blocks the line from the observed thing's focal point: first test to the camera, then (from the hit) once more, flagging the occlusion
// Leaf: UpdateCameraOcclusion
// Retail: Fable.exe (TLC, VC7.1) @ 0x0084DB30, 224 bytes incl. padding (ret 4).
// Callees: own virtual slot 6 (+0x18, occlusion enabled), CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (the camera
//          manager's observed thing, +0x40 -> +0x100, the inline accessor of the landed GetObservedThingCurrentMeshHeight
//          0x0084D8B0), CTCGraphicAppearance::GetCameraFocalPos(float) @ 0x004C12D0 (the thing's cached appearance TC +0x64),
//          CBase::IsCameraOcclusionFromTo(C3DVector*, C3DVector*, C3DVector*) @ 0x0084D2F0.
// Layout: CBase camera manager +0x40, Occluded flag +0xA0.
// Shape: the observed thing is a named local (fetched before the call pushes); C3DVector with a user copy ctor and
//          no operator=: the focal point (a named return) and the camera position are copied into
//          locals, the first hit is copied back through a pointer register.
class C3DVector { public:
    float X, Y, Z;
    C3DVector() { }
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) { }
};
struct CTCGraphicAppearance {
    C3DVector GetCameraFocalPos(float scale);   // 0x004C12D0
};
class CThing { public:
    unsigned char         _pad[0x64];
    CTCGraphicAppearance* PGraphicAppearance;   // +0x64
};
class CBaseObject;
class CBaseObjectPointer;
class CBaseIntelligentPointer {                       // vtable 0x0129C95C (Ego_r: vptr + PData, 8 bytes)
public:
    CBaseIntelligentPointer();                           // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                  // 0x00A01C10
    CBaseObject* GetPItem() const;                       // 0x00A01B50
    void SetPItem(CBaseObject* item);                    // 0x00A01B90
private:
    CBaseObjectPointer* PData;                           // +0x04
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {
public:
    T* GetPItem() const { return (T*)CBaseIntelligentPointer::GetPItem(); }
    void SetPItem(T* item) { CBaseIntelligentPointer::SetPItem((CBaseObject*)item); }
};
struct CCameraManager {
    unsigned char           _pad[0x100];
    CIntelligentPointer<CThing> m_observedThing;    // +0x100
    CThing* GetObservedThing() { return m_observedThing.GetPItem(); }
};

namespace NCameraModes {
class CBase {
public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual bool UsesCameraOcclusion();         // slot 6 (+0x18)
    unsigned char   _pad04[0x3c];
    CCameraManager* m_pManager;                 // +0x40
    unsigned char   _pad44[0x5c];
    bool            Occluded;                   // +0xA0
    bool IsCameraOcclusionFromTo(C3DVector* from, C3DVector* to, C3DVector* hit);   // 0x0084D2F0
    void UpdateCameraOcclusion(C3DVector& cameraPosition);
};
}

void NCameraModes::CBase::UpdateCameraOcclusion(C3DVector& cameraPosition)
{
    Occluded = false;
    if (!UsesCameraOcclusion())
        return;
    CThing* thing = m_pManager->GetObservedThing();
    C3DVector focal = thing->PGraphicAppearance->GetCameraFocalPos(1.0f);
    C3DVector from = focal;
    C3DVector to = cameraPosition;
    C3DVector hit;
    if (IsCameraOcclusionFromTo(&from, &to, &hit)) {
        cameraPosition = hit;
        Occluded = true;
        if (IsCameraOcclusionFromTo(&from, &cameraPosition, &hit))
            cameraPosition = hit;
    }
}