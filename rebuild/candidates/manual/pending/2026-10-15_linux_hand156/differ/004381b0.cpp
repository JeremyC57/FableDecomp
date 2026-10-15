// CScreenMessage::IsHardCoded() const -- true when the message text starts with the "(HC)" hard-coded marker
// Grade: DIFFER(118v117) -- frame slots swapped: retail keeps the marker at +4 and the converted text at +8 and reads the text through the returned pointer (`mov eax,[eax]`); ours (named local or const& binding) puts the text at +4 and re-reads it from the frame. Tried: temporary in the call (wrong evaluation order), named local, const&, an inline StartsWith(const CCharString&) on the temporary (not inlined: 71 bytes)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004381B0, length 117 bytes (+11 int3 padding).
// Callees: CCharString::CCharString(const char*, long) @ 0x0099EBF0 ("(HC)" @ 0x0123172C, length -1),
// CWideString::ConvertToCharString() const @ 0x0099B2C0 (hidden result pointer), strncmp @ 0x00BFEAF8,
// ~CCharString @ 0x0099EAE0 (temporary first, then the marker). "" = 0x0129AAF4.
// Shape: the marker (frame +4) is a named local and the converted text (frame +8) a temporary bound to a const
// reference (lifetime to the end of the body, used through the returned pointer), so the conversion runs before
// the strncmp arguments are read and the text dies first; GetPString/GetLength are the inline null-safe accessors (`p == 0 ? 0 : p->len`
// shares the tested zero); `== 0` on the strncmp result returns through `neg ; sbb ; inc`.
#include <string.h>

struct CCharStringData {
    const char* m_pText;                             // +0x00
    long        m_Length;                            // +0x04
};

struct CCharString {
    CCharStringData* m_pData;
    CCharString(const char* text, long length = -1); // 0x0099EBF0
    ~CCharString();                                  // 0x0099EAE0
    const char* GetPString() const { return m_pData == 0 ? "" : m_pData->m_pText; }
    long GetLength() const { return m_pData == 0 ? 0 : m_pData->m_Length; }
};

struct CWideString {
    void* m_pData;
    CCharString ConvertToCharString() const;         // 0x0099B2C0
};

class CScreenMessage {
public:
    bool IsHardCoded() const;
private:
    CWideString Text;                                // +0x00
    long        FramesToShowFor;                     // +0x04
    long        NumberOfCharactersShowing;           // +0x08
    long        LinesInText;                         // +0x0C
};

bool CScreenMessage::IsHardCoded() const
{
    CCharString marker("(HC)");
    const CCharString& text = Text.ConvertToCharString();
    return strncmp(text.GetPString(), marker.GetPString(), marker.GetLength()) == 0;
}
