#include <cstdio>
static long g_frame = 100;
long GFGetCurrentFrame() { return g_frame; }

struct C3DVector {
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
    EndFrame = GFGetCurrentFrame() + frames;
    FOV = fov;
}

static bool eq(const C3DVector& v, float x, float y, float z)
{
    float dx = v.X - x, dy = v.Y - y, dz = v.Z - z;
    return dx < 1e-5f && dx > -1e-5f && dy < 1e-5f && dy > -1e-5f && dz < 1e-5f && dz > -1e-5f;
}
int main()
{
    static CScriptModuleTransitionBetweenPoints m;
    C3DVector la0(1, 2, 3), la1(5, 10, 3), s0(0, 0, 0), s1(10, -20, 40);
    m.SetUpTransitionCamera(la0, la1, s0, s1, 4, 0.75f);
    bool ok = eq(m.StartPos, 0, 0, 0) && eq(m.LookAtPos, 1, 2, 3) && eq(m.Increment, 2.5f, -5, 10)
           && eq(m.LookAtIncrement, 1, 2, 0) && m.EndFrame == 104 && m.FOV == 0.75f && !m.PreventSelfTermination;
    g_frame = 7;
    m.SetUpTransitionCamera(la1, la0, s1, s0, 0, 0.5f);
    ok = ok && eq(m.StartPos, 10, -20, 40) && eq(m.LookAtPos, 5, 10, 3) && eq(m.Increment, 0, 0, 0)
            && eq(m.LookAtIncrement, 0, 0, 0) && m.EndFrame == 7 && m.FOV == 0.5f && m.PreventSelfTermination;
    if (!ok) { std::printf("00851ed0_TEST FAIL\n"); return 1; }
    std::printf("00851ed0_TEST PASS\n");
    return 0;
}