#ifndef FABLE_ENGINE_CGAMEMAP_H
#define FABLE_ENGINE_CGAMEMAP_H
#ifdef FABLE_RETAIL_CGAMEMAP
// New header (no repo copy; chunk 2055 scout, row 0x00821A80 CMap::LoadFromFile). Retail class: RTTI .?AVCGameMap@@ :
// CBaseClassNonCopyable@0 (ghidra_out/rtti_bases.tsv), vtable 0x0125D558 (slot 0 0x006B81A0, rtti-vtables.tsv); sizeof 0x24
// (`push 24h`, CMap::InitGameMap 0x0081EE12). Ego_r.pdb members in order (struct_layouts_egor.tsv; Ego_r vector 16 bytes, retail
// STLport 12, so Map.. sit 4 below the PDB offset): Cells vector<CGameMapCell> +0x04, Map +0x10, World +0x14, WorldSeed +0x18, SizeX
// +0x1C, SizeY +0x20 (retail 0x008220A3 / 0x008220B0 / 0x008220E1 / 0x008220E5 read +0x1C, +0x20, +0x04). CGameMapCell (Ego_r, 7 bytes):
// AtmosThemes[3] +0, AtmosBlends[2] +3, SoundTheme schar +5, Music +6 (retail stores +0..+5 at 0x0082239A..0x0082241D, *7 stride).
// The ctor ??0CGameMap@@AAE@AAVCWorld@@AAVCMap@@ABVCGameMapInit@@@Z and Clear / Update / AreaChanged are private in FableWin.pdb:
// CMap is a friend. Inline bodies: FableWin.pdb debug sizes (fwsize, chunk 2055 scout/fwsize.txt).
#include <stddef.h>
#include "rebuild_abi.h"
#include "stlport/fable_containers.h"
#include "engine/CBaseClassNonCopyable.h"
#include "engine/C2DCoordI.h"
class CWorld;class CMap;
class CGameMapCell {
public:
    // FableWin.pdb ?SetAtmosTheme@CGameMapCell@@QAEXJE@Z (11i/24b), ?SetAtmosBlend@CGameMapCell@@QAEXJE@Z (11i/25b),
    // ?SetSoundTheme@CGameMapCell@@QAEXC@Z (10i/22b): plain /Od of these texts gives exactly those sizes (fwsize SAME).
    void SetAtmosTheme(long index, unsigned char theme) { AtmosThemes[index] = theme; }
    void SetAtmosBlend(long index, unsigned char blend) { AtmosBlends[index] = blend; }
    void SetSoundTheme(signed char sound_theme) { SoundTheme = sound_theme; }
    unsigned char AtmosThemes[3];                                                  // +0x00
    unsigned char AtmosBlends[2];                                                  // +0x03
    signed char   SoundTheme;                                                      // +0x05
    unsigned char Music;                                                           // +0x06
};
class CGameMap : public CBaseClassNonCopyable {
    friend class CMap;
public:
    virtual ~CGameMap();                                                           // slot 0 (??_E 0x006B81A0)
    // FableWin.pdb ?GetSizeX@CGameMap@@QBEJXZ / ?GetSizeY@CGameMap@@QBEJXZ (9i/17b each = `return SizeX;`, fwsize SAME).
    long GetSizeX() const { return SizeX; }
    long GetSizeY() const { return SizeY; }
    // FableWin.pdb ?GetCell@CGameMap@@QAEAAVCGameMapCell@@ABVC2DCoordI@@@Z (92i debug = asserts; size proves nothing). Release index from
    // retail 0x008220DC..0x008220F5: `(SizeY - pos.Y - 1) * SizeX + pos.X`, *7 into Cells (+0x04). Text otherwise UNVERIFIED.
    CGameMapCell& GetCell(const C2DCoordI& pos) { return Cells[(SizeY - pos.Y - 1) * SizeX + pos.X]; }
private:
    std::vector<CGameMapCell> Cells;                                               // +0x04
    CMap*                     Map;                                                 // +0x10
    CWorld*                   World;                                               // +0x14
    unsigned long*            WorldSeed;                                           // +0x18
    long                      SizeX;                                               // +0x1C
    long                      SizeY;                                               // +0x20
};
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(sizeof(CGameMapCell) == 7);
FABLE_STATIC_ASSERT(sizeof(CGameMap) == 0x24);
#endif
#else
class CGameMap;
#endif // FABLE_RETAIL_CGAMEMAP
#endif // FABLE_ENGINE_CGAMEMAP_H
