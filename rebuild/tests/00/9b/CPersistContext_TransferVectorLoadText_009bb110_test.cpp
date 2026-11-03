#include <string>
#include <vector>
#include <cassert>
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <cctype>
static int live_strings=0,live_parsers=0;
class CCharString {
public:
 std::string value;
 CCharString(){++live_strings;}
 CCharString(const char*p):value(p){++live_strings;}
 CCharString(const std::string&p):value(p){++live_strings;}
 CCharString(const CCharString&p):value(p.value){++live_strings;}
 ~CCharString(){--live_strings;}
 CCharString&operator=(const CCharString&p){value=p.value;return *this;}
 bool operator==(const char*p)const{return value==p;}
};
CCharString operator+(const char*a,const CCharString&b){return CCharString(a+b.value);}
class CStringParser {
 std::string text;unsigned long pos;
 static bool word(char c){return std::isalnum((unsigned char)c)||c=='_';}
 void space(){while(pos<text.size()&&std::isspace((unsigned char)text[pos]))++pos;}
public:
 CStringParser(const std::string&t):text(t),pos(0){++live_parsers;}
 CStringParser(const CStringParser&p):text(p.text),pos(p.pos){++live_parsers;}
 ~CStringParser(){--live_parsers;}
 unsigned long GetStringPos()const{return pos;}
 bool SkipPastWholeString(const CCharString& name){std::size_t at=pos;while((at=text.find(name.value,at))!=std::string::npos){std::size_t end=at+name.value.size();if((at==0||!word(text[at-1]))&&(end==text.size()||!word(text[end]))){pos=(unsigned long)end;return true;}++at;}pos=(unsigned long)text.size();return false;}
 CCharString ReadAsSymbol(){space();return pos<text.size()?CCharString(text.substr(pos++,1)):CCharString("");}
 CCharString ReadAsString(){space();unsigned long begin=pos;while(pos<text.size()&&word(text[pos]))++pos;return CCharString(text.substr(begin,pos-begin));}
 long ReadAsInteger(){space();char*end=0;const char*begin=text.c_str()+pos;long n=std::strtol(begin,&end,10);assert(end!=begin);pos+=(unsigned long)(end-begin);return n;}
};
class CPersistContext_TransferableVectorGenericType;
class CPersistContext {
public:
 CStringParser*PParser;bool SingleTransfer,Finished;std::string clean;std::vector<std::string>errors;std::vector< std::pair<long,long> >ranges;
 CPersistContext(CStringParser&p,const std::string&t):PParser(&p),SingleTransfer(false),Finished(false),clean(t){}
 void PersistError(const CStringParser&,const CCharString&e){errors.push_back(e.value);}
 void CleanOutLine(long a,long b){assert(a>=0&&a<=b&&(std::size_t)b<=clean.size());ranges.push_back(std::make_pair(a,b));for(long i=a;i<b;++i)if(clean[i]!='\n')clean[i]=' ';}
 void TransferVectorLoadText(const char*,CPersistContext_TransferableVectorGenericType&);
};
class CPersistContext_TransferableVectorGenericType {public:virtual long Size()=0;virtual void Resize(long)=0;virtual void TransferOut(CPersistContext&,CCharString&,long)=0;virtual void TransferIn(CPersistContext&,CStringParser&,long)=0;};
class CPersistTraitsSLONG {public:static void TransferIn(CPersistContext&,CStringParser&p,long&v){v=p.ReadAsInteger();}};
template<class T>class CPersistTraits {public:T DefaultVal;void TransferIn(CPersistContext&c,CStringParser&p,T&v){long result;CPersistTraitsSLONG::TransferIn(c,p,result);v=(T)result;}};
class TestVector:public CPersistContext_TransferableVectorGenericType {
public:
 std::vector<long>values,resize_args,indices;std::vector<bool>finished_at_transfer;
 long Size(){return(long)values.size();}
 void Resize(long n){resize_args.push_back(n);assert(n>=0&&n<20);values.resize(n);}
 void TransferOut(CPersistContext&,CCharString&,long){assert(false);}
 void TransferIn(CPersistContext&c,CStringParser&p,long i){indices.push_back(i);finished_at_transfer.push_back(c.Finished);long v=p.ReadAsInteger();if(i>=0){assert(i<(long)values.size());values[i]=v;}}
};

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

struct Example {const char*cmd;const char*result;const char*error;int cleaned;int transfers;int finish_kind;};
static std::string values(const TestVector&v){std::string r;for(std::size_t i=0;i<v.values.size();++i){char b[24];std::sprintf(b,"%ld,",v.values[i]);r+=b;}return r;}
int main(){
 const Example examples[]={
 {"V.Add(7);","10,20,7,","",1,1,1},
 {"V.resize(3);","10,20,0,","",1,0,0},
 {"V.clear();","","",1,0,0},
 {"V[0]7;","7,20,","",1,1,2},
 {"V[1]7;","10,7,","",1,1,2},
 {"V[2]7;","10,20,7,","",1,1,2},
 {"V[3]7;","10,20,","Out of bounds vector access",0,0,0},
 {"V[-1]7;","10,20,","",1,1,2},
 {"V.Add?","10,20,","Unable to find opening bracket.",0,0,0},
 {"V.Add(7?","10,20,7,","Unable to find terminating bracket.",0,1,1},
 {"V.Add(7)?","10,20,7,","Unable to find terminating semicolon.",0,1,1},
 {"V.resize?","10,20,","Unable to find opening bracket.",0,0,0},
 {"V.resize(3?","10,20,","",0,0,0},
 {"V.resize(3)?","10,20,0,","Unable to find terminating semicolon.",0,0,0},
 {"V.clear?","10,20,","Unable to find opening bracket.",0,0,0},
 {"V.clear(?","10,20,","Unable to find terminating semicolon.",0,0,0},
 {"V.clear()?","10,20,","Unable to find terminating semicolon.",0,0,0},
 {"V[1?","10,20,","Unable to find terminating bracket.",0,0,0},
 {"V[1]7?","10,7,","Unable to find terminating semicolon.",0,1,2},
 {"V[2?","10,20,0,","Unable to find terminating bracket.",0,0,0},
 {"V.Other","10,20,","Unknown command Other",0,0,0},
 {"V.add","10,20,","Unknown command add",0,0,0},
 {"V!","10,20,","",0,0,0},
 {"XV.Add(7); Vextra.clear();","10,20,","",0,0,0},
 {"V . Add ( 7 ) ;","10,20,7,","",1,1,1},
 {"V.Add(\n7);","10,20,7,","",1,1,1}
 };
 int scenarios=0;
 for(unsigned int i=0;i<sizeof(examples)/sizeof(examples[0]);++i)for(int single=0;single<2;++single)for(int initially_finished=0;initially_finished<2;++initially_finished){
  {const Example&e=examples[i];std::string text="before ";text+=e.cmd;text+=" after";CStringParser parser(text);CPersistContext c(parser,text);TestVector v;v.values.push_back(10);v.values.push_back(20);c.SingleTransfer=single!=0;c.Finished=initially_finished!=0;
   c.TransferVectorLoadText("V",v);assert(parser.GetStringPos()==0);assert(live_strings==0&&live_parsers==1);
   if(initially_finished){assert(values(v)=="10,20,"&&c.errors.empty()&&c.ranges.empty()&&v.indices.empty()&&c.Finished);assert(c.clean==text);}
   else{assert(values(v)==e.result);assert((e.error[0]==0&&c.errors.empty())||(c.errors.size()==1&&c.errors[0]==e.error));assert((int)c.ranges.size()==e.cleaned);assert((int)v.indices.size()==e.transfers);assert(c.Finished==(single&&e.finish_kind));for(std::size_t n=0;n<v.finished_at_transfer.size();++n)assert(v.finished_at_transfer[n]==(single&&e.finish_kind==2));
    if(e.cleaned){assert(c.ranges[0].first==7);assert(c.ranges[0].second==7+(long)std::strlen(e.cmd));std::string expected=text;for(std::size_t j=7;j<7+std::strlen(e.cmd);++j)if(expected[j]!='\n')expected[j]=' ';assert(c.clean==expected);}else assert(c.clean==text);
   }++scenarios;
  }assert(live_strings==0&&live_parsers==0);
 }
 for(int single=0;single<2;++single){
  {std::string text="V.resize(1);\nV.Add(5);\nV[0]8;\nV.clear();";CStringParser p(text);CPersistContext c(p,text);c.SingleTransfer=single!=0;TestVector v;v.values.push_back(10);v.values.push_back(20);c.TransferVectorLoadText("V",v);assert(c.errors.empty());assert(c.ranges.size()==(single?2:4));assert(values(v)==(single?"10,5,":""));assert(c.Finished==(single!=0));++scenarios;}
  assert(live_strings==0&&live_parsers==0);
 }
 {std::string text="V.nope V.resize(0); V[0]9;";CStringParser p(text);CPersistContext c(p,text);TestVector v;c.TransferVectorLoadText("V",v);assert(c.errors.size()==1&&c.errors[0]=="Unknown command nope");assert(values(v)=="9,"&&c.ranges.size()==2);++scenarios;}
 assert(live_strings==0&&live_parsers==0&&scenarios==107);std::puts("009bb110_TEST PASS");return 0;
}