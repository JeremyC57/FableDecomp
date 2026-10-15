// NPlayerGui::CStatBar::AddChangeText(const CRGBColour&, const CWideString&, long, bool) -- pushes a new floating change text onto the bar's list, starting at one of the GUI's two change-text anchor positions chosen by the flag
// Leaf: AddChangeText
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0064DAE0, 75 bytes. Callees: CChangeText::CChangeText(const C2DVector&,
// const CRGBColour&, const CWideString&, long, bool) @ 0x00654F80 (0x24-byte record: two copies of the start position,
// the text at +0x10, the value, the colour, the flag, a zeroed float), the list's push_front(const CChangeText&) @
// 0x006567EC (landed), and CWideString::~CWideString @ 0x0099B510 (the temporary's only non-trivial member).
// Layout: CStatBar's owner pointer at +0x4 (its two C2DVector anchors at +0x4CC and +0x4D4) and the change-text list
// at +0x64.
// Shape: frame pointer + `leave` with pass-through argument pushes = #pragma optimize("y", off) + ("s", on). The anchor
// choice is an if/else of two whole push_front statements: the size optimiser cross-jumps their identical tails, so
// only `push flag ; add eax,anchor` stays in each arm and the false arm's flag folds to `push 0` (a ternary anchor
// argument pushes the flag once after the join, 73 v 75).
// The CChangeText temporary lives in the 0x24-byte frame and dies through its inline destructor (~CWideString only).
#pragma optimize("y", off)
#pragma optimize("s", on)
struct C2DVector { float X, Y; };

struct CRGBColour { unsigned char B, G, R, A; };

struct CWideString {
    void* m_pData;
    ~CWideString();                                  // 0x0099B510
};

struct CChangeText {
    C2DVector    m_startPos;                         // +0x00
    C2DVector    m_pos;                              // +0x08
    CWideString  m_text;                             // +0x10
    long         m_value;                            // +0x14
    CRGBColour   m_colour;                           // +0x18
    bool         m_flag;                             // +0x1C
    float        m_age;                              // +0x20
    CChangeText(const C2DVector& pos, const CRGBColour& colour, const CWideString& text, long value, bool flag);   // 0x00654F80
};

struct CChangeTextList {
    void* m_pHead;
    void push_front(const CChangeText& text);        // 0x006567EC
};

struct CStatBarOwner {
    unsigned char _pad00[0x4CC];
    C2DVector     m_changeTextAnchorA;               // +0x4CC
    C2DVector     m_changeTextAnchorB;               // +0x4D4
};

namespace NPlayerGui {

struct CStatBar {
    void*           __vftable;                       // +0x00
    CStatBarOwner*  m_pOwner;                        // +0x04
    unsigned char   _pad08[0x5C];
    CChangeTextList m_changeTexts;                   // +0x64
    void AddChangeText(const CRGBColour& colour, const CWideString& text, long value, bool flag);
};

void CStatBar::AddChangeText(const CRGBColour& colour, const CWideString& text, long value, bool flag)
{
    if (flag)
        m_changeTexts.push_front(CChangeText(m_pOwner->m_changeTextAnchorA, colour, text, value, flag));
    else
        m_changeTexts.push_front(CChangeText(m_pOwner->m_changeTextAnchorB, colour, text, value, flag));
}

}
