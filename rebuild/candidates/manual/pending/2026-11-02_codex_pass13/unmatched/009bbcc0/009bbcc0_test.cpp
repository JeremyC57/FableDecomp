// Selected reconstruction uses the agreed implicit CParsedItem construction opt-in; exact Main unchanged.
#include <cassert>
#include <cstdio>
#include <cstring>
#include <string>
#include <vector>
#ifndef _MSC_VER
#define __fastcall
#endif
static int strings=0,parsers=0,contexts=0;
class CCharString {public:char text[512];CCharString(){text[0]=0;++strings;}CCharString(const char*s){assert(std::strlen(s)<sizeof text);std::strcpy(text,s);++strings;}CCharString(const CCharString&s){std::strcpy(text,s.text);++strings;}~CCharString(){--strings;}CCharString&operator=(const CCharString&s){std::strcpy(text,s.text);return*this;}operator const char*()const{return text;}bool operator==(const char*s)const{return std::strcmp(text,s)==0;}};
struct ParserLifetime{ParserLifetime(){++parsers;}ParserLifetime(const ParserLifetime&){++parsers;}~ParserLifetime(){--parsers;}};
struct ContextLifetime{ContextLifetime(){++contexts;}~ContextLifetime(){--contexts;}};
struct Occurrence{unsigned long begin,afterName,end;bool semicolon,separator;std::string body;};
struct TransferRecord{bool defaults,single,clean,definitions,symbols;int callback;long base;std::string body,filename;};
struct TestState{std::vector<Occurrence>occ;std::vector<TransferRecord>records;int skipCalls,unknown,errors,writes,finishAfter,throwTransferAt;bool throwError,applied;std::string name;TestState():skipCalls(0),unknown(0),errors(0),writes(0),finishAfter(0),throwTransferAt(-1),throwError(false),applied(false){}};static TestState*ts;
static void __fastcall parser_error(const CCharString&){}static void __fastcall context_error(const CCharString&){}
class CStringParser{ParserLifetime life;public:const char*String;unsigned long BaseLineOffset;CCharString Filename,TempCharString;void (__fastcall*ErrorCallback)(const CCharString&);struct CCursorPos{bool InQuotes;unsigned long StringPos,Line;}CursorPos;unsigned next,active;
 CStringParser(const char*s,void (__fastcall*cb)(const CCharString&)=0,const CCharString&filename=CCharString(""),long base=0):String(s),BaseLineOffset(base),Filename(filename),ErrorCallback(cb),next(0),active(0){CursorPos.InQuotes=false;CursorPos.StringPos=0;CursorPos.Line=0;}
 CStringParser(const CCharString&s,void (__fastcall*cb)(const CCharString&)=0,const CCharString&filename=CCharString(""),long base=0):String(s),BaseLineOffset(base),Filename(filename),TempCharString(s),ErrorCallback(cb),next(0),active(0){CursorPos.InQuotes=false;CursorPos.StringPos=0;CursorPos.Line=0;}
 bool SkipPastWholeString(const CCharString&name){assert(name.text==ts->name);++ts->skipCalls;if(next==ts->occ.size())return false;active=next++;CursorPos.StringPos=ts->occ[active].afterName;return true;}
 bool SkipPastString(const CCharString&s){assert(std::strcmp(s.text,";")==0);const Occurrence&o=ts->occ[active];if(o.semicolon)CursorPos.StringPos=o.end;return o.semicolon;}
 unsigned long GetStringPos()const{return CursorPos.StringPos;}void SetStringPos(unsigned long p){assert(p==ts->occ[active].afterName);CursorPos.StringPos=p;}
 CCharString ReadAsSymbol(){assert(CursorPos.StringPos==ts->occ[active].afterName);++CursorPos.StringPos;return CCharString(ts->occ[active].separator?".":"!");}
 CCharString ReadAsStringUntilPastString(const CCharString&s){assert(std::strcmp(s.text,";")==0);const Occurrence&o=ts->occ[active];CursorPos.StringPos=o.end;return CCharString(o.body.c_str());}
 void (__fastcall*GetErrorCallback()const)(const CCharString&){return ErrorCallback;}const CCharString&GetFilename()const{return Filename;}
};
class CDefinitionManager{};class CSymbolMap{};
class CPersistContext_TransferableSubComponentGenericType;
class CPersistContext {ContextLifetime life;public:enum EMode{MODE_LOAD_TEXT};const CDefinitionManager*PDefinitionManager;const CSymbolMap*PSymbolMap;CStringParser*PParser;void (__fastcall*ErrorCallback)(const CCharString&);CCharString*PStringToClean;bool UseDefaults,UseCRCCheck,SingleTransfer,Finished;EMode Mode;
 CPersistContext(){}CPersistContext(CStringParser&p,void (__fastcall*cb)(const CCharString&),bool defaults,const CDefinitionManager*d,const CSymbolMap*s,CCharString*clean){Init(p,cb,defaults,d,s,clean);}
 void Init(CStringParser&p,void (__fastcall*cb)(const CCharString&),bool defaults,const CDefinitionManager*d,const CSymbolMap*s,CCharString*clean){Mode=MODE_LOAD_TEXT;PDefinitionManager=d;PSymbolMap=s;PParser=&p;ErrorCallback=cb;PStringToClean=clean;UseDefaults=defaults;UseCRCCheck=false;SingleTransfer=false;Finished=false;}
 void SetAsSingleTransfer(){SingleTransfer=true;}
 void PersistError(const CStringParser&,const CCharString&s)const{assert(std::strcmp(s.text,"Unable to find terminating semicolon.")==0);++ts->errors;}
 void CheckUnknownSymbols(){assert(ts->applied&&SingleTransfer);++ts->unknown;}
 void CleanOutLine(long start,long end){if(PStringToClean){char*text=const_cast<char*>((const char*)*PStringToClean);for(long i=start;i<end;++i)if(text[i]!='\n')text[i]=' ';}}
 void TransferSubComponentLoadText(const char*,CPersistContext_TransferableSubComponentGenericType&);
};
class CPersistContext_TransferableSubComponentGenericType{public:virtual void Transfer(CPersistContext&)=0;virtual bool AreDefaultValsApplied()=0;virtual void SetDefaulValsApplied(bool)=0;};
class MockComponent:public CPersistContext_TransferableSubComponentGenericType{CPersistContext&parent;public:MockComponent(CPersistContext&p):parent(p){}virtual bool AreDefaultValsApplied(){return ts->applied;}virtual void SetDefaulValsApplied(bool b){assert(b);ts->applied=b;++ts->writes;}virtual void Transfer(CPersistContext&c){TransferRecord r;r.defaults=c.UseDefaults;r.single=c.SingleTransfer;r.clean=c.PStringToClean!=0;r.definitions=c.PDefinitionManager!=0;r.symbols=c.PSymbolMap!=0;r.callback=c.ErrorCallback==context_error?2:c.ErrorCallback==parser_error?1:0;r.base=(long)c.PParser->BaseLineOffset;r.body=c.PParser->String;r.filename=(const char*)c.PParser->GetFilename();assert(c.Mode==CPersistContext::MODE_LOAD_TEXT&&!c.Finished&&!c.UseCRCCheck);ts->records.push_back(r);int n=(int)ts->records.size();if(ts->finishAfter==n)parent.Finished=true;}};

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

static int scenarios=0;
static void check(int count,bool initialApplied,bool initiallyFinished,bool clean,int finishAfter,int badAt,int missingAt,bool throwError,int transferThrow,int nameIndex){
 TestState state;ts=&state;state.applied=initialApplied;state.finishAfter=finishAfter;state.throwError=throwError;state.throwTransferAt=transferThrow;state.name=nameIndex?"LongComponent":"X";std::string input;
 for(int i=0;i<count;++i){Occurrence o;o.begin=(unsigned long)input.size();input+=state.name;o.afterName=(unsigned long)input.size();input+=".body\n";input+=(char)('0'+i);input+=';';o.end=(unsigned long)input.size();o.semicolon=i!=missingAt;o.separator=i!=badAt;o.body=std::string("body")+(char)('0'+i);state.occ.push_back(o);input+='\n';}
 std::string expected=input;int expectedTransfers=0,expectedWrites=0,expectedUnknown=0,expectedErrors=0,expectedSkip=0;bool expectedApplied=initialApplied,wasFound=false,stopped=false,expectedError=false,expectedTransferError=false;
 if(!initiallyFinished)for(int i=0;i<count;++i){++expectedSkip;wasFound=true;if(i==missingAt){++expectedErrors;if(throwError){expectedError=true;stopped=true;break;}}if(i==badAt){stopped=true;break;}++expectedTransfers;if(transferThrow==expectedTransfers-1){expectedTransferError=true;stopped=true;break;}++expectedWrites;++expectedUnknown;expectedApplied=true;if(clean&&i!=missingAt)for(unsigned long j=state.occ[i].begin;j<state.occ[i].end;++j)if(expected[j]!='\n')expected[j]=' ';if(finishAfter==expectedTransfers){stopped=true;break;}}
 if(!initiallyFinished&&!stopped)++expectedSkip;
 bool fallback=!wasFound&&!initialApplied;
 if(fallback){++expectedTransfers;if(transferThrow==expectedTransfers-1)expectedTransferError=true;else{++expectedWrites;expectedApplied=true;}}
 bool gotError=false,gotTransferError=false;
 {CCharString source(input.c_str());CCharString file("source.cfg");CStringParser original(source,parser_error,file,77);original.CursorPos.StringPos=5;CDefinitionManager defs;CSymbolMap symbols;CPersistContext parent(original,context_error,false,&defs,&symbols,clean?&source:0);parent.Finished=initiallyFinished;MockComponent comp(parent);parent.TransferSubComponentLoadText(state.name.c_str(),comp);
  assert(original.CursorPos.StringPos==5&&original.next==0);assert(source.text==expected);assert(gotError==expectedError&&gotTransferError==expectedTransferError);assert((int)state.records.size()==expectedTransfers&&state.writes==expectedWrites&&state.unknown==expectedUnknown&&state.errors==expectedErrors&&state.skipCalls==expectedSkip&&state.applied==expectedApplied);
  for(unsigned i=0;i<state.records.size();++i){const TransferRecord&r=state.records[i];if(fallback){assert(r.defaults&&!r.single&&!r.clean&&!r.definitions&&!r.symbols&&r.callback==1&&r.base==0&&r.body.empty()&&r.filename.empty());}else{assert(r.defaults==(i==0&&!initialApplied)&&r.single&&r.clean&&r.definitions&&r.symbols&&r.callback==2&&r.filename=="source.cfg"&&r.body==state.occ[i].body&&r.base==(long)state.occ[i].end);}}
 }
 assert(strings==0&&parsers==0&&contexts==0);++scenarios;
}
int main(){for(int n=0;n<=3;++n)for(int applied=0;applied<2;++applied)for(int finished=0;finished<2;++finished)for(int clean=0;clean<2;++clean)for(int finish=0;finish<=2;++finish)for(int bad=-1;bad<3;++bad)for(int missing=-1;missing<3;++missing)for(int error=0;error<1;++error)for(int tr=-1;tr<0;++tr)check(n,applied!=0,finished!=0,clean!=0,finish,bad,missing,error!=0,tr,n&1);assert(scenarios==1536);std::fprintf(stderr,"SCENARIOS %d\n",scenarios);std::puts("009bbcc0_TEST PASS");}
