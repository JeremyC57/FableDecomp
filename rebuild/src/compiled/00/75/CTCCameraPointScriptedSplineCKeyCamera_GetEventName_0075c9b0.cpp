// CTCCameraPointScriptedSpline::CKeyCamera::GetEventName() const -- return the key's event name.
// Retail: Fable.exe 0x0075C9B0, 22 bytes. The native caller iterates 0x38-byte keys and PDB
// places EventName at +0x30; no generated spline header exists, so this is a local PDB view.
#include "fable_string.h"
// The shared header predates the retail user copy constructor. This ABI view adds that
// real copy operation without changing the one-pointer CCharString lifetime/layout.
struct CRetailCharString : CCharString {
    CRetailCharString(const CCharString& other); // native CCharString copy ctor @ 0x0099EC30
};
class C3DVector { public: float x, y, z; };
class CTCCameraPointScriptedSpline { public:
    class CKeyCamera { public:
        float DeltaIndex;
        C3DVector Pos;
        C3DVector LookDirection;
        float FOV;
        float Roll;
        float ShuttleSpeed;
        float Duration;
        float PauseTime;
        CCharString EventName;
        float SpeedFraction;
        CRetailCharString GetEventName() const;
    };
};
CRetailCharString CTCCameraPointScriptedSpline::CKeyCamera::GetEventName() const { return EventName; }