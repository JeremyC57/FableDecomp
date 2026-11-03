#include <string>
#include <vector>
#include <map>
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
 const char* GetAsCharArray()const{return value.c_str();}
 bool operator<(const CCharString& o)const{return value<o.value;}
 bool operator!=(const char* o)const{return value!=o;}
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

CCharString GFFloatToCharString(double x,long precision=6){assert(precision==6);char b[100];std::sprintf(b,"%.6f",x);std::string s=b;while(s.size()&&s[s.size()-1]=='0')s.erase(s.size()-1);if(s.size()&&s[s.size()-1]=='.')s+='0';return CCharString(s);}
class C2DVector {public:float X,Y;C2DVector(float x=0,float y=0):X(x),Y(y){};C2DVector(const C2DVector& x):X(x.X),Y(x.Y){};};
class CMapInfo {public:CCharString LevelName;};
static int world_reads=0,name_reads=0;
class CRegion {public:
 std::vector<long> Maps,VisMaps;CCharString Name,DisplayName,RegionDefName,MiniMapGraphic;float MiniMapScale;C2DVector MiniMapOffset,world,name;bool AppearOnWorldMap;std::map<CCharString,C2DVector> MiniMapRegionExitTextOffsets;
 CRegion():MiniMapScale(0),AppearOnWorldMap(false){}
 const CCharString& GetName()const{return Name;}
 const CCharString& GetDisplayName()const{return DisplayName.value.empty()?Name:DisplayName;}
 const CCharString& GetRegionDefName()const{return RegionDefName;}
 bool IsOnWorldMap()const{return AppearOnWorldMap;}
 const CCharString& GetMiniMapGraphic()const{return MiniMapGraphic;}
 float GetMiniMapScale()const{return MiniMapScale;}
 const C2DVector& GetMiniMapOffset()const{return MiniMapOffset;}
 const C2DVector GetWorldMapOffset()const{++world_reads;return world;}
 const C2DVector GetNameGraphicOffset()const{++name_reads;return name;}
 const std::map<CCharString,C2DVector>& GetMiniMapRegionExitTextOffsets()const{return MiniMapRegionExitTextOffsets;}
 const std::vector<long>& GetMaps()const{return Maps;}
 std::vector<long>& GetVisMaps(){return VisMaps;}
};
class CWorldMap {public:std::vector<CMapInfo>Maps;std::vector<CRegion>Regions;void SaveRegionsToString(CCharString&);};
void CWorldMap::SaveRegionsToString(CCharString& save_text)
{
    long number_valid_regions=1;
    for(long region_index=1;region_index<Regions.size();++region_index) {
        CRegion& region=Regions[region_index];
        save_text += "NewRegion "+GFIntToCharString(region_index)+";\r\n";
        save_text += "RegionName \""+region.GetName()+"\";\r\n";
        save_text += "NewDisplayName \""+region.GetDisplayName()+"\";\r\n";
        save_text += "RegionDef \""+region.GetRegionDefName()+"\";\r\n";
        if(region.IsOnWorldMap()) save_text += "AppearOnWorldMap;\r\n";
        if(region.GetMiniMapGraphic()!="") save_text += "MiniMapGraphic "+region.GetMiniMapGraphic()+";\r\n";
        save_text += "MiniMapScale "+GFFloatToCharString(region.GetMiniMapScale())+";\r\n";
        save_text += "MiniMapOffsetX "+GFFloatToCharString(region.GetMiniMapOffset().X)+";\r\n";
        save_text += "MiniMapOffsetY "+GFFloatToCharString(region.GetMiniMapOffset().Y)+";\r\n";
        save_text += "WorldMapOffsetX "+GFFloatToCharString(region.GetWorldMapOffset().X)+";\r\n";
        save_text += "WorldMapOffsetY "+GFFloatToCharString(region.GetWorldMapOffset().Y)+";\r\n";
        save_text += "NameGraphicOffsetX "+GFFloatToCharString(region.GetNameGraphicOffset().X)+";\r\n";
        save_text += "NameGraphicOffsetY "+GFFloatToCharString(region.GetNameGraphicOffset().Y)+";\r\n";
        const std::map<CCharString,C2DVector>& text_offsets=region.GetMiniMapRegionExitTextOffsets();
        for(std::map<CCharString,C2DVector>::const_iterator it=text_offsets.begin();it!=text_offsets.end();++it) {
            save_text += "MiniMapRegionExitTextOffsetX["+it->first+"] "+GFFloatToCharString(it->second.X)+";\r\n";
            save_text += "MiniMapRegionExitTextOffsetY["+it->first+"] "+GFFloatToCharString(it->second.Y)+";\r\n";
        }
        const std::vector<long>& region_maps=region.GetMaps();
        for(std::vector<long>::const_iterator region_map_it=region_maps.begin();region_map_it!=region_maps.end();++region_map_it) {
            long region_map=*region_map_it;
            CMapInfo& map_info=Maps[region_map];
            CCharString fname,extname,dir_name;
            CAFile::SplitPath(map_info.LevelName,0,&dir_name,&fname,&extname);
            const char* pdir=strstr(dir_name.GetAsCharArray(),(const char*)NResourceDirectoryNames::GetLevelDir().ToCharString());
            CCharString relative_path;
            if(pdir) relative_path=pdir+NResourceDirectoryNames::GetLevelDir().GetLength();
            CCharString level_name=relative_path+fname+extname;
            save_text += "ContainsMap \""+level_name+"\";\r\n";
        }
        for(std::vector<long>::iterator it=region.GetVisMaps().begin();it!=region.GetVisMaps().end();++it) {
            long vis_map=*it;
            CMapInfo& map_info=Maps[vis_map];
            CCharString fname,extname,dir_name;
            CAFile::SplitPath(map_info.LevelName,0,&dir_name,&fname,&extname);
            const char* pdir=strstr((const char*)dir_name,(const char*)NResourceDirectoryNames::GetLevelDir().ToCharString());
            CCharString relative_path;
            if(pdir) relative_path=pdir+NResourceDirectoryNames::GetLevelDir().GetLength();
            CCharString level_name=relative_path+fname+extname;
            save_text += "SeesMap \""+level_name+"\";\r\n";
        }
        save_text += "EndRegion;\r\n\r\n";
    }
}

static std::string num(long n){char b[32];std::sprintf(b,"%ld",n);return b;}
int main(){
 const char* paths[]={"C:\\Levels\\Oak\\map.lev","Other\\map.lev","prefix\\Levels\\Oak\\map.lev","levels\\Oak\\map.lev","Levels\\Oak\\map.part.lev","Levels\\map","plain","","Levels\\folder\\.lev","x/Levels/folder/map.lev"};
 const char* rel[]={"Oak\\map.lev","map.lev","Oak\\map.lev","map.lev","Oak\\map.part.lev","map","plain","","folder\\.lev","map.lev"};
 const bool found[]={true,false,true,false,true,true,false,false,true,false};
 int scenarios=0;
 for(int p=0;p<10;++p)for(int count=0;count<5;++count)for(int mask=0;mask<16;++mask)for(int flags=0;flags<4;++flags){
  {CWorldMap w;w.Maps.resize(3);for(int m=0;m<3;++m)w.Maps[m].LevelName=paths[p];w.Regions.resize(count);
   directory="Levels\\";directory_reads=split_calls=world_reads=name_reads=0;std::string expected="prefix:";int items=0;
   for(int r=0;r<count;++r){CRegion& x=w.Regions[r];x.Name=std::string("region")+num(r);x.DisplayName=(r&1)?"":"display";x.RegionDefName="def";x.AppearOnWorldMap=(flags&1)!=0;x.MiniMapGraphic=(flags&2)?"mini.dds":"";x.MiniMapScale=1.25f;x.MiniMapOffset=C2DVector(-2.5f,3.75f);x.world=C2DVector(4.5f,-5.25f);x.name=C2DVector(6.75f,-7.5f);
    if(mask&1)x.Maps.push_back(2);if(mask&2)x.Maps.push_back(0);if(mask&4)x.VisMaps.push_back(1);if(mask&8)x.VisMaps.push_back(1);
    if(r%3){x.MiniMapRegionExitTextOffsets[CCharString("z")]=C2DVector(8.25f,-9.5f);x.MiniMapRegionExitTextOffsets[CCharString("Alpha")]=C2DVector(-10.75f,11.25f);}
    if(!r)continue;
    expected+="NewRegion "+num(r)+";\r\nRegionName \"region"+num(r)+"\";\r\nNewDisplayName \""+((r&1)?std::string("region")+num(r):std::string("display"))+"\";\r\nRegionDef \"def\";\r\n";
    if(flags&1)expected+="AppearOnWorldMap;\r\n";if(flags&2)expected+="MiniMapGraphic mini.dds;\r\n";
    expected+="MiniMapScale 1.25;\r\nMiniMapOffsetX -2.5;\r\nMiniMapOffsetY 3.75;\r\nWorldMapOffsetX 4.5;\r\nWorldMapOffsetY -5.25;\r\nNameGraphicOffsetX 6.75;\r\nNameGraphicOffsetY -7.5;\r\n";
    if(r%3)expected+="MiniMapRegionExitTextOffsetX[Alpha] -10.75;\r\nMiniMapRegionExitTextOffsetY[Alpha] 11.25;\r\nMiniMapRegionExitTextOffsetX[z] 8.25;\r\nMiniMapRegionExitTextOffsetY[z] -9.5;\r\n";
    for(int bit=0;bit<4;++bit)if(mask&(1<<bit)){expected+=std::string(bit<2?"ContainsMap \"":"SeesMap \"")+rel[p]+"\";\r\n";++items;}
    expected+="EndRegion;\r\n\r\n";
   }
   CCharString out("prefix:");int live_before=char_live;w.SaveRegionsToString(out);assert(out.value==expected);assert(char_live==live_before&&wide_live==0);assert(split_calls==items&&directory_reads==items*(found[p]?2:1));assert(world_reads==(count>0?count-1:0)*2&&name_reads==world_reads);
   for(int m=0;m<3;++m)assert(w.Maps[m].LevelName.value==paths[p]);for(int r=0;r<count;++r){assert(w.Regions[r].Name.value==std::string("region")+num(r));assert(w.Regions[r].Maps.size()==(unsigned)((mask&1?1:0)+(mask&2?1:0)));assert(w.Regions[r].VisMaps.size()==(unsigned)((mask&4?1:0)+(mask&8?1:0)));}
   ++scenarios;
  }assert(char_live==0&&wide_live==0);
 }
 assert(scenarios==3200);std::puts("004fd040_TEST PASS");return 0;
}
