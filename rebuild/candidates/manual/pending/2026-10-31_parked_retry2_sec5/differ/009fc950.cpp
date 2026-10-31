// CGraphicFrameData::InitialiseAsSurface(const CCountedPointer<CTexture>& texture, unsigned long width, unsigned short height, bool alpha, bool booleanAlpha) -- make the frame a whole-texture surface: take the texture, set size / pitch, the alpha flags, initialised, origin 0,0, and flag a screen graphic when the size differs from the texture's level-0 dimensions; returns true
// Leaf: InitialiseAsSurface
// Grade: DIFFER(224v230) -- now on the shared engine/CCountedPointer.h (the parked local {Data, Info} model removed;
//   bytes unchanged): our build keeps the source Data word in ebx (retail spills it into the dead texture slot), folds
//   the dimension compares into cmp ecx,[eax] and materialises ScreenGraphic differently
// Retail: Fable.exe (TLC, VC7.1) @ 0x009FC950, 230 bytes, `ret 14h`.
// Callees: the old texture's count-block destroy (+4, object in ecx), operator delete @ 0x00BFE9BC,
//   CTexture::GetDimensions(unsigned long level) @ 0x009F9DB0 (landed; C2DExtentsI returned through a hidden pointer into
//   the 8-byte frame, called twice).
// Layout: Ego_r.pdb CGraphicFrameData (27 bytes): ReferenceCount +0, Data +4, PTexture +8 (CCountedPointer {Data, Info}),
//   TLX +0x10, TLY +0x12, Width +0x14, Height +0x16, Pitch +0x18, flag byte +0x1a (Alpha, BooleanAlpha, UsesSurface,
//   SectionOfTPage, IsInitialised, ScreenGraphic). No engine header exists for the class.
// Shape: UsesSurface is set first (its own read-modify-write); the counted-pointer assignment reads the source's Data
//   and Info into locals before releasing the old block (Data homed in the dead `texture` slot); the four flag bits fold
//   into one byte update; both dimension compares read the extents' first word (retail compares Height with .X too);
//   ScreenGraphic = !(both equal).
void __cdecl operator delete(void* block);         // 0x00BFE9BC

struct C2DExtentsI {
    long X;
    long Y;
    C2DExtentsI(long x, long y) : X(x), Y(y) { }
};

struct CTexture {
    C2DExtentsI GetDimensions(unsigned long level);   // 0x009F9DB0
};

#include "engine/CCountedPointer.h"

struct CGraphicFrameData {
    unsigned long             ReferenceCount;      // +0x00
    void*                     Data;                // +0x04
    CCountedPointer<CTexture> PTexture;            // +0x08
    unsigned short            TLX;                 // +0x10
    unsigned short            TLY;                 // +0x12
    unsigned short            Width;               // +0x14
    unsigned short            Height;              // +0x16
    unsigned short            Pitch;               // +0x18
    unsigned char             Alpha : 1;           // +0x1a
    unsigned char             BooleanAlpha : 1;
    unsigned char             UsesSurface : 1;
    unsigned char             SectionOfTPage : 1;
    unsigned char             IsInitialised : 1;
    unsigned char             ScreenGraphic : 1;
    bool MatchesTexture(const CCountedPointer<CTexture>& texture) const
    {
        if (Width != texture->GetDimensions(0).X)
            return false;
        return Height == texture->GetDimensions(0).X;
    }
    bool InitialiseAsSurface(const CCountedPointer<CTexture>& texture, unsigned long width, unsigned short height,
                             bool alpha, bool booleanAlpha);
};

bool CGraphicFrameData::InitialiseAsSurface(const CCountedPointer<CTexture>& texture, unsigned long width,
                                            unsigned short height, bool alpha, bool booleanAlpha)
{
    UsesSurface = true;
    PTexture = texture;
    Width = width;
    Pitch = width;
    Height = height;
    Alpha = alpha;
    BooleanAlpha = booleanAlpha;
    SectionOfTPage = false;
    IsInitialised = true;
    TLX = 0;
    TLY = 0;
    ScreenGraphic = !MatchesTexture(texture);
    return true;
}
