// Grade: HELD(bytes match retail) -- test exercises a hand-written stand-in, not the candidate | Codex source 00/51/CRegion_operator_00519b80.cpp, flags , pass pattern 00519b80_TEST PASS
// CRegion::operator=(const CRegion&) -- assign all region state and retain the shared region definition
// Retail 0x00519B80 length 195; helper assignments are the retail STLport/string routines.
struct CLongVector { long* first; long* finish; long* last; CLongVector& operator=(const CLongVector&); };
struct CCharString { void* data; CCharString& operator=(const CCharString&); };
struct CDefPointee { virtual void v0(); virtual void v1(); long refs; };
struct CDefPointer { CDefPointee* Object; CDefPointer& operator=(const CDefPointer& o) { if(Object!=o.Object) { if(Object) { CDefPointee* old=Object; if(--old->refs==0) old->v1(); Object=0; } Object=o.Object; if(Object) ++Object->refs; } return *this; } };
struct C2DVector { float X,Y; };
struct CExitTextOffsetMap { void* header; long count; long compare; CExitTextOffsetMap& operator=(const CExitTextOffsetMap&); };
struct CRegion {
 CLongVector Maps,VisMaps; CCharString Name,DisplayName,RegionDefName; CDefPointer PDef; CCharString MiniMapGraphic; float MiniMapScale; C2DVector MiniMapOffset; CExitTextOffsetMap MiniMapRegionExitTextOffsets; C2DVector WorldMapOffset,NameGraphicOffset; unsigned char AppearOnWorldMap,CreatureGenerationEnabled,SoundThemesEnabled;
 __declspec(dllexport) CRegion& operator=(const CRegion&);
};
CRegion& CRegion::operator=(const CRegion& o) {
 Maps=o.Maps; VisMaps=o.VisMaps; Name=o.Name; DisplayName=o.DisplayName; RegionDefName=o.RegionDefName;
 PDef=o.PDef; MiniMapGraphic=o.MiniMapGraphic; MiniMapScale=o.MiniMapScale; MiniMapOffset=o.MiniMapOffset;
 MiniMapRegionExitTextOffsets=o.MiniMapRegionExitTextOffsets; WorldMapOffset=o.WorldMapOffset; NameGraphicOffset=o.NameGraphicOffset;
 AppearOnWorldMap=o.AppearOnWorldMap; CreatureGenerationEnabled=o.CreatureGenerationEnabled; SoundThemesEnabled=o.SoundThemesEnabled;
 return *this;
}