#pragma optimize("s", on)
// NPlayerGui::CDrawTargetInfo::CDrawTargetInfo(CPlayerGui*, std::vector<CDrawBase*>*) -- CDrawBase(gui, list), own vtable, then the attack and block alpha fades are constructed
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0064DAB4, length 44 bytes.
// Callees: NPlayerGui::CDrawBase::CDrawBase @ 0x0064CC65 (pass-through pushes: a size TU), CFadeInFadeOutBase::CFadeInFadeOutBase() @ 0x00436560 twice (on +0x28
// and +0x44; the manifest label "CTCHeroStats::SoberUp" is propagated -- the body zeroes the 0x1c-byte fade record and stores an opaque black
// colour). Vtable 0x0125A5DC (chunk label CDrawTargetInfo; the PDB layout fits: AttackAlphaFade +0x2c, BlockAlphaFade +0x48 -- retail 4 lower, the
// CDrawBase vector is 12 bytes in the shipped build). Shape: the vtable store sits between the first member's `lea ecx` and its constructor call.
struct CPlayerGui;

namespace NPlayerGui {

struct CRetailHandleVector {                       // VC6 Dinkumware vector<CEnginePrimitiveHandle>: three words
    void* m_pFirst;
    void* m_pLast;
    void* m_pEnd;
};

struct CFadeInFadeOutBase {
    unsigned char _body[0x1c];                     // Active, FadeInTime, FadeOutTime, Closing, Opening, OpenTimer, CloseTimer, ToColour
    CFadeInFadeOutBase();                          // 0x00436560
};

struct CDrawBase {
    virtual void v00();
    unsigned char       _body[0x1c - 4];           // +0x04 .. +0x1c
    CRetailHandleVector PrimitiveHandles;          // +0x1c
    CDrawBase(CPlayerGui* gui, void* drawList);    // 0x0064CC65
};

struct CDrawTargetInfo : CDrawBase {               // vtable 0x0125A5DC
    virtual void v00();
    CFadeInFadeOutBase AttackAlphaFade;            // +0x28
    CFadeInFadeOutBase BlockAlphaFade;             // +0x44
    CDrawTargetInfo(CPlayerGui* gui, void* drawList);
};

CDrawTargetInfo::CDrawTargetInfo(CPlayerGui* gui, void* drawList)
    : CDrawBase(gui, drawList), AttackAlphaFade(), BlockAlphaFade()
{
}

}
