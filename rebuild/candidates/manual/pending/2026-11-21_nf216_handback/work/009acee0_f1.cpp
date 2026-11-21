// CDefinitionManager::ParseEnumForSymbols(CStringParser& parser) -- read `enum ["TypeName"] { "A", "B" = n, "C", };` and add each entry to the symbol map as type name / entry name / value: "," keeps the running value, "=" reads it through a persist context, "}" must be followed by ";"
// Leaf: ?ParseEnumForSymbols@CDefinitionManager@@IAEXAAVCStringParser@@@Z
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x009ACEE0, 1017 bytes, `ret 4`, esp frame. Protected (Ego_r.pdb / Ego_d.pdb IAE); caller ParseStringForSymbols 0x009ADE90.
// Shape: the Ego_d.pdb locals (current_val, symbol_type_name, parsed_item; loop block val, symbol_name, comma_or_equals; "=" block
//   persist_context, val_traits) and its line table (lib def manager, lines 793..882; ref egod_lines.tsv), statement for statement:
//   every exit is `return;` after its Error (32 / 56-byte lines), the "}" arms are if / else, the separator test is an
//   if / else-if chain ending in the "Unknown symbol" Error, the value goes in through CPersistTraits<long>::TransferIn (20-byte line).
//   The running value advances in the `for` header: retail 0x009AD1D6 `inc ebx` sits after the two string destructors of the loop
//   body (a `current_val++;` last statement graded 1017v1017 with the inc before them); Ego_d line 810 holds test + increment.
// Same TU: CBasicString<char>::operator==(const char*) 0x004115A0 and its Compare 0x00411570 are template bodies visible to this TU
//   and called out of line (FABLE_RETAIL_CBASICSTRING_TEXT_EQ_NOINLINE + COMPARE_BODY): knowing they write no memory, VC7.1 keeps
//   comma_or_equals' data pointer in edi across the calls and tests it for null once (0x009ACFFB..0x009AD001, reused at 0x009AD0C2 /
//   0x009AD0D1 / 0x009AD25B). Without the bodies: 1032v1017.
// Callees: CStringParser PeekNextItemType 0x009BA900 / ReadAsString 0x009BA6F0 / ReadAsSymbol 0x009BA790 / ReadNextItem 0x009BA080 /
//   PeekNextItem 0x009BA8C0, CParsedItem GetSymbol 0x009B9870 / GetString 0x009B9880, CPersistContext ctor 0x009BADD0,
//   CPersistTraitsSLONG::TransferIn 0x009E2590, CSymbolMap::AddSymbolDirty 0x009E54E0, CDefinitionManager::Error 0x009AC9F0,
//   CCharString ctors / dtor / operator= / operator+ (0x0099E4B0, 0x0099EBF0, 0x0099EC30, 0x0099EAE0, 0x0099EFB0, 0x0099EFE0,
//   0x0099F570, 0x0099F600).
// Header opt-ins (hand3c3 overlay): FABLE_RETAIL_CDEFINITIONMANAGER_PARSEENUMFORSYMBOLS (the two protected declarations, the typed
//   ErrorCallback) and FABLE_RETAIL_CPARSEDITEM_ISSTRING (?IsString@CParsedItem@@QBE_NXZ).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCHARSTRING_CHAR_EQ
// Name fix (NF216, 2026-11-21, q12378): FABLE_RETAIL_CBASICSTRING_TEXT_EQ_NOINLINE -> FABLE_RETAIL_CBASICSTRING_TEXT_EQ_NOINLINE_COMPARE_ZERO
//   (with FABLE_RETAIL_CBASICSTRING_COMPARE_BODY): the pick-any ??8?$CBasicString@D@@QBE_NPBD@Z copy this object carries is now the
//   owner 0x004115A0's 88-byte body (Compare-result == 0, Compare expanded), equal in bytes and relocation targets.
#define FABLE_RETAIL_CBASICSTRING_TEXT_EQ_NOINLINE_COMPARE_ZERO
#define FABLE_RETAIL_CBASICSTRING_COMPARE_BODY
#define FABLE_RETAIL_CCHARSTRING_CONCAT_STRING
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CDEFSTRING_CTOR_SETSTRING
#define FABLE_RETAIL_CSYMBOLMAP
#define FABLE_RETAIL_CSYMBOLMAP_ADDSYMBOLDIRTY
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFPOINTER_POINTER_CTOR_ADOPT
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CRESOURCE
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CDEFINITIONMANAGER_INSTANTIATEDEF
#define FABLE_RETAIL_CDEFINITIONMANAGER_PARSEENUMFORSYMBOLS
#define FABLE_RETAIL_CSTRINGPARSER
#define FABLE_RETAIL_CSTRINGPARSER_PEEKNEXTITEM
#define FABLE_RETAIL_CPARSEDITEM
#define FABLE_RETAIL_CPARSEDITEM_SYMBOL
#define FABLE_RETAIL_CPARSEDITEM_ISSTRING
#define FABLE_RETAIL_CPERSISTTRAITS
#include "fable_string.h"
#include "engine/CDefString.h"
#include "engine/CDefClassBase.h"
#include "engine/CDefinitionManager.h"
#include "engine/CStringParser.h"
#include "engine/CDataOutputStream.h"
#include "engine/CPersistContext.h"
#include "engine/CPersistTraits.h"

void CDefinitionManager::ParseEnumForSymbols(CStringParser& parser)
{
    long current_val = 0;
    CCharString symbol_type_name;
    if (parser.PeekNextItemType() == PARSED_ITEM_STRING)
        symbol_type_name = parser.ReadAsString();
    else
        symbol_type_name = "NULL";
    parser.ReadAsSymbol();
    CParsedItem parsed_item;
    for (; parser.ReadNextItem(parsed_item); current_val++)
    {
        if (parsed_item.IsSymbol() && parsed_item.GetSymbol() == "}")
        {
            if (parser.ReadAsSymbol() == ";")
            {
                return;
            }
            else
            {
                Error(parser, "Unexpected symbol in enum.");
                return;
            }
        }
        if (!parsed_item.IsString())
        {
            Error(parser, "Expected string in enum");
            return;
        }
        long val;
        CCharString symbol_name = parsed_item.GetString();
        CCharString comma_or_equals = parser.ReadAsSymbol();
        if (comma_or_equals == ",")
        {
            val = current_val;
        }
        else if (comma_or_equals == '=')
        {
            CPersistContext persist_context(parser, ErrorCallback, true, this, PSymbolMap.Get(), 0);
            CPersistTraits<long> val_traits;
            val_traits.TransferIn(persist_context, parser, val);
            current_val = val;
        }
        else if (comma_or_equals == "}")
        {
            if (parser.ReadAsSymbol() == ";")
            {
                return;
            }
            else
            {
                Error(parser, "Unexpected symbol in enum.");
                return;
            }
        }
        else
        {
            Error(parser, CCharString("Unknown symbol after '") + symbol_name + "'.");
            return;
        }
        PSymbolMap->AddSymbolDirty(symbol_type_name, symbol_name, val);
        if (parser.PeekNextItem(parsed_item) && parsed_item.IsSymbol() && parsed_item.GetSymbol() == ",")
            parser.ReadNextItem(parsed_item);
    }
}