#include <stdio.h>
#include <string.h>
#include <ctype.h>
class CCharString{public:const char* Text;CCharString(const char* s):Text(s){}long GetLength()const{return Text?(long)strlen(Text):0;}const char* GetAsCharArray()const{return Text?Text:"";}char GetChar(long index)const{return GetAsCharArray()[index];}};
class CStringParser{public:class CCursorPos{public:bool InQuotes;unsigned long StringPos,Line;};const char* String;CCursorPos CursorPos;unsigned long GetStringPos()const{return CursorPos.StringPos;}long SkipWhitespace();bool SkipPastNextItemIfIs(const CCharString&);
    inline bool CalcInQuotes(bool inQuotes,const char* from,const char* to) const {
        for(const char* p=from;p!=to;++p)
            if(*p=='"'||*p=='\'')
                inQuotes=!inQuotes;
        return inQuotes;
    }
    inline void MoveStringPos(unsigned long n) {
        if(n>0) {
            const char* from=String+CursorPos.StringPos;
            CursorPos.InQuotes=CalcInQuotes(CursorPos.InQuotes,from,String+CursorPos.StringPos+n);
            CursorPos.StringPos+=n;
        }
    }
};
long CStringParser::SkipWhitespace(){
 if(String[CursorPos.StringPos]){
  const char* p=String+CursorPos.StringPos;
  for(;;){if(!isspace((unsigned char)*p)&&*p!='\n')break;if(!*p)break;++p;}
  unsigned long oldPosition=GetStringPos();
  MoveStringPos((p-String)-oldPosition);
  return (p-String)-oldPosition;
 }
 return 0;
}
bool CStringParser::SkipPastNextItemIfIs(const CCharString& str){
 CCursorPos old_pos=CursorPos;
 SkipWhitespace();
 long c1=0;
 while(c1<str.GetLength()&&str.GetChar(c1)==String[GetStringPos()+c1])++c1;
 if(c1==str.GetLength()){MoveStringPos(c1);return true;}
 CursorPos=old_pos;return false;
}
int main(){
 const char* prefixes[]={""," ","\t","\n"," \t\n"};const char* fragments[]={"foo","foo bar","\"foo\"","'x'","bar","","f","foo\"'"};const char* tokens[]={"",0,"foo","fo","f","foo\"'","\"foo\"","'x'","bar","foo bar","foobar","x"};int cases=0;
 for(int pre=0;pre<5;++pre)for(int f=0;f<8;++f)for(int t=0;t<12;++t)for(int start=0;start<3;++start)for(int quote=0;quote<2;++quote)for(int line=0;line<3;++line){
  char text[80];strcpy(text,prefixes[pre]);strcat(text,fragments[f]);CStringParser row;row.String=text;row.CursorPos.StringPos=start==0?0:start==1?(unsigned long)strlen(prefixes[pre]):(unsigned long)strlen(text);row.CursorPos.InQuotes=quote!=0;row.CursorPos.Line=line*99;
  CStringParser::CCursorPos old=row.CursorPos;CCharString token(tokens[t]);unsigned long p=old.StringPos;while(text[p]&&isspace((unsigned char)text[p]))++p;const char* wanted=tokens[t]?tokens[t]:"";unsigned long n=(unsigned long)strlen(wanted);bool success=strncmp(text+p,wanted,n)==0;bool in_quotes=old.InQuotes;if(success){for(unsigned long i=0;i<n;++i)if(wanted[i]=='\"'||wanted[i]=='\'')in_quotes=!in_quotes;p+=n;}else p=old.StringPos;
  bool got=row.SkipPastNextItemIfIs(token);if(got!=success||row.CursorPos.StringPos!=p||row.CursorPos.InQuotes!=(success?in_quotes:old.InQuotes)||row.CursorPos.Line!=old.Line||row.String!=text||token.Text!=tokens[t])return 1;++cases;
 }
 if(cases!=8640)return 1;puts("009b9e00_TEST PASS");return 0;
}