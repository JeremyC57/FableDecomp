// Behaviour test for CTCHeroMorph::CreateAlphaChannel @ 0x0071BCD0: same logic over in-memory model textures.
#include <cstdio>

class C2DExtentsI { public:
    long X, Y;
    C2DExtentsI() { }
    C2DExtentsI(const C2DExtentsI& o) : X(o.X), Y(o.Y) { }
};
class CSurfaceLockedRect { public:
    C2DExtentsI    Dimensions;
    long           Pitch;
    unsigned char* PMem;
};
class CTexture { public:
    long w, h;
    unsigned char* bits;
    int locks, unlocks;
    CSurfaceLockedRect Lock(unsigned long, unsigned long) { CSurfaceLockedRect r; r.Dimensions.X = w; r.Dimensions.Y = h; r.Pitch = w * 4; r.PMem = bits; ++locks; return r; }
    C2DExtentsI GetDimensions(unsigned long) const { C2DExtentsI d; d.X = w; d.Y = h; return d; }
    void Unlock(unsigned long) { ++unlocks; }
    unsigned long GetWidth(unsigned long level) const { return GetDimensions(level).X; }
    unsigned long GetHeight(unsigned long level) const { return GetDimensions(level).Y; }
};
class CTCHeroMorph { public:
    void CreateAlphaChannel(CTexture& texture, CTexture& mask) const
    {
        CSurfaceLockedRect textureRect;
        textureRect = texture.Lock(0, 0);
        CSurfaceLockedRect maskRect;
        maskRect = mask.Lock(0, 0);
        unsigned char* textureMem = textureRect.PMem;
        unsigned char* maskMem = maskRect.PMem;
        if (texture.GetWidth(0) == mask.GetWidth(0) && texture.GetHeight(0) == mask.GetHeight(0)) {
            long count = texture.GetWidth(0) * texture.GetHeight(0);
            for (long i = 0; i < count; ++i) {
                if (textureMem[i * 4 + 0] == maskMem[i * 4 + 0] &&
                    textureMem[i * 4 + 1] == maskMem[i * 4 + 1] &&
                    textureMem[i * 4 + 2] == maskMem[i * 4 + 2])
                    textureMem[i * 4 + 3] = 0;
            }
            texture.Unlock(0);
            mask.Unlock(0);
        }
    }
};

int main()
{
    unsigned char t[2 * 2 * 4] = { 1,2,3,255,  4,5,6,255,  7,8,9,255,  10,11,12,255 };
    unsigned char m[2 * 2 * 4] = { 1,2,3,0,    4,5,7,0,    7,8,9,0,    0,11,12,0 };
    CTexture tex = { 2, 2, t, 0, 0 };
    CTexture msk = { 2, 2, m, 0, 0 };
    CTCHeroMorph morph;
    morph.CreateAlphaChannel(tex, msk);
    bool ok = t[3] == 0 && t[7] == 255 && t[11] == 0 && t[15] == 255 && tex.unlocks == 1 && msk.unlocks == 1;
    // different size: nothing touched, nothing unlocked (both still locked)
    unsigned char t2[4] = { 1, 2, 3, 200 };
    CTexture tex2 = { 1, 1, t2, 0, 0 };
    morph.CreateAlphaChannel(tex2, msk);
    ok = ok && t2[3] == 200 && tex2.locks == 1 && tex2.unlocks == 0 && msk.unlocks == 1;
    if (!ok) { std::printf("0071bcd0_TEST FAIL\n"); return 1; }
    std::printf("0071bcd0_TEST PASS\n");
    return 0;
}