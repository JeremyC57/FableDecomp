#pragma optimize("s", on)
// NPlayerGui::CDrawHighlightingTargetingCursor::CDrawHighlightingTargetingCursor(CPlayerGui*, std::vector<CDrawBase*>*) -- CDrawBase(gui, list), own vtable, a null targeted-thing pointer, opaque black colour, zero widths/steps, then two primitive handles reserved
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006542A4, length 83 bytes.
// Callees: NPlayerGui::CDrawBase::CDrawBase @ 0x0064CC65 (pass-through pushes), CIntelligentPointer<CThing>::CIntelligentPointer(CThing*) @ 0x004A96F0 (null),
// the primitive-handle vector's Reserve @ 0x00443F90 (on +0x1c, count 2; manifest "CEnginePrimitiveRenderer::AddPrimitive" is propagated). Vtable 0x0125A720
// (NPlayerGui::CDrawHighlightingTargetingCursor; not in rtti-vtables.tsv). Size TU: the zero register ebx feeds the pointer constructor's argument, the three
// colour bytes and the three longs; the alpha 0xff and the float zeroes (`fldz`/`fstp`) are separate. The colour's own constructor
// stores B, G, R, A in that order (its parameters are (b, g, r, a)); the rest is the init list in declaration order. PDB layout (76 bytes): PTargetedThing +0x2c,
// CurrentColour +0x34, CurrentWidthScreenSpace +0x38, CurrentWidthWorldSpace +0x3c, CurrentFadeStep +0x40, Width +0x44, CursorType +0x48 -- retail 4 lower.
struct CPlayerGui;
struct CThing;

struct CIntelligentPointer_CThing {
    void* m_pItem;
    void* m_pInfo;
    CIntelligentPointer_CThing(CThing* thing);     // 0x004A96F0
};

struct CRGBColour {
    unsigned char R, G, B, A;
    CRGBColour(unsigned char b, unsigned char g, unsigned char r, unsigned char a) { B = b; G = g; R = r; A = a; }
};

namespace NPlayerGui {

struct CRetailHandleVector {                       // VC6 Dinkumware vector<CEnginePrimitiveHandle>
    void* m_pFirst;
    void* m_pLast;
    void* m_pEnd;
    void Reserve(long count);                      // 0x00443F90
};

struct CDrawBase {
    virtual void v00();
    unsigned char       _body[0x1c - 4];           // +0x04 .. +0x1c
    CRetailHandleVector PrimitiveHandles;          // +0x1c
    CDrawBase(CPlayerGui* gui, void* drawList);    // 0x0064CC65
};

struct CDrawHighlightingTargetingCursor : CDrawBase {   // vtable 0x0125A720
    virtual void v00();
    CIntelligentPointer_CThing PTargetedThing;     // +0x28
    CRGBColour                 CurrentColour;      // +0x30
    float                      CurrentWidthScreenSpace;   // +0x34
    float                      CurrentWidthWorldSpace;    // +0x38
    long                       CurrentFadeStep;    // +0x3c
    long                       Width;              // +0x40
    long                       CursorType;         // +0x44
    CDrawHighlightingTargetingCursor(CPlayerGui* gui, void* drawList);
};

CDrawHighlightingTargetingCursor::CDrawHighlightingTargetingCursor(CPlayerGui* gui, void* drawList)
    : CDrawBase(gui, drawList), PTargetedThing(0), CurrentColour(0, 0, 0, 0xff), CurrentWidthScreenSpace(0.0f),
      CurrentWidthWorldSpace(0.0f), CurrentFadeStep(0), Width(0), CursorType(0)
{
    PrimitiveHandles.Reserve(2);
}

}
