// Behaviour test for CEngineCamera::ConsoleAddDebugScript (0x00B317A0): eight console argument values (0x1C-byte
// CGenericVar records, ValFloat at +0x18) become world position, normalised look vector, FOV and time of one pushed
// debug-script element.
#include <cstdio>
#include <cmath>
struct Vec3 { float X, Y, Z; Vec3() {} Vec3(float x, float y, float z) : X(x), Y(y), Z(z) {}
    void Normalise() { float l = std::sqrt(X * X + Y * Y + Z * Z); X /= l; Y /= l; Z /= l; } };
struct GenericVar { unsigned char pad[0x18]; float ValFloat; };
struct GenericVarVector { GenericVar* Start; const GenericVar& operator[](unsigned n) const { return Start[n]; } };
struct DebugScriptElement { Vec3 WorldPos, LookVector; float FOV, TimeToNextElement; };
static DebugScriptElement g_elements[4]; static int g_n;
struct DebugScriptVector { void push_back(const DebugScriptElement& e) { g_elements[g_n++] = e; } };
struct Camera { DebugScriptVector DebugScript; };
static void ConsoleAddDebugScript(Camera& camera, const GenericVarVector& params)
{
    DebugScriptElement element;
    element.WorldPos = Vec3(params[0].ValFloat, params[1].ValFloat, params[2].ValFloat);
    element.LookVector = Vec3(params[3].ValFloat, params[4].ValFloat, params[5].ValFloat);
    element.FOV = params[6].ValFloat;
    element.TimeToNextElement = params[7].ValFloat;
    element.LookVector.Normalise();
    camera.DebugScript.push_back(element);
}
int main()
{
    GenericVar p[8]; float v[8] = { 1, 2, 3, 0, 3, 4, 7.5f, 2 };
    for (int i = 0; i < 8; ++i) p[i].ValFloat = v[i];
    GenericVarVector ps; ps.Start = p; Camera c;
    ConsoleAddDebugScript(c, ps);
    const DebugScriptElement& e = g_elements[0];
    if (g_n != 1 || e.WorldPos.X != 1 || e.WorldPos.Z != 3 || std::fabs(e.LookVector.Y - 0.6f) > 1e-5f || std::fabs(e.LookVector.Z - 0.8f) > 1e-5f || e.FOV != 7.5f || e.TimeToNextElement != 2) { std::printf("00b317a0_TEST FAIL\n"); return 1; }
    std::printf("00b317a0_TEST PASS\n");
    return 0;
}
