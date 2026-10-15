// NCameraModes::CScriptModuleFOVZoom::CalculateIncrementVector(const C3DVector& from, const C3DVector& to, long frames) -- Increment = (to - from) / frames
// Grade: DIFFER(130v106) -- retail keeps diff.x/diff.y on the x87 stack (only diff.z in the frame) and stores the 1/frames reciprocal in the dead `to` argument slot; ours spills the operator- result to a frame temporary and copies it. Tried operator- + operator*(inv), named inv local, operator/(float), scalar dx/dy/dz locals (84 bytes, no frame copy at all).
// Leaf: CalculateIncrementVector
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008520F0, length 112 bytes (106 + int3 padding). No callees.
// Shape: the difference through an inline member `operator-` (named local), then `diff * (1.0f / frames)`
// through an inline member `operator*(float)`: the reciprocal (`fild ; fdivr [1.0f]`) is stored in the
// dead `to` argument slot, the product lands in a frame temporary and goes to Increment (+0x68)
// through integer registers.
// Layout: PDB NCameraModes::CScriptModuleFOVZoom (0x84 bytes): LookAtPos +0x4c, EndFrame +0x58,
// WorldFrames +0x5c, FOVIncrement +0x60, FOVFraction +0x64, Increment +0x68, StartPos +0x74,
// ShortenOnOcclusion +0x80, ResetCamera +0x81, over the 0x4c-byte CScriptModuleBase.
struct C3DVector {
    float X, Y, Z;
    C3DVector() { }
    C3DVector operator-(const C3DVector& other) const
    {
        C3DVector result;
        result.X = X - other.X;
        result.Y = Y - other.Y;
        result.Z = Z - other.Z;
        return result;
    }
    C3DVector operator*(float s) const
    {
        C3DVector result;
        result.X = X * s;
        result.Y = Y * s;
        result.Z = Z * s;
        return result;
    }
};

namespace NCameraModes {

struct CScriptModuleFOVZoom {
    virtual void v00();
    unsigned char _pad04[0x48];
    C3DVector     LookAtPos;                         // +0x4c
    long          EndFrame;                          // +0x58
    long          WorldFrames;                       // +0x5c
    float         FOVIncrement;                      // +0x60
    float         FOVFraction;                       // +0x64
    C3DVector     Increment;                         // +0x68
    C3DVector     StartPos;                          // +0x74
    bool          ShortenOnOcclusion;                // +0x80
    bool          ResetCamera;                       // +0x81
    void CalculateIncrementVector(const C3DVector& from, const C3DVector& to, long frames);
};

void CScriptModuleFOVZoom::CalculateIncrementVector(const C3DVector& from, const C3DVector& to, long frames)
{
    C3DVector diff = to - from;
    float inv = 1.0f / frames;
    Increment = diff * inv;
}

}
