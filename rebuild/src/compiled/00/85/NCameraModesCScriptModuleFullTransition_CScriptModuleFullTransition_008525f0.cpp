// NCameraModes::CScriptModuleFullTransition::CScriptModuleFullTransition(const C3DVector& lookAt, const C3DVector& startPos, const C3DVector& endLookAt, const C3DVector& endPos, long frames) -- scripted camera move: remember the start position and look-at, the per-frame increments towards the end pair (over `frames`, 15 when 0) and the end frame
// Leaf: ??0CScriptModuleFullTransition
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008525F0, length 288 bytes (`ret 14h`).
// Callees: CScriptModuleBase::CScriptModuleBase @ 0x005ED070, CWorld::GetWorldFrame @ 0x0049D870 (static). Vtable 0x01275A44.
// Float pool: 0x0122DED8 = 1.0f.
// Layout: Ego_r.pdb CScriptModuleFullTransition = retail: IncrementForLookAtPoints +0x4c, Increment +0x58, LookAtPos +0x64,
// StartPos +0x70, EndFrame +0x7c (behind the 0x4c-byte CScriptModuleBase of the landed CScriptModuleCameraOnAStick 0x007AB450).
// Shape: LookAtPos / StartPos copied in the init list (three dword moves each), then the body: the frame count (default 15)
// lives in the dead `lookAt` slot for the `fild`, each increment is `(end - start) / (float)count` through C3DVector value
// operators (operator/ multiplies by the reciprocal, which VC computes once for both) (frame temporaries, the result copied with integer moves by the implicit operator=),
// Increment first; EndFrame adds the ORIGINAL frames argument to the world frame.
class C3DVector { public:
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector(const C3DVector& v) : X(v.X), Y(v.Y), Z(v.Z) { }
    C3DVector operator-(const C3DVector& v) const { return C3DVector(X - v.X, Y - v.Y, Z - v.Z); }
    C3DVector operator/(float d) const { float inverse = 1.0f / d; return C3DVector(X * inverse, Y * inverse, Z * inverse); }
};

class CWorld { public:
    static long GetWorldFrame();                // 0x0049D870
};

struct CScriptModuleBase {
    virtual void v00();
    unsigned char m_state[0x4C - 4];            // +0x4 .. +0x4C: base state, untouched here
    CScriptModuleBase();                        // 0x005ED070
};

namespace NCameraModes {

struct CScriptModuleFullTransition : CScriptModuleBase {
    virtual void v00();                         // own vtable (0x01275A44)
    C3DVector IncrementForLookAtPoints;         // +0x4c
    C3DVector Increment;                        // +0x58
    C3DVector LookAtPos;                        // +0x64
    C3DVector StartPos;                         // +0x70
    long      EndFrame;                         // +0x7c
    CScriptModuleFullTransition(const C3DVector& lookAt, const C3DVector& startPos, const C3DVector& endLookAt,
                                const C3DVector& endPos, long frames);
};

CScriptModuleFullTransition::CScriptModuleFullTransition(const C3DVector& lookAt, const C3DVector& startPos, const C3DVector& endLookAt,
                                                         const C3DVector& endPos, long frames)
    : CScriptModuleBase(), LookAtPos(lookAt), StartPos(startPos)
{
    long count = frames ? frames : 15;
    Increment = (endPos - startPos) / (float)count;
    IncrementForLookAtPoints = (endLookAt - lookAt) / (float)count;
    EndFrame = CWorld::GetWorldFrame() + frames;
}

}