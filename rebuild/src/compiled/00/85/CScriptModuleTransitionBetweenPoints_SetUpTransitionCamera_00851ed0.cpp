// CScriptModuleTransitionBetweenPoints::SetUpTransitionCamera(const C3DVector&, const C3DVector&, const C3DVector&, const C3DVector&, long, float) -- set up a camera move from start to end over N frames
// Retail 0x00851ED0, 365 bytes, ret 0x18. Callee: GFGetCurrentFrame 0x0049D870 (__fastcall; catalog label
// "GFGetPreMainMemoryUsed" is propagated). Pool: 1.0f 0x0122DED8.
// Layout: PDB CScriptModuleTransitionBetweenPoints (0x90) matches retail: PThing +0x4C, Increment +0x54,
// LookAtIncrement +0x60, LookAtPos +0x6C, StartPos +0x78, EndFrame +0x84, FOV +0x88, PreventSelfTermination +0x8C.
// No engine header exists for the class; the 0x4C-byte script-module base is opaque here.
// Shape: the per-frame increments are `(end - start) / (float)frames` through an inline C3DVector::operator/ that
// multiplies by `1.0f / d` (VC CSEs the reciprocal across both calls; a hand-written `scale` local spills early).
// With no frames both increments are zeroed by a chained assignment from a C3DVector(0,0,0) temporary and the
// move is flagged self-terminating; the flag store follows the second increment.
class CWorld {
public:
    static long __fastcall GetFrame();   // 0x0049D870
};            // 0x0049D870

class C3DVector { public:
    float X, Y, Z;
    C3DVector() {}
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
    C3DVector operator-(const C3DVector& v) const { return C3DVector(X - v.X, Y - v.Y, Z - v.Z); }
    C3DVector operator*(float s) const { return C3DVector(X * s, Y * s, Z * s); }
    C3DVector operator/(float d) const { float inv = 1.0f / d; return C3DVector(X * inv, Y * inv, Z * inv); }
};

struct CScriptModuleTransitionBetweenPoints {
    unsigned char _base[0x4c];
    void*     PThing_Item;            // +0x4c CIntelligentPointer<const CThing>
    void*     PThing_Next;            // +0x50
    C3DVector Increment;              // +0x54
    C3DVector LookAtIncrement;        // +0x60
    C3DVector LookAtPos;              // +0x6c
    C3DVector StartPos;               // +0x78
    long      EndFrame;               // +0x84
    float     FOV;                    // +0x88
    bool      PreventSelfTermination; // +0x8c
    void SetUpTransitionCamera(const C3DVector& lookAtStart, const C3DVector& lookAtEnd,
                               const C3DVector& startPos, const C3DVector& endPos, long frames, float fov);
};

void CScriptModuleTransitionBetweenPoints::SetUpTransitionCamera(const C3DVector& lookAtStart, const C3DVector& lookAtEnd,
                                                                 const C3DVector& startPos, const C3DVector& endPos,
                                                                 long frames, float fov)
{
    StartPos = startPos;
    LookAtPos = lookAtStart;
    if (frames <= 0) {
        Increment = LookAtIncrement = C3DVector(0.0f, 0.0f, 0.0f);
        PreventSelfTermination = true;
    } else {
        Increment = (endPos - startPos) / (float)frames;
        LookAtIncrement = (lookAtEnd - lookAtStart) / (float)frames;
        PreventSelfTermination = false;
    }
    EndFrame = CWorld::GetFrame() + frames;
    FOV = fov;
}