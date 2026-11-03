// CPersistContext::TransferVectorLoadText(const char*, CPersistContext_TransferableVectorGenericType&) -- parse vector persistence commands.
// Leaf: ?TransferVectorLoadText@CPersistContext@@IAEXPBDAAVCPersistContext_TransferableVectorGenericType@@@Z
#define FABLE_RETAIL_CPARSEDITEM_IMPLICIT_CONSTRUCTION
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCHARSTRING_PARSER_HELPERS
#define FABLE_RETAIL_CSTRINGPARSER
#define FABLE_RETAIL_CPERSISTCONTEXT
#define FABLE_RETAIL_CDATAOUTPUTSTREAM
#include <string.h>
#include "engine/CStringParser.h"
#include "engine/CPersistContext.h"
#include "engine/CPersistTraits.h"
#include "engine/CPersistContext_TransferableVectorGenericType.h"
CCharString __fastcall operator+(const char*,const CCharString&);
void CPersistContext::TransferVectorLoadText(const char* name,CPersistContext_TransferableVectorGenericType& vec)
{
    CStringParser parser(*PParser);
    long name_length=strlen(name);
    while(!Finished && parser.SkipPastWholeString(name)) {
        long text_start=parser.GetStringPos()-name_length;
        CCharString t=parser.ReadAsSymbol();
        if(t==".") {
            t=parser.ReadAsString();
            if(t=="Add") {
                if(parser.ReadAsSymbol()=="(") {
                    long index=vec.Size();
                    vec.Resize(index+1);
                    vec.TransferIn(*this,parser,index);
                    if(SingleTransfer) Finished=true;
                    if(parser.ReadAsSymbol()==")") {
                        if(parser.ReadAsSymbol()==";") CleanOutLine(text_start,parser.GetStringPos());
                        else PersistError(parser,"Unable to find terminating semicolon.");
                    } else PersistError(parser,"Unable to find terminating bracket.");
                } else PersistError(parser,"Unable to find opening bracket.");
            } else if(t=="resize") {
                if(parser.ReadAsSymbol()=="(") {
                    long array_size;
                    CPersistTraits<long> int_traits;
                    int_traits.TransferIn(*this,parser,array_size);
                    if(parser.ReadAsSymbol()==")") {
                        vec.Resize(array_size);
                        if(parser.ReadAsSymbol()==";") CleanOutLine(text_start,parser.GetStringPos());
                        else PersistError(parser,"Unable to find terminating semicolon.");
                    }
                } else PersistError(parser,"Unable to find opening bracket.");
            } else if(t=="clear") {
                if(parser.ReadAsSymbol()=="(") {
                    if(parser.ReadAsSymbol()==")") {
                        if(parser.ReadAsSymbol()==";") {
                            vec.Resize(0);
                            CleanOutLine(text_start,parser.GetStringPos());
                        } else PersistError(parser,"Unable to find terminating semicolon.");
                    } else PersistError(parser,"Unable to find terminating semicolon.");
                } else PersistError(parser,"Unable to find opening bracket.");
            } else PersistError(parser,"Unknown command "+t);
        } else if(t=="[") {
            long array_index;
            CPersistTraits<long> int_traits;
            int_traits.TransferIn(*this,parser,array_index);
            if(array_index==vec.Size()) vec.Resize(array_index+1);
            if(array_index<vec.Size()) {
                if(parser.ReadAsSymbol()=="]") {
                    if(SingleTransfer) Finished=true;
                    vec.TransferIn(*this,parser,array_index);
                    if(parser.ReadAsSymbol()==";") CleanOutLine(text_start,parser.GetStringPos());
                    else PersistError(parser,"Unable to find terminating semicolon.");
                } else PersistError(parser,"Unable to find terminating bracket.");
            } else PersistError(parser,"Out of bounds vector access");
        }
    }
}
