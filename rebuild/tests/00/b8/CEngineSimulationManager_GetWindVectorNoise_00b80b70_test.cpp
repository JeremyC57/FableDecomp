#include <stdio.h>
#include <math.h>
class C3DVector {
public:
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector operator*(float s) const { return C3DVector(X * s, Y * s, Z * s); }
    C3DVector operator+(const C3DVector& v) const { return C3DVector(X + v.X, Y + v.Y, Z + v.Z); }
    C3DVector& operator*=(float s) { X *= s; Y *= s; Z *= s; return *this; }
};
// noise stub: returns a value keyed on the sample's Z (time) so the two samples are distinguishable
static float g_noiseAtTime, g_noiseAtTimeMinus10;
static float Get3DNoise(const C3DVector& p) { return p.Z > 5.0f ? g_noiseAtTime : g_noiseAtTimeMinus10; }
struct Sim { float WindNoiseTime, WindNoiseOffset, WindNoisePerpendicular, MinWindMultiplier, MaxWindMultiplier, ClothWindFactor; C3DVector WindVector; };
static C3DVector GetWindVectorNoise(const Sim& s, const C3DVector& position)
{
    C3DVector wind = s.WindVector * s.ClothWindFactor;
    float noise = Get3DNoise(C3DVector(position.X + s.WindNoiseOffset, position.Y, s.WindNoiseTime));
    float minMultiplier = s.MinWindMultiplier;
    float multiplier = (noise + 1.0f) * (s.MaxWindMultiplier - minMultiplier) * 0.5f + minMultiplier;
    float perpendicular = Get3DNoise(C3DVector(position.X + s.WindNoiseOffset, position.Y, s.WindNoiseTime - 10.0f)) * s.WindNoisePerpendicular;
    wind = wind * multiplier + C3DVector(wind.Y, -wind.X, 0.0f) * perpendicular;
    wind *= s.ClothWindFactor;
    return wind;
}
static int Near(float a, float b) { return fabs(a - b) < 1e-4f; }
int main()
{
    Sim s; s.WindNoiseTime = 12.0f; s.WindNoiseOffset = 0.0f; s.WindNoisePerpendicular = 0.5f;
    s.MinWindMultiplier = 1.0f; s.MaxWindMultiplier = 3.0f; s.ClothWindFactor = 2.0f; s.WindVector = C3DVector(1.0f, 0.0f, 0.0f);
    g_noiseAtTime = 0.0f; g_noiseAtTimeMinus10 = 1.0f;    // multiplier 2, perpendicular 0.5
    C3DVector w = GetWindVectorNoise(s, C3DVector(0, 0, 0));
    // wind (2,0,0) -> *2 = (4,0,0) + (0,-2,0)*0.5 = (4,-1,0) -> *2 = (8,-2,0)
    int ok = Near(w.X, 8.0f) && Near(w.Y, -2.0f) && Near(w.Z, 0.0f);
    g_noiseAtTime = -1.0f; g_noiseAtTimeMinus10 = 0.0f;   // multiplier = min, no gust
    w = GetWindVectorNoise(s, C3DVector(0, 0, 0));
    ok = ok && Near(w.X, 4.0f) && Near(w.Y, 0.0f);
    if (!ok) { printf("00b80b70_TEST FAIL\n"); return 1; }
    printf("00b80b70_TEST PASS\n");
    return 0;
}