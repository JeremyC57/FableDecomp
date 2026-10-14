#pragma optimize("s", on)
// NPlayerGui::CDrawScreenFade::CDrawScreenFade(CPlayerGui*, std::vector<CDrawBase*>*) -- CDrawBase(gui, list), own vtable, the full-fade-drawn flag cleared, one primitive handle reserved
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0064E8DC, length 42 bytes.
// Callees: NPlayerGui::CDrawBase::CDrawBase @ 0x0064CC65 (pass-through pushes `push [esp+0Ch]` twice: a size TU), vector<CEnginePrimitiveHandle>::Resize(long)
// @ 0x00443F90 (on PrimitiveHandles +0x1c, count 1). Vtable 0x0125A640 (NPlayerGui::CDrawScreenFade: the chunk label, consistent with the PDB layout).
// PDB CDrawScreenFade (48 bytes): DrawnFullFadeOut at +0x2c, retail +0x28 (the CDrawBase vector is 12 bytes in the shipped build).
// Shape: the vtable store and the byte clear sit between the count push / `lea ecx` and the Resize call.
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
    unsigned char       _body[0x1c - 4];           // +0x04 .. +0x1c
    CRetailHandleVector PrimitiveHandles;          // +0x1c
    CDrawBase(CPlayerGui* gui, void* drawList);    // 0x0064CC65
};

struct CDrawScreenFade : CDrawBase {               // vtable 0x0125A640
    virtual void v00();
    bool DrawnFullFadeOut;                         // +0x28
    CDrawScreenFade(CPlayerGui* gui, void* drawList);
};

CDrawScreenFade::CDrawScreenFade(CPlayerGui* gui, void* drawList)
    : CDrawBase(gui, drawList), DrawnFullFadeOut(false)
{
    PrimitiveHandles.Resize(1);
}

}