// CPersistTraits<C2DVector>::TransferIn(CPersistContext* context, CStringParser* parser, C2DVector* value) -- text load of a 2D coordinate: either one ".X <float>" / ".Y <float>" member entry, or the full "C2DCoordF(x,y)" form
// Grade: DIFFER(701v750) -- only the member-name tests: retail keeps name.PStringData in esi across the X compare and jump-threads the null test into two chains (cmpsb ; je per name, CBasicString::operator== calls with `test al,al ; je`), ours keeps it in edx, materialises each result (sete al) and re-tests after a reload; frame, early-return "." test and the float-traits slot match
// Leaf: ?TransferIn@?$CPersistTraits@VC2DVector@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009E81E0, 752 bytes, `ret 0Ch` (traits `this` unused).
// Identity: the strings name it ("C2DCoordF" 0x0129C288, "Unknown command for C2DCoordF" 0x0129C268, "Unknown entry '"
//   0x0129C294 + "' in C2DCoordF" 0x0129C2A4, member names "X" 0x0129C2B8 / "Y" 0x0129C2B4) and the landed writer
//   CPersistTraits<C2DVector>::TransferOut 0x009E7F00 sits beside it; it is called by the CActionInputControl reader
//   0x004112A0 on a zero-defaulted traits object for ControlDirection. The catalog label
//   "CPersistTraits<CBrainBehaviour>::TransferIn" is propagated.
// Callees: CStringParser::PeekNextItemType @ 0x009BA900, ReadAsSymbol @ 0x009BA790, ReadAsString @ 0x009BA6F0 (hidden
//   CCharString results), CBasicString<char>::operator==(const char*) @ 0x004115A0, ~CCharString @ 0x0099EAE0,
//   CCharString(const char*, long) @ 0x0099EBF0, operator+(const char*, const CCharString&) @ 0x0099F690 and
//   operator+(const CCharString&, const char*) @ 0x0099F600 (`__fastcall`, hidden result in ecx), CPersistTraits<float>
//   ::TransferIn @ 0x0040F7E0, CPersistContext::Error(const CStringParser&, const CCharString&) @ 0x009BAA90. Strings
//   "." 0x01238794, "(" 0x0122E028, "," 0x0122E024, ")" 0x0122E020, "" 0x0122D70E.
// Shape: each symbol test is CCharString's inline operator==(const char*) (a null data pointer compares the literal
//   with "" through the strcmp intrinsic, otherwise CBasicString<char>::operator== out of line); the member form keeps
//   the read float and the float traits `this` on the dead context slot (context lives in ebx), which needs the float
//   traits' real body in the TU (GOTCHAS 24l recipe: auto_inline(off) + noinline stand-in); the closing ")" result is
//   discarded into the dead context slot. Speed-optimised (default) TU.
#include <string.h>
#pragma intrinsic(strcmp)
struct CBasicStringChar {
    char*         PData;
    unsigned long StringLength;
    bool operator==(const char* text) const;           // 0x004115A0
};
struct CCharString {
    CBasicStringChar* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                    // 0x0099EAE0
    __forceinline bool operator==(const char* text) const
    {
        if (PStringData == 0)
            return strcmp(text, "") == 0;
        return *PStringData == text;
    }
};
CCharString __fastcall operator+(const char* left, const CCharString& right);   // 0x0099F690
CCharString __fastcall operator+(const CCharString& left, const char* right);   // 0x0099F600
extern const char g_msgUnknownSymbol_0122dfe8[];       // 0x0122DFE8 (the float traits' message)

struct CStringParser {
    long PeekNextItemType();                           // 0x009BA900 (EParsedItemType)
    CCharString ReadAsSymbol();                        // 0x009BA790
    CCharString ReadAsString();                        // 0x009BA6F0
    float ReadAsFloat();                               // 0x009BA650
};
struct CSymbolMap {
    bool Lookup(const CCharString& name, long* value) const;   // 0x009E5170
};
struct CPersistContext {                               // Ego_r.pdb, 0x2C bytes
    void*       PDefinitionManager;                    // +0x00
    CSymbolMap* PSymbolMap;                            // +0x04
    void Error(const CStringParser& parser, const CCharString& message);   // 0x009BAA90
};

class C2DVector {
public:
    float X;
    float Y;
};

enum { PARSED_ITEM_SYMBOL = 2 };

template <class T> struct CPersistTraits;
template <> struct CPersistTraits<float> {
    float DefaultVal;
    void TransferIn(CPersistContext* context, CStringParser* parser, float* value) const;
};
// CPersistTraits<float>::TransferIn 0x0040F7E0 with its real body (the landed one) in the TU so VC7.1 homes the empty
// traits `this` on the live context slot as retail does. Not landed from here.
#pragma auto_inline(off)
__declspec(noinline) void CPersistTraits<float>::TransferIn(CPersistContext* context, CStringParser* parser, float* value) const
{
    if (parser->PeekNextItemType() == PARSED_ITEM_SYMBOL) {
        CCharString name = parser->ReadAsString();
        long symbol;
        if (!context->PSymbolMap->Lookup(name, &symbol))
            context->Error(*parser, g_msgUnknownSymbol_0122dfe8 + name);
        *value = (float)symbol;
    }
    else
        *value = parser->ReadAsFloat();
}
#pragma auto_inline(on)
template <> struct CPersistTraits<C2DVector> {
    C2DVector DefaultVal;
    void TransferIn(CPersistContext* context, CStringParser* parser, C2DVector* value);
};

void CPersistTraits<C2DVector>::TransferIn(CPersistContext* context, CStringParser* parser, C2DVector* value)
{
    CPersistTraits<float> floatTraits;
    if (parser->PeekNextItemType() == 1) {
        if (!(parser->ReadAsSymbol() == "."))
            return;
        CCharString name = parser->ReadAsString();
        float component;
        floatTraits.TransferIn(context, parser, &component);
        if (name == "X")
            value->X = component;
        else if (name == "Y")
            value->Y = component;
        else
            context->Error(*parser, "Unknown entry '" + name + "' in C2DCoordF");
    } else if (parser->ReadAsString() == "C2DCoordF") {
        if (parser->ReadAsSymbol() == "(") {
            floatTraits.TransferIn(context, parser, &value->X);
            if (parser->ReadAsSymbol() == ",") {
                floatTraits.TransferIn(context, parser, &value->Y);
                parser->ReadAsSymbol() == ")";
            }
        }
    } else {
        context->Error(*parser, CCharString("Unknown command for C2DCoordF"));
    }
}
