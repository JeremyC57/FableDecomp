#include <string>
#include <vector>
#include <stdio.h>
#include <stdlib.h>
static void check(bool p,const char*s){if(!p){printf("FAIL %s\n",s);exit(1);}}
struct CCharString{static long MemTrackingID;std::string s;CCharString(const char*p=""):s(p){}};long CCharString::MemTrackingID=202;
struct CWideString{static long MemTrackingID;std::wstring s;};long CWideString::MemTrackingID=101;
struct CScopedMemTrackingID{CScopedMemTrackingID(long id){check(id==11,"memory tracking scope id");}};
struct CDataInputStream{std::vector<unsigned char>data;unsigned int pos,chunk,slow;CDataInputStream(unsigned int c):pos(0),chunk(c),slow(0){}unsigned int read(unsigned int n){check(CWideString::MemTrackingID==11&&CCharString::MemTrackingID==11,"read allocation category");check(pos+n<=data.size(),"stream bounds");if(pos%chunk+n>chunk)++slow;unsigned int v=0;for(unsigned int i=0;i<n;++i)v|=(unsigned int)data[pos++]<<(i*8);return v;}long ReadSLONG(){return(long)(int)read(4);}CCharString ReadPresizedString(){unsigned int n=read(4);CCharString r;for(unsigned int i=0;i<n;++i)r.s+=(char)read(1);return r;}CCharString ReadNullTerminatedString(){CCharString r;unsigned int c;while((c=read(1))!=0)r.s+=(char)c;return r;}CWideString ReadNullTerminatedWString(){CWideString r;unsigned int c;while((c=read(2))!=0)r.s+=(wchar_t)c;return r;}};
namespace NGameText{struct CTextTag{long Pos;CCharString Name;};struct CTextEntry{CCharString SpeechBank,Narrator,SoundName;CWideString Text;std::vector<CTextTag>Tags;unsigned long resource;int updates;CTextEntry():resource(0),updates(0){}unsigned long Sizeof()const{unsigned long n=(unsigned long)(2*Text.s.size()+SpeechBank.s.size()+Narrator.s.size()+SoundName.s.size()+Tags.size()*8);for(unsigned int i=0;i<Tags.size();++i)n+=(unsigned long)Tags[i].Name.s.size();return n;}void SetResourceSize(unsigned long n){resource=n;++updates;}void ReadFromDataStream(CDataInputStream&info_stream);};}

void NGameText::CTextEntry::ReadFromDataStream(CDataInputStream& info_stream)
{
 long old_ws_id=CWideString::MemTrackingID;
 long old_cs_id=CCharString::MemTrackingID;
 CWideString::MemTrackingID=11;
 CCharString::MemTrackingID=11;
 CScopedMemTrackingID mem_id(11);
 Text=info_stream.ReadNullTerminatedWString();
 SpeechBank=info_stream.ReadPresizedString();
 Narrator=info_stream.ReadPresizedString();
 SoundName=info_stream.ReadPresizedString();
 long size=info_stream.ReadSLONG();
 Tags.resize(size);
 for(long c1=0;c1<size;++c1) {
  Tags[c1].Pos=info_stream.ReadSLONG();
  Tags[c1].Name=info_stream.ReadNullTerminatedString();
 }
 SetResourceSize(Sizeof());
 CWideString::MemTrackingID=old_ws_id;
 CCharString::MemTrackingID=old_cs_id;
}
static void word(CDataInputStream&s,unsigned int v,unsigned int width){for(unsigned int i=0;i<width;++i){s.data.push_back((unsigned char)(v&255));v>>=8;}}
static void narrow(CDataInputStream&s,const char*p,bool presized){if(presized)word(s,(unsigned int)std::string(p).size(),4);while(*p)word(s,(unsigned char)*p++,1);if(!presized)word(s,0,1);}
static void header(CDataInputStream&s,bool empty){if(!empty){word(s,'A',2);word(s,0x20ac,2);}word(s,0,2);narrow(s,empty?"":"bank",true);narrow(s,empty?"":"narrator",true);narrow(s,empty?"":"sound",true);}
int main(){
 for(unsigned int chunk=1;chunk<=8;chunk*=2){
  {CDataInputStream s(chunk);header(s,true);word(s,0,4);NGameText::CTextEntry e;e.Tags.resize(2);e.ReadFromDataStream(s);check(e.Tags.empty()&&e.Text.s.empty()&&e.SpeechBank.s.empty()&&e.resource==0&&e.updates==1,"zero records replace old data");check(s.pos==s.data.size()&&CWideString::MemTrackingID==101&&CCharString::MemTrackingID==202,"consumption and memory categories restored");}
  {CDataInputStream s(chunk);header(s,false);word(s,1,4);word(s,0xffffffffU,4);narrow(s,"",false);NGameText::CTextEntry e;e.ReadFromDataStream(s);check(e.Text.s.size()==2&&e.Text.s[1]==0x20ac&&e.SpeechBank.s=="bank"&&e.Narrator.s=="narrator"&&e.SoundName.s=="sound","wide unicode and three presized fields");check(e.Tags.size()==1&&e.Tags[0].Pos==-1&&e.Tags[0].Name.s.empty(),"signed position and empty tag");check(e.resource==29&&e.resource==e.Sizeof()&&e.updates==1,"resource size observes fully loaded state");check(s.pos==s.data.size()&&CWideString::MemTrackingID==101&&CCharString::MemTrackingID==202,"single-record state restoration");}
  {CDataInputStream s(chunk);header(s,false);word(s,3,4);word(s,0,4);narrow(s,"one",false);word(s,8,4);narrow(s,"two",false);word(s,8,4);narrow(s,"three",false);NGameText::CTextEntry e;e.ReadFromDataStream(s);check(e.Tags.size()==3&&e.Tags[0].Pos==0&&e.Tags[2].Pos==8&&e.Tags[1].Name.s=="two"&&e.Tags[2].Name.s=="three","multiple tags preserve order and duplicate positions");check(e.resource==e.Sizeof()&&s.pos==s.data.size()&&CWideString::MemTrackingID==101&&CCharString::MemTrackingID==202,"multiple record consumption and restoration");}
 }
 puts("009c8de0_TEST PASS");return 0;
}