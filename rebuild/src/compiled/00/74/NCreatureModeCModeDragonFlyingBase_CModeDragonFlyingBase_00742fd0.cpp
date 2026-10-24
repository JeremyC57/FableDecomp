// NCreatureMode::CModeDragonFlyingBase::~CModeDragonFlyingBase() -- free the flight spline's control points, then the CModeBase part: its vtable, the weapon anim filter's group string and the filter's base vtable
// Leaf: ??1CModeDragonFlyingBase
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00742FD0, 42 bytes (+ int3 padding).
// Caller: 0x00743960, the scalar deleting destructor of the derived CModeDragonStrafe (it destroys
// the strafe's own CAnimFilterOfGroup at +0x38 first, then calls this base destructor).
// Callees: free @ 0x00BFEA14 (CRT; CArray storage is malloc/free), ~CCharString @ 0x0099EAE0.
// Vtables: CTCCreatureModeManager::CModeBase 0x012673CC (+0x00), CAnimFilterBase 0x0122FFCC (+0x08).
// Layout (Ego_r.pdb): CModeBase (28 bytes) = vptr +0, ParentThing +4, CAnimFilterOfGroup
// WeaponBasedFilter +8 (vptr, CCharString Group +0xc, GroupCRC +0x10), FramesBeforeRemovingSelf
// +0x14, Active +0x18; CModeDragonFlyingBase adds CCardinalSpline FlightSpline +0x1c (CArray<C3DVector>
// ControlPoints whose data pointer is the first dword), SplinePercentageComplete, LegAnimAlpha.
// Shape: no vtable store at entry -- the abstract flying base is `__declspec(novtable)` (the shape of
// the landed 0x008CC280 mode destructor); the spline member's inline CArray destructor frees its
// data (`if (Data) free(Data)`, no reset); ~CModeBase is an empty inline user destructor (its vtable
// store), its filter member's implicit destructor runs the string dtor and then the empty inline
// CAnimFilterBase destructor stores the base vtable at +8.
#include <stdlib.h>

class CThing;

struct CCharString {
    void* PStringData;
    ~CCharString();                             // 0x0099EAE0
};

struct C3DVector {
    float X, Y, Z;
};

template<class T> struct CArray {
    T* Data;                                    // +0x0
    long Size;                                  // +0x4
    long Capacity;                              // +0x8
    long GrowBy;                                // +0xc
    ~CArray()
    {
        if (Data)
            free(Data);
    }
};

struct CCardinalSpline {
    CArray<C3DVector> ControlPoints;            // +0x00
    float Tension;                              // +0x10
    bool EndPointsConnected;                    // +0x14
};

struct CAnimFilterBase {                        // vtable 0x0122FFCC
    virtual void v00();
    ~CAnimFilterBase() { }
};

struct CAnimFilterOfGroup : CAnimFilterBase {   // 12 bytes
    CCharString Group;                          // +0x4
    unsigned long GroupCRC;                     // +0x8
};

namespace CTCCreatureModeManager {
struct CModeBase {                              // vtable 0x012673CC, 28 bytes
    virtual void v00();
    CThing* ParentThing;                        // +0x04
    CAnimFilterOfGroup WeaponBasedFilter;       // +0x08
    long FramesBeforeRemovingSelf;              // +0x14
    bool Active;                                // +0x18
    ~CModeBase() { }
};
}

namespace NCreatureMode {
struct __declspec(novtable) CModeDragonFlyingBase : CTCCreatureModeManager::CModeBase {
    CCardinalSpline FlightSpline;               // +0x1c
    float SplinePercentageComplete;
    float LegAnimAlpha;
    ~CModeDragonFlyingBase();
};

CModeDragonFlyingBase::~CModeDragonFlyingBase()
{
}
}