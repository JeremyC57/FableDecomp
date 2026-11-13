// CPersistContext::TransferSubComponentLoadText -- transfer named text subcomponents and apply defaults when absent.
// Leaf: ?TransferSubComponentLoadText@CPersistContext@@IAEXPBDAAVCPersistContext_TransferableSubComponentGenericType@@@Z
// Grade: DIFFER(1140v1106) -- original implicit parser copy; aggregate stack order and cleanup register residue.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCHARSTRING_PARSER_HELPERS
#define FABLE_RETAIL_CSTRINGPARSER
#define FABLE_RETAIL_CPARSEDITEM_IMPLICIT_CONSTRUCTION
#define FABLE_RETAIL_CPERSISTCONTEXT
#include <string.h>
#include "engine/CStringParser.h"
#include "engine/CPersistContext.h"
#include "engine/CPersistContext_TransferableSubComponentGenericType.h"

inline CCharString::operator const char*() const
{ if(!PStringData) return ""; return PStringData->Data.PeekData(); }

inline void CStringParser::Init(const CCharString& text,void (__fastcall* error_callback)(const CCharString&),const CCharString& filename,long base_line_offset)
{
    TempCharString=text;
    Init((const char*)text,error_callback,filename,base_line_offset);
}

inline CStringParser::CStringParser(const CCharString& text,void (__fastcall* error_callback)(const CCharString&),const CCharString& filename,long base_line_offset)
{ Init(text,error_callback,filename,base_line_offset); }

inline CPersistContext::CPersistContext(CStringParser& parser,void (__fastcall* error_callback)(const CCharString&),bool use_defaults,const CDefinitionManager* definitions,const CSymbolMap* symbols,CCharString* clean)
{ Init(parser,error_callback,use_defaults,definitions,symbols,clean); }
inline void CPersistContext::Init(CStringParser& parser,void (__fastcall* error_callback)(const CCharString&),bool use_defaults,const CDefinitionManager* definitions,const CSymbolMap* symbols,CCharString* clean)
{
    Mode=MODE_LOAD_TEXT;
    PDefinitionManager=definitions;
    PSymbolMap=symbols;
    PParser=&parser;
    ErrorCallback=error_callback;
    PStringToClean=clean;
    UseDefaults=use_defaults;
    UseCRCCheck=false;
    SingleTransfer=false;
    Finished=false;
}

inline void CPersistContext::CleanOutLine(long start,long end)
{
    if(PStringToClean) {
        char* text=const_cast<char*>((const char*)*PStringToClean);
        for(long i=start;i<end;++i) if(text[i]!='\n')text[i]=' ';
    }
}

void CPersistContext::TransferSubComponentLoadText(const char* name,CPersistContext_TransferableSubComponentGenericType& comp)
{
    bool found_something=false;
    long name_length=strlen(name);
    CStringParser parser(*PParser);
    while(!Finished && parser.SkipPastWholeString(name)) {
        found_something=true;
        long sub_text_start=parser.GetStringPos();
        long text_start=parser.GetStringPos()-name_length;
        long text_end=text_start;
        if(parser.SkipPastString(";")) text_end=parser.GetStringPos();
        else PersistError(parser,"Unable to find terminating semicolon.");
        parser.SetStringPos(sub_text_start);
        if(!(parser.ReadAsSymbol()==".")) return;
            CCharString sub_string=parser.ReadAsStringUntilPastString(";");
            CStringParser sub_parser(sub_string,parser.GetErrorCallback(),parser.GetFilename(),parser.GetStringPos());
            CPersistContext context(sub_parser,ErrorCallback,!comp.AreDefaultValsApplied(),PDefinitionManager,PSymbolMap,&sub_string);
            context.SetAsSingleTransfer();
            comp.Transfer(context);
            comp.SetDefaulValsApplied(true);
            context.CheckUnknownSymbols();
            CleanOutLine(text_start,text_end);

    }
    if(!found_something && !comp.AreDefaultValsApplied()) {
        CStringParser crap_parser("");
        CPersistContext crap_context(crap_parser,parser.GetErrorCallback(),true,0,0,0);
        comp.Transfer(crap_context);
        comp.SetDefaulValsApplied(true);
    }
}
