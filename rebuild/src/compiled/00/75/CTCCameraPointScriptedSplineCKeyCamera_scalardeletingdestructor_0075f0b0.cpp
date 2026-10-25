// CTCCameraPointScriptedSpline::CKeyCamera::`scalar deleting destructor'(unsigned int flags) -- the compiler-generated deleting destructor: the implicit ~CKeyCamera runs EventName's destructor (CCharString at +0x30), then operator delete when bit 0 of flags is set; returns this
// Leaf: ??_GCKeyCamera@CTCCameraPointScriptedSpline@@QAEPAXI@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0075F0B0, 33 bytes (+ int3 padding), thiscall, `ret 4`; unreferenced (no call or pointer to it in Fable.exe).
// Identity: CTCCameraPointScriptedSpline::CKeyCamera::`scalar deleting destructor' -- proven; evidence: FableWin.pdb name set holds ??_GCKeyCamera@CTCCameraPointScriptedSpline@@QAEPAXI@Z; Ego_r.pdb CKeyCamera has its only destructible member, CCharString EventName, at +48 = +0x30; the body sits next to the CKeyCamera constructor 0x0075F1F0.
//   Family G042 (8 deleting destructors that differ in the one member's offset and destructor; the propagated label
//   "CLandscapeBackgroundPatch::`vector deleting destructor'" is wrong for all eight).
// Callees: CCharString::~CCharString() 0x0099EAE0, operator delete 0x00BFE9BC.
// Shape: `delete p` on the class emits the non-virtual ??_G as its own COMDAT (the helper below is not graded).
void __cdecl operator delete(void* p);                         // 0x00BFE9BC
class C3DVector {                                            // 12 bytes
public:
    float X, Y, Z;
};
class CCharString {
public:
    ~CCharString();                                          // 0x0099EAE0
    void* m_pStringData;
};
class CTCCameraPointScriptedSpline {
public:
    class CKeyCamera {
    public:
        float DeltaIndex;                                   // +0x00
        C3DVector Pos;                                      // +0x04
        C3DVector LookDirection;                            // +0x10
        float FOV;                                          // +0x1c
        float Roll;                                         // +0x20
        float ShuttleSpeed;                                 // +0x24
        float Duration;                                     // +0x28
        float PauseTime;                                    // +0x2c
        CCharString EventName;                              // +0x30
        float SpeedFraction;                                // +0x34
    };
};
void DeleteOne(CTCCameraPointScriptedSpline::CKeyCamera* p) { delete p; }