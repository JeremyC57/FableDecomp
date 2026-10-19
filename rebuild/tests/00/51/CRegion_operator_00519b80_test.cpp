// Behaviour test for retail 0x00519b80: the candidate source below is embedded verbatim (from src/00519b80.cpp),
// followed by dependency stubs/spies and a main that asserts on the observable effects.
// ===== candidate source =====
// CRegion::operator=(const CRegion&) -- assign every region member; the counted region-def pointer is re-targeted
// Retail: Fable.exe (TLC, VC7.1) @ 0x00519B80, 195 bytes.
// Callees: vector<long>::operator= 0x00454886 (manifest "CActiveFile::operator=", propagated), CCharString::operator=
//   0x0099EFB0, map<CCharString,C2DVector>::operator= 0x00517370 (manifest "LTextTreeWalkThrough_CopyAssignment").
// Layout: PDB (Ego_r) CRegion member names/types; the retail STLport vector<long> is 12 bytes (PDB 16), so Maps +0x00,
//   VisMaps +0x0c, Name +0x18, DisplayName +0x1c, RegionDefName +0x20, PDef +0x24, MiniMapGraphic +0x28,
//   MiniMapScale +0x2c, MiniMapOffset +0x30, MiniMapRegionExitTextOffsets (12-byte STLport map) +0x38,
//   WorldMapOffset +0x44, NameGraphicOffset +0x4c, the three flags +0x54..+0x56.
//   engine/CRegion.h carries the Ego_r 16-byte vectors (every member after Maps is 4..8 bytes late), so this row
//   uses a local model. CDefPointer releases its pointee through virtual slot 1 when the count at +0x04 drops to 0.
struct CCharString { void* PStringData; CCharString& operator=(const CCharString&); };   // 0x0099EFB0
struct C2DVector { float X, Y; };
template<class T> struct CRetailVector { T* m_first; T* m_last; T* m_end; CRetailVector& operator=(const CRetailVector&); };   // long: 0x00454886
template<class K, class V> struct CRetailMap { void* m_header; unsigned long m_node_count; unsigned long m_compare; CRetailMap& operator=(const CRetailMap&); };   // 0x00517370
struct CDefPointeeBase { virtual void VFunc00(); virtual void Release(); long RefCount; };
struct CRegionDef;
template<class T> struct CDefPointer {
    CDefPointeeBase* Object;
    CDefPointer& operator=(const CDefPointer& o) {
        if (Object != o.Object) {
            if (Object) {
                CDefPointeeBase* old = Object;
                if (--old->RefCount == 0) old->Release();
                Object = 0;
            }
            Object = o.Object;
            if (Object) ++Object->RefCount;
        }
        return *this;
    }
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
    CRegion& operator=(const CRegion&);
};
CRegion& CRegion::operator=(const CRegion& o) {
    Maps = o.Maps;
    VisMaps = o.VisMaps;
    Name = o.Name;
    DisplayName = o.DisplayName;
    RegionDefName = o.RegionDefName;
    PDef = o.PDef;
    MiniMapGraphic = o.MiniMapGraphic;
    MiniMapScale = o.MiniMapScale;
    MiniMapOffset = o.MiniMapOffset;
    MiniMapRegionExitTextOffsets = o.MiniMapRegionExitTextOffsets;
    WorldMapOffset = o.WorldMapOffset;
    NameGraphicOffset = o.NameGraphicOffset;
    AppearOnWorldMap = o.AppearOnWorldMap;
    CreatureGenerationEnabled = o.CreatureGenerationEnabled;
    SoundThemesEnabled = o.SoundThemesEnabled;
    return *this;
}

// ===== harness =====
#include <stdio.h>
#include <string.h>
#include <stddef.h>
static int g_vec, g_str, g_map, g_rel; static unsigned char* g_dst; static int g_stroff[8];
CCharString& CCharString::operator=(const CCharString& o) { g_stroff[g_str++] = (int)((unsigned char*)this - g_dst); PStringData = o.PStringData; return *this; }
template<class T> CRetailVector<T>& CRetailVector<T>::operator=(const CRetailVector<T>& o) { ++g_vec; m_first = o.m_first; m_last = o.m_last; m_end = o.m_end; return *this; }
template<class K, class V> CRetailMap<K, V>& CRetailMap<K, V>::operator=(const CRetailMap<K, V>& o) { ++g_map; m_header = o.m_header; m_node_count = o.m_node_count; return *this; }
template struct CRetailVector<long>;
template struct CRetailMap<CCharString, C2DVector>;
struct CTestDef : CDefPointeeBase { };
void CDefPointeeBase::VFunc00() {}
void CDefPointeeBase::Release() { ++g_rel; }
int main() {
    CRegion a, b; memset(&a, 0, sizeof a); memset(&b, 0, sizeof b); g_dst = (unsigned char*)&b;
    CTestDef oldDef, newDef; oldDef.RefCount = 1; newDef.RefCount = 3;
    int ok = sizeof(CRegion) == 0x58 && offsetof(CRegion, PDef) == 0x24 && offsetof(CRegion, MiniMapRegionExitTextOffsets) == 0x38 && offsetof(CRegion, SoundThemesEnabled) == 0x56;
    a.Maps.m_last = (long*)0x10; a.VisMaps.m_end = (long*)0x20; a.Name.PStringData = (void*)0x30; a.MiniMapGraphic.PStringData = (void*)0x40;
    a.PDef.Object = &newDef; b.PDef.Object = &oldDef;
    a.MiniMapScale = 2.0f; a.MiniMapOffset.Y = 3.0f; a.MiniMapRegionExitTextOffsets.m_node_count = 4; a.WorldMapOffset.X = 5.0f;
    a.NameGraphicOffset.Y = 6.0f; a.AppearOnWorldMap = true; a.SoundThemesEnabled = true;
    CRegion* r = &(b = a);
    ok = ok && r == &b && g_vec == 2 && g_str == 4 && g_map == 1 && g_stroff[0] == 0x18 && g_stroff[3] == 0x28;
    ok = ok && b.Maps.m_last == (long*)0x10 && b.VisMaps.m_end == (long*)0x20 && b.Name.PStringData == (void*)0x30 && b.MiniMapGraphic.PStringData == (void*)0x40;
    ok = ok && oldDef.RefCount == 0 && g_rel == 1 && newDef.RefCount == 4 && b.PDef.Object == &newDef;
    ok = ok && b.MiniMapScale == 2.0f && b.MiniMapOffset.Y == 3.0f && b.MiniMapRegionExitTextOffsets.m_node_count == 4 && b.WorldMapOffset.X == 5.0f;
    ok = ok && b.NameGraphicOffset.Y == 6.0f && b.AppearOnWorldMap && !b.CreatureGenerationEnabled && b.SoundThemesEnabled;
    /* self-referencing pointer: same object -> no release, no extra reference */
    b = a; ok = ok && newDef.RefCount == 4 && g_rel == 1;
    printf(ok ? "00519b80_TEST PASS\n" : "00519b80_TEST FAIL\n"); return ok ? 0 : 1;
}