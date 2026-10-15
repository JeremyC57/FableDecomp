// CScreenMessage::CScreenMessage(const CWideString& text, long framesToShowFor, long numberOfCharactersShowing) -- copy the message text and timings, then count its lines (one plus every newline before the last character)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00438020, length 126 bytes (+2 int3 padding).
// PDB layout (16 bytes): CWideString Text +0x00, FramesToShowFor +0x04, NumberOfCharactersShowing +0x08,
// LinesInText +0x0C.
// Callees: CWideString::CWideString(const CWideString&) @ 0x0099B720, CWideString::ConvertToCharString() const
// @ 0x0099B2C0 (CCharString through a hidden pointer), ~CCharString @ 0x0099EAE0, CWideString::GetLength
// @ 0x0099B220 (manifest says CCharString::GetLength; it is called on the wide string). "" = 0x0129AAF4.
// Shape: the newline scan reads the narrow characters through a pointer taken from a temporary converted copy
// that dies at the end of its full expression (retail keeps the stale pointer in ebp; VC puts the temporary in
// the dead `text` argument slot); GetLength is called again every pass and inside the test.
struct CCharStringData {
    const char* m_pText;                             // +0x00
};

struct CCharString {
    CCharStringData* m_pData;
    ~CCharString();                                  // 0x0099EAE0
    const char* GetPString() const { return m_pData == 0 ? "" : m_pData->m_pText; }
};

struct CWideString {
    void* m_pData;
    CWideString(const CWideString& other);           // 0x0099B720
    long GetLength() const;                          // 0x0099B220
    CCharString ConvertToCharString() const;         // 0x0099B2C0
};

class CScreenMessage {
public:
    CScreenMessage(const CWideString& text, long framesToShowFor, long numberOfCharactersShowing);
private:
    CWideString Text;                                // +0x00
    long        FramesToShowFor;                     // +0x04
    long        NumberOfCharactersShowing;           // +0x08
    long        LinesInText;                         // +0x0C
};

CScreenMessage::CScreenMessage(const CWideString& text, long framesToShowFor, long numberOfCharactersShowing)
    : Text(text), FramesToShowFor(framesToShowFor), NumberOfCharactersShowing(numberOfCharactersShowing), LinesInText(1)
{
    const char* chars = text.ConvertToCharString().GetPString();
    for (long i = 0; i < text.GetLength(); ++i)
    {
        if (chars[i] == '\n' && i < text.GetLength() - 1)
            ++LinesInText;
    }
}
