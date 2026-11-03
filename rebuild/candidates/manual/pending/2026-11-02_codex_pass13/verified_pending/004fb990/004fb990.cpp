// CWorldMap::SaveMapsToString(CCharString&) -- serialize the active map list and identifiers.
// Leaf: ?SaveMapsToString@CWorldMap@@IAEXAAVCCharString@@@Z
#define FABLE_RETAIL_CWORLDMAP
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_SCOPED_PTR
#define FABLE_RETAIL_CCLASSFACTORY
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#include <string.h>
#include "engine/CMapInfo.h"
#include "engine/CWorldMap.h"
#include "engine/CWorld.h"
#include "engine/CThingManager.h"
#include "engine/CAFile.h"
namespace NResourceDirectoryNames { CWideString __fastcall GetLevelDir(); }
CCharString __fastcall GFIntToCharString(long);
CCharString __fastcall GFUHUGEToCharString(unsigned __int64);
CCharString __fastcall operator+(const CCharString&,const CCharString&);
CCharString __fastcall operator+(const char*,const CCharString&);
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
