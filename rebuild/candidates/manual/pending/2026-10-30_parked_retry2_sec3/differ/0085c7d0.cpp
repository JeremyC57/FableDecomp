// Grade: DIFFER(332v332): 5 diff lines (was 9) -- landed 0x007A1870 (float)(double) CrossProduct fixes the cross-product operand order; residue is the slot-70 GetFacingVector vptr register (eax v edx)
// NCameraModes::CMoveOnCylinder::OnInit() -- start the move-on-cylinder camera: give it a go-to physics leaf, take the observed thing's facing as the X axis, Z straight up, Y = X ^ Z, zero the angle, height parameter 0.75, and cut the camera
// Leaf: OnInit
// Callees: operator new 0x00BFEA1A, CPhysicsLeaf<GoTo, GotoOutsideSimple, Cartesian> ctor 0x0085C950, NCameraModes::CBase::SetPhysics(CCountedPointer, bool) 0x0084E810 (labelled ScriptEventList::InsertEvent), CBase::GetObservedThing 0x0084F1B0 (labelled _Cons_val), the thing physics' slot 70 (+0x118); counted-pointer deleter 0x00850000 (`if (p) delete p` through slot 3).
// Layout: Ego_r.pdb CMoveOnCylinder AxisX/AxisY/AxisZ/Theta/HeightParam at +0xC8.. less the 0x24 the retail NCameraModes::CBase lacks (+0xA4, +0xB0, +0xBC, +0xC8, +0xCC), as the landed CFirstPerson::OnInit 0x008735D0.
struct C3DVector {
    float X, Y, Z;
    C3DVector() {}
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) {}
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
};
struct C2DVector {
    float X, Y;
};
struct CCamera {
    unsigned char _body[0x38];
};

struct CThingPhysics {
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005(); virtual void v006(); virtual void v007();
    virtual void v008(); virtual void v009(); virtual void v010(); virtual void v011(); virtual void v012(); virtual void v013(); virtual void v014(); virtual void v015();
    virtual void v016(); virtual void v017(); virtual void v018(); virtual void v019(); virtual void v020(); virtual void v021(); virtual void v022(); virtual void v023();
    virtual void v024(); virtual void v025(); virtual void v026(); virtual void v027(); virtual void v028(); virtual void v029(); virtual void v030(); virtual void v031();
    virtual void v032(); virtual void v033(); virtual void v034(); virtual void v035(); virtual void v036(); virtual void v037(); virtual void v038(); virtual void v039();
    virtual void v040(); virtual void v041(); virtual void v042(); virtual void v043(); virtual void v044(); virtual void v045(); virtual void v046(); virtual void v047();
    virtual void v048(); virtual void v049(); virtual void v050(); virtual void v051(); virtual void v052(); virtual void v053(); virtual void v054(); virtual void v055();
    virtual void v056(); virtual void v057(); virtual void v058(); virtual void v059(); virtual void v060(); virtual void v061(); virtual void v062(); virtual void v063();
    virtual void v064(); virtual void v065(); virtual void v066(); virtual void v067(); virtual void v068(); virtual void v069();
    virtual C3DVector GetFacingVector() const;      // slot 70 (+0x118)
};
struct CThing {
    unsigned char _pad[0x60];
    CThingPhysics* m_pPhysics;              // +0x60
    CThingPhysics* GetPhysicsTC() { return m_pPhysics; }               // Ego_r ?GetPhysicsTC@CThing@@QAEPAVCTCPhysicsBase@@XZ (inline)
    const CThingPhysics* GetPhysicsTC() const { return m_pPhysics; }   // Ego_r ?GetPhysicsTC@CThing@@QBEPBVCTCPhysicsBase@@XZ (inline)
    const C3DVector& GetPosition() const;   // 0x004C73D0
};
struct CThingGameObject : CThing {};

class CGameCameraManager {
public:
    void* __vftable;                        // +0x00
    CCamera Camera;                         // +0x04
    CCamera OldCamera;                      // +0x3C
    unsigned char _pad74[0x3c];
    bool SingleCut;                         // +0xB0
    void CutCamera() { OldCamera = Camera; SingleCut = true; }
};

__forceinline C3DVector CrossProduct(const C3DVector& a, const C3DVector& b)
{
    const float* pa = &a.X; const float* pb = &b.X;
    return C3DVector((float)(double)(pb[2] * pa[1] - pa[2] * pb[1]), (float)(double)(*(&b.Y - 1) * pa[2] - pa[0] * pb[2]), (float)(double)(pb[1] * pa[0] - pa[1] * *(&b.Y - 1)));
}

struct CPhysicsBase {
    virtual void v00(); virtual void v01(); virtual void v02();
    virtual ~CPhysicsBase();                // slot 3 (+0x0c): scalar deleting destructor
};
template <class T> struct CCountedPointer {
    struct CHandle {
        long RefCount;
        void (__fastcall *Deleter)(T*);
        T* Object;
        static void __fastcall DeleteObject(T* object) { delete object; }
        CHandle(T* object) : RefCount(1), Deleter(&DeleteObject), Object(object) {}
    };
    T* Data;
    CHandle* Handle;
    CCountedPointer(T* data) : Data(data)
    {
        if (Data)
            Handle = new CHandle(Data);
        else
            Handle = 0;
    }
    CCountedPointer(const CCountedPointer& other);
};

namespace NCameraModes {
class CBase {
public:
    virtual void v00();
    unsigned char _pad04[0x3c];
    CGameCameraManager* CameraManager;      // +0x40
    unsigned char _pad44[0x60];
    const CThingGameObject* GetObservedThing() const;   // 0x0084F1B0
    void SetPhysics(CCountedPointer<CPhysicsBase> physics, bool flag);   // 0x0084E810
};
struct CPhysicsLeafGoTo : CPhysicsBase {   // CPhysicsLeaf<CPhysicsTargetModelGoTo, CPhysicsForceModelGotoOutsideSimple, CPhysicsCoordCartesian>
    unsigned char _body[0x10];
    CPhysicsLeafGoTo(CBase* mode, CGameCameraManager* manager);   // 0x0085C950
};
class CMoveOnCylinder : public CBase {
public:
    C3DVector AxisX;                        // +0xA4
    C3DVector AxisY;                        // +0xB0
    C3DVector AxisZ;                        // +0xBC
    float Theta;                            // +0xC8
    float HeightParam;                      // +0xCC
    virtual void OnInit();
};
}

void NCameraModes::CMoveOnCylinder::OnInit()
{
    SetPhysics(CCountedPointer<CPhysicsBase>(new CPhysicsLeafGoTo(this, CameraManager)), false);
    AxisX = GetObservedThing()->GetPhysicsTC()->GetFacingVector();
    AxisZ = C3DVector(0.0f, 0.0f, 1.0f);
    AxisY = CrossProduct(AxisX, AxisZ);
    Theta = 0.0f;
    HeightParam = 0.75f;
    CameraManager->CutCamera();
}
