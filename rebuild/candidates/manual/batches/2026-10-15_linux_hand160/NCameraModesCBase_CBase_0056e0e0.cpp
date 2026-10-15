// NCameraModes::CBase::~CBase() -- camera-mode root destructor: own vtable, then the members in reverse: GoTo's two attractor lists, the SubComponents vector of counted pointers, the camera-mode def pointer release, LookAt's two attractor lists
// Leaf: ??1CBase@NCameraModes
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0056E0E0, length 101 bytes (+11 int3 padding).
// Label correction: catalogued as CTCActionUseBed::~CTCActionUseBed (propagated); the only caller is the deleting destructor
// 0x0056E1F0, slot 0 of vtable 0x0124DD74 = NCameraModes::CBase (rtti-vtables.tsv), and the vtable stored here is that one.
// Callees: list<pair<C3DVector,float>>::~list @ 0x0056CF20 (x4; catalog label "CGuiControlTreePane::SortTreeRecursively" is a
// byte-twin), _Destroy_range<CCountedPointer<NCameraModes::CSubComponentBase>> @ 0x0056DBF0 (landed; `__fastcall` first/last,
// the iterator tag by reference at `lea [esp+7]`), free @ 0x00BFEA14.
// Shape: no base destructor (root class). The inline CDefPointer dtor keeps its zero store inside the null test and calls the
// def's inline ReleaseTemporary() (`dec [ecx+4] ; jne` with no reload; writing the decrement in the dtor itself re-reads the
// pointer); the vector dtor loads finish into edx before start into ecx.
// Layout: Ego_r.pdb NCameraModes::CBase (PDB 0xC8, retail 0xA4: STLport lists are 4 bytes, vectors 12): LookAt
// (CPhysicsState, 0x30 retail) +0x10 with Attractors +0x34 / IncidentalAttractors +0x38, CameraManager +0x40,
// PCameraModeDef +0x44, SubComponents +0x48, GoTo +0x54 (lists +0x78/+0x7C), CameraFOV +0x84 .. ViewOccluded +0xA0.
extern "C" void __cdecl free(void* block);         // 0x00BFEA14

struct C3DVector { float X, Y, Z; };
struct C2DVector { float X, Y; };
struct CAttractorList {                            // list<pair<C3DVector,float>> (STLport, 4 bytes)
    void* _M_node;
    ~CAttractorList();                             // 0x0056CF20
};
struct CPhysicsState {                             // retail 0x30 bytes
    C3DVector Position;                            // +0x00
    C3DVector Velocity;                            // +0x0c
    C3DVector Acceleration;                        // +0x18
    CAttractorList Attractors;                     // +0x24
    CAttractorList IncidentalAttractors;           // +0x28
    bool CullPhysics;                              // +0x2c
};
struct CCameraModeDef {
    virtual void v00();
    virtual void Destroy();                        // slot 1
    long RefCount;                                 // +0x04
    void ReleaseTemporary() { if (--RefCount == 0) Destroy(); }
};
template <class T> struct CDefPointer {
    T* Def;
    ~CDefPointer()
    {
        if (Def) {
            Def->ReleaseTemporary();
            Def = 0;
        }
    }
};
template <class T> struct CCountedPointer { T* Data; void* Count; };

namespace NCameraModes { struct CSubComponentBase; }
struct SNonscalarPtrIteratorTag { SNonscalarPtrIteratorTag() { } };
void __fastcall DestroyRange(CCountedPointer<NCameraModes::CSubComponentBase>* first,
                             CCountedPointer<NCameraModes::CSubComponentBase>* last,
                             const SNonscalarPtrIteratorTag& tag);   // 0x0056DBF0
struct CSubComponentVector {                       // vector<CCountedPointer<NCameraModes::CSubComponentBase>>
    CCountedPointer<NCameraModes::CSubComponentBase>* _M_start;
    CCountedPointer<NCameraModes::CSubComponentBase>* _M_finish;
    CCountedPointer<NCameraModes::CSubComponentBase>* _M_end_of_storage;
    ~CSubComponentVector()
    {
        DestroyRange(_M_start, _M_finish, SNonscalarPtrIteratorTag());
        if (_M_start)
            free(_M_start);
    }
};
struct CGameCameraManager;

namespace NCameraModes {
struct CBase {                                     // vtable 0x0124DD74
    float DefaultFOV;                              // +0x04
    float DefaultRollAngle;                        // +0x08
    float DefaultObservedPositionZOffset;          // +0x0c
    CPhysicsState LookAt;                          // +0x10
    CGameCameraManager* CameraManager;             // +0x40
    CDefPointer<CCameraModeDef> PCameraModeDef;    // +0x44
    CSubComponentVector SubComponents;             // +0x48
    CPhysicsState GoTo;                            // +0x54
    float CameraFOV;                               // +0x84
    float RollAngle;                               // +0x88
    float ObservedPositionZOffset;                 // +0x8c
    C2DVector RightStickVector;                    // +0x90
    C2DVector LeftStickVector;                     // +0x98
    bool ViewOccluded;                             // +0xa0
    virtual ~CBase();
};

CBase::~CBase()
{
}
}
