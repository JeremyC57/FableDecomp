// CTCCarryable::GetCarriedPositionInfo(C3DPositionInterpolator* position, C3DOrientationInterpolator* orientation, float time) -- fills both interpolators' start values from the carry pose, copies them to the end values and returns the pose query's result
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00705B00, length 96 bytes.
// Callees: CTCCarryable::GetCarriedPositionInfo(C3DVector*, CRightHandedSet*, bool, float) @ 0x007055D0 (manifest,
// `ret 10h`). Called from 0x007036C4.
// Layout: Ego_r.pdb C3DPositionInterpolator (vptr, StartPosition +4, EndPosition +0x10) and
// C3DOrientationInterpolator (vptr, StartOrientation +4, EndOrientation +0x1c; CRightHandedSet = Up, Forward).
// Shape: the overload is called with false and the time argument; its bool stays in eax across the two
// implicit POD assignments (12 and 24 bytes), which copy as dword pairs through ecx/edx with the
// destination advanced in place (`add ebp,10h`, `add ebx,1Ch`).
class C3DVector { public:
    float X;
    float Y;
    float Z;
};

class CRightHandedSet { public:
    C3DVector Up;
    C3DVector Forward;
};

class C3DPositionInterpolator { public:
    virtual void v00();
    C3DVector StartPosition;                        // +0x04
    C3DVector EndPosition;                          // +0x10
};

class C3DOrientationInterpolator { public:
    virtual void v00();
    CRightHandedSet StartOrientation;               // +0x04
    CRightHandedSet EndOrientation;                 // +0x1c
};

class CTCCarryable { public:
    virtual void v00();
    bool GetCarriedPositionInfo(C3DVector* position, CRightHandedSet* orientation, bool forceUpdate, float time);   // 0x007055D0
    bool GetCarriedPositionInfo(C3DPositionInterpolator* position, C3DOrientationInterpolator* orientation, float time);
};

bool CTCCarryable::GetCarriedPositionInfo(C3DPositionInterpolator* position, C3DOrientationInterpolator* orientation, float time)
{
    bool result = GetCarriedPositionInfo(&position->StartPosition, &orientation->StartOrientation, false, time);
    position->EndPosition = position->StartPosition;
    orientation->EndOrientation = orientation->StartOrientation;
    return result;
}