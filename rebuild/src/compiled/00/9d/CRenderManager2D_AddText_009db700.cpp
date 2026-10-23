// CRenderManager2D::AddText(const C3DVector& position, float scale, const CWideString& text, CFontBank* font, const CRGBColour& colour, unsigned long flags) -- queue a text record on the quick-draw list (viewport window, screen-space position, string, metrics, colour, font) unless the viewport is offscreen
// Leaf: AddText
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009DB700, length 266 bytes (`ret 18h`). FableWin.pdb
// ?AddText@CRenderManager2D@@AAEXABVC3DVector@@MABVCWideString@@ABVCFontBank@@ABVCRGBColour@@K@Z (the font by reference; the
// landed caller Draw3DScaledText 0x009DBFF0 declares it as a pointer, same code, kept here for one symbol spelling).
// Callees: CWideString::CWideString() @ 0x0099AED0, CRenderManagerCore::GetViewportBoxFloat @ 0x00A0AAA0 (`&DisplayManager->
// ViewportBoxFloat`; manifest "CVertexBufferWin32::DoSizeof"), GFAXToVX / GFAYToVY @ 0x009E1E10 / 0x009E1E40 (`__fastcall`
// float(float)), CWideString::operator= @ 0x0099B7D0, CQuickDrawTriInfo copy ctor @ 0x009DE890 (landed), the vector's
// _M_insert_overflow @ 0x009E1750, CTexture::Uninitialise @ 0x009F9F70 (the inline ~CTexture), ~CWideString @ 0x0099B510.
// Layout: engine/CRenderManagerCore.h agrees on DisplayManager +0x3a3c (CDisplayManager: ViewportOffscreen +0x1d8,
// ViewportBoxFloat +0x204); QuickDrawTriInfo (12-byte STLport vector of 0x3c-byte CQuickDrawTriInfo) sits at +0x3e94, 0x54 below
// the PDB's CRenderManager2D layout. CQuickDrawTriInfo as the landed copy ctor 0x009DE890; CTexture {PD3DTexture,
// ByteLength:28, AllocationSource:4} (its ctor clears the bit fields with a dead load of the word).
// Shape: one local record: default-constructed (string, texture), the Line/IsText bit stores, the window copy, the converted
// X/Y, Z, the text assignment, then font / colour / flags / scale in that statement order (the only one of the 24 orders
// that copies the colour through eax->ecx and loads the flags last, inside the push_back compare); the inline STLport push_back (null-checked placement copy, else
// _M_insert_overflow with the `__true_type` tag in the dead `position` slot), then the record's destructors.
struct C3DVector { float X, Y, Z; };
struct CRGBColour { unsigned char B, G, R, A; };
struct CFontBank;
struct __true_type { __true_type() { } };

inline void* operator new(unsigned int, void* where) { return where; }

float __fastcall GFAXToVX_009e1e10(float x);                     // 0x009E1E10
float __fastcall GFAYToVY(float y);                     // 0x009E1E40

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
    float       StringX;                                // +0x20 (union with BlendMode)
    float       StringY;                                // +0x24 (union with ZAverage)
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

struct CDisplayManager {
    unsigned char _pad000[0x1d8];
    bool          ViewportOffscreen;                    // +0x1d8
    unsigned char _pad1d9[0x2b];
    C2DBoxF       ViewportBoxFloat;                     // +0x204
};

struct CRenderManager2D {
    unsigned char           _pad0000[0x3a3c];
    CDisplayManager*        DisplayManager;             // +0x3a3c
    unsigned char           _pad3a40[0x454];
    CQuickDrawTriInfoVector QuickDrawTriInfo;           // +0x3e94
    const C2DBoxF& GetViewportBoxFloat() const;         // 0x00A0AAA0
    void AddText(const C3DVector& position, float scale, const CWideString& text, CFontBank* font,
                 const CRGBColour& colour, unsigned long flags);
};

void CRenderManager2D::AddText(const C3DVector& position, float scale, const CWideString& text, CFontBank* font,
                               const CRGBColour& colour, unsigned long flags)
{
    if (DisplayManager->ViewportOffscreen)
        return;
    CQuickDrawTriInfo info;
    info.Window = GetViewportBoxFloat();
    info.Line = false;
    info.IsText = true;
    info.StringX = GFAXToVX_009e1e10(position.X);
    info.StringY = GFAYToVY(position.Y);
    info.StringZ = position.Z;
    info.String = text;
    info.PFont = font;
    info.Colour = colour;
    info.StringFlags = flags;
    info.StringScale = scale;
    QuickDrawTriInfo.push_back(info);
}