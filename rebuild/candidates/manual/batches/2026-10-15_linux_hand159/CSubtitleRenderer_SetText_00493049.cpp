// CSubtitleRenderer::SetText(const CFontBank& font, const CCharString& text, const CCharString& button1, const CCharString& button2, const CCharString& button3, const CCharString& button4, unsigned long flags, float time, bool clickPast, bool pauseGame) -- show narrow text with four narrow button captions, all converted to wide, through the full overload
#pragma optimize("y", off)
#pragma optimize("s", on)
// Retail: Fable.exe (TLC, VC7.1) @ 0x00493049, 142 bytes; `ret 28h`. Callees: CCharString::ToWideString @ 0x0099E480 (x5),
//          the full SetText @ 0x00492973, ~CWideString @ 0x0099B510.
// Shape: no frame locals: VC homes each converted temporary in a dead parameter slot (right to left, [ebp+1Ch] .. [ebp+2Ch]).
// Leaf: ?SetText@CSubtitleRenderer@@QAEXABVCFontBank@@ABVCCharString@@1111KM_N2@Z
class CWideString {
public:
    unsigned short* m_pData;
    CWideString(const unsigned short* text);       // 0x0099B6B0 (wchar_t is unsigned short under the era defaults)
    ~CWideString();                                // 0x0099B510
};
class CCharString {
public:
    void* m_pStringData;
    CWideString ToWideString() const;              // 0x0099E480
};
class CFontBank;
struct CSubtitleRenderer {
    void SetText(const CFontBank& font, const CWideString& text, const CWideString& button1, const CWideString& button2,
                 const CWideString& button3, const CWideString& button4, unsigned long flags, float time,
                 bool clickPast, bool pauseGame);   // 0x00492973
    void SetText(const CFontBank& font, const CCharString& text, const CCharString& button1, const CCharString& button2,
                 const CCharString& button3, const CCharString& button4, unsigned long flags, float time, bool clickPast, bool pauseGame);
};

void CSubtitleRenderer::SetText(const CFontBank& font, const CCharString& text, const CCharString& button1, const CCharString& button2,
                                const CCharString& button3, const CCharString& button4, unsigned long flags, float time, bool clickPast, bool pauseGame)
{
    SetText(font, text.ToWideString(), button1.ToWideString(), button2.ToWideString(), button3.ToWideString(),
            button4.ToWideString(), flags, time, clickPast, pauseGame);
}
