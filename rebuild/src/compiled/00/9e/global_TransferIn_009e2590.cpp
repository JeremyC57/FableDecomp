// TransferIn(CPersistContext* context, CStringParser* parser, long* value) -- text persist read of a long with a tiny expression grammar: a symbol name is resolved through the context's symbol map (an unknown one reports "Unable to find symbol <name>"), anything else is read as an integer; then a following `|`, `+`, `-`, `<<` or `>>` reads the right-hand side recursively and combines it
// Leaf: ?TransferIn@@YIXPAVCPersistContext@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x009E2590, 513 bytes, `__fastcall` (context in ecx, parser in edx, value on the stack),
//   `ret 4`. 115 call sites (every CPersistTraits<long>-style text read; the landed 0x00755F40 and the 21 transferable-
//   component hooks such as 0x00EB68C0 call it with this signature). The catalog label "TransferIn" is kept; the PDB
//   class (CPersistTraits<long>) is not recoverable from the body because the traits object is never passed.
// Callees: CStringParser::PeekNextItemType 0x009BA900, CSymbolMap::UpdateDirty 0x009E5C60 (sorts the symbol vector map when
//   dirty; ICF-shared with the catalog's CTCVillage::OnInitialActivate), CStringParser::ReadAsString 0x009BA6F0 (hidden
//   result), CSymbolMap::FindSymbol 0x009E5170, operator+(const char*, const CCharString&) 0x0099F690 (`__fastcall`, hidden
//   result in ecx), CPersistContext::Error 0x009BAA90, ~CCharString 0x0099EAE0, CStringParser::ReadAsInteger 0x009BA540,
//   CCharString(const char*, long = -1) 0x0099EBF0, CStringParser::SkipPastNextItemIfIs 0x009B9E00.
// Globals: "Unable to find symbol " 0x0122DFE8; "|" 0x0129B9FC, "+" 0x01230A74, "-" 0x0122E630, "<<" 0x0129B9F8,
//   ">>" 0x0129B9F4.
// Shape: the symbol path refreshes the map (UpdateDirty) before reading the name, then looks the name up through the
//   re-read context pointer through an inline GetSymbolMap() accessor (so `this` is loaded before the argument pushes;
//   value starts at 0); every operator test is an IMPLICIT const char* -> CCharString conversion (re-addressed with `lea`
//   after the constructor; an explicit temporary pushes the constructor's eax) built in the dead `value` argument slot (the
//   pointer lives in edi), and the matching branch reads the block-scoped right-hand side into that same slot.
//   CStringParser uses engine/CStringParser.h; CPersistContext / CSymbolMap (no engine header) follow the landed
//   0x0040F7E0 model.
#define CStringParser CStringParserLayout
#include "engine/CStringParser.h"
#undef CStringParser

class CStringParser : public CStringParserLayout
{
public:
    long ReadAsInteger();   // 0x009BA540
};

class CCharString { public:
    CCharStringData* PStringData;
    CCharString(const char* text, long length = -1);           // 0x0099EBF0
    ~CCharString();                                            // 0x0099EAE0
};
CCharString __fastcall operator+(const char* left, const CCharString& right);   // 0x0099F690 (hidden result in ecx)
extern const char g_msgUnableToFindSymbol_0122dfe8[];          // 0x0122DFE8 "Unable to find symbol "

struct CStringParserMethods : CStringParser {
    long PeekNextItemType();                                   // 0x009BA900 (EParsedItemType)
    CCharString ReadAsString();                                // 0x009BA6F0
    bool SkipPastNextItemIfIs(const CCharString& item);        // 0x009B9E00
};

class CSymbolMap {
public:
    void UpdateDirty() const;                                  // 0x009E5C60
    bool FindSymbol(const CCharString& name, long& value) const;   // 0x009E5170
};

class CPersistContext { public:                                       // Ego_r.pdb, 0x2C bytes
    void* PDefinitionManager;                                  // +0x00
    CSymbolMap* PSymbolMap;                                    // +0x04
    void Error(const CStringParser& parser, const CCharString& message);   // 0x009BAA90
    CSymbolMap* GetSymbolMap() const { return PSymbolMap; }
};

enum { PARSED_ITEM_SYMBOL = 2 };

void __fastcall TransferIn(CPersistContext* context, CStringParser* parser, long* value)
{
    CStringParserMethods* p = static_cast<CStringParserMethods*>(parser);
    if (p->PeekNextItemType() == PARSED_ITEM_SYMBOL) {
        context->GetSymbolMap()->UpdateDirty();
        CCharString name = p->ReadAsString();
        long symbolValue = 0;
        if (!context->GetSymbolMap()->FindSymbol(name, symbolValue))
            context->Error(*parser, g_msgUnableToFindSymbol_0122dfe8 + name);
        *value = symbolValue;
    }
    else
        *value = p->ReadAsInteger();

    if (p->SkipPastNextItemIfIs("|")) {
        long rhs;
        TransferIn(context, parser, &rhs);
        *value |= rhs;
    }
    else if (p->SkipPastNextItemIfIs("+")) {
        long rhs;
        TransferIn(context, parser, &rhs);
        *value += rhs;
    }
    else if (p->SkipPastNextItemIfIs("-")) {
        long rhs;
        TransferIn(context, parser, &rhs);
        *value -= rhs;
    }
    else if (p->SkipPastNextItemIfIs("<<")) {
        long rhs;
        TransferIn(context, parser, &rhs);
        *value <<= rhs;
    }
    else if (p->SkipPastNextItemIfIs(">>")) {
        long rhs;
        TransferIn(context, parser, &rhs);
        *value >>= rhs;
    }
}