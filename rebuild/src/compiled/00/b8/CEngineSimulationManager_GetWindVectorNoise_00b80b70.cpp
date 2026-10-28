// CEngineSimulationManager::GetWindVectorNoise(const C3DVector& position) const -- the cloth wind at a world position: the manager's wind vector scaled by the cloth wind factor, its strength modulated by a Perlin noise sample between the min and max wind multipliers, plus a perpendicular gust from a second noise sample, all scaled by the cloth wind factor again
// Leaf: ?GetWindVectorNoise@CEngineSimulationManager@@QBE?AVC3DVector@@ABV2@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B80B70, length 364 bytes, `ret 8` (hidden return pointer + position).
// Name: Ego_r.pdb + FableWin.pdb `?GetWindVectorNoise@CEngineSimulationManager@@QBE?AVC3DVector@@ABV2@@Z`; the catalog label
//   `_global::ComputeWindVector_Impl` and the binding `?GetWindAt@CSimulationEnvironment@@QAE?AVC3DVector@@ABV2@@Z` are wrong
//   (NAME_FIXES.tsv). Members (Ego_r.pdb names, engine/CEngineSimulationManager.h offsets): PerlinNoise +0x28,
//   WindNoiseTime +0x1028, WindNoiseOffset +0x102C, WindNoisePerpendicular +0x1034, MinWindMultiplier +0x1038,
//   MaxWindMultiplier +0x103C, ClothWindFactor +0x1040, WindVector +0x1054.
// Callee: 0x00F361D0 ?Get3DNoise@CPerlinNoise@NFractal@@QBEMABVC3DVector@@@Z (Ego_r.pdb), called twice with ecx = this+0x28.
// Constants: 1.0f @ 0x0122DED8, 0.5f @ 0x0122F59C, 10.0f @ 0x01237F0C, 0.0f @ 0x0122DEDC.
// Shape: MinWindMultiplier is read once into a local (retail loads it first and reuses st(1)); the final scale is the
//   in-place `*=` (retail copies ClothWindFactor through edx into the dead position slot and reloads it per component).
class C3DVector {
public:
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector(const C3DVector& v) : X(v.X), Y(v.Y), Z(v.Z) { }
    C3DVector operator*(float s) const { return C3DVector(X * s, Y * s, Z * s); }
    C3DVector operator+(const C3DVector& v) const { return C3DVector(X + v.X, Y + v.Y, Z + v.Z); }
    C3DVector& operator*=(float s) { X *= s; Y *= s; Z *= s; return *this; }
};
namespace NFractal {
class CPerlinNoise {
public:
    float Get3DNoise(const C3DVector& position) const;   // 0x00F361D0
};
}
#define CEngineSimulationManager CEngineSimulationManagerLayout
#include "engine/CEngineSimulationManager.h"
#undef CEngineSimulationManager
class CEngineSimulationManager : public CEngineSimulationManagerLayout {
public:
    C3DVector GetWindVectorNoise(const C3DVector& position) const;
private:
    const NFractal::CPerlinNoise& GetPerlinNoise() const { return *reinterpret_cast<const NFractal::CPerlinNoise*>(PerlinNoise); }
    const C3DVector& GetWindVector() const { return *reinterpret_cast<const C3DVector*>(&WindVector_X); }
};

C3DVector CEngineSimulationManager::GetWindVectorNoise(const C3DVector& position) const
{
    C3DVector wind = GetWindVector() * ClothWindFactor;
    float noise = GetPerlinNoise().Get3DNoise(C3DVector(position.X + WindNoiseOffset, position.Y, WindNoiseTime));
    float minMultiplier = MinWindMultiplier;
    float multiplier = (noise + 1.0f) * (MaxWindMultiplier - minMultiplier) * 0.5f + minMultiplier;
    float perpendicular = GetPerlinNoise().Get3DNoise(C3DVector(position.X + WindNoiseOffset, position.Y, WindNoiseTime - 10.0f)) * WindNoisePerpendicular;
    wind = wind * multiplier + C3DVector(wind.Y, -wind.X, 0.0f) * perpendicular;
    wind *= ClothWindFactor;
    return wind;
}