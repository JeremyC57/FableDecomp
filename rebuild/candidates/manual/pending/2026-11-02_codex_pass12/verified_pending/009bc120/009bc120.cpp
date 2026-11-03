// CPersistContext::TransferVectorOfSubComponentsLoadText(const char*, CPersistContext_TransferableVectorOfSubComponentsGenericType&) -- parse vector resize commands and nested subcomponent assignments.
// Leaf: ?TransferVectorOfSubComponentsLoadText@CPersistContext@@IAEXPBDAAVCPersistContext_TransferableVectorOfSubComponentsGenericType@@@Z
#define FABLE_RETAIL_VENDOR_VECTOR_ORDINARY
#define FABLE_RETAIL_SUBCOMPONENT_TEXT
#define FABLE_RETAIL_REGION_GRAPH
#include <string.h>
#include "engine/CPersistContext.h"
#include "engine/CPersistContext_TransferableVectorOfSubComponentsGenericType.h"
#include "engine/CIntelligentPointer.h"
#include "engine/CPersistTraits.h"
#include "engine/CStringParser.h"
#pragma intrinsic(strlen,strcmp)
CCharString __fastcall operator+(const char*,const CCharString&);

template<class T> long __fastcall CBasicString<T>::Compare(const T* str1,const T* str2)
{
    while(*str1||*str2) {
        if(*str1<*str2)return -1;
        if(*str1>*str2)return 1;
        ++str1;++str2;
    }
    return 0;
}
template<class T> bool CBasicString<T>::operator==(const T* text)const { return Compare(PData,text)==0; }

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
    long name_length=(long)strlen(name);
    while(!Finished&&parser.SkipPastWholeString(name)) {
        long text_start=parser.GetStringPos()-name_length;
        CCharString t=parser.ReadAsSymbol();
        if(t==".") {
            t=parser.ReadAsString();
            if(t=="resize") {
                if(parser.ReadAsSymbol()=="(") {
                    CPersistTraits<long> int_traits;
                    long array_size;
                    int_traits.TransferIn(*this,parser,array_size);
                    if(parser.ReadAsSymbol()==")") {
                        comp.Resize(array_size);
                        if(parser.ReadAsSymbol()==";")CleanOutLine(text_start,parser.GetStringPos());
                        else PersistError(parser,"Unable to find terminating semicolon.");
                    }
                } else PersistError(parser,"Unable to find opening bracket.");
            } else PersistError(parser,"Unknown command "+t);
        } else if(t=="[") {
            CPersistTraits<long> int_traits;
            long array_index;
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
