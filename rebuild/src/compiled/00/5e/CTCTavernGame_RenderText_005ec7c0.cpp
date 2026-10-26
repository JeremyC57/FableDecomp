// CTCTavernGame::RenderText(const CEnginePrimitivePrepareInfo&, float x, float y, CWideString text, const CFontBank& font, CRGBColour colour) -- queues one 2D text primitive at a screen-fraction position: preloads the glyphs, adds a primitive handle and hands the engine the text primitive on layer 0x45
// Leaf: RenderText
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005EC7C0, 220 bytes (`ret 18h`: six argument dwords; the by-value text is
// destroyed by the callee). Ego_r.pdb `?RenderText@CTCTavernGame@@IAEXABVCEnginePrimitivePrepareInfo@@MMVCWideString@@
// ABVCFontBank@@VCRGBColour@@@Z`.
// Callees: CFontBank::Preload(const CWideString&, bool) const (vtable slot 4, `call [eax+10h]`; Ego_r.pdb
//          CStreamingFontBank::Preload); the handle vector's append-a-default-handle helper 0x005ED940 (no arguments;
//          builds a CEnginePrimitiveHandle and pushes it); CDisplayManager::GetRenderTargetDimensions() 0x009BEDC0 on
//          g_pDisplayDevice 0x013B8390 (const C2DExtentsI by hidden pointer); CWideString(const CWideString&) 0x0099B720;
//          the engine's primitive setter (CIEngine vtable slot 23, `call [edx+5Ch]`, on the prepare info's +0x34 pointer,
//          flags 0xC0; the landed NPlayerGui::CDraw2DText::Draw 0x006541D1 makes the same call); ~CWideString 0x0099B510 x2.
// Layout: retail CTCTavernGame::Handles +0x44 (8-byte CEnginePrimitiveHandle elements; local model -- engine/
//         CTCTavernGame.h carries the Ego_r.pdb offsets). CEnginePrimitive2DText (PDB 0x2c bytes): Type +0, Interpolation
//         +4, ThemeChannelDesc +8 (three bytes cleared), Pos +0xc, Layer +0x18, Scale +0x1c, Colour +0x20, Text +0x24,
//         PFont +0x28.
// Shape: the primitive is a frame local built through its inline PDB constructor (the leading `unsigned __int64` id is
//        not stored); Pos sits in the initialiser list through the 3-float C3DVector constructor, so the scaled
//        position (screen fraction times the render-target size) is stored straight from the x87 and the C2DVector
//        argument temporary vanishes; the by-value colour is copied through CRGBColour's user copy constructor
//        calling an inline Set(r, g, b, a) (all four bytes loaded first, stored byte by byte); the font pointer is
//        stored last, after the call's pushes; `Handles.back()` is `*(end() - 1)`.
#include <stddef.h>

class CWideString { public:
    void* m_data;
    CWideString(const CWideString& other);      // 0x0099B720
    ~CWideString();                             // 0x0099B510
};
class CRGBColour { public:
    unsigned char B, G, R, A;
    CRGBColour(const CRGBColour& other) { Set(other.R, other.G, other.B, other.A); }
    void Set(unsigned char r, unsigned char g, unsigned char b, unsigned char a) { R = r; G = g; B = b; A = a; }
};
class C2DVector { public:
    float X, Y;
    C2DVector(float x, float y) : X(x), Y(y) {}
};
class C3DVector { public:
    float X, Y, Z;
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
};
class C2DExtentsI { public:
    long X, Y;
    C2DExtentsI() {}
};
class CDisplayManager { public:
    const C2DExtentsI GetRenderTargetDimensions();   // 0x009BEDC0
};
extern CDisplayManager* g_pDisplayDevice;      // 0x013B8390

class CFontBank { public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03();
    virtual bool Preload(const CWideString& text, bool wait) const;   // slot 4
};

class CEnginePrimitiveThemeChannelDesc { public:
    unsigned char ChannelIndex0, ChannelIndex1, ChannelBlendFactor, Pad;
    CEnginePrimitiveThemeChannelDesc() : ChannelIndex0(0), ChannelIndex1(0), ChannelBlendFactor(0) {}
};
class CEnginePrimitive { public:
    long Type;                                  // +0x00 EEnginePrimitiveType
    long InterpolationSet;                      // +0x04 EEngineInterpolationSet
    CEnginePrimitiveThemeChannelDesc ThemeChannelDesc;   // +0x08
    CEnginePrimitive(long type, long interpolationSet) : Type(type), InterpolationSet(interpolationSet) {}
};
class CEnginePrimitive2DText : public CEnginePrimitive { public:
    C3DVector    Pos;                           // +0x0c
    long         Layer;                         // +0x18 EEngine2DLayer
    float        Scale;                         // +0x1c
    CRGBColour   Colour;                        // +0x20
    CWideString  Text;                          // +0x24
    const CFontBank* PFont;                     // +0x28
    CEnginePrimitive2DText(unsigned __int64, const C2DVector& pos, long layer, float scale, const CWideString& text,
                           const CFontBank& font, const CRGBColour& colour, long interpolationSet)
        : CEnginePrimitive(0x27, interpolationSet), Pos(pos.X, pos.Y, 0.0f), Layer(layer), Scale(scale), Colour(colour),
          Text(text), PFont(&font)
    {
    }
};

class CEnginePrimitiveHandle { public:                 // 8 bytes
    void* _vtable;
    void* Data;
};
struct CHandleVector {                          // STLport vector<CEnginePrimitiveHandle> (12 bytes)
    CEnginePrimitiveHandle* _M_start;
    CEnginePrimitiveHandle* _M_finish;
    CEnginePrimitiveHandle* _M_end_of_storage;
    CEnginePrimitiveHandle* end() { return _M_finish; }
    CEnginePrimitiveHandle& back() { return *(end() - 1); }
    void AddDefaultHandle_005ed940();           // 0x005ED940
};

class CIEngine { public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17();
    virtual void v18(); virtual void v19(); virtual void v20(); virtual void v21(); virtual void v22();
    virtual void AddPrimitive(CEnginePrimitiveHandle* handle, const CEnginePrimitive2DText* primitive, unsigned long flags);   // slot 23
};
class CEnginePrimitivePrepareInfo { public:
    unsigned char _pad[0x34];                   // CInterpolationInfo Interpolate
    CIEngine*     Engine;                       // +0x34
};

class CTCTavernGame { public:
    void*         __vfptr;                      // +0x00 (CTCBase)
    void*         ParentThing;                  // +0x04
    unsigned char _pad08[0x44 - 0x08];
    CHandleVector Handles;                      // +0x44
    void RenderText(const CEnginePrimitivePrepareInfo& info, float x, float y, CWideString text, const CFontBank& font,
                    CRGBColour colour);
};

void CTCTavernGame::RenderText(const CEnginePrimitivePrepareInfo& info, float x, float y, CWideString text,
                               const CFontBank& font, CRGBColour colour)
{
    font.Preload(text, false);
    Handles.AddDefaultHandle_005ed940();
    C2DExtentsI size = g_pDisplayDevice->GetRenderTargetDimensions();
    CEnginePrimitive2DText primitive(0, C2DVector(size.X * x, size.Y * y), 0x45, 1.0f, text, font, colour, 0);
    info.Engine->AddPrimitive(&Handles.back(), &primitive, 0xc0);
}