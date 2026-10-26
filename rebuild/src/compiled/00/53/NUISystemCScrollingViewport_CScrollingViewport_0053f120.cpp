// NUISystem::CScrollingViewport::CScrollingViewport(void* def) -- the CViewport base (0x0054EA00 with the def), the three vtables of this class (0x012468D4 at +0, 0x012468AC at +4, 0x012468A4 at +0x18: the primary and two secondary bases), the dwords +0x188 / +0x18c / +0x194 / +0x198 cleared and the byte +0x190 set, then GetDataFromDef (0x0053ED40)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0053F120, length 79 bytes.
// Callees: CViewport::CViewport @ 0x0054EA00, CScrollingViewport::GetDataFromDef @ 0x0053ED40. Vtables from rtti-vtables.tsv. Shape: `this` in esi, one zero register, `ret 4`.
struct IViewportPrimary_0053f120 {                    // the primary base (vptr at +0)
    virtual void v00();
};
struct IViewportSecondaryA_0053f120 {                 // the secondary base at +4
    virtual void a00();
    unsigned char _pad04[0x10];
};
struct IViewportSecondaryB_0053f120 {                 // the secondary base at +0x18
    virtual void b00();
};
struct CViewport : IViewportPrimary_0053f120, IViewportSecondaryA_0053f120, IViewportSecondaryB_0053f120 {   // 0x188 bytes
    unsigned char _pad1c[0x188 - 0x1c];
    CViewport(void* def);                          // 0x0054EA00
};

namespace NUISystem {
class CScrollingViewport : public CViewport { public:           // vtables 0x012468D4 / 0x012468AC / 0x012468A4
    virtual void v00();
    virtual void a00();
    virtual void b00();
    long m_188;                                    // +0x188
    long m_18c;                                    // +0x18c
    bool m_190;                                    // +0x190
    long m_194;                                    // +0x194
    long m_198;                                    // +0x198
    CScrollingViewport(void* def);
    void GetDataFromDef();                         // 0x0053ED40
};
}

NUISystem::CScrollingViewport::CScrollingViewport(void* def)
    : CViewport(def), m_188(0), m_18c(0), m_190(true), m_194(0), m_198(0)
{
    GetDataFromDef();
}