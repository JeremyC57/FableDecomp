// Behaviour test for retail 0x0051a900: the candidate source below is embedded verbatim (from src/0051a900.cpp),
// followed by dependency stubs/spies and a main that asserts on the observable effects.
// ===== candidate source =====
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
class CCharString { public: void* PStringData; CCharString(const CCharString&); };   // 0x0099EC30
class C2DVector { public: float X, Y; };
template<class T> struct CRetailVector { T* m_first; T* m_last; T* m_end; CRetailVector(const CRetailVector&); };   // long: 0x00514160
template<class K, class V> struct CRetailMap { void* m_header; unsigned long m_node_count; unsigned long m_compare; CRetailMap(const CRetailMap&); };   // 0x00518E80
class CDefPointeeBase { public: virtual void VFunc00(); virtual void Release(); long RefCount; };
class CRegionDef;
template<class T> class CDefPointer { public:
    CDefPointeeBase* Object;
    CDefPointer(const CDefPointer& o) : Object(o.Object) { if (Object) ++Object->RefCount; }
};
class CRegion { public:
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

// ===== harness =====
#include <stdio.h>
#include <string.h>
#include <stddef.h>
#include <new>
static int g_vec, g_str, g_map; static unsigned char* g_dst; static int g_stroff[8];
CCharString::CCharString(const CCharString& o) { g_stroff[g_str++] = (int)((unsigned char*)this - g_dst); PStringData = o.PStringData; }
template<class T> CRetailVector<T>::CRetailVector(const CRetailVector<T>& o) { ++g_vec; m_first = o.m_first; m_last = o.m_last; m_end = o.m_end; }
template<class K, class V> CRetailMap<K, V>::CRetailMap(const CRetailMap<K, V>& o) { ++g_map; m_header = o.m_header; m_node_count = o.m_node_count; m_compare = 0; }
template struct CRetailVector<long>;
template struct CRetailMap<CCharString, C2DVector>;
struct CTestDef : CDefPointeeBase { };
void CDefPointeeBase::VFunc00() {}
void CDefPointeeBase::Release() {}
static unsigned char a_buf[sizeof(CRegion)], b_buf[sizeof(CRegion)];
int main() {
    memset(a_buf, 0, sizeof a_buf); memset(b_buf, 0xcd, sizeof b_buf); g_dst = b_buf;
    CRegion& a = *(CRegion*)a_buf; CTestDef def; def.RefCount = 2;
    int ok = sizeof(CRegion) == 0x58 && offsetof(CRegion, MiniMapGraphic) == 0x28 && offsetof(CRegion, WorldMapOffset) == 0x44;
    a.Maps.m_first = (long*)0x11; a.VisMaps.m_last = (long*)0x22; a.RegionDefName.PStringData = (void*)0x33; a.PDef.Object = &def;
    a.MiniMapScale = 1.5f; a.MiniMapOffset.X = 2.5f; a.MiniMapRegionExitTextOffsets.m_header = (void*)0x44; a.NameGraphicOffset.X = 7.0f;
    a.CreatureGenerationEnabled = true;
    CRegion* b = new (b_buf) CRegion(a);
    ok = ok && g_vec == 2 && g_str == 4 && g_map == 1 && g_stroff[0] == 0x18 && g_stroff[1] == 0x1c && g_stroff[2] == 0x20 && g_stroff[3] == 0x28;
    ok = ok && b->Maps.m_first == (long*)0x11 && b->VisMaps.m_last == (long*)0x22 && b->RegionDefName.PStringData == (void*)0x33;
    ok = ok && b->PDef.Object == &def && def.RefCount == 3 && b->MiniMapScale == 1.5f && b->MiniMapOffset.X == 2.5f;
    ok = ok && b->MiniMapRegionExitTextOffsets.m_header == (void*)0x44 && b->NameGraphicOffset.X == 7.0f;
    ok = ok && !b->AppearOnWorldMap && b->CreatureGenerationEnabled && !b->SoundThemesEnabled;
    printf(ok ? "0051a900_TEST PASS\n" : "0051a900_TEST FAIL\n"); return ok ? 0 : 1;
}