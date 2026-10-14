// CEngineScreenEffectColourFilter::Render(void* params, void* view, long) -- when enabled (+8): compute the filter's four corner vectors from the params and the camera (the first and third arguments) and, when that succeeds, draw them for the view (the second)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B21850, length 93 bytes.
// Callees: ComputeCorners @ 0x00B214C0 (on `this`: params, camera and the four 12-byte locals by address; a bool in al), DrawCorners @ 0x00B202D0 (on `this`: view and the
// same four locals). Shape: a 0x30-byte frame of four C3DVectors pushed last-first with `lea` into eax/ecx/edx, each argument read from the stack once, between
// the pushes (`ret 0Ch`). The manifest labels of the callees ("CSplinePosParamData::TransformFromPosition",
// "Transform_VectorWithQuaternion") are propagated.
struct C3DVector { float X, Y, Z; };

struct CEngineScreenEffectColourFilter {
    unsigned char _pad[8];
    bool          m_enabled;                       // +0x08
    bool ComputeCorners(void* params, void* camera, C3DVector& a, C3DVector& b, C3DVector& c, C3DVector& d);   // 0x00B214C0
    void DrawCorners(void* view, C3DVector& a, C3DVector& b, C3DVector& c, C3DVector& d);                   // 0x00B202D0
    void Render(void* params, void* view, void* camera);
};

void CEngineScreenEffectColourFilter::Render(void* params, void* view, void* camera)
{
    if (m_enabled) {
        C3DVector a, b, c, d;
        if (ComputeCorners(params, camera, a, b, c, d))
            DrawCorners(view, a, b, c, d);
    }
}