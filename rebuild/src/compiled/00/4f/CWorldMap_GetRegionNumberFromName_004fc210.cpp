// CWorldMap::GetRegionNumberFromName(const CCharString&) const -- index of the region with this name, 0 when none
// Leaf: GetRegionNumberFromName
// Retail: Fable.exe @ 0x004FC210, 141 bytes. PDB ?GetRegionNumberFromName@CWorldMap@@QBEJABVCCharString@@@Z
// Callee: the __fastcall text compare 0x00411570 inside the inlined CCharString operator== (same block, null blocks,
// lengths, then compare == 0), as in the landed 0x00A14EE0.
// Retail CWorldMap (the engine header engine/CWorldMap.h is the PDB layout with 16-byte containers; retail STLport
// vectors / CArrays are 12 bytes and the map member is 12 bytes, so everything after +0x14 sits lower):
//   +0x20 Maps (CArray<CMapInfo>: start +0x20, finish +0x24; PDB +0x24), +0x2C Regions (vector<CRegion>, 0x58-byte
//   elements; PDB +0x34), +0x38 RegionTime, +0x44 PRegionGraph, +0x4C SoundThemesDisabled, +0x50 Granularity,
//   +0x54 GranularityShift (PDB +0x64), +0x58 MapPlacementPitch (PDB +0x68), +0x5C Area {TLX, TLY, BRX, BRY}
//   (PDB +0x6C), +0x6C MapPlacement (vector<unsigned short>, PDB +0x7C), +0x9C LoadedRegion (PDB +0xBC).
// CMapInfo (0x48 bytes, PDB = retail): +0x00 Area, +0x10 PMap, +0x18 LevelName, +0x1C LevelScriptName,
// +0x20 MapUID, +0x24 Used ... +0x40 IsSea.
// CRegion (retail 0x58 bytes; PDB 100 with 16-byte vectors): +0x00 Maps (vector<long>), +0x0C VisMaps, +0x18 Name
// (PDB +0x20), +0x1C DisplayName, +0x20 RegionDefName, ...
#include "fable_compiler.h"
template <class T> struct CRetailVector {
    T* Start;                                   // +0x00
    T* Finish;                                  // +0x04
    T* EndOfStorage;                            // +0x08
    unsigned long size() const { return Finish - Start; }
    const T& operator[](unsigned long i) const { return Start[i]; }
};
// Shape: region 0 is skipped; the name's data block is loaded once, the element name pointer strength-reduced
// (start + 0x70, step 0x58) while size() is recomputed every pass.
// NF52 (round 7): the name test is fable_string.h's inline CCharString::operator==(const CCharString&) (data pointers, null tests,
// then CBasicString<char>::operator==: StringLength and !Compare), binding the protected static ?Compare@?$CBasicString@D@@KIJPBD0@Z
// (0x00411570); its template body is visible (FABLE_RETAIL_CBASICSTRING_COMPARE_BODY) as the row's same-TU body was. Were local
// CCharStringData / CCharString models, the free operator== and the made-up CompareText body. The CRetailVector / CRegion / CWorldMap
// models are pre-existing.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CBASICSTRING_COMPARE_BODY
#include "fable_string.h"

class CRegion { public:
    unsigned char _maps[0x18];                  // +0x00 Maps, VisMaps
    CCharString   Name;                         // +0x18 (PDB +0x20)
    unsigned char _rest[0x3c];                  // +0x1C .. +0x58
};

class CWorldMap { public:
    unsigned char          _pad[0x2c];
    CRetailVector<CRegion> Regions;             // +0x2C
    long GetRegionNumberFromName(const CCharString& name) const;
};

long CWorldMap::GetRegionNumberFromName(const CCharString& name) const
{
    for (unsigned long i = 1; i < Regions.size(); ++i)
        if (Regions[i].Name == name)
            return i;
    return 0;
}