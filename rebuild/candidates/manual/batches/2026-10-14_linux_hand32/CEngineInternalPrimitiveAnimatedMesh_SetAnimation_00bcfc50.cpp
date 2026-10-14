// CEngineInternalPrimitiveAnimatedMesh::SetAnimation(const SAnimationState&) -- copy the animation interpolator (+0x70), the two vectors (+0x88 / +0x94) and the orientation interpolator (+0xa0) from the state record
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00BCFC50, length 85 bytes.
// Callees: C3DAnimationInterpolator::operator= @ 0x00BD85F0 (member +0x70 from record +0), C3DOrientationInterpolator::operator= @ 0x006B0E00 (member +0xa0
// from record +0x30). Shape: the two C3DVector assignments are the engine's memberwise operator= (three dword moves through a re-based `lea` on both
// sides), the record pointer stays in edi. No PDB layout for the animated mesh; member names are descriptive.
struct C3DAnimationInterpolator {
    unsigned char _body[0x18];
    C3DAnimationInterpolator& operator=(const C3DAnimationInterpolator& other);   // 0x00BD85F0
};

struct C3DOrientationInterpolator {
    unsigned char _body[0x10];
    C3DOrientationInterpolator& operator=(const C3DOrientationInterpolator& other);   // 0x006B0E00
};

struct C3DVector {
    float X, Y, Z;
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) { }   // the engine's memberwise copy; the assignment is the implicit one
};

struct SAnimationState {
    C3DAnimationInterpolator   m_animation;        // +0x00
    C3DVector                  m_position;         // +0x18
    C3DVector                  m_scale;            // +0x24
    C3DOrientationInterpolator m_orientation;      // +0x30
};

struct CEngineInternalPrimitiveAnimatedMesh {
    unsigned char              _pad[0x70];
    C3DAnimationInterpolator   m_animation;        // +0x70
    C3DVector                  m_position;         // +0x88
    C3DVector                  m_scale;            // +0x94
    C3DOrientationInterpolator m_orientation;      // +0xa0
    void SetAnimation(const SAnimationState& state);
};

void CEngineInternalPrimitiveAnimatedMesh::SetAnimation(const SAnimationState& state)
{
    m_animation = state.m_animation;
    m_position = state.m_position;
    m_scale = state.m_scale;
    m_orientation = state.m_orientation;
}
