// CPersistTraits<CHeroMarriageInfo>::TransferIn(CPersistContext* context, CStringParser* parser, CHeroMarriageInfo* value) -- text load of the hero's marriage record "(wifeUID,homeUID,state,gay)" into a defaulted record
// Leaf: ?TransferIn@?$CPersistTraits@VCHeroMarriageInfo@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00580194, 440 bytes, `ret 0Ch` (traits `this` unused).
// Ego_r.pdb: CHeroMarriageInfo (0x18) = { unsigned __int64 WifeUID +0, unsigned __int64 MaritalHomeUID +8, EMarriageState
//   State +0x10, bool GayMarriage +0x14 }; the binary twin is 0x0057F9E9.
// Callees: CStringParser::ReadAsSymbol @ 0x009BA790 (hidden CCharString result), CStringParser::ReadInt64 @ 0x009BA5B0
//   (edx:eax), CBasicString<char>::Compare @ 0x00411570 (`__fastcall` (a, b), long), ~CCharString @ 0x0099EAE0,
//   TransferIn(CPersistContext*, CStringParser*, long*) @ 0x009E2590 (`__fastcall`), CPersistTraits<bool>::TransferIn
//   @ 0x00403FF0 (real body in the TU as a noinline stand-in). Strings: "(" 0x0122E028, "," 0x0122E024, "" 0x0122D70E.
// Shape: the record is defaulted through a zeroed temporary copied with `rep movsd`; each symbol test is CCharString's
//   inline operator== (a null data pointer compares the literal with "" through the strcmp intrinsic, otherwise the
//   inline CBasicString<char>::operator== calls Compare) and the symbol temporary dies before its test; the state comes
//   through the inline by-value TransferInLong() helper of the landed 0x00755F40; the empty bool traits lives in its own
//   block (dead before the closing read, so it shares the parser slot with the closing symbol temporary as `this`; a
//   `CPersistTraits<bool>()` temporary would zero its byte); the closing ")" is read and discarded. EBP frame + size TU (`#pragma optimize("y", off)` +
//   `("s", on)`).
#pragma optimize("y", off)
#pragma optimize("s", on)
#include <string.h>
#pragma intrinsic(strcmp)
struct CBasicStringChar {
    char*         PData;
    unsigned long StringLength;
    static long __fastcall Compare(const char* a, const char* b);   // 0x00411570
    __forceinline bool operator==(const char* text) const { return Compare(PData, text) == 0; }
};
struct CCharString {
    CBasicStringChar* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                    // 0x0099EAE0
    const char* GetString() const { return PStringData == 0 ? "" : PStringData->PData; }
    __forceinline bool operator==(const char* text) const
    {
        if (PStringData == 0)
            return strcmp(text, "") == 0;
        return *PStringData == text;
    }
};
CCharString __fastcall operator+(const char* left, const CCharString& right);   // 0x0099F690
extern "C" int __cdecl _stricmp(const char* a, const char* b);                 // 0x00BFEBA8
__forceinline bool EqualsNoCase(const CCharString& a, const CCharString& b) { return _stricmp(a.GetString(), b.GetString()) == 0; }
struct CStringParser {
    CCharString ReadAsString();                        // 0x009BA6F0
    CCharString ReadAsSymbol();                        // 0x009BA790
    __int64 ReadInt64();                               // 0x009BA5B0
};
struct CPersistContext {
    void Error(const CStringParser& parser, const CCharString& message);       // 0x009BAA90
};
void __fastcall TransferIn(CPersistContext* context, CStringParser* parser, long* value);   // 0x009E2590
__forceinline long TransferInLong(CPersistContext* context, CStringParser* parser)
{
    long value;
    TransferIn(context, parser, &value);
    return value;
}

enum EMarriageState { MARRIAGE_STATE_NONE = 0 };
class CHeroMarriageInfo {                              // 0x18 bytes
public:
    unsigned __int64 WifeUID;                          // +0x00
    unsigned __int64 MaritalHomeUID;                   // +0x08
    EMarriageState   State;                            // +0x10
    bool             GayMarriage;                      // +0x14
    CHeroMarriageInfo() : WifeUID(0), MaritalHomeUID(0), State(MARRIAGE_STATE_NONE), GayMarriage(false) {}
};

template <class T> struct CPersistTraits;
template <> struct CPersistTraits<bool> {
    void TransferIn(CPersistContext* context, CStringParser* parser, bool* value);   // 0x00403FF0
};
// CPersistTraits<bool>::TransferIn 0x00403FF0 with its real body in the TU (GOTCHAS 24l recipe: auto_inline(off) +
// noinline): with the body visible VC7.1 homes the empty traits `this` on the dead-for-writes parser slot as retail does
// (an extern declaration puts it on the value slot). Graded on its own as 0x00403FF0; not landed from here.
#pragma auto_inline(off)
__declspec(noinline) void CPersistTraits<bool>::TransferIn(CPersistContext* context, CStringParser* parser, bool* value)
{
    CCharString token = parser->ReadAsString();
    if (EqualsNoCase(token, "TRUE") || EqualsNoCase(token, "BTRUE"))
        *value = true;
    else if (EqualsNoCase(token, "FALSE") || EqualsNoCase(token, "BFALSE"))
        *value = false;
    else
        context->Error(*parser, "Unknown boolean value entry " + token);
}
#pragma auto_inline(on)
template <> struct CPersistTraits<CHeroMarriageInfo> {
    void TransferIn(CPersistContext* context, CStringParser* parser, CHeroMarriageInfo* value);
};

void CPersistTraits<CHeroMarriageInfo>::TransferIn(CPersistContext* context, CStringParser* parser, CHeroMarriageInfo* value)
{
    *value = CHeroMarriageInfo();
    if (parser->ReadAsSymbol() == "(") {
        value->WifeUID = parser->ReadInt64();
        if (parser->ReadAsSymbol() == ",") {
            value->MaritalHomeUID = parser->ReadInt64();
            if (parser->ReadAsSymbol() == ",") {
                value->State = (EMarriageState)TransferInLong(context, parser);
                if (parser->ReadAsSymbol() == ",") {
                    {
                        CPersistTraits<bool> boolTraits;
                        boolTraits.TransferIn(context, parser, &value->GayMarriage);
                    }
                    parser->ReadAsSymbol();
                }
            }
        }
    }
}