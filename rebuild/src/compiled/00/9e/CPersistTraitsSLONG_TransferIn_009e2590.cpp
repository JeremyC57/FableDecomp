// CPersistTraitsSLONG::TransferIn(CPersistContext& context, CStringParser& parser, long& value) -- text persist read of a long with a tiny expression grammar: a symbol name is resolved through the context's symbol map (an unknown one reports "Unable to find symbol <name>"), anything else is read as an integer; then a following `|`, `+`, `-`, `<<` or `>>` reads the right-hand side recursively and combines it
// Leaf: TransferIn
// Retail: Fable.exe (TLC, VC7.1) @ 0x009E2590, 513 bytes, `__fastcall` (context in ecx, parser in edx, value on the stack),
//   `ret 4`. 115 call sites (every CPersistTraits<long>-style text read; the landed 0x00755F40 and the 21 transferable-
//   component hooks such as 0x00EB68C0 call it with this signature). Name: Ego_r.pdb + FableWin.pdb
//   ?TransferIn@CPersistTraitsSLONG@@SIXAAVCPersistContext@@AAVCStringParser@@AAJ@Z (static `__fastcall`, engine/CPersistTraits.h;
//   the ecx = context / edx = parser / pushed long& call shape at 0x006C93BD; FableWin order puts it 0x30 after TransferOut
//   0x009E2560 as 0x00B2BE00 after 0x00B2BDD0).
// Callees: CStringParser::PeekNextItemType 0x009BA900, CSymbolMap::UpdateDirtyQuick 0x009E5C60 (sorts the symbol vector map when
//   dirty; ICF-shared with the catalog's CTCVillage::OnInitialActivate), CStringParser::ReadAsString 0x009BA6F0 (hidden
//   result), CSymbolMap::FindSymbol 0x009E5170, operator+(const char*, const CCharString&) 0x0099F690 (`__fastcall`, hidden
//   result in ecx), CPersistContext::PersistError 0x009BAA90, ~CCharString 0x0099EAE0, CStringParser::ReadAsInteger 0x009BA540,
//   CCharString(const char*, long = -1) 0x0099EBF0, CStringParser::SkipPastNextItemIfIs 0x009B9E00.
// Globals: "Unable to find symbol " 0x0122DFE8; "|" 0x0129B9FC, "+" 0x01230A74, "-" 0x0122E630, "<<" 0x0129B9F8,
//   ">>" 0x0129B9F4.
// Shape: the symbol path refreshes the map (UpdateDirtyQuick) before reading the name, then looks the name up through the
//   re-read context pointer through an inline GetSymbolMap() accessor (so `this` is loaded before the argument pushes;
//   value starts at 0); every operator test is an IMPLICIT const char* -> CCharString conversion (re-addressed with `lea`
//   after the constructor; an explicit temporary pushes the constructor's eax) built in the dead `value` argument slot (the
//   pointer lives in edi), and the matching branch reads the block-scoped right-hand side into that same slot.
//   CStringParser uses engine/CStringParser.h, CPersistContext engine/CPersistContext.h (PeekPSymbolMap, PersistError),
//   CPersistTraitsSLONG engine/CPersistTraits.h; CSymbolMap (no engine header) follows the landed 0x0040F7E0 model.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CPERSISTCONTEXT
#define FABLE_RETAIL_CPERSISTCONTEXT_PEEKPSYMBOLMAP
#define FABLE_RETAIL_CDATAINPUTSTREAM
#define FABLE_RETAIL_CMEMORYDATAINPUTSTREAM
#define FABLE_RETAIL_CDATAOUTPUTSTREAM
#include "engine/CMemoryDataInputStream.h"
#include "engine/CDataOutputStream.h"
#include "engine/CPersistTraits.h"
#define CStringParser CStringParserLayout
#include "engine/CStringParser.h"
#undef CStringParser

class CCharString;
enum EParsedItemType { PARSED_ITEM_SYMBOL = 2 };              // Ego_r / FableWin global enum (enumerator name descriptive)
class CStringParser : public CStringParserLayout
{
public:
    long ReadAsInteger();                                      // 0x009BA540
    EParsedItemType PeekNextItemType();                        // 0x009BA900 (Ego_r / FableWin)
    CCharString ReadAsString();                                // 0x009BA6F0 (Ego_r / FableWin)
    bool SkipPastNextItemIfIs(const CCharString& item);        // 0x009B9E00 (Ego_r / FableWin)
};

extern const char g_msgUnableToFindSymbol_0122dfe8[];          // 0x0122DFE8 "Unable to find symbol "


class CSymbolMap {
public:
    void UpdateDirtyQuick() const;                                  // 0x009E5C60
    bool FindSymbol(const CCharString& name, long& value) const;   // 0x009E5170
};



void __fastcall CPersistTraitsSLONG::TransferIn(CPersistContext& context, CStringParser& parser, long& value)
{
    CStringParser* p = &parser;
    if (p->PeekNextItemType() == PARSED_ITEM_SYMBOL) {
        context.PeekPSymbolMap()->UpdateDirtyQuick();
        CCharString name = p->ReadAsString();
        long symbolValue = 0;
        if (!context.PeekPSymbolMap()->FindSymbol(name, symbolValue))
            context.PersistError(parser, g_msgUnableToFindSymbol_0122dfe8 + name);
        value = symbolValue;
    }
    else
        value = p->ReadAsInteger();

    if (p->SkipPastNextItemIfIs("|")) {
        long rhs;
        TransferIn(context, parser, rhs);
        value |= rhs;
    }
    else if (p->SkipPastNextItemIfIs("+")) {
        long rhs;
        TransferIn(context, parser, rhs);
        value += rhs;
    }
    else if (p->SkipPastNextItemIfIs("-")) {
        long rhs;
        TransferIn(context, parser, rhs);
        value -= rhs;
    }
    else if (p->SkipPastNextItemIfIs("<<")) {
        long rhs;
        TransferIn(context, parser, rhs);
        value <<= rhs;
    }
    else if (p->SkipPastNextItemIfIs(">>")) {
        long rhs;
        TransferIn(context, parser, rhs);
        value >>= rhs;
    }
}