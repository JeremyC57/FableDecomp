// CSubtitleRenderer::SetText(const CFontBank& font, const CCharString& text, const CCharString& button1, const CCharString& button2, const CCharString& button3, unsigned long flags, float time, bool clickPast, bool pauseGame) -- show narrow text with three narrow button captions (all converted to wide) and an empty fourth caption through the full overload
// Grade: DIFFER(147v149) -- the four converted-caption temporaries sit in the frame in retail ([ebp-14h]..[ebp-8]); the lane homes them in dead parameter slots ([ebp+1Ch]..[ebp+28h]) and shrinks the frame to `push ecx` (same lane gap as 0x00492ED1)
#pragma optimize("y", off)
#pragma optimize("s", on)
// Retail: Fable.exe (TLC, VC7.1) @ 0x00492FB4, 149 bytes; `ret 24h`. Callees: CWideString(const unsigned short*) @ 0x0099B6B0 (L"" @ 0x0122D70C),
//          CCharString::ToWideString @ 0x0099E480 (x4), the full SetText @ 0x00492973, ~CWideString @ 0x0099B510.
// Shape: the empty fourth caption is a named local built first; the converted strings are argument temporaries (right to left).
// Leaf: ?SetText@CSubtitleRenderer@@QAEXABVCFontBank@@ABVCCharString@@111KM_N2@Z
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
                 const CCharString& button3, unsigned long flags, float time, bool clickPast, bool pauseGame);
};

void CSubtitleRenderer::SetText(const CFontBank& font, const CCharString& text, const CCharString& button1, const CCharString& button2,
                                const CCharString& button3, unsigned long flags, float time, bool clickPast, bool pauseGame)
{
    CWideString button4(L"");
    SetText(font, text.ToWideString(), button1.ToWideString(), button2.ToWideString(), button3.ToWideString(), button4,
            flags, time, clickPast, pauseGame);
}
