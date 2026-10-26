// CEngineWeatherRenderer::AddWeatherMask(const CWeatherMask& mask, long unused) -- copy the 92-byte mask into the ring slot CurrentMask and advance the slot index modulo 3
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B50C80, length 59 bytes.
// Callees: none. Shape: the slot address is `CurrentMask * 0x5c + this + 0x66c` (imul by the element size, lea with the array base), the
// POD mask is copied with `rep movsd` (23 words), then CurrentMask is re-read, incremented and reduced with a signed `idiv` by 3 (the
// remainder in edx is stored). `ret 8`: the second dword argument is never read. Layout from engine/CEngineWeatherRenderer.h
// (WeatherMaskList +0x66c = CWeatherMask[3], CurrentMask +0x780) and the CWeatherMask PDB record (bounding sphere, box, inverse
// transform, scale: 92 bytes).
class CWeatherMask { public:                              // 92 bytes, PDB: BSphere +0, BBox +0x10, InvTransform +0x28, Scale +0x58
    float BSphere[4];
    float BBox[6];
    float InvTransform[12];
    float Scale;
};

class CEngineWeatherRenderer { public:
    unsigned char _pad0[0x66c];
    CWeatherMask  WeatherMaskList[3];              // +0x66c
    long          CurrentMask;                     // +0x780
    void AddWeatherMask(const CWeatherMask& mask, long unused);
};

void CEngineWeatherRenderer::AddWeatherMask(const CWeatherMask& mask, long unused)
{
    WeatherMaskList[CurrentMask] = mask;
    CurrentMask = (CurrentMask + 1) % 3;
}