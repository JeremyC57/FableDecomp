// Self-contained behavior fixture for the exact selected context constructor, line cleaner and parser body.
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <vector>
#ifndef _MSC_VER
#define __fastcall
#endif
static void Check(bool b){if(!b){puts("FAIL");exit(1);}}
class CCharString {public:std::string Text;CCharString(){}CCharString(const char* p,long n=-1):Text(n<0?std::string(p):std::string(p,n)){}CCharString(const CCharString&x):Text(x.Text){}CCharString&operator=(const CCharString&x){Text=x.Text;return*this;}bool operator==(const char*x)const{return Text==x;}const char*GetAsCharArray()const{return Text.empty()?"":&const_cast<std::string&>(Text)[0];}};
CCharString __fastcall operator+(const char* a,const CCharString& b){CCharString result;result.Text=std::string(a)+b.Text;return result;}
class CStringParser {
public:
 std::string String;unsigned long Position;CCharString Filename;void (__fastcall *ErrorCallback)(const CCharString&);long Offset;
 CStringParser(const char*s):String(s),Position(0),ErrorCallback(0),Offset(0){}
 CStringParser(const CStringParser&p):String(p.String),Position(p.Position),Filename(p.Filename),ErrorCallback(p.ErrorCallback),Offset(p.Offset){}
 CStringParser(const CCharString&s,void (__fastcall *cb)(const CCharString&),const CCharString&file,long offset):String(s.Text),Position(0),Filename(file),ErrorCallback(cb),Offset(offset){}
 void White(){while(Position<String.size()&&(String[Position]==' '||String[Position]=='\n'||String[Position]=='\t'))++Position;}
 bool SkipPastWholeString(const CCharString& name){size_t found=String.find(name.Text,Position);if(found==std::string::npos)return false;Position=(unsigned long)(found+name.Text.size());return true;}
 unsigned long GetStringPos()const{return Position;}
 const CCharString&GetFilename()const{return Filename;}
 void (__fastcall *GetErrorCallback()const)(const CCharString&){return ErrorCallback;}
 CCharString ReadAsSymbol(){White();if(Position>=String.size())return CCharString("");return CCharString(String.c_str()+Position++,1);}
 CCharString ReadAsString(){White();unsigned long begin=Position;while(Position<String.size()&&((String[Position]>='a'&&String[Position]<='z')||(String[Position]>='A'&&String[Position]<='Z')||String[Position]=='_'))++Position;return CCharString(String.c_str()+begin,Position-begin);}
 CCharString ReadAsStringUntilPastString(const CCharString& end){White();size_t found=String.find(end.Text,Position);if(found==std::string::npos)found=String.size();else found+=end.Text.size();CCharString result(String.c_str()+Position,(long)(found-Position));Position=(unsigned long)found;return result;}
 long Integer(){White();char* end=0;long value=strtol(String.c_str()+Position,&end,10);Position=(unsigned long)(end-String.c_str());return value;}
};
class CDefinitionManager;class CSymbolMap;class CMemoryDataInputStream;class CDataOutputStream;
class CPersistContext_TransferableVectorOfSubComponentsGenericType;
static std::vector<std::string> errors;
static unsigned checkedContexts=0;
class CPersistContext {
public:
 enum EMode{MODE_LOAD_TEXT=0,MODE_SAVE_TEXT=1,MODE_LOAD_BINARY=2,MODE_SAVE_BINARY=3,MODE_CLEAR=4};
 const CDefinitionManager*PDefinitionManager;const CSymbolMap*PSymbolMap;CStringParser*PParser;void(__fastcall*ErrorCallback)(const CCharString&);CCharString*PStringToClean;bool UseDefaults,UseCRCCheck,SingleTransfer,Finished;EMode Mode;bool Binary;CCharString*PStringToSaveTo;CMemoryDataInputStream*PLoadStream;CDataOutputStream*PSaveStream;
 CPersistContext(CStringParser&,void (__fastcall*)(const CCharString&),bool,const CDefinitionManager*,const CSymbolMap*,CCharString*);
 void SetAsSingleTransfer(){SingleTransfer=true;}
 void PersistError(const CStringParser&,const CCharString&message)const{errors.push_back(message.Text);}
 void CheckUnknownSymbols(){Check(SingleTransfer);Check(Mode==MODE_LOAD_TEXT);++checkedContexts;}
 void CleanOutLine(long,long);
 void TransferVectorOfSubComponentsLoadText(const char*,CPersistContext_TransferableVectorOfSubComponentsGenericType&);
};
class CPersistContext_TransferableVectorOfSubComponentsGenericType{public:virtual long Size()=0;virtual void Resize(long)=0;virtual bool AreDefaultValsApplied(long)=0;virtual void SetDefaultValsApplied(long,bool)=0;virtual void Transfer(CPersistContext&,long)=0;};
template<class T>class CPersistTraits{public:T DefaultVal;void TransferIn(CPersistContext&,CStringParser&p,T&v){v=(T)p.Integer();}};
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

class RecordedComponents:public CPersistContext_TransferableVectorOfSubComponentsGenericType {
public:std::vector<long> Values;std::vector<int> Applied;std::vector<int> DefaultsObserved;unsigned ResizeCalls,TransferCalls;
 RecordedComponents():ResizeCalls(0),TransferCalls(0){}
 long Size(){return (long)Values.size();}
 void Resize(long n){Check(n>=0&&n<100);Values.resize(n,0);Applied.resize(n,0);++ResizeCalls;}
 bool AreDefaultValsApplied(long n){Check(n>=0&&n<Size());return Applied[n]!=0;}
 void SetDefaultValsApplied(long n,bool flag){Check(n>=0&&n<Size());Applied[n]=flag?1:0;}
 void Transfer(CPersistContext&ctx,long n){Check(n>=0&&n<Size());Check(ctx.SingleTransfer);Check(ctx.PParser->ReadAsString()=="amount");Check(ctx.PParser->ReadAsSymbol()=="=");Values[n]=ctx.PParser->Integer();Check(ctx.PParser->ReadAsSymbol()==";");DefaultsObserved.push_back(ctx.UseDefaults?1:0);++TransferCalls;}
};
static void BlankExceptNewlines(const CCharString&s){for(size_t n=0;n<s.Text.size();++n)Check(s.Text[n]==' '||s.Text[n]=='\n');}
int main(){unsigned cases=0;
for(int count=1;count<=8;++count)for(int value=0;value<4;++value){
 char buffer[256];sprintf(buffer,"v.resize(%d);\nv[%d].amount=%d;v[%d].amount=%d;",count,count-1,value,count-1,value+1);CCharString clean(buffer);CStringParser parser(buffer);CPersistContext context(parser,0,true,0,0,&clean);RecordedComponents comp;errors.clear();checkedContexts=0;context.TransferVectorOfSubComponentsLoadText("v",comp);Check(errors.empty());Check(comp.Size()==count&&comp.Values[count-1]==value+1);Check(comp.TransferCalls==2&&comp.ResizeCalls==1);Check(comp.DefaultsObserved[0]==1&&comp.DefaultsObserved[1]==0&&comp.Applied[count-1]);Check(checkedContexts==2&&!context.Finished&&parser.Position==0);BlankExceptNewlines(clean);++cases;
}
{const char* text="v[0].amount=3;v[1].amount=4;";CCharString clean(text);CStringParser parser(text);CPersistContext context(parser,0,true,0,0,&clean);RecordedComponents comp;errors.clear();context.TransferVectorOfSubComponentsLoadText("v",comp);Check(errors.empty()&&comp.Size()==2&&comp.ResizeCalls==2&&comp.Values[0]==3&&comp.Values[1]==4);BlankExceptNewlines(clean);++cases;}
{const char* text="v.resize(1);v[0].amount=3;v[1].amount=4;";CCharString clean(text);CStringParser parser(text);CPersistContext context(parser,0,true,0,0,&clean);context.SetAsSingleTransfer();RecordedComponents comp;errors.clear();context.TransferVectorOfSubComponentsLoadText("v",comp);Check(errors.empty()&&context.Finished&&comp.Size()==1&&comp.Values[0]==3&&comp.TransferCalls==1);Check(clean.Text.find("v[1]")!=std::string::npos);++cases;}
{CStringParser parser("v.resize(2);");CPersistContext context(parser,0,true,0,0,0);context.Finished=true;RecordedComponents comp;errors.clear();context.TransferVectorOfSubComponentsLoadText("v",comp);Check(errors.empty()&&comp.Size()==0&&comp.ResizeCalls==0);++cases;}
{CStringParser parser("v.resize(2);");CPersistContext context(parser,0,true,0,0,0);RecordedComponents comp;errors.clear();context.TransferVectorOfSubComponentsLoadText("v",comp);Check(errors.empty()&&comp.Size()==2);++cases;}
const char*bad[]={"v.unknown;","v.resize 2);","v.resize(2)!","v[4].amount=1;","v[0)!amount=1;"};
const char*messages[]={"Unknown command unknown","Unable to find opening bracket.","Unable to find terminating semicolon.","Out of bounds vector access","Unable to find terminating bracket."};
for(int n=0;n<5;++n){CStringParser parser(bad[n]);CPersistContext context(parser,0,true,0,0,0);RecordedComponents comp;errors.clear();context.TransferVectorOfSubComponentsLoadText("v",comp);Check(errors.size()==1&&errors[0]==messages[n]);++cases;}
{CStringParser parser("v.resize(2]");CPersistContext context(parser,0,true,0,0,0);RecordedComponents comp;errors.clear();context.TransferVectorOfSubComponentsLoadText("v",comp);Check(errors.empty()&&comp.Size()==0);++cases;}
Check(cases==42);puts("009bc120_TEST PASS");return 0;}
