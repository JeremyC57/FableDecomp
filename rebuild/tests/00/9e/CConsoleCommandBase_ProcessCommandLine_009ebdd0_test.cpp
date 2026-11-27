#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string>
#include <vector>
#define CHECK(x) do {if(!(x)){printf("case %d line %d\n",trial,__LINE__);exit(1);}}while(0)
static int trial;
struct CCharString:std::string {
 CCharString(){} CCharString(const char*s):std::string(s){} CCharString(const std::string&s):std::string(s){}
 char GetChar(int n)const{return (*this)[n];} bool operator!=(char c)const{return size()!=1||(*this)[0]!=c;}
 bool operator!=(const CCharString&s)const{return compare(s)!=0;}
 CCharString GetAsLowerCase()const{CCharString copy=*this;for(size_t i=0;i<size();++i)copy[i]=(char)tolower(copy[i]);return copy;}
};
static CCharString operator+(const CCharString&a,const CCharString&b){CCharString s=a;s+=b;return s;}
static CCharString operator+(const CCharString&a,const char*b){CCharString s=a;s+=b;return s;}
static CCharString operator+(const char*a,const CCharString&b){CCharString s=a;s+=b;return s;}
static CCharString GFIntToCharString(long i){char b[32];sprintf(b,"%ld",i);return b;}
namespace {CCharString OPEN_BRACKET("("),CLOSE_BRACKET(")"),SEPARATOR(",");}
struct CParsedItem{char value;bool IsSymbol()const{return value=='('||value==')'||value==','||value==';';}
 CCharString ConvertToString()const{return std::string(1,value);} CCharString GetSymbol()const{return ConvertToString();}};
struct CStringParser{CCharString s;size_t pos;CStringParser(const CCharString&c):s(c),pos(0){}
 bool ReadAsStringUntilString(const CCharString&c,CParsedItem&){size_t q=s.find(c);pos=q==s.npos?s.size():q;return q!=s.npos;}
 bool PeekNextItem(CParsedItem&i){if(pos==s.size())return false;i.value=s[pos];return true;}
 bool ReadNextItem(CParsedItem&i){if(!PeekNextItem(i))return false;++pos;return true;}};
struct CGenericVar{int value;bool ReadFromParser(CStringParser&p){CParsedItem i;if(!p.ReadNextItem(i)||!isdigit(i.value))return false;value=i.value-'0';return true;}};
struct CConsoleCommandParameters{int count;long GetSize()const{return count;}CGenericVar GetCorrectlyTypedParameter(int)const{CGenericVar v={-1};return v;}
 CCharString GetParameterTypeAsString(int)const{return "number";}};
struct CConsole{std::vector<CCharString> messages;void SendMessageToUser(const CCharString&s){messages.push_back(s);}};
struct CConsoleCommandBase{CConsoleCommandParameters ParamTypes;unsigned executions;std::vector<CGenericVar> args;
 CCharString GetName()const{return "DeMO";}void CallCommandFunction(const std::vector<CGenericVar>&v){++executions;args=v;}
 bool ProcessCommandLine(const CCharString&,CConsole&);};
bool CConsoleCommandBase::ProcessCommandLine(const CCharString& cmd, CConsole& user)
{
    CStringParser parser(cmd);
    CParsedItem parsed_item;
    std::vector<CGenericVar> function_parameters;
    bool next_item_exists = false;
    bool help_required = false;
    parser.ReadAsStringUntilString(OPEN_BRACKET, parsed_item);
    next_item_exists = parser.ReadNextItem(parsed_item);
    if (!next_item_exists || !parsed_item.IsSymbol() || parsed_item.GetSymbol() != OPEN_BRACKET.GetChar(0)) {
        user.SendMessageToUser("command requires leading bracket");
        help_required = true;
    }
    long param_count = 0;
    parser.PeekNextItem(parsed_item);
    while (param_count < ParamTypes.GetSize() && !help_required) {
        CGenericVar next_param = ParamTypes.GetCorrectlyTypedParameter(param_count);
        if (next_param.ReadFromParser(parser)) {
            function_parameters.push_back(next_param);

        } else {
            user.SendMessageToUser("parameter " + GFIntToCharString(param_count) + " is of incorrect type");
            help_required = true;
            break;
        }
            next_item_exists = parser.ReadNextItem(parsed_item);
            ++param_count;
            if (param_count < ParamTypes.GetSize() && (!next_item_exists || !parsed_item.IsSymbol() || parsed_item.GetSymbol() != SEPARATOR)) {
                user.SendMessageToUser(CCharString("expected ") + SEPARATOR + " after parameter " + GFIntToCharString(param_count));
                help_required = true;
                break;
            }
    }
    if (param_count == 0)
        next_item_exists = parser.ReadNextItem(parsed_item);
    if (param_count == ParamTypes.GetSize() && !help_required && next_item_exists) {
        if (!parsed_item.IsSymbol()) {
            user.SendMessageToUser("cannot interpret " + parsed_item.ConvertToString());
            help_required = true;
        } else if (parsed_item.GetSymbol() != CLOSE_BRACKET.GetChar(0)) {
            user.SendMessageToUser(CCharString("expected ") + CLOSE_BRACKET + " instead of " + parsed_item.GetSymbol());
            help_required = true;
        } else if (parser.ReadNextItem(parsed_item)) {
            user.SendMessageToUser("ignoring stuff after closing bracket");
        }
    }
    if (help_required) {
    CCharString help_message = GetName().GetAsLowerCase() + " takes ";
    if (ParamTypes.GetSize() == 0) {
        help_message += "no parameters";
    } else {
        help_message += GFIntToCharString(ParamTypes.GetSize()) + " parameter";
        if (ParamTypes.GetSize() == 1) help_message += ": "; else help_message += "s: ";
        if(false) {} else for (long c1 = 0; c1 < ParamTypes.GetSize(); ++c1)
            help_message += ParamTypes.GetParameterTypeAsString(c1) + " ";
    }
    user.SendMessageToUser(help_message);
    return false;
    }
    CallCommandFunction(function_parameters);
    return true;
}
static void expect(CConsole&c,size_t i,const CCharString&msg){CHECK(i<c.messages.size()&&c.messages[i]==msg);}
int main(){
 for(trial=0;trial<16384;++trial){
  CConsoleCommandBase command;CConsole user;int count=(trial>>3)%5,mode=trial%8;
  command.executions=0;command.ParamTypes.count=count;CCharString input=mode==1?"Demo":"Demo(";
  int bad=count?((trial>>7)%count):0,sep=count>1?((trial>>9)%(count-1)):0;
  for(int j=0;j<count;++j){input+=(mode==2&&j==bad)?'x':(char)('0'+(trial+j)%10);if(j+1<count)input+=(mode==3&&j==sep)?';':',';}
  input+=mode==4?'x':(mode==5?';':')');if(mode==6)input+='t';
  if(mode==7)input.erase(input.size()-1);
  CCharString error;bool failure=false;
  if(mode==1){error="command requires leading bracket";failure=true;}
  else if(mode==2&&count){error="parameter "+GFIntToCharString(bad)+" is of incorrect type";failure=true;}
  else if(mode==3&&count>1){error="expected , after parameter "+GFIntToCharString(sep+1);failure=true;}
  else if(mode==4){error="cannot interpret x";failure=true;}
  else if(mode==5){error="expected ) instead of ;";failure=true;}
  CHECK(command.ProcessCommandLine(input,user)==!failure);
  if(failure){CHECK(!command.executions&&user.messages.size()==2);expect(user,0,error);CCharString usage="demo takes ";
   if(!count)usage+="no parameters";else{usage+=GFIntToCharString(count);usage+=count==1?" parameter: ":" parameters: ";for(int j=0;j<count;++j)usage+="number ";}expect(user,1,usage);
  }else{CHECK(command.executions==1&&command.args.size()==(size_t)count);for(int j=0;j<count;++j)CHECK(command.args[j].value==(trial+j)%10);
   if(mode==6){CHECK(user.messages.size()==1);expect(user,0,"ignoring stuff after closing bracket");}else CHECK(user.messages.empty());}
 }
 puts("009ebdd0_TEST PASS");return 0;
}