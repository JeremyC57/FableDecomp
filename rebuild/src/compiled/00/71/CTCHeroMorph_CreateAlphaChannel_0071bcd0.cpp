// CTCHeroMorph::CreateAlphaChannel(CTexture& texture, CTexture& mask) const -- for two same-sized textures, makes every texel of `texture` whose RGB equals the matching `mask` texel fully transparent (alpha byte 0)
// Leaf: CreateAlphaChannel
// Retail: Fable.exe (TLC, VC7.1) @ 0x0071BCD0, 298 bytes (`ret 8`). Ego_r.pdb ?CreateAlphaChannel@CTCHeroMorph@@ABEXAAVCTexture@@0@Z
//   (private const).
// Callees: CTexture::Lock(unsigned long, unsigned long) -> CSurfaceLockedRect @ 0x009FA450 (both textures, level 0, flags 0),
//   CTexture::GetDimensions(unsigned long) const -> C2DExtentsI @ 0x009F9DB0 (four times for the size test, twice for the texel
//   count, through the inline GetWidth/GetHeight), CTexture::Unlock(unsigned long) @ 0x009F9DE0 (both, only on the same-size path). Spellings as Ego_r.pdb and the
//   bound ?Lock@CTexture@@QAE?AVCSurfaceLockedRect@@KK@Z / ?GetDimensions@CTexture@@QBE?AVC2DExtentsI@@K@Z / ?Unlock@CTexture@@QAEXK@Z.
// Layout: C2DExtentsI {X, Y} and CSurfaceLockedRect {Dimensions, Pitch, PMem} as Ego_r.pdb (both `class`). Ego_d.pdb emits
//   ??0C2DExtentsI@@QAE@XZ / @JJ@Z / @ABV0@@Z and ??0CSurfaceLockedRect@@QAE@XZ / @ABV0@@Z: C2DExtentsI has a user default and copy
//   constructor, which makes CSurfaceLockedRect's implicit ones non-trivial. CTexture / CTCHeroMorph carry only the methods used
//   (`this` of CTCHeroMorph is never read; no engine/CTCHeroMorph.h exists).
// Shape (what matched): the locked rects are default-constructed locals assigned from Lock (the copy stores X, Y, Pitch; PMem stays
//   in ebp/ebx). With C2DExtentsI's user constructors the Lock temporaries are packed with the GetDimensions temporaries into
//   retail's 0x20 frame (a POD C2DExtentsI gives each Lock temporary its own slot: 0x38). The size test and the texel count go
//   through the Ego_d.pdb inlines CTexture::GetWidth / GetHeight(unsigned long) const (`return GetDimensions(level).X / .Y`):
//   the count's two temporaries stay live together (slots +0x10 and +0) and the width is fetched first. Spelled with
//   GetDimensions(0).X / .Y directly, VC canonicalises the product to fetch Y first in either operand order. The texel loop walks
//   4-byte texels comparing bytes 0..2 and clearing byte 3.
class C2DExtentsI { public:
    long X;
    long Y;
    C2DExtentsI() { }
    C2DExtentsI(const C2DExtentsI& o) : X(o.X), Y(o.Y) { }
};
class CSurfaceLockedRect { public:
    C2DExtentsI    Dimensions;
    long           Pitch;
    unsigned char* PMem;
};
class CTexture { public:
    CSurfaceLockedRect Lock(unsigned long level, unsigned long flags);      // 0x009FA450
    C2DExtentsI GetDimensions(unsigned long level) const;                   // 0x009F9DB0
    void Unlock(unsigned long level);                                       // 0x009F9DE0
    unsigned long GetWidth(unsigned long level) const { return GetDimensions(level).X; }    // Ego_d.pdb inline
    unsigned long GetHeight(unsigned long level) const { return GetDimensions(level).Y; }   // Ego_d.pdb inline
};
class CTCHeroMorph { public:
private:
    void CreateAlphaChannel(CTexture& texture, CTexture& mask) const;
};

void CTCHeroMorph::CreateAlphaChannel(CTexture& texture, CTexture& mask) const
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