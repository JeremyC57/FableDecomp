// CPersistTraits<CAppearanceModifierGraphics::CEntry>::TransferIn(CPersistContext* context, CStringParser* parser, CEntry* value) -- text load of one appearance-modifier graphics entry "(long,float,long,long,float,long)"; a missing separator stops the read
// Leaf: ?TransferIn@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00686240, 637 bytes (+ int3 padding), `ret 0Ch` (traits `this` unused), no frame.
// Label: the catalog's CPersistTraits<CAppearanceModifierGraphics> names the container; the body reads one CEntry (Ego_r.pdb
//   CAppearanceModifierGraphics = { CArray<CAppearanceModifierGraphics::CEntry> Graphics }; CEntry's members are not in the PDB
//   dump, so they are named by offset and by the reader used: long +0, float +4, long +8, long +0xC, float +0x10, long +0x14).
// Callees: CStringParser::ReadAsSymbol @ 0x009BA790 (hidden CCharString result), CBasicString<char>::operator==(const char*)
//   @ 0x004115A0, ~CCharString @ 0x0099EAE0, TransferIn(CPersistContext*, CStringParser*, long*) @ 0x009E2590 (`__fastcall`),
//   CPersistTraits<float>::TransferIn @ 0x0040F7E0 (real body in the TU as a noinline stand-in). Strings: "(" 0x0122E028, "," 0x0122E024, "" 0x0122D70E.
// Shape: each symbol test is CCharString's inline operator== (a null data pointer compares the literal with "" through the
//   strcmp intrinsic, otherwise CBasicString<char>::operator==); the longs come through the inline by-value TransferInLong()
//   helper (its local in the parser slot); the float traits is a block-local object whose real TransferIn body is in the TU
//   (VC7.1 then homes its `this` on the value slot); the closing ")" is read and discarded. Speed-optimised (default) TU.
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
enum { PARSED_ITEM_SYMBOL = 2 };
void __fastcall TransferIn(CPersistContext* context, CStringParser* parser, long* value);   // 0x009E2590
__forceinline long TransferInLong(CPersistContext* context, CStringParser* parser)
{
    long value;
    TransferIn(context, parser, &value);
    return value;
}

class CAppearanceModifierGraphics {
public:
    struct CEntry {                                    // 0x18 bytes
        long  Long00;                                  // +0x00
        float Float04;                                 // +0x04
        long  Long08;                                  // +0x08
        long  Long0C;                                  // +0x0C
        float Float10;                                 // +0x10
        long  Long14;                                  // +0x14
    };
};

template <class T> struct CPersistTraits;
template <> struct CPersistTraits<float> {
    float DefaultVal;
    void TransferIn(CPersistContext* context, CStringParser* parser, float* value) const;   // 0x0040F7E0
};
// CPersistTraits<float>::TransferIn 0x0040F7E0 with its real body (the landed one) in the TU (GOTCHAS 24l recipe:
// auto_inline(off) + noinline) so VC7.1 homes the never-initialised traits `this` on the value slot as retail does
// (declared only, it lands on the parser slot). Not landed from here.
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
template <> struct CPersistTraits<CAppearanceModifierGraphics::CEntry> {
    void TransferIn(CPersistContext* context, CStringParser* parser, CAppearanceModifierGraphics::CEntry* value);
};

void CPersistTraits<CAppearanceModifierGraphics::CEntry>::TransferIn(CPersistContext* context, CStringParser* parser,
                                                                     CAppearanceModifierGraphics::CEntry* value)
{
    if (parser->ReadAsSymbol() == "(") {
        value->Long00 = TransferInLong(context, parser);
        if (parser->ReadAsSymbol() == ",") {
            {
                CPersistTraits<float> floatTraits;
                floatTraits.TransferIn(context, parser, &value->Float04);
            }
            if (parser->ReadAsSymbol() == ",") {
                value->Long08 = TransferInLong(context, parser);
                if (parser->ReadAsSymbol() == ",") {
                    value->Long0C = TransferInLong(context, parser);
                    if (parser->ReadAsSymbol() == ",") {
                        {
                            CPersistTraits<float> floatTraits;
                            floatTraits.TransferIn(context, parser, &value->Float10);
                        }
                        if (parser->ReadAsSymbol() == ",") {
                            value->Long14 = TransferInLong(context, parser);
                            parser->ReadAsSymbol();
                        }
                    }
                }
            }
        }
    }
}