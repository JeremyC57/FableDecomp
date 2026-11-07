// CPersistContext::TransferVectorOfSubComponentsLoadText(const char*, CPersistContext_TransferableVectorOfSubComponentsGenericType&) -- parse a vector-of-subcomponents' text commands: `.resize(n);` and `[i].<subcomponent text>;` (each element transferred through a nested single-transfer context)
// Leaf: ?TransferVectorOfSubComponentsLoadText@CPersistContext@@IAEXPBDAAVCPersistContext_TransferableVectorOfSubComponentsGenericType@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x009BC120, 1639 bytes.
// Identity: Ego_r.pdb / FableWin.pdb ?TransferVectorOfSubComponentsLoadText@CPersistContext@@IAEXPBDAAVCPersistContext_TransferableVectorOfSubComponentsGenericType@@@Z
//   (protected); Codex pass 12 reached RELOCATION_MATCH in its own header context; rebased onto our CPersistContext / CStringParser
//   headers (the sibling 0x009BB110 defines and includes).
// Callees: CStringParser members (CheckUnknownSymbols 0x009BBBF0 and the parser reads), CCharString 0x0099EBF0 / 0x0099EAE0 / operator+
//   (const char*, const CCharString&), the nested single-transfer CPersistContext(CStringParser&, ...) ctor (expanded here from its
//   in-TU body), CleanOutLine (in-TU inline body), the generic vector's virtual resize / element transfer slots, CPersistTraitsSLONG::
//   TransferIn 0x009E2590, the error callback.
// Shape: CleanOutLine expands with retail's `[ecx+eax]` SIB order only through the FABLE_RETAIL_CCHARSTRING_GETASCHARARRAY_SELECT opt-in
//   (GetAsCharArray as a preset pointer + `if` select instead of `?:`); with the default accessor VC7.1 emits `[eax+ecx]` (c1-c3, c5: md4).
//   The row-local CBasicString<T>::Compare / operator==(const T*) bodies give the inline literal compares (the landed 0x009BB110 shape).
#define FABLE_RETAIL_CPARSEDITEM
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCHARSTRING_GETASCHARARRAY_SELECT
#define FABLE_RETAIL_CSTRINGPARSER
#define FABLE_RETAIL_CPERSISTCONTEXT
#define FABLE_RETAIL_CDATAOUTPUTSTREAM
#include <string.h>
#include "engine/CStringParser.h"
#include "engine/CPersistContext.h"
#include "engine/CDataOutputStream.h"
#include "engine/CPersistTraits.h"
#include "engine/CPersistContext_TransferableVectorOfSubComponentsGenericType.h"
template<class T> inline long __fastcall CBasicString<T>::Compare(const T* a,const T* b)
{
    while(*a || *b) {
        if(*a<*b)return -1;
        if(*a>*b)return 1;
        ++a; ++b;
    }
    return 0;
}
template<class T> inline bool CBasicString<T>::operator==(const T* text) const
{ return Compare(PData,text)==0; }
CCharString __fastcall operator+(const char*,const CCharString&);
CPersistContext::CPersistContext(CStringParser& parser,void (__fastcall *error_handler)(const CCharString&),bool use_defaults,const CDefinitionManager* pdef_manager,const CSymbolMap* psymbols,CCharString* pstring_to_clear)
{
    Mode=MODE_LOAD_TEXT;
    PDefinitionManager=pdef_manager;
    PSymbolMap=psymbols;
    PParser=&parser;
    ErrorCallback=error_handler;
    PStringToClean=pstring_to_clear;
    UseDefaults=use_defaults;
    UseCRCCheck=false;
    Finished=false;
    SingleTransfer=false;
}
void CPersistContext::CleanOutLine(long start,long end)
{
    if(PStringToClean){
        char* text=(char*)PStringToClean->GetAsCharArray();
        for(long i=start;i<end;++i)if(text[i]!='\n')text[i]=' ';
    }
}
void CPersistContext::TransferVectorOfSubComponentsLoadText(const char* name,CPersistContext_TransferableVectorOfSubComponentsGenericType& comp)
{
    CStringParser parser(*PParser);
    long name_length=strlen(name);
    while(!Finished&&parser.SkipPastWholeString(name)) {
        long text_start=parser.GetStringPos()-name_length;
        CCharString t=parser.ReadAsSymbol();
        if(t==".") {
            t=parser.ReadAsString();
            if(t=="resize") {
                if(parser.ReadAsSymbol()=="(") {
                    long array_size;
                    CPersistTraits<long> int_traits;
                    int_traits.TransferIn(*this,parser,array_size);
                    if(parser.ReadAsSymbol()==")") {
                        comp.Resize(array_size);
                        if(parser.ReadAsSymbol()==";")CleanOutLine(text_start,parser.GetStringPos());
                        else PersistError(parser,"Unable to find terminating semicolon.");
                    }
                } else PersistError(parser,"Unable to find opening bracket.");
            } else PersistError(parser,"Unknown command "+t);
        } else if(t=="[") {
            long array_index;
            CPersistTraits<long> int_traits;
            int_traits.TransferIn(*this,parser,array_index);
            if(array_index==comp.Size())comp.Resize(array_index+1);
            if(array_index<comp.Size()) {
                if(parser.ReadAsSymbol()=="]"&&parser.ReadAsSymbol()==".") {
                    CCharString sub_string=parser.ReadAsStringUntilPastString(";");
                    CStringParser sub_parser(sub_string,parser.GetErrorCallback(),parser.GetFilename(),parser.GetStringPos());
                    CPersistContext context(sub_parser,ErrorCallback,!comp.AreDefaultValsApplied(array_index),PDefinitionManager,PSymbolMap,&sub_string);
                    context.SetAsSingleTransfer();
                    comp.Transfer(context,array_index);
                    context.CheckUnknownSymbols();
                    if(SingleTransfer)Finished=true;
                    CleanOutLine(text_start,parser.GetStringPos());
                    comp.SetDefaultValsApplied(array_index,true);
                } else PersistError(parser,"Unable to find terminating bracket.");
            } else PersistError(parser,"Out of bounds vector access");
        }
    }
}