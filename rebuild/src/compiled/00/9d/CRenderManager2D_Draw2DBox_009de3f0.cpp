// CRenderManager2D::Draw2DBox(const C2DBoxF&, const CRGBColour&, unsigned long) -- draw a filled 2D box at depth 0 by forwarding to Draw2DBoxWithZ
// Leaf: ?Draw2DBox@CRenderManager2D@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009DE3F0, length 25 bytes.
// PDB (FableWin): ?Draw2DBox@CRenderManager2D@@QAE_NABVC2DBoxF@@ABVCRGBColour@@K@Z. The catalog label
// "ConstructString" (fastcall, CWideString*) is propagated and wrong: the body keeps `this` in ecx, pushes
// (flags, colour, 0.0f, box) and calls CRenderManager2D::Draw2DBoxWithZ (0x009DDC60,
// ?Draw2DBoxWithZ@CRenderManager2D@@QAE_NABVC2DBoxF@@MABVCRGBColour@@K@Z); the float 0 is the immediate `push 0`.
// Not a tail jump because the forwarded argument list is longer than the incoming one (`ret 0Ch`).
class C2DBoxF;
class CRGBColour;

class CRenderManager2D
{
public:
    bool Draw2DBoxWithZ(const C2DBoxF& box, float z, const CRGBColour& colour, unsigned long flags);   // 0x009DDC60
    bool Draw2DBox(const C2DBoxF& box, const CRGBColour& colour, unsigned long flags);
};

bool CRenderManager2D::Draw2DBox(const C2DBoxF& box, const CRGBColour& colour, unsigned long flags)
{
    return Draw2DBoxWithZ(box, 0.0f, colour, flags);
}