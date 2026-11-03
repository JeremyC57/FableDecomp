#include <string>
#include <vector>
#include <cassert>
#include <cstdio>
#include <cstring>
#ifdef _MSC_VER
typedef unsigned __int64 UInt64;
#else
typedef unsigned long UInt64;
#endif
static int char_live=0,wide_live=0,directory_reads=0,split_calls=0;
class CCharString {
public:
 std::string value;
 CCharString(){++char_live;}
 CCharString(const char* p):value(p){++char_live;}
 CCharString(const std::string& p):value(p){++char_live;}
 CCharString(const CCharString& p):value(p.value){++char_live;}
 ~CCharString(){--char_live;}
 operator const char*()const{return value.c_str();}
 CCharString& operator=(const char* p){value=p;return *this;}
 CCharString& operator+=(const char* p){value+=p;return *this;}
 CCharString& operator+=(const CCharString& p){value+=p.value;return *this;}
};
class CWideString {
public:
 std::string value;
 CWideString(const std::string& p):value(p){++wide_live;}
 CWideString(const CWideString& p):value(p.value){++wide_live;}
 ~CWideString(){--wide_live;}
 CCharString ToCharString()const{return CCharString(value);}
 long GetLength()const{return (long)value.size();}
};
CCharString operator+(const CCharString&a,const CCharString&b){return CCharString(a.value+b.value);}
CCharString operator+(const CCharString&a,const char*b){return CCharString(a.value+b);}
CCharString operator+(const char*a,const CCharString&b){return CCharString(a+b.value);}
CCharString GFIntToCharString(long n){if(n>2147483647L)n=-1L-(long)(4294967295UL-(unsigned long)n);char b[32];std::sprintf(b,"%ld",n);return CCharString(b);}
CCharString GFUHUGEToCharString(UInt64 n){std::string s;do{s.insert(s.begin(),(char)('0'+n%10));n/=10;}while(n);return CCharString(s);}
static std::string directory;
namespace NResourceDirectoryNames {CWideString GetLevelDir(){++directory_reads;return CWideString(directory);}}
class CAFile {
public:
 static void SplitPath(const CCharString& path,CCharString* drive,CCharString* dir,CCharString* file,CCharString* ext){
  assert(!drive&&dir&&file&&ext);++split_calls;
  std::string p=path.value;std::size_t start=p.size()>1&&p[1]==':'?2:0;
  std::size_t slash=p.find_last_of("\\/");std::size_t basename=slash==std::string::npos?start:slash+1;
  dir->value=p.substr(start,basename-start);
  std::size_t dot=p.find_last_of('.');if(dot==std::string::npos||dot<basename)dot=p.size();
  file->value=p.substr(basename,dot-basename);ext->value=p.substr(dot);
 }
};
class C2DBoxI {public:long TLX,TLY,BRX,BRY;C2DBoxI():TLX(0),TLY(0),BRX(0),BRY(0){};};
class CMapInfo {public:C2DBoxI Area;CCharString LevelName,LevelScriptName;unsigned int MapUID;bool Used,IsSea,LoadedOnPlayerProximity;CMapInfo():MapUID(0),Used(false),IsSea(false),LoadedOnPlayerProximity(false){}};
class CThingManager {public:UInt64 uid;UInt64 GetCurrentThingUID()const{return uid;}};
class CWorld {public:CThingManager manager;CThingManager& GetThingManager(){return manager;}};
class CWorldMap {public:CWorld World;unsigned int CurrentMapUIDCount;std::vector<CMapInfo> Maps;void SaveMapsToString(CCharString&);};

void CWorldMap::SaveMapsToString(CCharString& save_text)
{
    long number_valid_maps=1;
    save_text += "MapUIDCount "+GFIntToCharString(CurrentMapUIDCount)+";\r\n";
    save_text += "ThingManagerUIDCount "+GFUHUGEToCharString(World.GetThingManager().GetCurrentThingUID())+";\r\n";
    for(long c1=1;c1<Maps.size();++c1) {
        CMapInfo& map_info=Maps[c1];
        if(map_info.Used) {
            CCharString fname,extname,dir_name;
            CAFile::SplitPath(map_info.LevelName,0,&dir_name,&fname,&extname);
            const char* pdir=strstr((const char*)dir_name,(const char*)NResourceDirectoryNames::GetLevelDir().ToCharString());
            CCharString relative_path;
            if(pdir) relative_path=pdir+NResourceDirectoryNames::GetLevelDir().GetLength();
            CCharString level_name=relative_path+fname+extname;
            save_text += "NewMap "+GFIntToCharString(number_valid_maps)+";\r\n";
            save_text += "MapX "+GFIntToCharString(map_info.Area.TLX)+";\r\n";
            save_text += "MapY "+GFIntToCharString(map_info.Area.TLY)+";\r\n";
            save_text += "LevelName \""+level_name+"\";\r\n";
            save_text += "LevelScriptName \""+map_info.LevelScriptName+"\";\r\n";
            save_text += "MapUID "+GFIntToCharString(map_info.MapUID)+";\r\n";
            if(map_info.IsSea) save_text += "IsSea TRUE;\r\n";
            else save_text += "IsSea FALSE;\r\n";
            if(map_info.LoadedOnPlayerProximity) save_text += "LoadedOnPlayerProximity TRUE;\r\n";
            else save_text += "LoadedOnPlayerProximity FALSE;\r\n";
            save_text += "EndMap;\r\n\r\n";
            ++number_valid_maps;
        }
    }
}

static std::string num(long n){char b[40];std::sprintf(b,"%ld",n);return b;}
static std::size_t occurrences(const std::string&s,const std::string&x){std::size_t n=0,p=0;while((p=s.find(x,p))!=std::string::npos){++n;p+=x.size();}return n;}
int main(){
 const char* paths[]={"C:\\Levels\\Oak\\map.lev","Other\\map.lev","prefix\\Levels\\Oak\\map.lev","levels\\Oak\\map.lev","Levels\\Oak\\map.part.lev","Levels\\map"};
 const char* rel[]={"Oak\\map.lev","map.lev","Oak\\map.lev","map.lev","Oak\\map.part.lev","map"};
 int scenarios=0;
 for(int p=0;p<6;++p)for(int mask=0;mask<16;++mask)for(int flags=0;flags<4;++flags){
  {CWorldMap world;world.CurrentMapUIDCount=37;world.World.manager.uid=((UInt64)1<<40)+19;world.Maps.resize(5);
   world.Maps[0].Used=true;world.Maps[0].LevelName="ignored.lev";
   directory="Levels\\";directory_reads=split_calls=0;
   std::string expected="prefix:MapUIDCount 37;\r\nThingManagerUIDCount 1099511627795;\r\n";int number=0;
   for(int i=1;i<=4;++i){CMapInfo&m=world.Maps[i];m.Used=(mask&(1<<(i-1)))!=0;m.LevelName=paths[p];m.LevelScriptName="quest";m.Area.TLX=-50+i;m.Area.TLY=100*i;m.MapUID=900+i;m.IsSea=(flags&1)!=0;m.LoadedOnPlayerProximity=(flags&2)!=0;
    if(m.Used){++number;expected+="NewMap "+num(number)+";\r\nMapX "+num(-50+i)+";\r\nMapY "+num(100*i)+";\r\nLevelName \""+rel[p]+"\";\r\nLevelScriptName \"quest\";\r\nMapUID "+num(900+i)+";\r\n";expected+=(flags&1)?"IsSea TRUE;\r\n":"IsSea FALSE;\r\n";expected+=(flags&2)?"LoadedOnPlayerProximity TRUE;\r\n":"LoadedOnPlayerProximity FALSE;\r\n";expected+="EndMap;\r\n\r\n";}
   }
   CCharString out("prefix:");int live_before=char_live;world.SaveMapsToString(out);assert(out.value==expected);assert(char_live==live_before&&wide_live==0);assert(split_calls==number);assert(directory_reads==number*((p==1||p==3)?1:2));assert(occurrences(out.value,"NewMap ")== (std::size_t)number);
   for(int j=1;j<=4;++j){assert(world.Maps[j].LevelName.value==paths[p]);assert(world.Maps[j].MapUID==(unsigned int)(900+j));}
   ++scenarios;
  }assert(char_live==0&&wide_live==0);
 }
 {CWorldMap w;w.CurrentMapUIDCount=4294967295U;w.World.manager.uid=(UInt64)0-1;CCharString out;w.SaveMapsToString(out);assert(out.value=="MapUIDCount -1;\r\nThingManagerUIDCount 18446744073709551615;\r\n");++scenarios;}
 {CWorldMap w;w.CurrentMapUIDCount=0;w.World.manager.uid=0;CCharString out;w.SaveMapsToString(out);assert(out.value=="MapUIDCount 0;\r\nThingManagerUIDCount 0;\r\n");++scenarios;}
 assert(char_live==0&&wide_live==0&&scenarios==386);
 std::puts("004fb990_TEST PASS");return 0;
}
