// CPersistTraits<CCharString>::TransferIn(CPersistContext* context, CStringParser* parser, CCharString* value) -- reads a double-quoted string: opening quote, text up to the closing quote, closing quote; a missing quote reports a parse error
// Leaf: TransferIn
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0040E510, 209 bytes (+15 int3 padding), `ret 0Ch`.
// Callees: 0x009BA790 CStringParser::ReadAsSymbol() (by value), 0x0099E960 CCharString::operator!=(const char*) const,
// 0x0099EBF0 CCharString::CCharString(const char*, long), 0x009BA330 CStringParser::ReadAsStringUntilString(const
// CCharString&) (by value), 0x0099EFB0 CCharString::operator=, 0x009BAA90 CPersistContext::Error(const CStringParser&,
// const CCharString&), 0x0099EAE0 ~CCharString. Strings "\"" @ 0x0122DF24, "Unable to find closing quote" @ 0x0122DF04.
// The element reader the landed CPersistContext_TransferableVector<CCharString>::TransferIn 0x0049B070 calls.
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
    CCharString& operator=(const CCharString& other);            // 0x0099EFB0
    bool operator!=(const char* text) const;                     // 0x0099E960
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
void CPersistTraits<CCharString>::TransferIn(CPersistContext* context, CStringParser* parser, CCharString* value)
{
    if (static_cast<CStringParserReader*>(parser)->ReadAsSymbol() != "\"")
    {
        context->Error(*parser, "Unable to find closing quote");
        return;
    }
    *value = static_cast<CStringParserReader*>(parser)->ReadAsStringUntilString("\"");
    if (static_cast<CStringParserReader*>(parser)->ReadAsSymbol() != "\"")
        context->Error(*parser, "Unable to find closing quote");
}