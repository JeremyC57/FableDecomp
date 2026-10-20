// NParticleEngine::CPSCSplineBase::CSplineRandomOffsetData::CSplineRandomOffsetData(CCardinalSpline* initialSpline) -- copy the initial spline as the current one, start an empty previous spline, and size the offset limit/speed arrays to the control-point count
// Leaf: ??0CSplineRandomOffsetData
// Retail: Fable.exe (TLC, VC7.1) @ 0x00B21DD0, 181 bytes, `ret 4`.
// Callees: CArray<C3DVector>::operator= @ 0x0075B8D0 (the copy constructor of ThisSpline's array assigns after zeroing),
//   CArray<C3DVector>::resize(size_type, const C3DVector&) @ 0x0075C040 (catalog "vector<CBrainBehaviour>::resize",
//   a byte twin over the same 12-byte element; the landed CTCCreatureFlyInCircles ctor 0x007DDF90 calls it too).
// Layout (Ego_r.pdb, retail-sized: CArray is the 12-byte STLport vector so CCardinalSpline is 0x14): InitialSpline +0,
//   ThisSpline +4, PreviousSpline +0x18 (Tension 0.5f, ends not connected), OffsetOnlyAtStartup +0x2C, ResetEachFrame
//   +0x2D, SplinePosOffsetLimits +0x30, SplinePosOffsetSpeed +0x3C.
// Shape: everything is the initialiser list in declaration order; the copied spline is CCardinalSpline's implicit
//   memberwise copy over CArray's copy constructor (zero the three words, then the out-of-line assignment); the arrays
//   use the STLport `_Vector_base` + alloc-proxy shape; the two resizes share one uninitialised C3DVector() default
//   argument slot in the 12-byte frame.
struct C3DVector {
    float X, Y, Z;
    C3DVector() {}
};
template <class T> struct SRetailAllocProxy {   // STLport _STLP_alloc_proxy: the end-of-storage pointer
    T* _M_data;
    SRetailAllocProxy(T* data) : _M_data(data) {}
};
template <class T> struct CArray {              // STLport vector<T> (12 bytes in retail)
    T* _M_start;
    T* _M_finish;
    SRetailAllocProxy<T> _M_end_of_storage;
    CArray() : _M_start(0), _M_finish(0), _M_end_of_storage(0) {}
    CArray(const CArray& other) : _M_start(0), _M_finish(0), _M_end_of_storage(0) { *this = other; }
    CArray& operator=(const CArray& other);                                  // 0x0075B8D0
    void resize(unsigned int newSize, const C3DVector& value = C3DVector());  // 0x0075C040
    unsigned int size() const { return (unsigned int)(_M_finish - _M_start); }
};
struct CCardinalSpline {
    CArray<C3DVector> ControlPoints;    // +0x00
    float             Tension;          // +0x0C
    bool              EndPointsConnected; // +0x10
    CCardinalSpline() : Tension(0.5f), EndPointsConnected(false) {}
};
namespace NParticleEngine {
struct CPSCSplineBase {
    struct CSplineRandomOffsetData {
        CCardinalSpline*  InitialSpline;          // +0x00
        CCardinalSpline   ThisSpline;             // +0x04
        CCardinalSpline   PreviousSpline;         // +0x18
        bool              OffsetOnlyAtStartup;    // +0x2C
        bool              ResetEachFrame;         // +0x2D
        CArray<C3DVector> SplinePosOffsetLimits;  // +0x30
        CArray<C3DVector> SplinePosOffsetSpeed;   // +0x3C
        CSplineRandomOffsetData(CCardinalSpline* initialSpline);
    };
};
CPSCSplineBase::CSplineRandomOffsetData::CSplineRandomOffsetData(CCardinalSpline* initialSpline)
    : InitialSpline(initialSpline), ThisSpline(*initialSpline), OffsetOnlyAtStartup(false), ResetEachFrame(false)
{
    SplinePosOffsetLimits.resize(InitialSpline->ControlPoints.size());
    SplinePosOffsetSpeed.resize(SplinePosOffsetLimits.size());
}
}