// CEngineInternalPrimitiveAnimatedMesh::GetWorldPosition(C3DVector& position) -- the interpolated world position between this frame's start and end positions, using the engine interpolation factor of the primitive's interpolation set; always true
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00BCEB40, length 146 bytes (`ret 4`).
// Globals: the engine CEngineInterpolationInfo @ 0x0143B8E0 (112 bytes, the landed StoreToGlobal_0143b8e0 copies into it);
// InterpolationSetList[set].WFInterpolate is `[set*12 + 0x0143B934]` (list at +0x4C, field +8).
// Layout: InterpolationSet is the PDB CEngineInternalPrimitiveBase member at +0x2C; the start/end positions at +0x88 / +0x94 are
// retail-only (no PDB layout for this class here), modelled locally.
// Shape: the factor is a named float read first (`fld [eax*4+...]` before the differences); `(end - start) * t + start` through the
// inline C3DVector operators (as the line midpoint 0x00BC7D40), the sum copied into the argument by integer moves.
struct C3DVector {
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector operator+(const C3DVector& v) const { return C3DVector(X + v.X, Y + v.Y, Z + v.Z); }
    C3DVector operator-(const C3DVector& v) const { return C3DVector(X - v.X, Y - v.Y, Z - v.Z); }
    C3DVector operator*(float s) const { return C3DVector(X * s, Y * s, Z * s); }
};

struct CEngineInterpolationInfoSet {
    float GTPredictedRenderAt;
    float GTPredictedTimeSinceLastRenderFrame;
    float WFInterpolate;
};

struct CEngineInterpolationInfo {
    CEngineInterpolationInfoSet Sets[6];
    long                        WFServerCurrentTime;   // +0x48
    CEngineInterpolationInfoSet InterpolationSetList[3];   // +0x4c
};
extern CEngineInterpolationInfo g_EngineInterpolationInfo;   // 0x0143B8E0 (list WFInterpolate at 0x0143B934)

class CEngineInternalPrimitiveAnimatedMesh {
public:
    virtual void v000();
    unsigned char _pad04[0x28];
    long          InterpolationSet;                // +0x2c EEngineInterpolationSet
    unsigned char _pad30[0x58];
    C3DVector     m_positionStart;                 // +0x88
    C3DVector     m_positionEnd;                   // +0x94
    virtual bool GetWorldPosition(C3DVector& position);
};

bool CEngineInternalPrimitiveAnimatedMesh::GetWorldPosition(C3DVector& position)
{
    float t = g_EngineInterpolationInfo.InterpolationSetList[InterpolationSet].WFInterpolate;
    position = (m_positionEnd - m_positionStart) * t + m_positionStart;
    return true;
}