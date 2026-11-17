// CWorldMap::GetLevelDimensions(CAFile&) const -- reads a level file's map header and returns the world box (0,0,SizeX,SizeY)
// Retail 0x00506BB0, 0x8E bytes (CWorldMap.cpp TU). Allocates a CFileFormatMapHeader (PDB 67618 = 0x10822 bytes,
// packed: CReplaceSerialise base ByteLength +0 / SerialiseVersionNumber +4, WorldSizeX +13, WorldSizeY +17,
// LargeThemePalette +22, AtmosThemePalette +33822) with operator new 0x00BFEA1A; its constructor only runs the
// CFileLargeThemePalette constructor 0x0050EE40 on both palettes (manifest label CLayer::SetAllPolys is a
// propagation). The file is rewound (CAFile vtable slot 5, +0x14), a CFileFormatLevel (25 bytes) is read with
// CReplaceSerialise<CFileFormatLevel,...>::ReadFromFile 0x0051A240 (manifest names the EditMapBrushHeader twin),
// the file is moved to its MapDataOffset (+5) and the header read with 0x0051CBB0. The header is owned by a small
// scoped holder, so the result box is written into the caller's slot before the plain `operator delete`
// (0x00BFE9BC; the header has a trivial destructor). `this` is unused.
// Name fix (agentNF54): ported onto the shared records engine/CFileFormatBrush.h (FABLE_RETAIL_CFILEFORMATBRUSH +
//   FABLE_RETAIL_CFILEFORMAT_MAPHEADER: CFileFormatMapHeader is a CReplaceSerialise<CFileFormatMapHeader,COldFileFormatMapHeader8>),
//   engine/CAFile.h (slot 5 SetPosition) and engine/CWorldMap.h (my FABLE_RETAIL_CWORLDMAP_LEVELHEADER opt-in). The header read now binds
//   the PDB ?ReadFromFile@?$CReplaceSerialise@VCFileFormatMapHeader@@VCOldFileFormatMapHeader8@@@@QAEXAAVCAFile@@PAVCASuspendableProcess@@@Z
//   (Ego_r.pdb 0x004E9260 + FableWin.pdb 0x01CA3CB0; the local model bound a CFileFormatMapHeader member taking CAFile*), and the
//   self row the PDB ?GetLevelDimensions@CWorldMap@@IBE?AVC2DBoxI@@AAVCAFile@@@Z (Ego_r 0x004D4730, FableWin 0x01C8C5F0: protected
//   const). The scoped holder is boost::scoped_ptr<CFileFormatMapHeader> (FableWin.pdb ??1?$scoped_ptr@VCFileFormatMapHeader@@@boost@@QAE@XZ,
//   0x01CA6EC0; its checked_delete expanded here: FABLE_RETAIL_SCOPED_PTR_RESET), replacing the local CScopedPtr stand-in.
#define FABLE_RETAIL_SCOPED_PTR
#define FABLE_RETAIL_SCOPED_PTR_RESET
#define FABLE_RETAIL_CWORLDMAP
#define FABLE_RETAIL_CWORLDMAP_LEVELHEADER
#define FABLE_RETAIL_C2DBOXI_ORIGINAL_FOUR_COORD_CTOR
#define FABLE_RETAIL_C2DBOXI
#define FABLE_RETAIL_CFILEFORMATBRUSH
#define FABLE_RETAIL_CFILEFORMAT_MAPHEADER
#include "engine/CFileFormatBrush.h"
#include "engine/CWorldMap.h"

C2DBoxI CWorldMap::GetLevelDimensions(CAFile& file) const
{
    boost::scoped_ptr<CFileFormatMapHeader> header(new CFileFormatMapHeader);
    file.SetPosition(0);
    CFileFormatLevel level;
    level.ReadFromFile(file, 0);
    file.SetPosition(level.MapDataOffset);
    header->ReadFromFile(file, 0);
    return C2DBoxI(0, 0, header->WorldSizeX, header->WorldSizeY);
}
