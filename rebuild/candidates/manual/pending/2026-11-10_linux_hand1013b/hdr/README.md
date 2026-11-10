# Chunk 1013b header overlay

Graded with `FABLE_HDR_OVERLAY=$A/hdr` (every grade, gall.sh and q.py). Copied from rebuild/include at HEAD, then changed only
under the opt-ins below. Every change sits under its own `#ifdef FABLE_RETAIL_*`, forward declarations included; the two new
headers are new files (nothing includes them unless a row does).

| File | Opt-in | What it adds | Used by |
|---|---|---|---|
| engine/CTBaseSingleton.h | NEW | `template<class T> class CTBaseSingleton`: private static `CCountedPointer<T> PClass` (Ego_r / FableWin `dynamic initializer for 'CTBaseSingleton<CEngineManager>::PClass'`), protected ctor, declared virtual dtor, inline allocating `static T& FABLE_FASTCALL Get()` (`if (PClass.IsNull()) PClass.Reset(new T); return *PClass;`, the body 0x00436FB0 and its expansion in 0x00507650) | 0x00507650 (src), 0x004FEB70 (differ) |
| engine/CEngineManager.h | NEW | `class CEngineManager : public CTBaseSingleton<CEngineManager>` (0x14 bytes: vptr, `std::list<CIEngine*> Engines`, `std::vector<C2DBoxI> ChangedAreas`), inline ctor, declared dtor, AddEngine 0x006404D0, AddMap 0x006403A0, AddChangedArea 0x00640560 (Ego_r / FableWin names) | 0x00507650, 0x004FEB70 |
| engine/CWorldMap.h | FABLE_RETAIL_CWORLDMAP_LOADWORLDFROMBINARY | public `LoadWorldFromBinaryFile(CWideString&)`, protected `SetMapPlacement(const C2DBoxI&, long)` 0x004FC9C0 (declarations only) | 0x00507650 |
| engine/CRegion.h | FABLE_RETAIL_CREGION_LOADBINARY | declared ctor / copy ctor / dtor and `LoadBinary(CDataInputStream&)` 0x006BC510; forward decl of CDataInputStream inside the ifdef | 0x00507650 |
| engine/CMapInfo.h | FABLE_RETAIL_CMAPINFO_LOADBINARY | declared ctor / copy ctor and `LoadBinary(CDataInputStream&)` 0x004FB4F0; forward decl inside the ifdef | 0x00507650 |
| stlport/fable_containers.h | FABLE_RETAIL_STLP_FILL_CHAR | STLport 4.6's `fill(char*, char*, const char&)` (memset) and `fill_n(char*, Size, const char&)` overloads; without them vector<char>(n)'s fill is too costly to inline and VC7.1 calls it out of line | 0x00507650 |
| engine/CEngineWorldMap.h | FABLE_RETAIL_CENGINEWORLDMAP_GETMAPXY (inside FABLE_RETAIL_CENGINEWORLDMAP) | inline `GetMap(long x, long y) const` (FableWin.pdb ?GetMap@CEngineWorldMap@@QBEPAVCEngineMap@@JJ@Z): the 32x32-cell index grid lookup | 0x00B43720 / 0x00B43870 / 0x00B439C0 / 0x00B41770 |
| engine/CIDrawMap.h | FABLE_RETAIL_CIDRAWMAP_DRAW (inside FABLE_RETAIL_CIDRAWMAP) | virtual slots 1..9 in retail order (CMap vtable 0x0127252C, CIDrawMap 0x01272494); forward decls C3DVector / C2DExtentsI inside the ifdef | the three peeks, 0x00B41770 |
| engine/CEngineMap.h | FABLE_RETAIL_CENGINEMAP_PEEK, nested _PEEKNORMAL and _PEEKWATER | inline PeekWidth / PeekHeight / PeekWorldX / PeekWorldY / PeekGameMap / MapHasDataAtPoint, declarations of PeekThemeId / PeekThemeBlend / PeekLandscapeHeight, private inline QuantiseLandscapeHeight (GFFloatToLongNear, included under the ifdef); PEEKNORMAL: PeekMapNormal, PeekInterpolatedMapNormal; PEEKWATER: PeekHasWater, PeekHasWaterFast, PeekWaterDepth, PeekInterpolatedWaterHeight | the five peeks |

No engine/CASoundManager.h names are used. The landed 0x00507650 needs only the first six rows of the table.
