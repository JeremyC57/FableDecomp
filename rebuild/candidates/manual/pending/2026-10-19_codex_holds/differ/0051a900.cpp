// Grade: HELD(bytes match retail) -- test exercises a hand-written stand-in, not the candidate | Codex source 00/51/CRegion_CRegion_0051a900.cpp, flags , pass pattern 0051a900_TEST PASS
// CRegion::CRegion(const CRegion&) -- copy region vectors, strings, definition pointer, map and scalar settings
// Retail 0x0051A900 length 166; retail STLport vectors are 12 bytes and the map copy is out of line.
struct CLongVector { long* first; long* finish; long* last; CLongVector(const CLongVector&); };
struct CCharString { void* data; CCharString(const CCharString&); };
struct CDefPointee { virtual void v0(); virtual void v1(); long refs; };
struct CDefPointer { CDefPointee* Object; CDefPointer(const CDefPointer& o):Object(o.Object) { if(Object) ++Object->refs; } };
struct C2DVector { float X,Y; };
struct CExitTextOffsetMap { void* header; long count; long compare; CExitTextOffsetMap(const CExitTextOffsetMap&); };
struct CRegion {
 CLongVector Maps,VisMaps; CCharString Name,DisplayName,RegionDefName; CDefPointer PDef; CCharString MiniMapGraphic; float MiniMapScale; C2DVector MiniMapOffset; CExitTextOffsetMap MiniMapRegionExitTextOffsets; C2DVector WorldMapOffset,NameGraphicOffset; unsigned char AppearOnWorldMap,CreatureGenerationEnabled,SoundThemesEnabled;
 __declspec(dllexport) CRegion(const CRegion&);
};
CRegion::CRegion(const CRegion& o):Maps(o.Maps),VisMaps(o.VisMaps),Name(o.Name),DisplayName(o.DisplayName),RegionDefName(o.RegionDefName),PDef(o.PDef),MiniMapGraphic(o.MiniMapGraphic),MiniMapScale(o.MiniMapScale),MiniMapOffset(o.MiniMapOffset),MiniMapRegionExitTextOffsets(o.MiniMapRegionExitTextOffsets),WorldMapOffset(o.WorldMapOffset),NameGraphicOffset(o.NameGraphicOffset),AppearOnWorldMap(o.AppearOnWorldMap),CreatureGenerationEnabled(o.CreatureGenerationEnabled),SoundThemesEnabled(o.SoundThemesEnabled) {}