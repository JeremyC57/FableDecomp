// Grade: DIFFER(749v752) -- x87 scheduling/operand order: retail loads the three scaled components from frame copies (fld [esp+1Ch]/[esp+18h]/[esp+14h], fmul each, stores after all three) while the lane reads them straight from the source vector and stores each product at once; the cross-product block that follows multiplies in the opposite operand order (fmul [ecx+8] / fld [ecx+4] / fmul [eax+8]).
// NCameraModes::CPCMouseLook::OnInit() -- start the PC mouse-look camera
// Leaf: OnInit
struct C3DVector {
    float X, Y, Z;
    C3DVector() {}
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) {}
    C3DVector operator*(float k) const { return C3DVector(X * k, Y * k, Z * k); }
};
inline C3DVector CrossProduct(const C3DVector& a, const C3DVector& b)
{
    return C3DVector(a.Y * b.Z - a.Z * b.Y, a.Z * b.X - a.X * b.Z, a.X * b.Y - a.Y * b.X);
}

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
    CThingPhysics* GetPhysics() const { return m_pPhysics; }
};
struct CThingGameObject : CThing {};

struct CCameraModeDef {
    unsigned char _pad[0xb0];
    float DollyLength;                      // +0xB0
};

void __fastcall Destroy_00850000(void* object);
struct CCPPointerInfo {
    long RefCount;
    void (__fastcall* DeleteFunc)(void*);
    void* Data;
    CCPPointerInfo(void* data) : RefCount(1), DeleteFunc(Destroy_00850000), Data(data) {}
};
namespace NCameraModes { class CBase; class CPhysicsBase; }
struct CPhysicsPointer {                    // CCountedPointer<NCameraModes::CPhysicsBase>
    NCameraModes::CPhysicsBase* Data;
    CCPPointerInfo* Info;
    CPhysicsPointer(NCameraModes::CPhysicsBase* p) : Data(p)
    {
        if (p)
            Info = new CCPPointerInfo(Data);
        else
            Info = 0;
    }
};

class CGameCameraManager {
public:
    unsigned char _pad[0x98];
    NCameraModes::CBase* CurrentMode;       // +0x98 (CurrentMode.Data)
    bool HasCameraMode(long mode);          // 0x00697AC0
};

namespace NCameraModes {
class CPhysicsBase { public: unsigned char _body[0x14]; };
class CPhysicsGoTo : public CPhysicsBase {  // CPhysicsLeaf<CPhysicsTargetModelGoTo, CPhysicsForceModelGotoOutsideSimple, CPhysicsCoordCartesian>
public:
    CPhysicsGoTo(CBase* mode, CGameCameraManager* manager);   // 0x0085C950
};
struct CRandomPacketOfVariablesDefiningTheCameraState {
    float AngleXY;
    float AngleUpDown;
    float DollyLength;
    float DeltaAngleXY;
    long RecentreTimer;
    C3DVector ViewPosOffset;
};
class CBase {
public:
    virtual void v00();
    unsigned char _pad04[0x3c];
    CGameCameraManager* CameraManager;      // +0x40
    const CCameraModeDef* PCameraModeDef;   // +0x44
    unsigned char _pad48[0x5c];
    const CThingGameObject* GetObservedThing() const;   // 0x0084F1B0
    void SetPhysics(CPhysicsPointer physics, bool immediate);   // 0x0084E810
};
class CPCMouseLook : public CBase {
public:
    void* MouseInputs;                      // +0xA4
    C3DVector AxisX;                        // +0xA8
    C3DVector AxisY;                        // +0xB4
    C3DVector AxisZ;                        // +0xC0
    CRandomPacketOfVariablesDefiningTheCameraState StateCurrent;   // +0xCC
    CRandomPacketOfVariablesDefiningTheCameraState StateLast;      // +0xEC
    virtual void OnInit();
};
}
using namespace NCameraModes;

void CPCMouseLook::OnInit()
{
    StateCurrent.ViewPosOffset = C3DVector(0.0f, 0.0f, 0.0f);
    CGameCameraManager* manager = CameraManager;
    if (manager->CurrentMode == 0) {
        AxisX = GetObservedThing()->GetPhysics()->GetFacingVector() * -1.0f;
        AxisZ = C3DVector(0.0f, 0.0f, 1.0f);
        AxisY = CrossProduct(AxisX, AxisZ);
        StateCurrent.AngleXY = 0.0f;
        StateCurrent.AngleUpDown = 0.125f;
        StateCurrent.DollyLength = PCameraModeDef->DollyLength;
    } else if (manager->HasCameraMode(17)) {
        CPCMouseLook* last = (CPCMouseLook*)manager->CurrentMode;
        AxisZ = last->AxisZ;
        AxisX = last->AxisX;
        AxisY = last->AxisY;
        StateCurrent.AngleXY = last->StateCurrent.AngleXY;
        StateCurrent.AngleUpDown = last->StateCurrent.AngleUpDown;
        StateCurrent.DollyLength = last->StateCurrent.DollyLength;
    } else {
        AxisX = GetObservedThing()->GetPhysics()->GetFacingVector() * -1.0f;
        AxisZ = C3DVector(0.0f, 0.0f, 1.0f);
        AxisY = CrossProduct(AxisX, AxisZ);
        StateCurrent.AngleXY = 0.0f;
        StateCurrent.AngleUpDown = 0.125f;
        StateCurrent.DollyLength = PCameraModeDef->DollyLength;
    }
    SetPhysics(CPhysicsPointer(new CPhysicsGoTo(this, CameraManager)), false);
}
