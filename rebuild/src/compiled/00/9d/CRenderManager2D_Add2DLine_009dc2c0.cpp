// CRenderManager2D::Add2DLine(const CTVertexRHWColSpecTex1& v0, const CTVertexRHWColSpecTex1& v1, unsigned long blendMode, const CTexture* texture) -- queue a screen-space line: both vertices converted to viewport space (x and y to -1..1, z 0, rhw 1) on the vertex list, then a Line record (window, blend mode, texture) on the quick-draw list, unless the viewport is offscreen
// Leaf: Add2DLine
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009DC2C0, length 509 bytes (`ret 10h`).
// Callees: CRenderManagerCore::GetViewportBoxFloat @ 0x00A0AAA0 (manifest "CVertexBufferWin32::DoSizeof"; once per
// converted coordinate and once for the window), the vertex vector's _M_insert_overflow @ 0x009E14B0 (manifest
// "VectorInsertRange"), CWideString() @ 0x0099AED0, CTexture::operator= @ 0x009FA1C0, CQuickDrawTriInfo copy ctor
// @ 0x009DE890 (landed), the record vector's _M_insert_overflow @ 0x009E1750 (manifest "CCombatWheel_ReallocTriArray"),
// CTexture::Uninitialise @ 0x009F9F70 (inline ~CTexture), ~CWideString @ 0x0099B510.
// Layout: as the landed AddText 0x009DB700 (DisplayManager +0x3A3C, ViewportOffscreen +0x1D8, the 12-byte STLport
// QuickDrawTriInfo vector +0x3E94, CQuickDrawTriInfo / CTexture models); the 32-byte vertex vector sits just before it
// at +0x3E88. The record's +0x20 union is {BlendMode, ZAverage} here (StringX/StringY for text).
// Shape: one vertex local (rep movsd copy) reused for both ends; each coordinate goes through an inline helper that
// takes the coordinate by const reference (a by-value float parameter adds an integer copy through the dead argument
// slot) and calls GetViewportBoxFloat once: x / width * 2 - 1 (`fadd st,st`), 1 - y / height * 2; z 0, rhw 1. Inline
// STLport push_backs with the `__true_type` tag in the dead argument slot. The record: blend mode, the texture copy
// when one is given, the window, Line set / IsText cleared, pushed, then its destructors.
class CTVertexRHWColSpecTex1 { public:                         // 32 bytes
    float X, Y, Z, RHW;
    unsigned long Diffuse;
    unsigned long Specular;
    float U, V;
};
class CRGBColour { public: unsigned char B, G, R, A; };
class CFontBank;
struct __true_type { __true_type() { } };

inline void* operator new(unsigned int, void* where) { return where; }


class C2DBoxF { public:
    float TLX, TLY, BRX, BRY;
};

class CWideString { public:
    void* m_pData;
    CWideString();                                      // 0x0099AED0
    ~CWideString();                                     // 0x0099B510
    const CWideString& operator=(const CWideString& other);   // 0x0099B7D0
};

struct IDirect3DTexture9;
class CTexture { public:
    IDirect3DTexture9* PD3DTexture;                     // +0x00
    unsigned long      ByteLength : 28;                 // +0x04
    unsigned long      AllocationSource : 4;
    CTexture& operator=(const CTexture& other);         // 0x009FA1C0
    CTexture() : PD3DTexture(0) { ByteLength = 0; AllocationSource = 0; }
    ~CTexture() { Uninitialise(); }
    void Uninitialise();                                // 0x009F9F70
};

class CQuickDrawTriInfo { public:
    unsigned char Line   : 1;                           // +0x00 bit 0
    unsigned char IsText : 1;                           // +0x00 bit 1
    C2DBoxF     Window;                                 // +0x04
    CWideString String;                                 // +0x14
    CTexture    UploadedTexture;                        // +0x18
    unsigned long BlendMode;                            // +0x20 (union with StringX)
    float       ZAverage;                               // +0x24 (union with StringY)
    float       StringZ;                                // +0x28
    float       StringScale;                            // +0x2c
    unsigned long StringFlags;                          // +0x30
    CRGBColour  Colour;                                 // +0x34
    CFontBank*  PFont;                                  // +0x38
    CQuickDrawTriInfo() { }
    CQuickDrawTriInfo(const CQuickDrawTriInfo& other);  // 0x009DE890
};

struct CQuickDrawTriInfoVector {                        // STLport vector<CQuickDrawTriInfo>
    CQuickDrawTriInfo* _M_start;
    CQuickDrawTriInfo* _M_finish;
    CQuickDrawTriInfo* _M_end_of_storage;
    void _M_insert_overflow(CQuickDrawTriInfo* position, const CQuickDrawTriInfo& value, const __true_type&,
                            unsigned int count, bool atEnd);     // 0x009E1750
    void push_back(const CQuickDrawTriInfo& value)
    {
        if (_M_finish != _M_end_of_storage) {
            new (_M_finish) CQuickDrawTriInfo(value);
            ++_M_finish;
        }
        else
            _M_insert_overflow(_M_finish, value, __true_type(), 1, true);
    }
};
struct CVertexVector {                                  // STLport vector<CTVertexRHWColSpecTex1>
    CTVertexRHWColSpecTex1* _M_start;
    CTVertexRHWColSpecTex1* _M_finish;
    CTVertexRHWColSpecTex1* _M_end_of_storage;
    void _M_insert_overflow(CTVertexRHWColSpecTex1* position, const CTVertexRHWColSpecTex1& value, const __true_type&,
                            unsigned int count, bool atEnd);     // 0x009E14B0
    void push_back(const CTVertexRHWColSpecTex1& value)
    {
        if (_M_finish != _M_end_of_storage) {
            new (_M_finish) CTVertexRHWColSpecTex1(value);
            ++_M_finish;
        }
        else
            _M_insert_overflow(_M_finish, value, __true_type(), 1, true);
    }
};

class CDisplayManager { public:
    unsigned char _pad000[0x1d8];
    bool          ViewportOffscreen;                    // +0x1d8
    unsigned char _pad1d9[0x2b];
    C2DBoxF       ViewportBoxFloat;                     // +0x204
};

struct CRenderManager2D {
    unsigned char           _pad0000[0x3a3c];
    CDisplayManager*        DisplayManager;             // +0x3a3c
    unsigned char           _pad3a40[0x448];
    CVertexVector           TriVertices;                // +0x3e88
    CQuickDrawTriInfoVector QuickDrawTriInfo;           // +0x3e94
    const C2DBoxF& GetViewportBoxFloat() const;         // 0x00A0AAA0
    float ToViewportX(const float& x) const
    {
        const C2DBoxF& box = GetViewportBoxFloat();
        return x / (box.BRX - box.TLX) * 2.0f - 1.0f;
    }
    float ToViewportY(const float& y) const
    {
        const C2DBoxF& box = GetViewportBoxFloat();
        return 1.0f - y / (box.BRY - box.TLY) * 2.0f;
    }
    void Add2DLine(const CTVertexRHWColSpecTex1& v0, const CTVertexRHWColSpecTex1& v1, unsigned long blendMode,
                   const CTexture* texture);
};

void CRenderManager2D::Add2DLine(const CTVertexRHWColSpecTex1& v0, const CTVertexRHWColSpecTex1& v1,
                                 unsigned long blendMode, const CTexture* texture)
{
    if (DisplayManager->ViewportOffscreen)
        return;
    CTVertexRHWColSpecTex1 vertex = v0;
    vertex.X = ToViewportX(vertex.X);
    vertex.Y = ToViewportY(vertex.Y);
    vertex.Z = 0.0f;
    vertex.RHW = 1.0f;
    TriVertices.push_back(vertex);
    vertex = v1;
    vertex.X = ToViewportX(vertex.X);
    vertex.Y = ToViewportY(vertex.Y);
    vertex.Z = 0.0f;
    vertex.RHW = 1.0f;
    TriVertices.push_back(vertex);
    CQuickDrawTriInfo info;
    info.BlendMode = blendMode;
    if (texture)
        info.UploadedTexture = *texture;
    info.Window = GetViewportBoxFloat();
    info.Line = true;
    info.IsText = false;
    QuickDrawTriInfo.push_back(info);
}