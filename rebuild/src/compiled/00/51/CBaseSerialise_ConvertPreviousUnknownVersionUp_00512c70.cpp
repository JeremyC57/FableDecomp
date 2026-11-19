// CBaseSerialise<COldFileFormatMapHeader1,1>::ConvertPreviousUnknownVersionUp(const void* old_void) -- the first step of the map-header
//   version chain: there is no older version to convert from, so a fresh version-1 header (SerialiseVersionNumber 1) in a new counted pointer
// Leaf: ?ConvertPreviousUnknownVersionUp@?$CBaseSerialise@VCOldFileFormatMapHeader1@@$00@@SI?AV?$CCountedPointer@VCOldFileFormatMapHeader1@@@@PBX@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00512C70, 107 bytes (+ int3 padding); static __fastcall: hidden result in ecx, old_void in edx
//   (unused), plain `ret`.
// Identity (NF93 q11430): Ego_r.pdb 0x004E1220 / FableWin.pdb 0x01CC8A90 ?ConvertPreviousUnknownVersionUp@?$CBaseSerialise@VCOldFileFormatMapHeader1@@$00@@
//   SI?AV?$CCountedPointer@VCOldFileFormatMapHeader1@@@@PBX@Z; FableWin's v1 -> v2 step (0x01CC85A4) calls it where retail 0x005148D0 calls
//   0x00512C70, and the landed 0x005148D0 binds this spelling (engine/CFileFormatBrush.h OLDMAPHEADER12). Was the made-up
//   CreateCountedFlagRecord_00512c70 over local SFlagRecord / CCountedPointer / count-block models (its "flag byte" is the version, 1).
// Callees: operator new 0x00BFEA1A (the 5-byte header, then the 12-byte count block), CCountedPointer<COldFileFormatMapHeader1>::DeleteData
//   0x0050D230 (stored in the count block), operator delete 0x00BFE9BC.
// Shape: `CCountedPointer<T> new_thing(new T); return new_thing;` -- VC7.1 has no NRVO here, so the named local is copied into the hidden
//   slot (add-reference) and then destroyed (release).
#define FABLE_RETAIL_CFILEFORMATBRUSH
#define FABLE_RETAIL_CFILEFORMAT_HEIGHTMAP_V5
#define FABLE_RETAIL_CFILEFORMAT_OLDMAPHEADER12
#include "engine/CFileFormatBrush.h"
template<class T,unsigned short Version>
CCountedPointer<T> FABLE_FASTCALL CBaseSerialise<T,Version>::ConvertPreviousUnknownVersionUp(const void* old_void)
{
 CCountedPointer<T> new_thing(new T);
 return new_thing;
}
template CCountedPointer<COldFileFormatMapHeader1> FABLE_FASTCALL
CBaseSerialise<COldFileFormatMapHeader1,1>::ConvertPreviousUnknownVersionUp(const void*);
