#include <cstdio>
#include <cstring>
#include <new>
template <class T> struct CArray { T* m_pFirst; T* m_pLast; T* m_pEnd; CArray() : m_pFirst(0), m_pLast(0), m_pEnd(0) { } };
struct CRGBFloatColour { float R, G, B, A; };
struct CIDrawEnvironmentTextureSet { CArray<long> Textures; CArray<float> Blends; };
struct CThemeChannel {
    CRGBFloatColour Colours[19]; float Scalers[30]; CIDrawEnvironmentTextureSet TextureSets[3];
    CThemeChannel() { }
};
int main()
{
    static unsigned char raw[sizeof(CThemeChannel)];
    std::memset(raw, 0xAB, sizeof raw);
    CThemeChannel* c = new (raw) CThemeChannel;
    bool ok = true;
    for (int i = 0; i < 3; ++i)
        if (c->TextureSets[i].Textures.m_pFirst || c->TextureSets[i].Textures.m_pLast || c->TextureSets[i].Textures.m_pEnd ||
            c->TextureSets[i].Blends.m_pFirst || c->TextureSets[i].Blends.m_pLast || c->TextureSets[i].Blends.m_pEnd) ok = false;
    if (raw[0] != 0xAB || ((unsigned char*)&c->Scalers[29])[0] != 0xAB) ok = false;   // Colours / Scalers untouched
    if (!ok) { std::printf("006b5fb0_TEST FAIL\n"); return 1; }
    std::printf("006b5fb0_TEST PASS\n");
    return 0;
}