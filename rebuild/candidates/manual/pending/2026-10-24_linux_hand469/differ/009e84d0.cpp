// CPersistTraits<C3DVector>::TransferIn(CPersistContext* context, CStringParser* parser, C3DVector* value) -- text load of a 3D coordinate: either one ".X/.Y/.Z <float>" member entry, or the full "C3DCoordF(x,y,z)" form
// Grade: DIFFER(867v917) -- as 0x009E81E0: retail jump-threads the member-name null test (two compare chains, branches straight off cmpsb / test al) and keeps the discarded closing ")" compare (null arm and result store into the dead value slot); ours materialises each name result (sete al) and re-tests, and drops the unused ")" result
// Leaf: ?TransferIn@?$CPersistTraits@VC3DVector@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009E84D0, 917 bytes, `ret 0Ch` (traits `this` unused).
// Identity: the strings name it ("C3DCoordF" 0x0129C2DC, "Unknown command for C3DCoordF" 0x0129C2BC, "Unknown entry '"
//   0x0129C294 + " in C3DCoordF" 0x0129C2E8 (retail drops the closing quote), member names "X" 0x0129C2B8 / "Y" 0x0129C2B4
//   / "Z" 0x0129C2F8); the 3D twin of the C2DVector reader 0x009E81E0 in the same TU.
// Callees: as 0x009E81E0: PeekNextItemType @ 0x009BA900, ReadAsSymbol @ 0x009BA790, ReadAsString @ 0x009BA6F0,
//   CBasicString<char>::operator== @ 0x004115A0, ~CCharString @ 0x0099EAE0, CCharString(const char*, long) @ 0x0099EBF0,
//   operator+ @ 0x0099F690 / 0x0099F600, CPersistTraits<float>::TransferIn @ 0x0040F7E0, CPersistContext::Error
//   @ 0x009BAA90. Strings "." 0x01238794, "(" 0x0122E028, "," 0x0122E024, ")" 0x0122E020, "" 0x0122D70E.
// Shape: the 0x009E81E0 reader with a third component (early-return "." test, the float and the float traits `this` on
//   the dead context slot via the noinline float-traits stand-in). Speed-optimised (default) TU.
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

class C3DVector {
public:
    float X;
    float Y;
    float Z;
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
template <> struct CPersistTraits<C3DVector> {
    C3DVector DefaultVal;
    void TransferIn(CPersistContext* context, CStringParser* parser, C3DVector* value);
};

void CPersistTraits<C3DVector>::TransferIn(CPersistContext* context, CStringParser* parser, C3DVector* value)
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
        else if (name == "Z")
            value->Z = component;
        else
            context->Error(*parser, "Unknown entry '" + name + " in C3DCoordF");
    } else if (parser->ReadAsString() == "C3DCoordF") {
        if (parser->ReadAsSymbol() == "(") {
            floatTraits.TransferIn(context, parser, &value->X);
            if (parser->ReadAsSymbol() == ",") {
                floatTraits.TransferIn(context, parser, &value->Y);
                if (parser->ReadAsSymbol() == ",") {
                    floatTraits.TransferIn(context, parser, &value->Z);
                    parser->ReadAsSymbol() == ")";
                }
            }
        }
    } else {
        context->Error(*parser, CCharString("Unknown command for C3DCoordF"));
    }
}
