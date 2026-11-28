// Leaf: ?TransferIn@?$CPersistTraits@VC3DVector@@@@QAEXAAVCPersistContext@@AAVCStringParser@@AAVC3DVector@@@Z
// Flags:
#define FABLE_RETAIL_CBASICSTRING_COMPARE_BODY
#define FABLE_RETAIL_CBASICSTRING_TEXT_EQ_COMPARE_ZERO
#define FABLE_RETAIL_CPARSEDITEM
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCHARSTRING_FORCEINLINE_TEXT_EQ
#define FABLE_RETAIL_CSTRINGPARSER
#define FABLE_RETAIL_CPERSISTCONTEXT
#define FABLE_RETAIL_CPERSISTCONTEXT_PEEKPSYMBOLMAP
#define FABLE_RETAIL_CSYMBOLMAP
#define FABLE_RETAIL_CSYMBOLMAP_FINDSYMBOL
#define FABLE_RETAIL_CDATAOUTPUTSTREAM
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_CPERSISTTRAITS_FLOAT_TRANSFERIN_BODY
#include <string.h>
#include "engine/CStringParser.h"
#include "engine/CPersistContext.h"
#include "engine/CDataOutputStream.h"
#include "engine/C3DVector.h"
#include "engine/CPersistTraits.h"

void CPersistTraits<C3DVector>::TransferIn(CPersistContext& persist_context, CStringParser& parser, C3DVector& val)
{
    if (parser.PeekNextItemType() == PARSED_ITEM_SYMBOL) {
        // Retail returns at the first failed parser assertion.
        if (!(parser.ReadAsSymbol() == "."))
            return;
        CCharString name = parser.ReadAsString();
        CPersistTraits<float> float_traits;
        float fval;
        float_traits.TransferIn(persist_context, parser, fval);
        if (name == "X")
            val.X = fval;
        else if (name == "Y")
            val.Y = fval;
        else if (name == "Z")
            val.Z = fval;
        else
            persist_context.PersistError(parser, "Unknown entry '" + name + " in C3DCoordF");
    } else if (parser.ReadAsString() == "C3DCoordF") {
        CPersistTraits<float> float_traits;
        if (!(parser.ReadAsSymbol() == "("))
            return;
        float_traits.TransferIn(persist_context, parser, val.X);
        if (!(parser.ReadAsSymbol() == ","))
            return;
        float_traits.TransferIn(persist_context, parser, val.Y);
        if (!(parser.ReadAsSymbol() == ","))
            return;
        float_traits.TransferIn(persist_context, parser, val.Z);
        if (!(parser.ReadAsSymbol() == ")"))
            return;
    } else {
        persist_context.PersistError(parser, "Unknown command for C3DCoordF");
    }
}
#include <stdio.h>
#include <stdlib.h>
static int failures, runs, live_strings, errors, lookups, reads, peeks;
static char last_error[160];
static void check(bool v) { if (!v) { if (failures<3) printf("failure at case %d\n",runs);++failures; } }
static const char* chars(const CCharString& str) {
    CCharStringData* data;memcpy(&data,&str,sizeof(data));return data?data->Data.PeekData():"";
}
CCharString::CCharString(const char* text,long length) : PStringData(0) {
    if (!text) return;
    if (length<0) length=(long)strlen(text);
    PStringData=(CCharStringData*)malloc(sizeof(CCharStringData));memset(PStringData,0,sizeof(CCharStringData));
    char* copy=(char*)malloc(length+1);memcpy(copy,text,length);copy[length]=0;
    memcpy(PStringData,&copy,sizeof(copy));memcpy((char*)PStringData+4,&length,sizeof(length));
    long one=1;memcpy((char*)PStringData+sizeof(CBasicString<char>),&one,sizeof(one));++live_strings;
}
static long change_references(CCharStringData* p,long delta) {
    char* field=(char*)p+sizeof(CBasicString<char>);long n;memcpy(&n,field,sizeof(n));n+=delta;memcpy(field,&n,sizeof(n));return n;
}
CCharString::CCharString(const CCharString& rhs) : PStringData(rhs.PStringData) { if(PStringData) change_references(PStringData,1); }
CCharString::~CCharString() {
    if (PStringData && change_references(PStringData,-1)==0) { free((void*)PStringData->Data.PeekData());free(PStringData);--live_strings; }
}
static CCharString join(const char* a,const char* b) {
    char joined[160];check(strlen(a)+strlen(b)<sizeof(joined));strcpy(joined,a);strcat(joined,b);return CCharString(joined);
}
CCharString __fastcall operator+(const CCharString& a,const char* b) { return join(chars(a),b); }
CCharString __fastcall operator+(const char* a,const CCharString& b) { return join(a,chars(b)); }
struct Token { EParsedItemType type;const char* text;float number; };
static Token stream[12];static int length;
static void token(EParsedItemType type,const char* text,float number=0) {
    stream[length].type=type;stream[length].text=text;stream[length].number=number;++length;
}
static Token take() {
    ++reads;if (reads>length) { check(false);Token t={PARSED_ITEM_NULL,0,0};return t; }
    return stream[reads-1];
}
EParsedItemType CStringParser::PeekNextItemType() { ++peeks;return reads<length?stream[reads].type:PARSED_ITEM_NULL; }
CCharString CStringParser::ReadAsSymbol() { return CCharString(take().text); }
CCharString CStringParser::ReadAsString() { return CCharString(take().text); }
float CStringParser::ReadAsFloat() { return take().number; }
static union {double align;unsigned char b[sizeof(CStringParser)];} parser_object;
static union {double align;unsigned char b[sizeof(CPersistContext)];} context_object;
static union {double align;unsigned char b[sizeof(CSymbolMap)];} symbol_object;
bool CSymbolMap::FindSymbol(const CCharString& name,long& out) const {
    check(this==(const CSymbolMap*)symbol_object.b);++lookups;
    if (strcmp(chars(name),"gold")==0) {out=17;return true;}out=-88;return false;
}
void CPersistContext::PersistError(const CStringParser& parser,const CCharString& message) const {
    check(this==(CPersistContext*)context_object.b && &parser==(CStringParser*)parser_object.b);
    check(strlen(chars(message))<sizeof(last_error));strcpy(last_error,chars(message));++errors;
}
static void start() { ++runs;length=reads=peeks=errors=lookups=0;last_error[0]=0;check(live_strings==0); }
static void run(float x,float y,float z,int count,int peek_count,int lookup_count,const char* error,int error_count) {
    CPersistTraits<C3DVector> traits;C3DVector v(1,2,3);
    traits.TransferIn(*(CPersistContext*)context_object.b,*(CStringParser*)parser_object.b,v);
    check(v.X==x && v.Y==y && v.Z==z && length==count && reads==count);
    check(peeks==peek_count && lookups==lookup_count && errors==error_count && strcmp(last_error,error)==0 && live_strings==0);
}
static float scalar(unsigned kind,float f) {
    if (kind==0) {token(PARSED_ITEM_FLOAT,0,f);return f;}
    if (kind==1) {token(PARSED_ITEM_STRING,"gold");return 17;}
    token(PARSED_ITEM_STRING,"missing");return -88;
}
int main() {
    const CSymbolMap* symbols=(CSymbolMap*)symbol_object.b;memcpy(context_object.b+4,&symbols,sizeof(symbols));
    const char* names[]={"X","Y","Z","Other","",0};const char* wrong[]={"?","",0};
    for(int i=0;i<3;++i) {
        start();token(PARSED_ITEM_SYMBOL,wrong[i]);run(1,2,3,1,1,0,"",0);
        start();token(PARSED_ITEM_STRING,wrong[i]);run(1,2,3,1,1,0,"Unknown command for C3DCoordF",1);
    }
    for(int i=0;i<6;++i) for(unsigned k=0;k<3;++k) {
        start();token(PARSED_ITEM_SYMBOL,".");token(PARSED_ITEM_STRING,names[i]);float f=scalar(k,-2.5f);
        char msg[160]="";if(k==2)strcpy(msg,"Unable to find symbol missing");
        if(i>=3) {strcpy(msg,"Unknown entry '");strcat(msg,names[i]?names[i]:"");strcat(msg," in C3DCoordF");}
        run(i==0?f:1,i==1?f:2,i==2?f:3,3,2,k!=0,msg,(k==2)+(i>=3));
    }
    for(int stop=0;stop<=4;++stop) for(unsigned mix=0;mix<27;++mix) {
        start();token(PARSED_ITEM_STRING,"C3DCoordF");
        float expect[3]={1,2,3};int count=1,peek_count=1,lookup_count=0,errs=0;unsigned choices=mix;
        for(int j=0;j<=3;++j) {
            if(j==stop){token(PARSED_ITEM_SYMBOL,wrong[mix%3]);++count;break;}
            token(PARSED_ITEM_SYMBOL,j==0?"(":j==3?")":",");++count;
            if(j<3){unsigned k=choices%3;choices/=3;expect[j]=scalar(k,4.5f+j);++count;++peek_count;if(k)++lookup_count;if(k==2)++errs;}
        }
        run(expect[0],expect[1],expect[2],count,peek_count,lookup_count,errs?"Unable to find symbol missing":"",errs);
    }
    if(failures)return 1;
    puts("009e84d0_TEST PASS");return 0;
}