// Grade: RELOCATION_MATCH (Fable landing 2026-10-19 after the Codex review hold: test exercises a hand-written stand-in, not the candidate)
// CRegion::CRegion(const CRegion&) -- member-wise copy construction; the counted region-def pointer gains a reference
// Retail: Fable.exe (TLC, VC7.1) @ 0x0051A900, 166 bytes.
// Callees: vector<long> copy ctor 0x00514160 (label "Std_Vector_Construct_Copy"), CCharString copy ctor 0x0099EC30,
//   map<CCharString,C2DVector> copy ctor 0x00518E80 (label "CRegion_Constructor", propagated).
// Layout: PDB (Ego_r) CRegion member names/types; the retail STLport vector<long> is 12 bytes (PDB 16), so Maps +0x00,
//   VisMaps +0x0c, Name +0x18, DisplayName +0x1c, RegionDefName +0x20, PDef +0x24, MiniMapGraphic +0x28,
//   MiniMapScale +0x2c, MiniMapOffset +0x30, MiniMapRegionExitTextOffsets (12-byte STLport map) +0x38,
//   WorldMapOffset +0x44, NameGraphicOffset +0x4c, the three flags +0x54..+0x56.
//   engine/CRegion.h carries the Ego_r 16-byte vectors (every member after Maps is 4..8 bytes late), so this row
//   uses a local model. CDefPointer releases its pointee through virtual slot 1 when the count at +0x04 drops to 0.
struct CCharString { void* PStringData; CCharString(const CCharString&); };   // 0x0099EC30
struct C2DVector { float X, Y; };
template<class T> struct CRetailVector { T* m_first; T* m_last; T* m_end; CRetailVector(const CRetailVector&); };   // long: 0x00514160
template<class K, class V> struct CRetailMap { void* m_header; unsigned long m_node_count; unsigned long m_compare; CRetailMap(const CRetailMap&); };   // 0x00518E80
struct CDefPointeeBase { virtual void VFunc00(); virtual void Release(); long RefCount; };
struct CRegionDef;
template<class T> struct CDefPointer {
    CDefPointeeBase* Object;
    CDefPointer(const CDefPointer& o) : Object(o.Object) { if (Object) ++Object->RefCount; }
};
struct CRegion {
    CRetailVector<long> Maps;                           // +0x00
    CRetailVector<long> VisMaps;                        // +0x0c
    CCharString Name;                                   // +0x18
    CCharString DisplayName;                            // +0x1c
    CCharString RegionDefName;                          // +0x20
    CDefPointer<const CRegionDef> PDef;                 // +0x24
    CCharString MiniMapGraphic;                         // +0x28
    float MiniMapScale;                                 // +0x2c
    C2DVector MiniMapOffset;                            // +0x30
    CRetailMap<CCharString, C2DVector> MiniMapRegionExitTextOffsets; // +0x38
    C2DVector WorldMapOffset;                           // +0x44
    C2DVector NameGraphicOffset;                        // +0x4c
    bool AppearOnWorldMap;                              // +0x54
    bool CreatureGenerationEnabled;                     // +0x55
    bool SoundThemesEnabled;                            // +0x56
    CRegion(const CRegion&);
};
CRegion::CRegion(const CRegion& o)
    : Maps(o.Maps), VisMaps(o.VisMaps), Name(o.Name), DisplayName(o.DisplayName), RegionDefName(o.RegionDefName),
      PDef(o.PDef), MiniMapGraphic(o.MiniMapGraphic), MiniMapScale(o.MiniMapScale), MiniMapOffset(o.MiniMapOffset),
      MiniMapRegionExitTextOffsets(o.MiniMapRegionExitTextOffsets), WorldMapOffset(o.WorldMapOffset),
      NameGraphicOffset(o.NameGraphicOffset), AppearOnWorldMap(o.AppearOnWorldMap),
      CreatureGenerationEnabled(o.CreatureGenerationEnabled), SoundThemesEnabled(o.SoundThemesEnabled)
{
}