// CSurface::CanDoProperSignedConversion(const C2DBoxI* destinationBox, const C2DBoxI* sourceBox, const CPixelFormat& sourceFormat) -- the signed-surface fast copy applies only when one side is signed, both boxes (when given) cover the whole surface, the source is an unsigned 4-byte format and this surface a signed 2-byte one
// Leaf: CanDoProperSignedConversion
// Retail: Fable.exe (TLC, VC7.1) @ 0x009F35D0, 271 bytes (+1 int3 padding), `ret 0Ch`. Called from CopyFromSurface
//   0x009F37E0 (landed; its extern declaration gives the argument order).
// Callees: IDirect3DSurface9::GetDesc (COM slot 12, `call [ecx+30h]`, `this` pushed), CPixelFormat::CPixelFormat
//   (D3DFORMAT) 0x009E3830, CPixelFormat::IsSigned() const 0x009E3970, CPixelFormat::GetByteColourDepth() const
//   0x009E3800, CSurface::GetWidth() const 0x00415BD0 / GetHeight() const 0x00415BF0 (both `GetDimensions().X/Y`),
//   CSurface::GetPixelFormat() const 0x009F2F60 (by value).
// Layout: engine/CSurface.h (+4 PD3DSurface); CPixelFormat is one FormatIndex word.
// Shape: VC7.1's inline-once band: this surface's format is the first GetPixelFormat() call (expanded inline: the
//   D3DSURFACE_DESC frame and the in-place constructor), the destination box's width check the first GetWidth() call
//   (expanded inline through GetDimensions, re-using the desc frame); every later call goes out of line, so the
//   expanded first calls are written through inline helpers (the landed 0x00535CD0-band rule of chunk 225). The two
//   late GetPixelFormat() temporaries are built in the dead `sourceFormat` slot. The success path is NESTED (`if (ok)
//   { ... return true; } return false;`): it keeps the callee-saved pushes at entry and puts the `mov al,1` epilogue
//   before the shared `xor al,al` one (flat early returns shrink-wrap the first test; a do/while(0) + break form puts
//   the false exit first).
#include "engine/CSurface.h"

struct D3DSURFACE_DESC_Local {                     // D3DSURFACE_DESC, 0x20 bytes
    unsigned long Format;                          // +0x00 D3DFORMAT
    unsigned long Type;                            // +0x04
    unsigned long Usage;                           // +0x08
    unsigned long Pool;                            // +0x0c
    unsigned long MultiSampleType;                 // +0x10
    unsigned long MultiSampleQuality;              // +0x14
    unsigned long Width;                           // +0x18
    unsigned long Height;                          // +0x1c
};

struct IDirect3DSurface9 {
    virtual void __stdcall v00(); virtual void __stdcall v04(); virtual void __stdcall v08(); virtual void __stdcall v0c();
    virtual void __stdcall v10(); virtual void __stdcall v14(); virtual void __stdcall v18(); virtual void __stdcall v1c();
    virtual void __stdcall v20(); virtual void __stdcall v24(); virtual void __stdcall v28(); virtual void __stdcall v2c();
    virtual long __stdcall GetDesc(D3DSURFACE_DESC_Local* desc);   // slot 12 (+0x30)
};

struct C2DBoxI { long left, top, right, bottom; };
struct C2DExtentsI {
    long X;
    long Y;
    C2DExtentsI(long x, long y) : X(x), Y(y) {}
};

struct CPixelFormat {
    long FormatIndex;                              // +0x00
    CPixelFormat(unsigned long d3dFormat);         // 0x009E3830
    bool IsSigned() const;                         // 0x009E3970
    unsigned long GetByteColourDepth() const;      // 0x009E3800
};

struct CSurface_Methods : CSurface {
    CPixelFormat GetPixelFormat() const;           // 0x009F2F60
    unsigned long GetWidth() const;                // 0x00415BD0
    unsigned long GetHeight() const;               // 0x00415BF0
    // the inline expansions of the first GetPixelFormat() / GetWidth() calls
    CPixelFormat GetPixelFormatInline() const
    {
        D3DSURFACE_DESC_Local desc;
        PD3DSurface->GetDesc(&desc);
        return CPixelFormat(desc.Format);
    }
    C2DExtentsI GetDimensionsInline() const
    {
        D3DSURFACE_DESC_Local desc;
        PD3DSurface->GetDesc(&desc);
        return C2DExtentsI(desc.Width, desc.Height);
    }
    unsigned long GetWidthInline() const { return GetDimensionsInline().X; }
    bool CanDoProperSignedConversion(const C2DBoxI* destinationBox, const C2DBoxI* sourceBox, const CPixelFormat& sourceFormat);
};

bool CSurface_Methods::CanDoProperSignedConversion(const C2DBoxI* destinationBox, const C2DBoxI* sourceBox,
                                                   const CPixelFormat& sourceFormat)
{
    CPixelFormat format = GetPixelFormatInline();
    if (format.IsSigned() || sourceFormat.IsSigned()) {
        if (!destinationBox || (destinationBox->left == 0 && destinationBox->top == 0
                                && destinationBox->right == GetWidthInline() && destinationBox->bottom == GetHeight())) {
            if (!sourceBox || (sourceBox->left == 0 && sourceBox->top == 0
                               && sourceBox->right == GetWidth() && sourceBox->bottom == GetHeight())) {
                if (!sourceFormat.IsSigned() && sourceFormat.GetByteColourDepth() == 4
                    && GetPixelFormat().IsSigned() && GetPixelFormat().GetByteColourDepth() == 2)
                    return true;
            }
        }
    }
    return false;
}
