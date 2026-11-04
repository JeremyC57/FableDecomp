// CRenderManager2D::Draw3DScaledText(const C3DVector& position, float scale, CWideString text, CFontBank* font, const CRGBColour& colour, unsigned long flags) -- queue a scaled text string at a 3D position (forwarded to AddText) and report success
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009DBFF0, length 49 bytes (`ret 18h`). Callees: CRenderManager2D::AddText
// @ 0x009DB700 (prototype from the manifest: C3DVector*, float, CWideString*, CFontBank*, CRGBColour*, unsigned long),
// CWideString::~CWideString @ 0x0099B510.
// Shape: a pure forwarder on the same `this` (ecx untouched); the by-value CWideString parameter is passed on by address
// (`lea edx,[esp+18h]`: its own argument slot) and destroyed by this callee afterwards (`lea ecx,[esp+0Ch] ; call
// ~CWideString`), which also blocks the sibling-call jump; the bool result is the constant `mov al,1`.
// Leaf: Draw3DScaledText
// Plan phase 3 (2026-11-04), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
#include "engine/C3DVector.h"
class CRGBColour { public: unsigned char B, G, R, A; };
class CFontBank;

class CWideString { public:
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