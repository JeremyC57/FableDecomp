// NParticleEngine::CPSCSplineBase::CSplineRandomOffsetData::GetPointAtIndex(C3DVector& out, long index, float time) -- copy control point `index` of the current spline (or of last frame's spline) into `out`
// Leaf: GetPointAtIndex
// Retail: Fable.exe (TLC, VC7.1) @ 0x00B21090, 197 bytes, `ret 0Ch` (the time argument is unused).
// Identity: Ego_r.pdb `?GetPointAtIndex@CSplineRandomOffsetData@CPSCSplineBase@NParticleEngine@@QAEXAAVC3DVector@@JM@Z`.
// Callee: CArray<C3DVector>::operator= @ 0x0075B8D0 (the inlined CCardinalSpline assignment).
// Shape: the sibling of the landed GetPointAtPos 0x00B20FD0 with a control point read instead of an evaluation:
//   an offset-at-startup spline reads ThisSpline directly; otherwise, when the previous frame's spline lost sync
//   with the current one (control-point counts differ, ThisSpline's count computed first), it is re-snapshotted
//   (array, Tension, EndPointsConnected) and the current point is returned; else last frame's point. The 12-byte
//   C3DVector copies are POD dword moves; the model is the native 20-byte CCardinalSpline of the landed siblings.
class C3DVector { public: float X, Y, Z; };
template<class T> class CArray { public:
    T* m_begin; T* m_end; T* m_capacity;
    CArray& operator=(const CArray& other);                 // 0x0075B8D0
    long Size() const { return (long)(m_end - m_begin); }
    T* begin_() const { return m_begin; }
    T& operator[](long i) { return *(begin_() + i); }
};
class CCardinalSpline { public:
    CArray<C3DVector> ControlPoints;
    float Tension;
    bool EndPointsConnected;
    CCardinalSpline& operator=(const CCardinalSpline& other) {
        ControlPoints = other.ControlPoints; Tension = other.Tension; EndPointsConnected = other.EndPointsConnected; return *this;
    }
};
struct CSplineRandomOffsetData {
    CCardinalSpline* InitialSpline;
    CCardinalSpline ThisSpline;
    CCardinalSpline PreviousSpline;
    bool OffsetOnlyAtStartup;
    bool ResetEachFrame;
    CArray<C3DVector> SplinePosOffsetLimits;
    CArray<C3DVector> SplinePosOffsetSpeed;
    void GetPointAtIndex(C3DVector& out, long index, float time);
};
void CSplineRandomOffsetData::GetPointAtIndex(C3DVector& out, long index, float time)
{
    if (OffsetOnlyAtStartup) { out = ThisSpline.ControlPoints[index]; return; }
    if (PreviousSpline.ControlPoints.Size() != ThisSpline.ControlPoints.Size()) {
        PreviousSpline = ThisSpline;
        out = ThisSpline.ControlPoints[index];
    }
    else
        out = PreviousSpline.ControlPoints[index];
}