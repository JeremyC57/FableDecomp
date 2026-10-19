// NParticleEngine::CPSCSplineBase::CSplineRandomOffsetData::GetPointAtPos(out,position,time) -- evaluate current point or previous-frame point, synchronizing changed spline counts
// Leaf: GetPointAtPos
// Retail00B20FD0,177 bytes through ret12 at00B2107E;15 trailing int3 bytes excluded.
// Same PDB/native20-byte CCardinalSpline model as canonical Update00B21160. No shared
// spline/vector headers exist. Array assignment0075B8D0; spline evaluator00A567A0.
// Stable-count path evaluates current into a discarded local before evaluating previous toout;
// changed count snapshots current and evaluates current. Third float parameter is unused.
struct C3DVector { float X, Y, Z; };
template<class T> struct CArray {
    T* m_begin; T* m_end; T* m_capacity;
    CArray& operator=(const CArray& other);
    long Size() const { return (long)(m_end-m_begin); }
};
struct CCardinalSpline {
    CArray<C3DVector> ControlPoints;
    float Tension;
    bool EndPointsConnected;
    void GetPoint(C3DVector*,float); //00A567A0
    CCardinalSpline& operator=(const CCardinalSpline& other) {
        ControlPoints=other.ControlPoints; Tension=other.Tension; EndPointsConnected=other.EndPointsConnected; return *this;
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
    void GetPointAtPos(C3DVector* out,float position,float time);
};
void CSplineRandomOffsetData::GetPointAtPos(C3DVector* out,float position,float time) {
    if(OffsetOnlyAtStartup) {ThisSpline.GetPoint(out,position);return;}
    if(PreviousSpline.ControlPoints.Size()!=ThisSpline.ControlPoints.Size()) {PreviousSpline=ThisSpline;ThisSpline.GetPoint(out,position);}
    else {C3DVector current;ThisSpline.GetPoint(&current,position);PreviousSpline.GetPoint(out,position);}
}