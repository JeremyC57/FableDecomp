// NPlayerGui::CDrawMouseCursor::CDrawMouseCursor(CPlayerGui*, std::vector<CDrawBase*>*) -- CDrawBase(gui, list), own vtable, a zero cursor position, then one primitive handle reserved
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00658240, length 50 bytes.
// Callees: NPlayerGui::CDrawBase::CDrawBase(CPlayerGui*, vector<CDrawBase*>*) @ 0x0064CC65, vector<CEnginePrimitiveHandle>::Resize(long) @ 0x00443F90
// (on PrimitiveHandles at +0x1c, count 1; the manifest label "CEnginePrimitiveRenderer::AddPrimitive" is propagated). Vtable 0x0125A814 (not in
// rtti-vtables.tsv; the next drawer vtable after CDrawQuickAccessIconsPC 0x0125A804). PDB CDrawMouseCursor (52 bytes): Pos (C2DVector) at +0x2c,
// retail +0x28 (the CDrawBase vector is 12 bytes in the shipped build). Shape: a speed body (the two arguments are loaded into registers before
// the base call, not pushed through), the vtable store sits between the count push and the member call, and the two position words share
// the eax zero register -- a 2D coordinate of two 4-byte integers (C2DCoordI-style), not float stores (`fldz`/`fstp` or immediates).
struct CPlayerGui;

namespace NPlayerGui {

struct CRetailHandleVector {                       // VC6 Dinkumware vector<CEnginePrimitiveHandle>: three words
    void* m_pFirst;
    void* m_pLast;
    void* m_pEnd;
    void Resize(long count);                       // 0x00443F90
};

struct CDrawBase {
    virtual void v00();
    unsigned char       _body[0x1c - 4];           // +0x04 .. +0x1c: the cached GUI pointers and the drawing flag
    CRetailHandleVector PrimitiveHandles;          // +0x1c
    CDrawBase(CPlayerGui* gui, void* drawList);    // 0x0064CC65
};

struct C2DCoordI {
    long X;
    long Y;
    C2DCoordI(long x, long y) : X(x), Y(y) { }
};

struct CDrawMouseCursor : CDrawBase {              // vtable 0x0125A814
    virtual void v00();
    C2DCoordI Pos;                                 // +0x28
    CDrawMouseCursor(CPlayerGui* gui, void* drawList);
};

CDrawMouseCursor::CDrawMouseCursor(CPlayerGui* gui, void* drawList)
    : CDrawBase(gui, drawList), Pos(0, 0)
{
    PrimitiveHandles.Resize(1);
}

}