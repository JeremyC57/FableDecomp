// CRenderManager2D::Draw3DScaledText(const C3DVector& position, float scale, CWideString text, CFontBank* font, const CRGBColour& colour, unsigned long flags) -- queue a scaled text string at a 3D position (forwarded to AddText) and report success
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009DBFF0, length 49 bytes (`ret 18h`). Callees: CRenderManager2D::AddText
// @ 0x009DB700 (prototype from the manifest: C3DVector*, float, CWideString*, CFontBank*, CRGBColour*, unsigned long),
// CWideString::~CWideString @ 0x0099B510.
// Shape: a pure forwarder on the same `this` (ecx untouched); the by-value CWideString parameter is passed on by address
// (`lea edx,[esp+18h]`: its own argument slot) and destroyed by this callee afterwards (`lea ecx,[esp+0Ch] ; call
// ~CWideString`), which also blocks the sibling-call jump; the bool result is the constant `mov al,1`.
// Leaf: Draw3DScaledText
struct C3DVector { float X, Y, Z; };
struct CRGBColour { unsigned char B, G, R, A; };
struct CFontBank;

struct CWideString {
    void* m_pStringData;
    ~CWideString();                                     // 0x0099B510
};

struct CRenderManager2D {
    void AddText(const C3DVector& position, float scale, const CWideString& text, CFontBank* font,
                 const CRGBColour& colour, unsigned long flags);                                // 0x009DB700
    bool Draw3DScaledText(const C3DVector& position, float scale, CWideString text, CFontBank* font,
                          const CRGBColour& colour, unsigned long flags);
};

bool CRenderManager2D::Draw3DScaledText(const C3DVector& position, float scale, CWideString text, CFontBank* font,
                                        const CRGBColour& colour, unsigned long flags)
{
    AddText(position, scale, text, font, colour, flags);
    return true;
}