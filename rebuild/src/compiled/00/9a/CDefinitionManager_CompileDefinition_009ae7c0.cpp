// 009ae7c0 CDefinitionManager::CompileDefinition
#define FABLE_RETAIL_CARRAY_ORIGINAL_INDEX_BASE_PASS22
#define FABLE_RETAIL_CDEFSTRING_CTOR_SETSTRING
// Leaf: ?CompileDefinition@CDefinitionManager@@IAEXAAVCStringParser@@ABVCCharString@@1@Z
// Flags: /O2 /Oy
#define FABLE_RETAIL_COMPILE_DEFINITION_PASS29
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCHARSTRING_GETCHAR
#define FABLE_RETAIL_CCHARSTRING_CONCAT_STRING
#define FABLE_RETAIL_CCHARSTRING_FORCEINLINE_TEXT_EQ
#define FABLE_RETAIL_CBASICSTRING_COMPARE_BODY
#define FABLE_RETAIL_CBASICSTRING_TEXT_EQ_NOINLINE_COMPARE_ZERO
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CDEFSTRING
#define FABLE_RETAIL_CDEFSTRING_CAST_OUTOFLINE
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFCLASSBASE_SETASTEMPLATE
#define FABLE_RETAIL_CDEFINITIONMANAGER_INSTANTIATEDEF
#define FABLE_RETAIL_CDEFINITIONMANAGER_INSTANTIATESUBDEF
#define FABLE_RETAIL_CDEFINITIONMANAGER_PARSEENUMFORSYMBOLS
#define FABLE_RETAIL_CSYMBOLMAP
#define FABLE_RETAIL_CSYMBOLMAP_FINDSYMBOL
#define FABLE_RETAIL_CDEFPOINTER_POINTER_CTOR_ADOPT
#define FABLE_RETAIL_CSTRINGPARSER
#define FABLE_RETAIL_CSTRINGPARSER_PEEKNEXTITEM
#define FABLE_RETAIL_CSTRINGPARSER_INIT_STRING_INLINE
#define FABLE_RETAIL_CPARSEDITEM
#define FABLE_RETAIL_SYMBOLMAP_ENUM_PASS29
#define FABLE_RETAIL_CPERSISTCONTEXT
#include "engine/CStringParser.h"
#include "engine/CDefClassBase.h"
#include "engine/CDefinitionManager.h"
#include "engine/CPersistContext.h"
void CDefinitionManager::CompileDefinition(CStringParser& in_parser, const CCharString& def_type_name, const CCharString& def_instantiation_name)
{
    long def_index;
    if (PDefinitionSymbolMap->FindSymbol(def_instantiation_name, def_index)) {
        CDefPointer<CDefClassBase> def = GetPDefFromGlobalIndex(def_index);
        long start_line = in_parser.GetLine();
        CParsedItem parsed_item;
        bool specialised = false;
        if (in_parser.PeekNextItem(parsed_item) && parsed_item.GetType() == PARSED_ITEM_STRING && parsed_item.GetString() == "specialises") {
            in_parser.ReadAsString();
            CCharString specialise_class_name = in_parser.ReadAsString();
            CDefPointer<CDefClassBase> specialise_class = GetPDefFromInstantiationName(specialise_class_name);
            if (specialise_class.Get()) {
                if (specialise_class->IsSetup()) {
                    specialised = true;
                    bool originally_template = def->IsTemplate();
                    CCharString original_name(static_cast<CCharString>(def->GetInstantiationName()));
                    def->Copy(specialise_class.Get());
                    def->SetAsSetup(false);
                    def->SetAsTemplate(originally_template);
                    def->SetInstantiationName(original_name);
                } else {
                    Error(in_parser, CCharString("Def ") + specialise_class_name + " is not setup to specialise.");
                }
            } else
                Error(in_parser, CCharString("Def ") + specialise_class_name + " does not exist.");
        }
        CCharString def_text = in_parser.ReadAsStringUntilString("#end_definition");
        CStringParser def_parser(def_text, ErrorCallback, "", start_line);
        bool use_defaults = true;
        if (specialised || def->AreDefaultValsApplied()) use_defaults = false;
        while (def_parser.SkipPastString("<")) {
            long begin_pos = def_parser.GetNoCharsIn() - 1;
            CCharString sub_def_name = def_parser.ReadAsStringUntilString(">");
            bool success = def_parser.SkipPastNextItemIfIs(">");
            CCharString end_marker = "<\\" + sub_def_name + ">";
            CCharString sub_text = def_parser.ReadAsStringUntilString(end_marker);
            success = def_parser.SkipPastString(end_marker);
            long end_pos = def_parser.GetNoCharsIn();
            for (long c1 = begin_pos; c1 < end_pos; ++c1)
                if (def_text.GetChar(c1) != '\n')
                    def_text.SetChar(c1, ' ');
            CompileSubDefinition(sub_text, sub_def_name, def_index);
        }
        def_parser.Init(def_text, ErrorCallback, "", start_line);
        CPersistContext def_persist_context(def_parser, ErrorCallback, use_defaults, this, PSymbolMap.Get(), &def_text);
        def->Transfer(def_persist_context);
        def->SetAsSetup(true);
        def->SetDefaultValsApplied(true);
        def_persist_context.CheckUnknownSymbols();
    } else
        Error(in_parser, "Unable to find def class called " + def_instantiation_name);
}
#ifdef _MSC_VER
#pragma auto_inline(off)
#endif
CDefPointer<CDefClassBase> CDefinitionManager::GetPDefFromInstantiationName(const CCharString& instantiation_name)
{
    long global_index = GetDefGlobalIndexFromName(instantiation_name);
    if (global_index > 0)
        return GetPDefFromGlobalIndex(global_index);
    return CDefPointer<CDefClassBase>();
}
#ifdef _MSC_VER
#pragma auto_inline(on)
#endif