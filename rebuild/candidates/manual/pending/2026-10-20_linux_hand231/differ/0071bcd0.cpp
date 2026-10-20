// Grade: DIFFER(298v298) -- frame layout only: instruction stream identical, retail packs both Lock temporaries and the GetDimensions temporaries into one 0x20 frame (ours 0x38, Lock temps get their own slots)
// CTCHeroMorph::CreateAlphaChannel(CTexture& texture, CTexture& mask) const -- for two same-sized textures, makes every texel of `texture` whose RGB equals the matching `mask` texel fully transparent (alpha byte 0)
// Leaf: CreateAlphaChannel
//
// Retail 0x0071BCD0, 298 bytes (`ret 8`). Ego_r.pdb `?CreateAlphaChannel@CTCHeroMorph@@ABEXAAVCTexture@@0@Z`.
// Callees: CTexture::Lock(ulong, ulong) -> CSurfaceLockedRect @ 0x009FA450 (both textures, level 0, flags 0),
// CTexture::GetDimensions(ulong) const -> C2DExtentsI @ 0x009F9DB0 (four times for the size test, twice more for the
// texel count), CTexture::Unlock(ulong) @ 0x009F9DE0 (both, only on the same-size path).
// Layout: CSurfaceLockedRect {Dimensions {X, Y}, Pitch, PMem} (16 bytes, returned through a hidden pointer and copied to a
// local, PMem kept in a register); C2DExtentsI {X, Y}.
// Shape: the size test re-queries GetDimensions per field (X of both, then Y of both); the texel loop walks 4-byte texels,
// comparing bytes 0..2 and clearing byte 3. `this` is never used.
struct C2DExtentsI {
    long X;
    long Y;
};
struct CSurfaceLockedRect {
    C2DExtentsI Dimensions;
    long Pitch;
    unsigned char* PMem;
};
class CTexture {
public:
    CSurfaceLockedRect Lock(unsigned long level, unsigned long flags);      // 0x009FA450
    C2DExtentsI GetDimensions(unsigned long level) const;                   // 0x009F9DB0
    void Unlock(unsigned long level);                                       // 0x009F9DE0
};
static __forceinline unsigned char* LockedMemory(CSurfaceLockedRect rect) { return rect.PMem; }
class CTCHeroMorph {
public:
    void CreateAlphaChannel(CTexture& texture, CTexture& mask) const;
};

void CTCHeroMorph::CreateAlphaChannel(CTexture& texture, CTexture& mask) const
{
    unsigned char* textureMem = LockedMemory(texture.Lock(0, 0));
    unsigned char* maskMem = LockedMemory(mask.Lock(0, 0));
    if (texture.GetDimensions(0).X == mask.GetDimensions(0).X &&
        texture.GetDimensions(0).Y == mask.GetDimensions(0).Y) {
        long count = texture.GetDimensions(0).X * texture.GetDimensions(0).Y;
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
