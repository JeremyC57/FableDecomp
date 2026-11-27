#include <stdio.h>
#include <stdlib.h>
#include <wchar.h>
#include <string.h>
#include <vector>
static void check(bool b){if(!b)abort();}
static unsigned seed;
static bool exists,loaded,closed; static int opts,callbacks,checks,loads,fonts,systems,lookups,info,entries,lengths,rows,cursor,step;
struct CWideString {
 struct Text {const wchar_t* p;Text(const wchar_t* x):p(x){} bool operator==(const wchar_t* x) const{return wcscmp(p,x)==0;} unsigned size()const{return wcslen(p);} } s;
 CWideString(const wchar_t* x=L""):s(x){}
 long GetLength()const{++lengths;return (long)s.size();}
};
struct CCharString {
 struct Text {const char* p; Text(const char* x):p(x){} bool operator==(const char*x)const{return strcmp(p,x)==0;} bool empty()const{return *p==0;} } s;unsigned id;
 CCharString(const char*x=""):s(x),id(0){}
 unsigned long GetCRC()const {return id+100;}
};
struct CAFile{
 static void SplitPath(const CWideString& p,CWideString* drive,CWideString* dir,CWideString* name,CWideString* ext){
  check(p.s==L"x:/film/movie.bik"&&!ext);*drive=L"x:";*dir=L"/film/";*name=L"movie";
 }
 static CWideString MakePath(const CWideString& d,const CWideString& dir,const CWideString& n,const CWideString& e){
  check(d.s==L"x:"&&dir.s==L"/film/"&&n.s==L"movie"&&e.s==L"txt");return L"x:/film/movie.txt";}
 static bool PathExists(const CWideString& x){check(x.s==L"x:/film/movie.txt");++checks;return exists;}
};
enum FileAccessType{FILE_ACCESS_READ=1};
struct CDiskFileWin32:CAFile{
 CDiskFileWin32(const CWideString& x,FileAccessType a,unsigned f){check(x.s==L"x:/film/movie.txt"&&a==FILE_ACCESS_READ&&f==2);}
 void ReadVar(CCharString& s,void*p){check(!p&&!loaded);s="movie data";loaded=true;++loads;}
 void Close(){check(loaded&&!closed);closed=true;}
};
struct CParsedItem{};
static long minute(int row,bool end){return int((seed+row*7)%10)-3+(end?1:0);}
static float second(int row,bool end){return float((seed/4+row*3)%180)/4.0f+(end?.25f:0.0f);}
struct CStringParser{
 CStringParser(const CCharString& s,void*a,const CCharString& name,long base){check(s.s=="movie data"&&closed&&!a&&name.s.empty()&&!base);}
 bool NextItemExists(){check(!step);return cursor<rows;}
 CCharString ReadAsString(){check(step++==0);CCharString x;x.id=cursor;return x;}
 long ReadAsInteger(){check(step==1||step==4);return minute(cursor,step++==4);}
 void ReadNextItem(CParsedItem&){check(step==2||step==5);++step;}
 float ReadAsFloat(){check(step==3||step==6);bool last=step==6;float v=second(cursor,last);if(last){step=0;++cursor;}else ++step;return v;}
};
static int kind(int i){return (seed/16+i)%3;}
namespace NGameText{
 struct CDataBankTextEntry{};
 struct CTextEntry{unsigned index;CWideString GetText() const {return L"subtitle";}};
 struct CDataBank{
  CDataBankTextEntry entry; CTextEntry text[5];
  unsigned FindIndexBySymbolCRC(unsigned v){++lookups;check(v==99u+cursor);return kind(v-100)==0?0:v-99;}
  CDataBankTextEntry* GetTextInfo(unsigned i){++info;check(i&&i<=5);return kind(i-1)==2?&entry:0;}
  CTextEntry* GetPTextEntry(unsigned i,bool load){++entries;check(load&&i&&i<=5&&kind(i-1)==2);text[i-1].index=i;return &text[i-1];}
 };
}
struct CSubTitle{NGameText::CTextEntry* TextEntry;long FrameStart,FrameEnd;float StartTime,EndTime;};
struct CFontManager{int GetFont(const CCharString& s){check(s.s=="ENG_ARIAL_18");++fonts;return 45;}};
struct CSystemManager{CFontManager font;CFontManager* GetFontManager(){return &font;}int GetRetailSoundManager007C(){return 72;}} systemManager;
static CSystemManager* GFGetSystemManager(){++systems;return &systemManager;}
struct CMovieSubtitles{
 static unsigned __fastcall GetCharWidthW(unsigned short x){return x;}
 long FrameCounter,Fps;bool IsSubtitleTime;int PFont,PSoundManager;NGameText::CDataBank* pTextBank;std::vector<CSubTitle> SubTitles;
 long GetFrameNumber(long mins,float secs){float t=mins*60+secs;return (long)(Fps*t);}
 void ParseSubtitles(const CWideString& path_name);
};
static void WordWrap_SetOption(unsigned a,unsigned b){check(a==0&&b==1);++opts;}
static void WordWrap_SetCallback(unsigned (__fastcall*f)(unsigned short),void*p){check(f==CMovieSubtitles::GetCharWidthW&&!p);++callbacks;}
void CMovieSubtitles::ParseSubtitles(const CWideString& path_name)
{
    unsigned int flags = 1;
    WordWrap_SetOption(0, flags);
    WordWrap_SetCallback(GetCharWidthW, 0);
    FrameCounter = 0;
    IsSubtitleTime = false;
    CWideString drive;
    CWideString directory;
    CWideString file_name;
    CAFile::SplitPath(path_name, &drive, &directory, &file_name, 0);
    file_name = CAFile::MakePath(drive, directory, file_name, L"txt");
    SubTitles.clear();
    if (!pTextBank) return;
    if (!CAFile::PathExists(file_name)) return;
    CDiskFileWin32 fl(file_name, FILE_ACCESS_READ, 2);
    CCharString text;
    fl.ReadVar(text, 0);
    fl.Close();
    CStringParser line_parser(text, 0, CCharString(""), 0);
    CParsedItem itm;
    while (line_parser.NextItemExists()) {
        CCharString SymbolName = line_parser.ReadAsString();
        long smins = line_parser.ReadAsInteger();
        line_parser.ReadNextItem(itm);
        float ssecs = line_parser.ReadAsFloat();
        long emins = line_parser.ReadAsInteger();
        line_parser.ReadNextItem(itm);
        float esecs = line_parser.ReadAsFloat();
        unsigned long text_index = pTextBank->FindIndexBySymbolCRC(SymbolName.GetCRC());
        if (text_index) {
          const NGameText::CDataBankTextEntry* Entry = pTextBank->GetTextInfo(text_index);
          if (Entry) {
            CSubTitle sub;
            sub.TextEntry = pTextBank->GetPTextEntry(text_index, true);
            sub.FrameStart=GetFrameNumber(smins,ssecs);
            sub.FrameEnd=GetFrameNumber(emins,esecs);
            sub.StartTime=smins*60+ssecs;
            sub.EndTime=emins*60+esecs;
            long total = sub.TextEntry->GetText().GetLength();
            SubTitles.push_back(sub);
          }
        }
    }
    PFont = GFGetSystemManager()->GetFontManager()->GetFont("ENG_ARIAL_18");
    PSoundManager = GFGetSystemManager()->GetRetailSoundManager007C();

}
int main(){
 NGameText::CDataBank bank;
 for(seed=0;seed<65536;++seed){
  CMovieSubtitles m;bool present=(seed&1)!=0;exists=(seed&2)!=0;bool read=present&&exists;
  rows=(seed/4)%6;cursor=step=opts=callbacks=checks=loads=fonts=systems=lookups=info=entries=lengths=0;loaded=closed=false;
  m.Fps=(long)((seed/32)%61);m.pTextBank=present?&bank:0;m.FrameCounter=8;m.IsSubtitleTime=true;m.PFont=-3;m.PSoundManager=-4;
  CSubTitle old={0};m.SubTitles.push_back(old);m.ParseSubtitles(L"x:/film/movie.bik");
  check(opts==1&&callbacks==1&&checks==int(present)&&loads==int(read)&&closed==read&&m.FrameCounter==0&&!m.IsSubtitleTime);
  check(fonts==int(read)&&systems==2*int(read)&&m.PFont==(read?45:-3)&&m.PSoundManager==(read?72:-4));
  unsigned count=0,found=0;for(int i=0;read&&i<rows;++i){if(kind(i)>0)++found;if(kind(i)!=2)continue;check(count<m.SubTitles.size());const CSubTitle& sub=m.SubTitles[count++];check(sub.TextEntry==&bank.text[i]);
   float start=minute(i,false)*60+second(i,false),end=minute(i,true)*60+second(i,true);
   check(sub.StartTime==start&&sub.EndTime==end&&sub.FrameStart==(long)(start*m.Fps)&&sub.FrameEnd==(long)(end*m.Fps));}
  check(count==m.SubTitles.size()&&entries==int(count)&&info==int(found)&&lengths==int(count)&&lookups==(read?rows:0)&&cursor==(read?rows:0));
 }
 puts("009bd460_TEST PASS");return 0;
}