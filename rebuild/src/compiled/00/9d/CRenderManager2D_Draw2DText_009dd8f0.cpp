// CRenderManager2D::Draw2DText(const C2DVector& position, CWideString text, CFontBank* font, const CRGBColour& colour, unsigned long flags) -- queue a text string at a 2D screen position (z = 0, scale 1) and report success
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009DD8F0, length 109 bytes (`ret 14h`).
// Callees: CWideString::CWideString(const CWideString&) @ 0x0099B720, CRenderManager2D::AddText @ 0x009DB700
// (C3DVector, float scale, CWideString, CFontBank*, CRGBColour, flags; see the landed Draw3DScaledText 0x009DBFF0),
// CWideString::~CWideString @ 0x0099B510 (x2: the copy, then the by-value parameter).
// Shape: the 3D position and the string copy are NAMED locals built before the argument pushes (VC homes the string
// copy in the dead position-argument slot); unnamed temporaries in the call are built after the trailing pushes.
// `mov al,1`.
// Leaf: Draw2DText
class C2DVector { public: float X, Y; };
class C3DVector { public:
    float X, Y, Z;
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
};
class CRGBColour { public: unsigned char B, G, R, A; };
class CFontBank;

class CWideString { public:
    void* m_pStringData;
    CWideString(const CWideString& other);              // 0x0099B720
    ~CWideString();                                     // 0x0099B510
};

struct CRenderManager2D {
    void AddText(const C3DVector& position, float scale, const CWideString& text, CFontBank* font,
                 const CRGBColour& colour, unsigned long flags);                                // 0x009DB700
    bool Draw2DText(const C2DVector& position, CWideString text, CFontBank* font, const CRGBColour& colour,
                    unsigned long flags);
};

bool CRenderManager2D::Draw2DText(const C2DVector& position, CWideString text, CFontBank* font,
                                  const CRGBColour& colour, unsigned long flags)
{
    C3DVector screenPosition(position.X, position.Y, 0.0f);
    CWideString string(text);
    AddText(screenPosition, 1.0f, string, font, colour, flags);
    return true;
}