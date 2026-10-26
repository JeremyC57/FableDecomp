// CScreenMessage::IsHardCoded() const -- true when the message text starts with the "(HC)" hard-coded marker
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004381B0, length 117 bytes (+11 int3 padding).
// Callees: CCharString::CCharString(const char*, long) @ 0x0099EBF0 ("(HC)" @ 0x0123172C, length -1),
// CWideString::ConvertToCharString() const @ 0x0099B2C0 (hidden result pointer), strncmp @ 0x00BFEAF8,
// ~CCharString @ 0x0099EAE0 (temporary first, then the marker). "" = 0x0129AAF4.
// Shape: the marker (frame +4) is a named local; the converted text is the temporary OBJECT of an inline
// StartsWith(const CCharString&) member call in the return expression, so the conversion runs before the strncmp
// arguments are read, the temporary gets its own frame word (+8, live for the whole full expression) and is read through
// the returned pointer (`mov eax,[eax]`), and it dies before the marker. GetPString/GetLength are the inline null-safe
// accessors (`p == 0 ? 0 : p->len` shares the tested zero); `!strncmp(...)` returns through `neg ; sbb ; inc`.
#include <string.h>

class CCharStringData { public:
    const char* m_pText;                             // +0x00
    long        m_Length;                            // +0x04
};

class CCharString { public:
    CCharStringData* m_pData;
    CCharString(const char* text, long length = -1); // 0x0099EBF0
    ~CCharString();                                  // 0x0099EAE0
    const char* GetPString() const { return m_pData == 0 ? "" : m_pData->m_pText; }
    long GetLength() const { return m_pData == 0 ? 0 : m_pData->m_Length; }
    bool StartsWith(const CCharString& prefix) const
    {
        return !strncmp(GetPString(), prefix.GetPString(), prefix.GetLength());
    }
};

class CWideString { public:
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
    return Text.ConvertToCharString().StartsWith(marker);
}