// CMap::LoadBrushesFromFile(CAFile&) -- load scripted map brushes, cells and associated thing changes.
// Leaf: ?LoadBrushesFromFile@CMap@@QAEXAAVCAFile@@@Z
// Flags: /GS
#define FABLE_RETAIL_CRESOURCE_PRIVATE_MEMBERS
#define FABLE_RETAIL_IVREFCOUNT_PRIVATE
#define FABLE_RETAIL_DEFPTR_ORDINARY_INLINE
#define FABLE_RETAIL_MAP_BRUSHES
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_DEFINITION_MANAGER
#include "engine/CCharString.h"
#include "engine/CMap.h"
#include "engine/CWorld.h"
#include "engine/CWorldMap.h"
#include "engine/CThing.h"
#include "engine/CThingManager.h"
#include "engine/CGameDefinitionManager.h"
#include "engine/CScriptedMapBrush.h"
#include "engine/CFileFormatBrush.h"
void CMap::LoadBrushesFromFile(CAFile& file)
{
    file.SetPosition(0);
    CFileFormatLevel level_header;
    level_header.ReadFromFile(file,0);
    CScriptedMapBrushInit init;
    CFileFormatBrushHeader file_brush_header;
    CFileFormatBrush file_brush;
    CFileFormatBrushCell file_brush_cell;
    CFileFormatBrushThingToMove file_brush_thing_to_move;
    CFileFormatBrushThingToDelete file_brush_thing_to_delete;
    CFileFormatBrushThingToAdd file_brush_thing_to_add;
    CThingManager& thing_manager=World.GetThingManager();
    const long map_number=World.PeekWorldMap().GetMapNumberAt(WorldPos);
    if(!level_header.BrushDataOffset)return;
    file.SetPosition(level_header.BrushDataOffset);
    file_brush_header.ReadFromFile(file,0);
    const CGameDefinitionManager& def_manager=DefinitionManager;
    CFileLargeThemePalette& file_palette=file_brush_header.LargeThemePalette;
    for(long c1=0;c1<256;++c1) {
        if(file_palette.GetThemePaletteEntry(c1).ThemePaletteThemeDefIndex) {
            long new_theme_def_index=def_manager.GetDefGlobalIndexFromName(file_palette.GetThemePaletteEntry(c1).ThemePaletteEntryName);
            if(new_theme_def_index)file_palette.GetThemePaletteEntry(c1).ThemePaletteThemeDefIndex=new_theme_def_index;
            else file_palette.GetThemePaletteEntry(c1).ThemePaletteThemeDefIndex=1;
        }
    }
    for(long c1=0;c1<file_brush_header.NoBrushes;++c1) {
        file_brush.ReadFromFile(file,0);
        init.GameArea=file_brush.GameArea;
        init.Name=file_brush.ScriptName;
        init.CreationTimeSeconds=file_brush.CreationTimeSeconds;
        init.CreationPattern=file_brush.CreationPattern;
        CScriptedMapBrush& brush=CreateScriptedMapBrush(init);
        C2DCoordI tl_pos=file_brush.GameArea.GetTLPos();
        C2DCoordI br_pos=file_brush.GameArea.GetBRPos();
        for(long y=tl_pos.Y;y<br_pos.Y;++y) {
            for(long x=tl_pos.X;x<br_pos.X;++x) {
                file_brush_cell.ReadFromFile(file,0);
                brush.SetGameHeight(x,y,file_brush_cell.Height);
                for(long cblend=0;cblend<2;++cblend)brush.SetThemeBlend(x,y,file_brush_cell.ThemeBlend[cblend],cblend);
                for(long ctheme=0;ctheme<3;++ctheme)brush.SetTheme(x,y,file_palette.GetThemePaletteEntry(file_brush_cell.Theme[ctheme]).ThemePaletteThemeDefIndex,ctheme);
            }
        }
        for(long cthing=0;cthing<file_brush.NumberOfThingsToMove;++cthing) {
            file_brush_thing_to_move.ReadFromFile(file,0);
            brush.SetThingToMove(file_brush_thing_to_move.UniqueID,file_brush_thing_to_move.Position,file_brush_thing_to_move.FacingAngles);
        }
        for(long cthing=0;cthing<file_brush.NumberOfThingsToDelete;++cthing) {
            file_brush_thing_to_delete.ReadFromFile(file,0);
            brush.SetThingToDelete(file_brush_thing_to_delete.UniqueID);
        }
        for(long cthing=0;cthing<file_brush.NumberOfThingsToAdd;++cthing) {
            file_brush_thing_to_add.ReadFromFile(file,0);
            brush.SetThingToAdd(file_brush_thing_to_add.UniqueID);
            CThing* pthing=thing_manager.GetPThingWithUID(file_brush_thing_to_add.UniqueID);
            if(pthing)pthing->SetAsActivated(false);
        }
    }
}
