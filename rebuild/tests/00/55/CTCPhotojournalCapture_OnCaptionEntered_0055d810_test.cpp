#include <stdio.h>
#include <string.h>
#include <string>
#include <vector>
struct CWideString { std::wstring data;CWideString(const wchar_t* t=L""):data(t){} CWideString(const std::wstring& s):data(s){} };
static CWideString operator+(const CWideString& a,const CWideString& b){return CWideString(a.data+b.data);}
static CWideString operator+(const CWideString& a,const wchar_t* b){return CWideString(a.data+b);}
struct CCharString {
 std::vector<char> data;
 CCharString(const char* t=""){data.assign(t,t+strlen(t)+1);}
 const char* GetAsCharArray()const{return &data[0];}
 CCharString& operator+=(const CCharString& b){data.pop_back();data.insert(data.end(),b.data.begin(),b.data.end());return *this;}
 CWideString ToWideString()const{std::wstring w;for(const char* p=GetAsCharArray();*p;++p)w+=(unsigned char)*p;return CWideString(w);}
    void Replace(const CCharString& What,const CCharString& With) {
        CCharString ret_str;
        char* data = (char*)GetAsCharArray();
        char* what = (char*)What.GetAsCharArray();
        char* prev = data;
        char* res = strstr(data,what);
        while (res) {
            *res=0;
            res+=strlen(what);
            ret_str+=CCharString(prev);
            ret_str+=With;
            prev=res;
            res=strstr(res,what);
        }
        ret_str+=CCharString(prev);
        *this=ret_str;
    }
};
struct _SYSTEMTIME{unsigned long id;};
static std::string date_text;static unsigned long current_id,clock_reads,date_reads;static std::wstring base_dir;
static void GetSystemTime(_SYSTEMTIME* time){++clock_reads;time->id=current_id;}
struct CDateAndTime{
 unsigned long id;CDateAndTime(const _SYSTEMTIME& time):id(time.id){}
 CCharString ToString()const{++date_reads;return id==current_id?CCharString(date_text.c_str()):CCharString("bad-time");}
};
struct CUserProfileManager {
 CWideString ProfileName;
 static CUserProfileManager& Get(){static CUserProfileManager instance;return instance;}
 const CWideString& GetProfileName()const{return ProfileName;}
};
namespace NResourceDirectoryNames {static CWideString GetPhotojournalDir(){return CWideString(base_dir);}}
static CWideString GetValidProfileName(const CWideString& n){return n.data.empty()?CWideString(L"Default"):n;}
struct CTCPhotojournalCapture {
 CWideString Description,FileName;unsigned long closes;
 void ClosePhotojournalCapture(){if(closes!=clock_reads-1||FileName.data.empty())closes=999999;else ++closes;}
 void OnCaptionEntered(const CWideString&);
};
void CTCPhotojournalCapture::OnCaptionEntered(const CWideString& caption_param)
{
    Description=caption_param;
    _SYSTEMTIME systime; GetSystemTime(&systime);
    CDateAndTime date(systime);
    CCharString date_amended=date.ToString();
    date_amended.Replace("\\","-");
    date_amended.Replace("/","-");
    date_amended.Replace(":",".");
    FileName=NResourceDirectoryNames::GetPhotojournalDir()+GetValidProfileName(CUserProfileManager::Get().GetProfileName())+L"\\"+date_amended.ToWideString()+L".tga";
    ClosePhotojournalCapture();
}
int main(){
 const wchar_t* profiles[]={L"",L"Ada",L"\x03a9Profile"};
 const wchar_t* dirs[]={L"C:\\Photos\\",L"",L"/test/data/"};
 const wchar_t* captions[]={L"",L"New title",L"\x03bb"};
 const char chars[]="a/:\\7";
 CTCPhotojournalCapture capture;capture.closes=0;clock_reads=date_reads=0;
 for(int bits=0;bits<625;++bits)for(int profile=0;profile<3;++profile)for(int dir=0;dir<3;++dir)for(int cap=0;cap<3;++cap){
  current_id=bits*27+profile*9+dir*3+cap;int q=bits;date_text.erase();std::wstring safe;
  int length=bits?4:0;
  for(int i=0;i<length;++i){char c=chars[q%5];q/=5;date_text+=c;safe+=c=='/'||c=='\\'?L'-':c==':'?L'.':(wchar_t)c;}
  CUserProfileManager::Get().ProfileName=CWideString(profiles[profile]);base_dir=dirs[dir];
  capture.OnCaptionEntered(CWideString(captions[cap]));
  std::wstring expected=base_dir+(profile?profiles[profile]:L"Default")+L"\\"+safe+L".tga";
  if(capture.Description.data!=captions[cap]||capture.FileName.data!=expected||capture.closes!=current_id+1||date_reads!=clock_reads||clock_reads!=current_id+1)return 1;
 }
 puts("0055d810_TEST PASS");return 0;
}