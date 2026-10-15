// CSubtitleRenderer::SetText(const CFontBank& font, const CWideString& text, unsigned long flags, float time, bool clickPast, bool pauseGame) -- show text with no button captions (four empty wide strings) through the full overload
#pragma optimize("y", off)
#pragma optimize("s", on)
// Retail: Fable.exe (TLC, VC7.1) @ 0x00492E4B, 134 bytes; `ret 18h`. Callees: CWideString(const unsigned short*) @ 0x0099B6B0 (L"" @ 0x0122D70C),
//          the full SetText(font, text, b1, b2, b3, b4, flags, time, bool, bool) @ 0x00492973, ~CWideString @ 0x0099B510.
// Shape: frame-pointer size TU; the four empty captions are named locals built before any argument is pushed,
//        declared last caption first (the first declared takes the lowest frame slot, [ebp-10h], and is passed as button4).
// Leaf: ?SetText@CSubtitleRenderer@@QAEXABVCFontBank@@ABVCWideString@@KM_N2@Z
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
    void SetText(const CFontBank& font, const CWideString& text, unsigned long flags, float time, bool clickPast, bool pauseGame);
};

void CSubtitleRenderer::SetText(const CFontBank& font, const CWideString& text, unsigned long flags, float time, bool clickPast, bool pauseGame)
{
    CWideString button4(L"");
    CWideString button3(L"");
    CWideString button2(L"");
    CWideString button1(L"");
    SetText(font, text, button1, button2, button3, button4, flags, time, clickPast, pauseGame);
}