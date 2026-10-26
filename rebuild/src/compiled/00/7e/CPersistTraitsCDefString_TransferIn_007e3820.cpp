// CPersistTraits<CDefString>::TransferIn(CPersistContext* context, CStringParser* parser, CDefString* value) -- reads a double-quoted def name and resolves it to its CDefString table index; a missing quote reports a parse error
// Leaf: TransferIn
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007E3820, 209 bytes (+15 int3 padding), `ret 0Ch`.
// Callees: 0x009BA790 CStringParser::ReadAsSymbol() (by value), 0x0099E960 CCharString::operator!=(const char*) const,
// 0x0099EBF0 CCharString::CCharString(const char*, long), 0x009BA330 CStringParser::ReadAsStringUntilString(const
// CCharString&) (by value), 0x009D5060 CDefString::SetString(const CCharString&), 0x009BAA90 CPersistContext::Error(const CStringParser&,
// const CCharString&), 0x0099EAE0 ~CCharString. Strings "\"" @ 0x0122DF24, "Unable to find quoted string" @ 0x01270500,
// "Unable to find closing quote" @ 0x0122DF04.
// The manifest's `CPersistTraits<CCharString>` is propagated: the landed CPersistContext_TransferableVector<CDefString>::
// TransferIn 0x00F33480 tail-jumps here, and the text is handed to CDefString::SetString (0x009D5060, a void member:
// no `mov eax,esi`, so not a constructor; Ego_r.pdb ?SetString@CDefString@@QAEXABVCCharString@@@Z) instead of assigned.
// The CCharString twin is 0x0040E510 (same shape, operator= and one error message for both quotes).
// CDefString is one long table index (Ego_r.pdb); `value->SetString(text)` gives `mov ecx,[esp+18h] ; push eax ; call`.
// Shape: the `parser` argument slot is dead once the parser is in esi, so every by-value symbol / message temporary is
// built there ([esp+14h]); the ReadAsStringUntilString result is the one `push ecx` frame word. The early error return
// is tail-merged by VC into the second error call (the first arm pushes its message and jumps to the shared `push esi ;
// call Error`). The implicit const char* -> CCharString conversions use the (text, -1) constructor.
#include "engine/CStringParser.h"

class CPersistContext;

class CCharString { public:
    struct SBlock* m_pBlock;
    CCharString(const char* text, long length = -1);             // 0x0099EBF0
    ~CCharString();                                              // 0x0099EAE0
    bool operator!=(const char* text) const;                     // 0x0099E960
};

class CDefString {
public:
    long Index;
    void SetString(const CCharString& name);                     // 0x009D5060
};

// Method view over the shared engine header (the reader touches no parser fields directly).
struct CStringParserReader : CStringParser {
    CCharString ReadAsSymbol();                                  // 0x009BA790
    CCharString ReadAsStringUntilString(const CCharString& end); // 0x009BA330
};

class CPersistContext { public:
    void Error(const CStringParser& parser, const CCharString& message);   // 0x009BAA90
};

template <class T> struct CPersistTraits {
    T DefaultVal;
    void TransferIn(CPersistContext* context, CStringParser* parser, T* value);
};

template <>
void CPersistTraits<CDefString>::TransferIn(CPersistContext* context, CStringParser* parser, CDefString* value)
{
    if (static_cast<CStringParserReader*>(parser)->ReadAsSymbol() != "\"")
    {
        context->Error(*parser, "Unable to find quoted string");
        return;
    }
    value->SetString(static_cast<CStringParserReader*>(parser)->ReadAsStringUntilString("\""));
    if (static_cast<CStringParserReader*>(parser)->ReadAsSymbol() != "\"")
        context->Error(*parser, "Unable to find closing quote");
}