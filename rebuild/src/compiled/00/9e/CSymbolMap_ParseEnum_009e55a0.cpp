// Flags: (none)
#define FABLE_RETAIL_SYMBOLMAP_ENUM_TRAIT_OWNER_PASS29
// Leaf: ?ParseEnum@CSymbolMap@@IAE_NAAVCStringParser@@@Z
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CSTRINGPARSER
#define FABLE_RETAIL_CPARSEDITEM
#define FABLE_RETAIL_CPARSEDITEM_SYMBOL
#define FABLE_RETAIL_CSYMBOLMAP
#define FABLE_RETAIL_CSYMBOLMAP_ADDSYMBOLDIRTY
#define FABLE_RETAIL_CSYMBOLMAP_CURRENTFILENAME
#define FABLE_RETAIL_SYMBOLMAP_ENUM_PASS29
#define FABLE_RETAIL_CCHARSTRING_FORCEINLINE_TEXT_EQ
#define FABLE_RETAIL_CBASICSTRING_EQ_OUTOFLINE
#include "engine/CStringParser.h"
#include "engine/CSymbolMap.h"
#include "engine/CPersistContext.h"
#include "engine/CDataOutputStream.h"
#include "engine/CPersistTraits.h"
bool CSymbolMap::ParseEnum(CStringParser& parser)
{
    CParsedItem parser_item;
    if (parser.ReadNextItem(parser_item) == true) {
    CCharString symbol_type_name;
    if (parser_item.GetType() == PARSED_ITEM_STRING) {
        symbol_type_name = parser_item.GetString();
        if (!parser.ReadNextItem(parser_item)) {
            Error(CurrentFilename, parser.GetLine(), "Unexpected EOF while reading enum");
            return false;
        }
    } else
        symbol_type_name = "NULL";
    if (parser_item.GetType() == PARSED_ITEM_SYMBOL && !(parser_item.GetSymbol() != "{")) {
    long current_val = 0;
    while (parser.ReadNextItem(parser_item)) {
        if (parser_item.GetType() == PARSED_ITEM_SYMBOL && parser_item.GetSymbol() == "}") {
            if (!parser.ReadNextItem(parser_item)) {
                Error(CurrentFilename, parser.GetLine(), "Unexpected EOF in enum def line");
                return false;
            }
            if (parser_item.GetType() == PARSED_ITEM_SYMBOL && parser_item.GetSymbol() == ";")
                break;
        }
        if (parser_item.GetType() != PARSED_ITEM_STRING)
            Error(CurrentFilename, parser.GetLine(), "Expected string in enum");
        {
        CCharString symbol_name(parser_item.GetString());
        if (!parser.ReadNextItem(parser_item)) {
            Error(CurrentFilename, parser.GetLine(), "Unexpected EOF in enum");
            return false;
        }
        if (parser_item.GetType() == PARSED_ITEM_SYMBOL && parser_item.GetSymbol() == ",") {
            AddSymbolDirty(symbol_type_name, symbol_name, current_val);
        } else if (parser_item.GetType() == PARSED_ITEM_SYMBOL && parser_item.GetSymbol() == "}") {
            AddSymbolDirty(symbol_type_name, symbol_name, current_val);
            if (!parser.ReadNextItem(parser_item)) {
                Error(CurrentFilename, parser.GetLine(), "Unexpected EOF in enum");
                return false;
            }
            if (parser_item.GetType() != PARSED_ITEM_SYMBOL || parser_item.GetSymbol() != ";")
                Error(CurrentFilename, parser.GetLine(), "Expecting ';' in enum");
            break;
        } else if (parser_item.GetType() == PARSED_ITEM_SYMBOL && parser_item.GetSymbol() == "=") {
            CPersistContext persist_context(parser, 0, true, 0, this, 0);
            CPersistTraits<long> val_traits;
            val_traits.TransferIn(persist_context, parser, current_val);
            AddSymbolDirty(symbol_type_name, symbol_name, current_val);
            if (!parser.ReadNextItem(parser_item)) {
                Error(CurrentFilename, parser.GetLine(), "Unexpected EOF in enum");
                return false;
            }
            if (parser_item.GetType() == PARSED_ITEM_SYMBOL) {
                if (parser_item.GetSymbol() == ",") {
            } else if (parser_item.GetSymbol() == "}") {
                if (!parser.ReadNextItem(parser_item)) {
                    Error(CurrentFilename, parser.GetLine(), "Unexpected EOF in enum");
                    return false;
                }
                if (parser_item.GetType() != PARSED_ITEM_SYMBOL || parser_item.GetSymbol() != ";")
                    Error(CurrentFilename, parser.GetLine(), "Expecting ';' in enum");
                break;
            } else
                Error(CurrentFilename, parser.GetLine(), "Expecting ',' '=' or '}' in enum");
            } else
                Error(CurrentFilename, parser.GetLine(), "Expecting ',' '=' or '}' in enum");
        } else
            Error(CurrentFilename, parser.GetLine(), "Expecting ',' '=' or '}' in enum");
        }
        ++current_val;
    }
    return true;
    }
    Error(CurrentFilename, parser.GetLine(), "Expecting a brace in enum");
    return false;
    }
    Error(CurrentFilename, parser.GetLine(), "Cannot read Definition Type name");
    return false;
}