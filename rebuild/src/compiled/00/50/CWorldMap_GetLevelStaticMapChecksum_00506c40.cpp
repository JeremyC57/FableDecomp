// CWorldMap::GetLevelStaticMapChecksum(const CCharString&) const -- open the level file from disk, read its level record and map header, and return the map's static checksum
// Leaf: GetLevelStaticMapChecksum
// Retail: Fable.exe (TLC, VC7.1) @ 0x00506C40, 249 bytes (256 with padding), `ret 4`.
//   PDB ?GetLevelStaticMapChecksum@CWorldMap@@QBEKABVCCharString@@@Z (Ego_r.pdb 0x004D47D0, FableWin.pdb 0x01C8C7D0: const).
// Callees: operator new 0x00BFEA1A (x3), CCharString::ToWideString 0x0099E480, CDiskFileWin32::CDiskFileWin32(const
//   CWideString&, FileAccessType, unsigned long) 0x0099AD80, ~CWideString 0x0099B510, CFileLargeThemePalette ctor
//   0x0050EE40 (x2), CAFile slot 5 SetPosition (x2), CReplaceSerialise<CFileFormatLevel>::ReadFromFile 0x0051A240,
//   CReplaceSerialise<CFileFormatMapHeader>::ReadFromFile 0x0051CBB0, operator delete 0x00BFE9BC (x2); the counted
//   block's deleter 0x0050BE40.
// Layout: CFileFormatLevel (25 bytes, packed) and CFileFormatMapHeader (0x10822 bytes, StaticMapChecksum +0x1081E)
//   from the landed CWorldMap::GetLevelDimensions(CAFile&) 0x00506BB0, whose body this one inlines.
// Shape: the file is a CCountedPointer<CAFile> built from the new-expression (the ToWideString argument temporary gets
//   the construction flag in ebx, its slot zeroed first; the inline ctor allocates the {1, deleter, object} block only
//   for a non-null object); the header is a scoped holder deleted after the checksum is read; the file block is
//   released last (`dec [ebp]`).
// Name fix (agentNF54): ported onto engine/CFileFormatBrush.h (FABLE_RETAIL_CFILEFORMATBRUSH + FABLE_RETAIL_CFILEFORMAT_MAPHEADER),
//   engine/CAFile.h, engine/CDiskFileWin32.h, engine/CCountedPointer.h, fable_string.h and engine/CWorldMap.h (my
//   FABLE_RETAIL_CWORLDMAP_LEVELHEADER opt-in). The header read now binds the PDB
//   ?ReadFromFile@?$CReplaceSerialise@VCFileFormatMapHeader@@VCOldFileFormatMapHeader8@@@@QAEXAAVCAFile@@PAVCASuspendableProcess@@@Z
//   (Ego_r.pdb 0x004E9260 + FableWin.pdb 0x01CA3CB0); the self row the const PDB spelling; the holder is
//   boost::scoped_ptr<CFileFormatMapHeader> (FableWin.pdb 0x01CA6EC0) and the deleter the counted pointer's DeleteData
//   (0x0050BE40, naming wave 1 CCountedPointer<CAFile>::DeleteData).
#define FABLE_RETAIL_SCOPED_PTR
#define FABLE_RETAIL_SCOPED_PTR_RESET
#define FABLE_RETAIL_CDISKFILEWIN32
#define FABLE_RETAIL_CWORLDMAP
#define FABLE_RETAIL_CWORLDMAP_LEVELHEADER
#define FABLE_RETAIL_CFILEFORMATBRUSH
#define FABLE_RETAIL_CFILEFORMAT_MAPHEADER
#include "fable_string.h"
#include "engine/CDiskFileWin32.h"
#include "engine/CFileFormatBrush.h"
#include "engine/CWorldMap.h"

unsigned long CWorldMap::GetLevelStaticMapChecksum(const CCharString& levelName) const
{
    CCountedPointer<CAFile> file(new CDiskFileWin32(levelName.ToWideString(), FILE_ACCESS_READ, 2));
    boost::scoped_ptr<CFileFormatMapHeader> header(new CFileFormatMapHeader);
    file->SetPosition(0);
    CFileFormatLevel level;
    level.ReadFromFile(*file, 0);
    file->SetPosition(level.MapDataOffset);
    header->ReadFromFile(*file, 0);
    return header->StaticMapChecksum;
}
