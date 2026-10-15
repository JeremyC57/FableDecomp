// CSubtitleRenderer::SetText(const CFontBank& font, const CCharString& text, unsigned long flags, float time, bool clickPast, bool pauseGame) -- show narrow text (converted to wide) with no button captions through the full overload
// Grade: DIFFER(152v152) -- the converted-text argument temporary sits in the frame in retail ([ebp-14h], sub esp,14h); the lane homes it in the dead pauseGame parameter slot ([ebp+1Ch]) (lane gap: argument temporaries in dead parameter slots when the function also has a frame)
#pragma optimize("y", off)
#pragma optimize("s", on)
// Retail: Fable.exe (TLC, VC7.1) @ 0x00492ED1, 152 bytes; `ret 18h`. Callees: CWideString(const unsigned short*) @ 0x0099B6B0 (L"" @ 0x0122D70C),
//          CCharString::ToWideString @ 0x0099E480, the full SetText @ 0x00492973, ~CWideString @ 0x0099B510.
// Shape: as 0x00492E4B (four named empty captions, declared last first); the converted text is an argument temporary built last.
// Leaf: ?SetText@CSubtitleRenderer@@QAEXABVCFontBank@@ABVCCharString@@KM_N2@Z
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
    void SetText(const CFontBank& font, const CCharString& text, unsigned long flags, float time, bool clickPast, bool pauseGame);
};

void CSubtitleRenderer::SetText(const CFontBank& font, const CCharString& text, unsigned long flags, float time, bool clickPast, bool pauseGame)
{
    CWideString button4(L"");
    CWideString button3(L"");
    CWideString button2(L"");
    CWideString button1(L"");
    SetText(font, text.ToWideString(), button1, button2, button3, button4, flags, time, clickPast, pauseGame);
}
