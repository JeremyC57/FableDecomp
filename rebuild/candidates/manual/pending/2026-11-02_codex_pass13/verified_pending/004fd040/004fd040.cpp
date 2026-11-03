// CWorldMap::SaveRegionsToString(CCharString&) -- append region settings and their contained and visible map paths.
// Leaf: ?SaveRegionsToString@CWorldMap@@IAEXAAVCCharString@@@Z
#define FABLE_RETAIL_CWORLDMAP
#define FABLE_RETAIL_SCOPED_PTR
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CREGION
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_REGION_SERIALIZATION
#define FABLE_RETAIL_CCHARSTRING_CHAR_ARRAY_BODY
#include <string.h>
#include "engine/CMapInfo.h"
#include "engine/CRegion.h"
#include "engine/CWorldMap.h"
#include "engine/CAFile.h"
namespace NResourceDirectoryNames { CWideString __fastcall GetLevelDir(); }
CCharString __fastcall GFIntToCharString(long);
CCharString __fastcall GFFloatToCharString(double,long=6);
CCharString __fastcall operator+(const CCharString&,const CCharString&);
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
