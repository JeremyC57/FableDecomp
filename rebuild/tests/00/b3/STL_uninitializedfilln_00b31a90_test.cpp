#include <cstdio>
#include <new>

// Model of std::__uninitialized_fill_n over CEngineCamera::CDebugScriptElement: n placement copies, returns first + n.
class C3DVector { public:
    float X, Y, Z;
    C3DVector() : X(0), Y(0), Z(0) { }
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) { }
};
struct CDebugScriptElement {
    C3DVector WorldPos;
    C3DVector LookVector;
    float FOV;
    float TimeToNextElement;
};
static CDebugScriptElement* FillN(CDebugScriptElement* first, unsigned int n, const CDebugScriptElement& x)
{
    CDebugScriptElement* cur = first;
    for (; n > 0; --n, ++cur)
        new (cur) CDebugScriptElement(x);
    return cur;
}

int main()
{
    CDebugScriptElement x;
    x.WorldPos.X = 1.0f; x.WorldPos.Y = 2.0f; x.WorldPos.Z = 3.0f;
    x.LookVector.X = 4.0f; x.LookVector.Y = 5.0f; x.LookVector.Z = 6.0f;
    x.FOV = 7.0f; x.TimeToNextElement = 8.0f;
    CDebugScriptElement buf[4];
    for (int i = 0; i < 4; ++i) buf[i].FOV = -1.0f;
    CDebugScriptElement* end = FillN(buf, 3, x);
    if (end != buf + 3) { std::printf("00b31a90_TEST FAIL\n"); return 1; }
    for (int i = 0; i < 3; ++i)
        if (buf[i].WorldPos.Y != 2.0f || buf[i].LookVector.Z != 6.0f || buf[i].FOV != 7.0f || buf[i].TimeToNextElement != 8.0f) { std::printf("00b31a90_TEST FAIL\n"); return 1; }
    if (buf[3].FOV != -1.0f || FillN(buf, 0, x) != buf) { std::printf("00b31a90_TEST FAIL\n"); return 1; }
    std::printf("00b31a90_TEST PASS\n");
    return 0;
}