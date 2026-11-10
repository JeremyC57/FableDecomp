#include <stdio.h>
#include <wchar.h>
static int live, calls, converted, failures;
static unsigned long want_flags;
static float want_time;
static bool want_update,want_change;
static const wchar_t* want_message;
class CFontBank {public:int id;};
static const CFontBank* want_font;
class CWideString {public:const wchar_t* text;CWideString(const wchar_t* t):text(t){++live;}CWideString(const CWideString& s):text(s.text){++live;}~CWideString(){--live;}};
class CCharString {public:const wchar_t* converted_text;CWideString ToWideString()const{++converted;return CWideString(converted_text);}};
class CSubtitleRenderer {public:
void SetText(const CFontBank&,const CCharString&,unsigned long,float,bool,bool);
void SetText(const CFontBank& font,const CWideString& message,const CWideString& a,const CWideString& b,const CWideString& c,const CWideString& d,unsigned long flags,float time,bool update,bool change){
 ++calls;if(&font!=want_font||wcscmp(message.text,want_message)||a.text[0]||b.text[0]||c.text[0]||d.text[0]||flags!=want_flags||time!=want_time||update!=want_update||change!=want_change||live!=5)++failures;
}
};
void CSubtitleRenderer::SetText(const CFontBank& font, const CCharString& message,
    unsigned long flags, float display_time, bool update_while_paused, bool change_player_mode)
{
    SetText(font, message.ToWideString(), L"", L"", L"", L"", flags,
            display_time, update_while_paused, change_player_mode);
}

int main(){
 const wchar_t* messages[]={L"",L"caption",L"line1\nline2",L"\x20ac"};
 unsigned long flags[]={0,1,0x12345678UL,0xffffffffUL};
 float times[]={-1.0f,0,0.125f,1000.0f};
 CFontBank font;CSubtitleRenderer renderer;CCharString message;want_font=&font;
 for(int m=0;m<4;++m)for(int f=0;f<4;++f)for(int t=0;t<4;++t)for(int u=0;u<2;++u)for(int c=0;c<2;++c){
  want_message=messages[m];message.converted_text=messages[m];want_flags=flags[f];want_time=times[t];want_update=u!=0;want_change=c!=0;calls=converted=0;
  renderer.SetText(font,message,want_flags,want_time,want_update,want_change);
  if(calls!=1||converted!=1||live!=0||failures||message.converted_text!=messages[m])return 1;
 }
 puts("00492ed1_TEST PASS");return 0;
}