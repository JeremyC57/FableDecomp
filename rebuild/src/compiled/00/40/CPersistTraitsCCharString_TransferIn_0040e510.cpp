// CPersistTraits<CCharString>::TransferIn(CPersistContext& context, CStringParser& parser, CCharString& value) -- reads a double-quoted string: opening quote, text up to the closing quote, closing quote; a missing quote reports a parse error
// Leaf: ?TransferIn@?$CPersistTraits@VCCharString@@@@QAEXAAVCPersistContext@@AAVCStringParser@@AAVCCharString@@@Z
// Name (2026-10-31, hand621 NAME_FIXES): Ego_r.pdb 0x0040C1F0 / FableWin.pdb 0x0189E580 take references (was the pointer
//   form); the context's error reporter is ?PersistError@CPersistContext@@QBE... (public const). Bytes unchanged.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0040E510, 209 bytes (+15 int3 padding), `ret 0Ch`.
// Callees: 0x009BA790 CStringParser::ReadAsSymbol() (by value), 0x0099E960 CCharString::operator!=(const char*) const,
// 0x0099EBF0 CCharString::CCharString(const char*, long), 0x009BA330 CStringParser::ReadAsStringUntilString(const
// CCharString&) (by value), 0x0099EFB0 CCharString::operator=, 0x009BAA90 CPersistContext::PersistError(const CStringParser&,
// const CCharString&), 0x0099EAE0 ~CCharString. Strings "\"" @ 0x0122DF24, "Unable to find closing quote" @ 0x0122DF04.
// The element reader the landed CPersistContext_TransferableVector<CCharString>::TransferIn 0x0049B070 calls.
// Shape: the `parser` argument slot is dead once the parser is in esi, so every by-value symbol / message temporary is
// built there ([esp+14h]); the ReadAsStringUntilString result is the one `push ecx` frame word. The early error return
// is tail-merged by VC into the second error call (the first arm pushes its message and jumps to the shared `push esi ;
// call PersistError`). The implicit const char* -> CCharString conversions use the (text, -1) constructor.
#define CStringParser CStringParserLayout
#include "engine/CStringParser.h"
#undef CStringParser
class CCharString;

class CStringParser : public CStringParserLayout
{
public:
    CCharString ReadAsSymbol();   // 0x009BA790
    CCharString ReadAsStringUntilString(const CCharString& end);   // 0x009BA330 ?ReadAsStringUntilString@CStringParser@@QAE?AVCCharString@@ABV2@@Z
};

class CPersistContext;

class CCharString { public:
    struct SBlock* m_pBlock;
    CCharString(const char* text, long length = -1);             // 0x0099EBF0
    ~CCharString();                                              // 0x0099EAE0
    const CCharString& operator=(const CCharString& other);            // 0x0099EFB0
    bool operator!=(const char* text) const;                     // 0x0099E960
};

// NF176 (2026-11-20, q4658): ReadAsStringUntilString moved from the CStringParserReader stand-in (?ReadAsStringUntilString@
//   CStringParserReader@@QAE?AVCCharString@@ABV2@@Z) onto the row's own CStringParser model, so 0x009BA330 binds the Ego_r.pdb /
//   FableWin.pdb ?ReadAsStringUntilString@CStringParser@@QAE?AVCCharString@@ABV2@@Z like the 24 other landed callers; the stand-in is gone.

class CPersistContext { public:
    void PersistError(const CStringParser& parser, const CCharString& message) const;   // 0x009BAA90
};

template <class T> struct CPersistTraits {
    T DefaultVal;
    void TransferIn(CPersistContext& context, CStringParser& parser, T& value);
};

template <>
void CPersistTraits<CCharString>::TransferIn(CPersistContext& context, CStringParser& parser, CCharString& value)
{
    if (parser.ReadAsSymbol() != "\"")
    {
        context.PersistError(parser, "Unable to find closing quote");
        return;
    }
    value = parser.ReadAsStringUntilString("\"");
    if (parser.ReadAsSymbol() != "\"")
        context.PersistError(parser, "Unable to find closing quote");
}