#include <stdio.h>
#include <stdlib.h>
#include <string.h>
struct CCharString {
 char data[96];
 CCharString(const char* value=""){strcpy(data,value);}
 long GetLength() const{return (long)strlen(data);}
 bool StartsWith(const CCharString& p)const{return strncmp(data,p.data,p.GetLength())==0;}
 CCharString GetSubString(unsigned long start,unsigned long end) const{CCharString r;unsigned long n=(unsigned long)GetLength(); if(start<n){if(end>n-start)end=n-start;memcpy(r.data,data+start,end);r.data[end]=0;}return r;}
};
struct CWideString {long n;CWideString(long len=0):n(len){}long GetLength() const{return n;}};
template<class T> struct CArray { T items[12];unsigned int count;CArray():count(0){}unsigned int size()const{return count;}const T& operator[](unsigned int i)const{return items[i];}};
namespace NGameText {struct CTextTag{long Pos;CCharString Name;}; struct CTextEntry{CCharString Narrator,SpeechBank;CWideString Text;CArray<CTextTag> Tags;const CCharString& GetNarrator()const{return Narrator;}const CCharString& GetSpeechBank()const{return SpeechBank;}CWideString GetText()const{return Text;}const CArray<CTextTag>& GetTags()const{return Tags;}};}
struct CASoundBank {int id;};static CASoundBank known={8},fallback={1};
struct CASoundManager{int byname,byid;CASoundBank* GetPSoundBank(const CCharString& n){++byname;return strcmp(n.data,"KNOWN")?0:&known;}CASoundBank* GetPSoundBank(long n){++byid;return n==1?&fallback:0;}}sm;static CASoundManager* PSoundManager=&sm;
struct CGlobalDef{};template<class T>struct CDefPointer{T* p;CDefPointer():p(0){}};
struct CDefinitionManager{template<class T>bool GetDef(const CCharString& n,CDefPointer<const T>* v)const{static T t;if(strcmp(n.data,"DEFAULT_GLOBAL_DEFS"))abort();v->p=&t;return true;}};
struct CGameDefinitionManager: CDefinitionManager{static CGameDefinitionManager& Get(){static CGameDefinitionManager g;return g;}};
enum EConversationAttitude{ATT_OK=2};struct CCRCSymbolMap{bool FindSymbol(const CCharString& n,long& x){if(strcmp(n.data,"CHEER"))return false;x=2;return true;}}g_MiscDefTypesSymbolMap;
enum EParsedItemType{PARSED_ITEM_NULL,PARSED_ITEM_SYMBOL,PARSED_ITEM_STRING,PARSED_ITEM_INTEGER};
struct CStringParser{CCharString s;CStringParser(const CCharString& x,void*,const CCharString&,long):s(x){}bool NextItemExists()const{return s.data[0]!=0;}EParsedItemType PeekNextItemType()const{return s.data[0]>='0'&&s.data[0]<='9'?PARSED_ITEM_INTEGER:PARSED_ITEM_SYMBOL;}long ReadAsInteger(){return atoi(s.data);}};
template<class V>struct FixtureTags {struct Pair{float at;V value;};Pair p[16];int count,updates;FixtureTags():count(0),updates(0){}void Insert(float at,const V& v){p[count].at=at;p[count++].value=v;}void Update(){++updates;for(int i=1;i<count;++i)for(int k=i;k>0&&p[k].at<p[k-1].at;--k){Pair temp=p[k];p[k]=p[k-1];p[k-1]=temp;}}};
struct Camera{int kind;CCharString text;long index;};
struct CSubtitledMessage{
 CASoundBank* PSpeechBank;long SpeechIndex;bool Speech3D;CWideString Text;CCharString Narrator;FixtureTags<EConversationAttitude> AttitudeTags;FixtureTags<CCharString> AnimationTags;FixtureTags<Camera> CameraTags;
 CSubtitledMessage(const NGameText::CTextEntry&);
 bool AddMasterShotCameraTagFromText(float t,const CCharString& x){Camera c;c.kind=1;c.text=x;c.index=0;CameraTags.Insert(t,c);return true;}
 bool AddCameraTagFromText(float t,const CCharString& x){Camera c;c.kind=2;c.text=x;c.index=0;CameraTags.Insert(t,c);return true;}
 bool AddCameraTagFromIndex(long n,float t,CDefPointer<const CGlobalDef> d){if(!d.p)abort();Camera c;c.kind=3;c.index=n;CameraTags.Insert(t,c);return true;}
};
CSubtitledMessage::CSubtitledMessage(const NGameText::CTextEntry& entry)
    : PSpeechBank(0), SpeechIndex(0), Speech3D(true), Text(entry.GetText()), Narrator(entry.GetNarrator())
{
    PSpeechBank = PSoundManager->GetPSoundBank(entry.GetSpeechBank());
    if (PSpeechBank == 0)
        PSpeechBank = PSoundManager->GetPSoundBank(1);
    long textLength = Text.GetLength();
    const CArray<NGameText::CTextTag>& tags = entry.GetTags();
    CDefPointer<const CGlobalDef> globalDefs;
    {
        CCharString defsName("DEFAULT_GLOBAL_DEFS");
        CGameDefinitionManager::Get().CDefinitionManager::GetDef<CGlobalDef>(defsName, &globalDefs);
    }
    for (long i = 0; i < tags.size(); ++i) {
        float time = (float)tags[i].Pos / (float)textLength;
        const CCharString& tag = tags[i].Name;
        if (tag.StartsWith("ANIM:")) {
            AnimationTags.Insert(time, tag.GetSubString(5, tag.GetLength()));
        } else if (tag.StartsWith("CAM_SHOT:")) {
            AddMasterShotCameraTagFromText(time, tag.GetSubString(9, tag.GetLength()));
        } else if (tag.StartsWith("CAM:")) {
            AddCameraTagFromText(time, tag.GetSubString(4, tag.GetLength()));
        } else if (tag.StartsWith("CAM")) {
            CCharString rest = tag.GetSubString(3, tag.GetLength());
            CStringParser parser(rest, 0, CCharString(""), 0);
            if (parser.NextItemExists() && parser.PeekNextItemType() == PARSED_ITEM_INTEGER) {
                long camera_number=parser.ReadAsInteger();
                bool camera_tag_check=AddCameraTagFromIndex(camera_number, time, globalDefs);
            }
        } else {
            long attitude = 0;
            if (g_MiscDefTypesSymbolMap.FindSymbol(tags[i].Name, attitude))
                AttitudeTags.Insert(time, (EConversationAttitude)attitude);
        }
    }
    AttitudeTags.Update();
    AnimationTags.Update();
    CameraTags.Update();
}

int main(){
 const char* n[]={"ANIM:wave","CAM_SHOT:wide","CAM:close","CAM17","CHEER","CAMname","CAM","","not-recognised"};
 for(int bank=0;bank<2;++bank)for(int len=1;len<=3;++len)for(int rot=0;rot<9;++rot)for(int num=0;num<=9;++num){
  NGameText::CTextEntry e;e.Text.n=len*16;e.Narrator="NARRATOR";e.SpeechBank=bank?"KNOWN":"MISSING";e.Tags.count=num;
  for(int i=0;i<num;++i){e.Tags.items[i].Name=n[(rot+i)%9];e.Tags.items[i].Pos=(num-i)*2;}
  sm.byname=sm.byid=0;CSubtitledMessage m(e);
  if(sm.byname!=1||sm.byid!=1-bank||m.PSpeechBank!=(bank?&known:&fallback)||m.SpeechIndex||!m.Speech3D||m.Text.n!=e.Text.n||strcmp(m.Narrator.data,"NARRATOR"))return 1;
  int ai=0,ci=0,ti=0;
  for(int i=num-1;i>=0;--i){int k=(rot+i)%9;float t=(float)((num-i)*2)/(float)e.Text.n;
   if(k==0){if(m.AnimationTags.p[ai].at!=t||strcmp(m.AnimationTags.p[ai++].value.data,"wave"))return 2;}
   else if(k>=1&&k<=3){FixtureTags<Camera>::Pair p=m.CameraTags.p[ci++];if(p.at!=t||p.value.kind!=k||(k==1&&strcmp(p.value.text.data,"wide"))||(k==2&&strcmp(p.value.text.data,"close"))||(k==3&&p.value.index!=17))return 3;}
   else if(k==4){if(m.AttitudeTags.p[ti].at!=t||m.AttitudeTags.p[ti++].value!=ATT_OK)return 4;}
  }
  if(ai!=m.AnimationTags.count||ci!=m.CameraTags.count||ti!=m.AttitudeTags.count||m.AttitudeTags.updates!=1||m.AnimationTags.updates!=1||m.CameraTags.updates!=1)return 5;
 }
 puts("004ce550_TEST PASS");return 0;
}