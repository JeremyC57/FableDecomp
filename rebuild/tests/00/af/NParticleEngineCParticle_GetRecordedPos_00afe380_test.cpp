#include <cstdio>
#ifdef _MSC_VER
#pragma optimize("y", off)
__forceinline long CeilToLong(float value)
{
    long result; float one; float half; float back;
    half = 0.5f; one = 1.0f;
    __asm {
        fld value
        fadd half
        fistp result
        fild result
        fsub one
        fstp back
        mov eax, value
        cmp eax, back
        jne done
        dec result
    done:
    }
    return result;
}
#else
#include <math.h>
inline long CeilToLong(float value) { return (long)ceil((double)value); }   // the FPU block is ceil() for the magnitudes used here
#endif
class C3DVector { public:
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector operator-(const C3DVector& v) const { return C3DVector(X - v.X, Y - v.Y, Z - v.Z); }
    C3DVector operator+(const C3DVector& v) const { return C3DVector(X + v.X, Y + v.Y, Z + v.Z); }
    C3DVector operator*(float f) const { return C3DVector(X * f, Y * f, Z * f); }
};
struct CParticle {
    C3DVector PositionBuffer[4];
    C3DVector GetRecordedPos(float time, float offset) const
    {
        float pos = time + 1.0f - offset;
        long index = CeilToLong(pos);
        if (index < 1) index = 1; else if (index > 3) index = 3;
        float frac = (float)index - pos;
        const C3DVector& newer = PositionBuffer[index - 1];
        const C3DVector& older = PositionBuffer[index];
        return older + (newer - older) * frac;
    }
};
static bool Near(float a, float b) { float d = a - b; return d > -0.001f && d < 0.001f; }
int main()
{
    CParticle p;
    for (int i = 0; i < 4; ++i) { p.PositionBuffer[i].X = (float)(10 * i); p.PositionBuffer[i].Y = (float)i; p.PositionBuffer[i].Z = 0.0f; }
    C3DVector r = p.GetRecordedPos(0.5f, 0.0f);          // pos 1.5 -> index 2, frac 0.5 -> between buffer[1] (10) and [2] (20): 15
    bool ok = Near(r.X, 15.0f) && Near(r.Y, 1.5f);
    r = p.GetRecordedPos(0.0f, 0.75f);                    // pos 0.25 -> index 1 (clamped), frac 0.75 -> buffer[1] + (buffer[0]-buffer[1])*0.75 = 2.5
    ok = ok && Near(r.X, 2.5f);
    r = p.GetRecordedPos(5.0f, 0.0f);                     // pos 6 -> index 3 (clamped), frac -3 -> buffer[3] + (buffer[2]-buffer[3])*-3 = 60
    ok = ok && Near(r.X, 60.0f);
    std::printf(ok ? "00afe380_TEST PASS\n" : "00afe380_TEST FAIL\n");
    return ok ? 0 : 1;
}