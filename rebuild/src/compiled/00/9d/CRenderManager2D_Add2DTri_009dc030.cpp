// CRenderManager2D::Add2DTri(const CTVertexRHWColSpecTex1& v0, const CTVertexRHWColSpecTex1& v1, const CTVertexRHWColSpecTex1& v2, long blendMode, CTexture* texture) -- queue a screen-space triangle: the three vertices converted to viewport space (x and y to -1..1, z 0, rhw 1) on the vertex list, then a triangle record (window, blend mode, texture) on the quick-draw list, unless the viewport is offscreen
// Leaf: Add2DTri
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009DC030, length 645 bytes (`ret 14h`); PDB ?Add2DTri@CRenderManager2D@@AAEXABVCTVertexRHWColSpecTex1@@00JPAVCTexture@@@Z.
// Callees: CRenderManagerCore::GetViewportBoxFloat @ 0x00A0AAA0 (once per converted coordinate and once for the window),
// the vertex vector's _M_insert_overflow @ 0x009E14B0, CWideString() @ 0x0099AED0, CTexture::operator= @ 0x009FA1C0,
// CQuickDrawTriInfo copy ctor @ 0x009DE890, the record vector's _M_insert_overflow @ 0x009E1750,
// CTexture::Uninitialise @ 0x009F9F70 (inline ~CTexture), ~CWideString @ 0x0099B510.
// Layout and shape: the landed sibling Add2DLine 0x009DC2C0 with a third vertex and both record bits cleared
// (`and cl,0FCh`): the 32-byte vertex vector +0x3E88, the QuickDrawTriInfo vector +0x3E94.
struct CTVertexRHWColSpecTex1 {                         // 32 bytes
    float X, Y, Z, RHW;
    unsigned long Diffuse;
    unsigned long Specular;
    float U, V;
};
struct CRGBColour { unsigned char B, G, R, A; };
struct CFontBank;
struct __true_type { __true_type() { } };

inline void* operator new(unsigned int, void* where) { return where; }


struct C2DBoxF {
    float TLX, TLY, BRX, BRY;
};

struct CWideString {
    void* m_pData;
    CWideString();                                      // 0x0099AED0
    ~CWideString();                                     // 0x0099B510
    CWideString& operator=(const CWideString& other);   // 0x0099B7D0
};

struct IDirect3DTexture9;
struct CTexture {
    IDirect3DTexture9* PD3DTexture;                     // +0x00
    unsigned long      ByteLength : 28;                 // +0x04
    unsigned long      AllocationSource : 4;
    CTexture& operator=(const CTexture& other);         // 0x009FA1C0
    CTexture() : PD3DTexture(0) { ByteLength = 0; AllocationSource = 0; }
    ~CTexture() { Uninitialise(); }
    void Uninitialise();                                // 0x009F9F70
};

struct CQuickDrawTriInfo {
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

struct CDisplayManager {
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
    void Add2DTri(const CTVertexRHWColSpecTex1& v0, const CTVertexRHWColSpecTex1& v1, const CTVertexRHWColSpecTex1& v2,
                  long blendMode, CTexture* texture);
};

void CRenderManager2D::Add2DTri(const CTVertexRHWColSpecTex1& v0, const CTVertexRHWColSpecTex1& v1,
                                const CTVertexRHWColSpecTex1& v2, long blendMode, CTexture* texture)
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
    vertex = v2;
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
    info.Line = false;
    info.IsText = false;
    QuickDrawTriInfo.push_back(info);
}