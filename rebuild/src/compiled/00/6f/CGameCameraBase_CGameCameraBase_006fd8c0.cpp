// CGameCameraBase::CGameCameraBase(CWorld& world) -- build the internal camera at the origin looking down +Z (up 1,1,1), default the last camera, store the world, set the orbit defaults (theta 0.2, phi 1.25, radius 10 and their fixed copies), empty the two coordinate lists, arm the trigger frame 1.5 s ahead and clear the building UID
// Leaf: ??0CGameCameraBase
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006FD8C0, length 294 bytes (+ int3 padding), `ret 4`.
// Signature from Ego_r.pdb: ??0CGameCameraBase@@QAE@AAVCWorld@@@Z. Vtable 0x01264A8C (RTTI CGameCameraBase); the root
//   CAIGameCameraBase's own vtable store (0x0125C1F4) is dead and dropped.
// Callees: CCamera::CCamera(const C3DVector&, const C3DVector&, const C3DVector&, float, float) @ 0x00A0C130, malloc
//   (import thunk @ 0x00BFEA0E, the two 0x14-byte list sentinels), __ftol2 @ 0x00BFEA70. Globals: the frame rate
//   @ 0x01375550 (long). Float pool: 0x0125AADC = 1.5f.
// Layout: engine/CGameCameraBase.h is the PDB layout; retail matches it up to +0xa8, then the STLport lists are 4 bytes
//   (DrawCoordList +0xa8, LocalThingPosList +0xac), TriggerFrame +0xb0, BuildingUID +0xb8, PhiChanger +0xc0 -- so `this`
//   is a local model. CCamera (PDB 0x38): HeightLocked +0x24, ViewVecZLocked +0x25 (the inline default ctor clears them).
// Shape: the derived vtable is stored first; the camera's three C3DVector arguments are temporaries filled with
//   immediates after the pushes (the trailing 0.0f rides in the shared zero register); repeated float constants go
//   through ecx/eax; each list is `_M_node(0)` then the malloc'd self-linked sentinel, the two mallocs' stack cleanup
//   merged into one `add esp,8`.
extern "C" void* __cdecl malloc(unsigned int size);     // 0x00BFEA0E
extern long g_framesPerSecond;                          // 0x01375550

struct C3DVector {
    float X, Y, Z;
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
};
struct CCamera {                                        // 0x38 bytes
    float          WorldPosition[3];                    // +0x00
    float          RHSet[6];                            // +0x0c CRightHandedSet
    bool           HeightLocked;                        // +0x24
    bool           ViewVecZLocked;                      // +0x25
    unsigned long  FOVFlags;                            // +0x28
    float          HorizontalFOV;                       // +0x2c
    float          VerticalFOV;                         // +0x30
    float          Zoom;                                // +0x34
    CCamera() : HeightLocked(false), ViewVecZLocked(false) {}
    CCamera(const C3DVector& position, const C3DVector& up, const C3DVector& lookAt, float fov, float zoom);   // 0x00A0C130
};

template <class T> struct CList {                       // retail STLport list: the sentinel pointer only
    struct Node { Node* Next; Node* Prev; T Value; };
    Node* m_node;
    CList() : m_node(0)
    {
        Node* node = (Node*)malloc(sizeof(Node));
        node->Next = node;
        node->Prev = node;
        m_node = node;
    }
};

struct CWorld;
struct CAIGameCameraBase {                              // vtable 0x0125C1F4 (store dead)
    virtual void v00();
    CAIGameCameraBase() {}
};

struct CGameCameraBase : CAIGameCameraBase {            // vtable 0x01264A8C
    CCamera          InternalCam;                       // +0x04
    CCamera          LastInternalCam;                   // +0x3c
    float            LastPointingAt[3];                 // +0x74
    float            PointingAt[3];                     // +0x80
    CWorld*          World;                             // +0x8c
    float            Theta;                             // +0x90
    float            Phi;                               // +0x94
    float            Rad;                               // +0x98
    float            ThetaFixed;                        // +0x9c
    float            RadFixed;                          // +0xa0
    bool             ActAsFixedCam;                     // +0xa4
    bool             GoingIntoBuilding;                 // +0xa5
    CList<C3DVector> DrawCoordList;                     // +0xa8
    CList<C3DVector> LocalThingPosList;                 // +0xac
    long             TriggerFrame;                      // +0xb0
    unsigned __int64 BuildingUID;                       // +0xb8
    float            PhiChanger;                        // +0xc0
    CGameCameraBase(CWorld& world);
    virtual void v00();
};

CGameCameraBase::CGameCameraBase(CWorld& world)
    : InternalCam(C3DVector(0.0f, 0.0f, 0.0f), C3DVector(1.0f, 1.0f, 1.0f), C3DVector(0.0f, 0.0f, 1.0f), 0.19444f, 0.0f),
      World(&world), Theta(0.2f), Phi(1.25f), Rad(10.0f), ThetaFixed(0.2f), RadFixed(10.0f),
      ActAsFixedCam(false), GoingIntoBuilding(false),
      TriggerFrame((long)(g_framesPerSecond * 1.5f)), BuildingUID(0), PhiChanger(0.1f)
{
}