// NParticleEngine::CPSCSplineBase::CSplineRandomOffsetData::Update() -- unless offsets are applied only at start-up: keeps the current spline as the previous one, optionally resets the current spline to the initial one, then re-offsets the spline positions
// Leaf: Update
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B21160, 79 bytes (+1 int3 padding). Callees: retail
// CArray<C3DVector>::operator= @ 0x0075B8D0 (manifest "Std_Vector_Assign"; self-test, /12 sizes), and
// CSplineRandomOffsetData::OffsetSplinePositions @ 0x00B206B0 (tail jump). Caller: 0x00B215EE.
// Layout (PDB CSplineRandomOffsetData, retail-sized: CArray is three pointers here, so CCardinalSpline is 0x14 bytes):
// InitialSpline +0x0, ThisSpline +0x4, PreviousSpline +0x18, OffsetOnlyAtStartup +0x2C, ResetEachFrame +0x2D (the two
// CArray<C3DVector> limit/speed arrays follow and are not touched here).
// Shape: CCardinalSpline's assignment is the memberwise copy (out-of-line CArray assignment, then Tension and
// EndPointsConnected through registers) inlined at both sites; the bool flags are `mov al ; test al,al` tests; the
// final member call is a sibling-call `jmp`.
class C3DVector { public: float X, Y, Z; };

template<class T> class CArray { public:                       // retail CArray<T>: begin/end/capacity
    T* m_begin; T* m_end; T* m_capacity;
    CArray& operator=(const CArray& other);              // 0x0075B8D0 for T = C3DVector
};

class CCardinalSpline { public:
    CArray<C3DVector> ControlPoints;                     // +0x0
    float             Tension;                           // +0xC
    bool              EndPointsConnected;                // +0x10
    CCardinalSpline& operator=(const CCardinalSpline& other)
    {
        ControlPoints = other.ControlPoints;
        Tension = other.Tension;
        EndPointsConnected = other.EndPointsConnected;
        return *this;
    }
};

struct CSplineRandomOffsetData {
    CCardinalSpline*   InitialSpline;                    // +0x00
    CCardinalSpline    ThisSpline;                       // +0x04
    CCardinalSpline    PreviousSpline;                   // +0x18
    bool               OffsetOnlyAtStartup;              // +0x2C
    bool               ResetEachFrame;                   // +0x2D
    CArray<C3DVector>  SplinePosOffsetLimits;            // +0x30
    CArray<C3DVector>  SplinePosOffsetSpeed;             // +0x3C
    void OffsetSplinePositions();                        // 0x00B206B0
    void Update();
};

void CSplineRandomOffsetData::Update()
{
    if (OffsetOnlyAtStartup)
        return;
    PreviousSpline = ThisSpline;
    if (ResetEachFrame)
        ThisSpline = *InitialSpline;
    OffsetSplinePositions();
}