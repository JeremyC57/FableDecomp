// NCameraModes::CBase::CBase(CGameCameraManager& cameraManager, const CDefPointer<const CCameraModeDef>& cameraModeDef) -- camera-mode root constructor: default FOV 0.2 / roll 0 / Z offset 0, empty LookAt and GoTo physics states (two STLport attractor lists each), the manager and the counted mode def, empty sub-component vector, zero stick vectors; then both physics states are reset (zero position / velocity / acceleration, physics culled)
// Grade: DIFFER(357v357) -- residue: only the tail differs: retail reuses bl (just stored as ViewOccluded = false) as `mov bl,1` for the two CullPhysics = true stores and stores them after `pop edi`; this body loads 1 into al and stores them before the pop. Every earlier byte matches. Body shape is a flattened stand-in for the inline NCameraModes::CPhysicsState::Reset() (Ego_r.pdb ?Reset@CPhysicsState@NCameraModes@@QAEXXZ); calling LookAt.Reset() / GoTo.Reset() instead CSEs &LookAt / &GoTo into ebp / edi (113-ish instruction diff).
// Leaf: ??0CBase@NCameraModes@@QAE@AAVCGameCameraManager@@ABV?$CDefPointer@$$CBVCCameraModeDef@@@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0084E420, 357 bytes, thiscall, `ret 8`, 26 callers (the derived camera-mode constructors).
// Identity (proven): stores vtable 0x0124DD74, whose RTTI locator names .?AVCBase@NCameraModes@@ (also rtti-vtables.tsv);
//   Ego_r.pdb 0x008461C0 / FableWin.pdb 0x02882C50 ??0CBase@NCameraModes@@QAE@AAVCGameCameraManager@@ABV?$CDefPointer@$$CBVCCameraModeDef@@@@@Z
//   is the only CBase constructor (two arguments = `ret 8`; the second is the counted def copied with `inc [eax+4]`).
//   The catalog label "CActionParam_Constructor" is descriptive and wrong.
// Callees: malloc 0x00BFEA0E (STLport list sentinel nodes, 0x18 bytes, x4).
// Layout: Ego_r.pdb NCameraModes::CBase (0xC8 there, 0xA4 retail: STLport lists 4 bytes, vectors 12): DefaultFOV +0x04,
//   DefaultRollAngle +0x08, DefaultObservedPositionZOffset +0x0C, LookAt CPhysicsState +0x10 (0x30), CameraManager +0x40,
//   PCameraModeDef +0x44, SubComponents +0x48, GoTo +0x54, CameraFOV +0x84, RollAngle +0x88, ObservedPositionZOffset +0x8C,
//   RightStickVector +0x90, LeftStickVector +0x98, ViewOccluded +0xA0 (the landed ~CBase 0x0056E0E0 model).
extern "C" void* __cdecl malloc(unsigned int size);              // 0x00BFEA0E

class C3DVector {
public:
    float X, Y, Z;
    C3DVector() { }
    
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
};
class C2DVector {
public:
    float X, Y;
    C2DVector(float x, float y) : X(x), Y(y) { }
};

namespace std {
template <class T1, class T2> struct pair {
    T1 first;
    T2 second;
};
template <class T> class allocator { public: allocator() { } };
template <class T, class A = allocator<T> > class list {         // STLport list: one pointer to the sentinel node
    struct node { node* next; node* prev; T data; };
    node* _M_node;
public:
    list() : _M_node(0)
    {
        node* sentinel = (node*)malloc(sizeof(node));
        sentinel->next = sentinel;
        sentinel->prev = sentinel;
        _M_node = sentinel;
    }
    ~list();
};
template <class T, class A = allocator<T> > class vector {        // STLport vector (12 bytes)
public:
    vector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) { }
    ~vector();
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
};
}

class CIVCountedPointeeBase {
public:
    virtual ~CIVCountedPointeeBase();
    void AddRef() const { ++IVRefCount; }
    mutable long IVRefCount;                                     // +0x04
};
template <class T> class CDefPointer {
public:
    CDefPointer(const CDefPointer& other) : Object(other.Object) { if (Object) Object->AddRef(); }
    ~CDefPointer();
    T* Object;
};
template <class T> class CCountedPointer { public: T* Data; void* Info; };
class CCameraModeDef : public CIVCountedPointeeBase { };
class CGameCameraManager;

namespace NCameraModes {
class CSubComponentBase;
class CPhysicsState {
public:
    CPhysicsState() : CullPhysics(false) { }
    void Reset()
    {
        const C3DVector zero(0.0f, 0.0f, 0.0f);
        Velocity = zero;
        Acceleration = zero;
        Position = zero;
        CullPhysics = true;
    }
    C3DVector Position;                                          // +0x00
    C3DVector Velocity;                                          // +0x0C
    C3DVector Acceleration;                                      // +0x18
    std::list<std::pair<C3DVector, float> > Attractors;          // +0x24
    std::list<std::pair<C3DVector, float> > IncidentalAttractors;   // +0x28
    bool      CullPhysics;                                       // +0x2C
};
class CBase {
public:
    CBase(CGameCameraManager& cameraManager, const CDefPointer<const CCameraModeDef>& cameraModeDef);
    virtual ~CBase();
    float               DefaultFOV;                              // +0x04
    float               DefaultRollAngle;                        // +0x08
    float               DefaultObservedPositionZOffset;          // +0x0C
    CPhysicsState       LookAt;                                  // +0x10
    CGameCameraManager* CameraManager;                           // +0x40
    CDefPointer<const CCameraModeDef> PCameraModeDef;            // +0x44
    std::vector<CCountedPointer<CSubComponentBase> > SubComponents;   // +0x48
    CPhysicsState       GoTo;                                    // +0x54
    float               CameraFOV;                               // +0x84
    float               RollAngle;                               // +0x88
    float               ObservedPositionZOffset;                 // +0x8C
    C2DVector           RightStickVector;                        // +0x90
    C2DVector           LeftStickVector;                         // +0x98
    bool                ViewOccluded;                            // +0xA0
};

CBase::CBase(CGameCameraManager& cameraManager, const CDefPointer<const CCameraModeDef>& cameraModeDef)
    : DefaultFOV(0.2f), DefaultRollAngle(0.0f), DefaultObservedPositionZOffset(0.0f), CameraManager(&cameraManager),
      PCameraModeDef(cameraModeDef), CameraFOV(0.2f), RollAngle(0.0f), ObservedPositionZOffset(0.0f),
      RightStickVector(0.0f, 0.0f), LeftStickVector(0.0f, 0.0f)
{
    {
        const C3DVector zero(0.0f, 0.0f, 0.0f);
        LookAt.Velocity = zero;
        LookAt.Acceleration = zero;
        LookAt.Position = zero;
    }
    {
        const C3DVector zero(0.0f, 0.0f, 0.0f);
        GoTo.Velocity = zero;
        GoTo.Acceleration = zero;
        GoTo.Position = zero;
    }
    ViewOccluded = false;
    LookAt.CullPhysics = true;
    GoTo.CullPhysics = true;
}
}   // namespace NCameraModes
