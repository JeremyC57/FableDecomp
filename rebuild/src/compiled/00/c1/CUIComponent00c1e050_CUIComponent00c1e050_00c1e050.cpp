// CUIComponent_00c1e050::CUIComponent_00c1e050(CUIOwner*, void*, void*, const SUIRect&) -- UI display-element constructor: base constructor with (owner, rect), own vtable, the second and third arguments stored at +0x20 / +0x24
// Leaf: ??0CUIComponent_00c1e050
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C1E050, 46 bytes (+2 int3 padding). Callee: the display-element base
// constructor @ 0x00C1EAA0 (owner, 20-byte rect copied by value into +0x8; registers the element in the owner's list at
// +0x2C; base vtable 0x012B3404). Own vtable 0x012B33A4 (no-RTTI module). Caller 0x00C0E7B5 (`new` of 0x28 bytes).
// Label: the manifest "CStringAndCage::CStringAndCage" (eight camera-mode parameters) is propagated; this body takes
// four arguments (`ret 10h`). The sibling 0x00C1E110 (landed, five arguments, EH-framed) has the same base; the class is
// named by address in the same way.
// Shape: the two argument stores are init-list members; the base constructor's arguments are pushed right to left.
struct CUIOwner_00c1e050;
struct SUIRect { long m_words[5]; };

struct CUIComponentBase_00c1e050 {
    virtual void v00();
    unsigned char _body[0x1c];
    CUIComponentBase_00c1e050(CUIOwner_00c1e050* owner, const SUIRect& rect);   // 0x00C1EAA0
};

struct CUIComponent_00c1e050 : CUIComponentBase_00c1e050 {   // vtable 0x012B33A4
    virtual void v00();
    void* m_pSecond;                       // +0x20
    void* m_pThird;                        // +0x24
    CUIComponent_00c1e050(CUIOwner_00c1e050* owner, void* second, void* third, const SUIRect& rect);
};

CUIComponent_00c1e050::CUIComponent_00c1e050(CUIOwner_00c1e050* owner, void* second, void* third, const SUIRect& rect)
    : CUIComponentBase_00c1e050(owner, rect), m_pSecond(second), m_pThird(third)
{
}