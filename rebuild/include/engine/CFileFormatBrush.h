#ifndef FABLE_ENGINE_CFILEFORMATBRUSH_H
#define FABLE_ENGINE_CFILEFORMATBRUSH_H
// (chunk 2055 overlay: one nested opt-in FABLE_RETAIL_CFILEFORMAT_MAPHEADER added at the end of the pack(1) scope; default text unchanged.)
// Retail serialized file records (header wave 2026-11-03, Codex pass 12 row 0x00821010 CMap::LoadBrushesFromFile): define
// FABLE_RETAIL_CFILEFORMATBRUSH. Packing (owner rule 2026-11-03; evidence rows/00821010/PACKING_EVIDENCE.md of the Codex pass 12
// handoff): Ego_r.pdb and FableWin.pdb both keep these records byte-aligned -- CReplaceSerialise / CBaseSerialise 5 bytes
// (ByteLength +0, SerialiseVersionNumber +4), CFileFormatLevel 25 (BrushDataOffset +13), CFileLargeThemePaletteEntry 132,
// CFileFormatBrushHeader 33801, CFileFormatBrush 73 (GameArea +5, ScriptName +21), CFileFormatBrushCell 14 (Height +5),
// CFileFormatBrushThingToMove 37 (UniqueID +5, Position +13), ...ThingToDelete / ...ThingToAdd 13 -- and retail 0x00821010
// addresses every record field at those unaligned displacements (all 2101 bytes graded RELOCATION_MATCH); helper 0x00826A00
// allocates the 73-byte CFileFormatBrush, 0x0051A240 the 25-byte CFileFormatLevel. The pack(1) scope covers exactly these
// records (the class's own header, owner convention); the static asserts below pin every size and the unaligned offsets.
#ifndef FABLE_RETAIL_CFILEFORMATBRUSH
#error engine/CFileFormatBrush.h holds the packed retail file records only: define FABLE_RETAIL_CFILEFORMATBRUSH
#endif
#include <stddef.h>
#ifdef FABLE_RETAIL_CFILEFORMAT_HEIGHTMAP_V5
#include "engine/CCountedPointer.h"
#endif
#include "engine/CAFile.h"
#include "engine/C2DBoxI.h"
#include "engine/C3DVector.h"
#include "engine/C3DAngles.h"
#pragma pack(push,1)
template<class T,unsigned short Version>class CBaseSerialise {public:
 unsigned long ByteLength;unsigned char SerialiseVersionNumber;
 CBaseSerialise():SerialiseVersionNumber(Version){}
#ifdef FABLE_RETAIL_CFILEFORMAT_HEIGHTMAP_V5
 static unsigned short FABLE_FASTCALL GetClassVersionNumber(){return Version;}
#endif
 bool ReadFromFile(CAFile&file,CASuspendableProcess*psuspendable_process){
  unsigned long length;file.ReadVar(length,psuspendable_process);
  long old_pos=file.GetPosition();unsigned char version;
  file.ReadVar(version,psuspendable_process);file.SetPosition(old_pos);
  ByteLength=length;file.Read((char*)this+sizeof(ByteLength),length-sizeof(ByteLength),psuspendable_process);
  return true;
 }
};
template<class T,class Old>class CReplaceSerialise {public:
 unsigned long ByteLength;unsigned char SerialiseVersionNumber;
 void ReadFromFile(CAFile&,CASuspendableProcess*);
#ifdef FABLE_RETAIL_CFILEFORMAT_HEIGHTMAP_V5
 static unsigned short FABLE_FASTCALL GetClassVersionNumber(){return (unsigned short)(Old::GetClassVersionNumber()+1);}
 static CCountedPointer<T> FABLE_FASTCALL ConvertPreviousUnknownVersionUp(void* old_void);
#endif
};
class COldFileFormatLevel3;class COldFileFormatBrush;
class CFileFormatLevel:public CReplaceSerialise<CFileFormatLevel,COldFileFormatLevel3>{public:
 long MapDataOffset,ThingDataOffset,BrushDataOffset,NavigationDataOffset,NavigationSectionsOffset;
};
class CFileLargeThemePaletteEntry {public:CFileLargeThemePaletteEntry();char ThemePaletteEntryName[128];long ThemePaletteThemeDefIndex;};
class CFileLargeThemePalette {CFileLargeThemePaletteEntry FileThemePaletteEntry[256];public:
 CFileLargeThemePalette();
 CFileLargeThemePaletteEntry&GetThemePaletteEntry(long index){return FileThemePaletteEntry[index];}
 const CFileLargeThemePaletteEntry&PeekThemePaletteEntry(long index)const{return FileThemePaletteEntry[index];}
};
class CFileFormatBrushHeader:public CBaseSerialise<CFileFormatBrushHeader,1>{public:unsigned long NoBrushes;CFileLargeThemePalette LargeThemePalette;};
class CFileFormatBrush:public CReplaceSerialise<CFileFormatBrush,COldFileFormatBrush>{public:
 C2DBoxI GameArea;char ScriptName[32];float CreationTimeSeconds;long CreationPattern;
 long NumberOfThingsToMove,NumberOfThingsToDelete,NumberOfThingsToAdd;
};
class CFileFormatBrushCell:public CBaseSerialise<CFileFormatBrushCell,1>{public:float Height;unsigned char Theme[3],ThemeBlend[2];};
class CFileFormatBrushThingToMove:public CBaseSerialise<CFileFormatBrushThingToMove,1>{public:unsigned __int64 UniqueID;C3DVector Position;C3DAngles FacingAngles;};
class CFileFormatBrushThingToDelete:public CBaseSerialise<CFileFormatBrushThingToDelete,1>{public:unsigned __int64 UniqueID;};
class CFileFormatBrushThingToAdd:public CBaseSerialise<CFileFormatBrushThingToAdd,1>{public:unsigned __int64 UniqueID;};
#ifdef FABLE_RETAIL_CFILEFORMAT_HEIGHTMAP_V5
// Retail opt-in (Codex 2026-11-10): both PDBs prove sizes9/10/15/17/25,
// Height+5, SoundLayer+17 and MinimapZone+21. These original file records
// reuse this header's existing byte-aligned serialization scope.
class COldFileFormatHeightMapCell1:public CBaseSerialise<COldFileFormatHeightMapCell1,1>{public:
 float Height;
};
class COldFileFormatHeightMapCell2:public CReplaceSerialise<COldFileFormatHeightMapCell2,COldFileFormatHeightMapCell1>{public:
 float Height;unsigned char TextureBlendAlpha;
};
class COldFileFormatHeightMapCell3:public CReplaceSerialise<COldFileFormatHeightMapCell3,COldFileFormatHeightMapCell2>{public:
 float Height;unsigned char TextureBlendAlpha,EngineTheme[3],EngineBlend[2];
};
class COldFileFormatHeightMapCell4:public CReplaceSerialise<COldFileFormatHeightMapCell4,COldFileFormatHeightMapCell3>{public:
 float Height;unsigned char TextureBlendAlpha,EngineTheme[3],EngineBlend[2],Passable,CameraPassable;
};
class COldFileFormatHeightMapCell5:public CReplaceSerialise<COldFileFormatHeightMapCell5,COldFileFormatHeightMapCell4>{public:
 float Height;unsigned char TextureBlendAlpha,EngineTheme[3],EngineBlend[2],Passable,CameraPassable;
 long SoundLayer,MinimapZone;
 void ConvertFromOld(const COldFileFormatHeightMapCell4* old_thing){
  long count=0;
  Height=old_thing->Height;TextureBlendAlpha=old_thing->TextureBlendAlpha;
  count=3;
  for(long c1=0;c1<count;++c1)EngineTheme[c1]=old_thing->EngineTheme[c1];
  count=2;
  for(long c1=0;c1<count;++c1)EngineBlend[c1]=old_thing->EngineBlend[c1];
  Passable=old_thing->Passable;CameraPassable=old_thing->CameraPassable;
  SoundLayer=MinimapZone=0;
 }
};
FABLE_STATIC_ASSERT(sizeof(COldFileFormatHeightMapCell5)==25);
FABLE_STATIC_ASSERT(offsetof(COldFileFormatHeightMapCell5,SoundLayer)==17);
#endif
#ifdef FABLE_RETAIL_CFILEFORMAT_MAPHEADER
// Nested opt-in (chunk 2055 scout, row 0x00821A80 CMap::LoadFromFile): the three map-file records that row reads, in this header's
// byte-aligned scope. Ego_r.pdb layouts (struct_layouts_egor.tsv): CFileFormatMapHeader 67618 = 0x10822 (retail `push 10822h` at
// 0x00821A9D; ThingUIDCount +5 / +9 read at 0x00821B48, UsingExtraMapCells +0x15 at 0x00821F95, LargeThemePalette +0x16 at 0x00821B54,
// VersionNumber +0x8416 at 0x00821B5D, NoSoundThemeEntries +0x841A at 0x00821DD5, AtmosThemePalette +0x841E (ctor 0x00821ABE; entries'
// ThemePaletteThemeDefIndex at +0x849E, 0x008221BB)); CFileFormatHeightMapCell 21 (fields read at [record+5..+0x14], 0x00821FE3..
// 0x00822098; the same layout as the landed 0x008275B0 model); CFileFormatGameMapCell 11 (AtmosThemes +5, AtmosBlends +8,
// SoundThemePaletteIndex +10: 0x0082238F..0x008223F9). Base templates: FableWin/Ego_r ?ReadFromFile@?$CReplaceSerialise@VCFileFormatMapHeader@@
// VCOldFileFormatMapHeader8@@@@..(0x0051CBB0), ..VCFileFormatHeightMapCell@@VCOldFileFormatHeightMapCell6@@..(0x008275B0, same TU),
// ?ReadFromFile@?$CBaseSerialise@VCFileFormatGameMapCell@@$00@@QAE_NAAVCAFile@@PAVCASuspendableProcess@@@Z (inline in retail).
// No user constructors: FableWin's ??0CFileFormatMapHeader@@QAE@XZ (14i/39b) and ??0CFileFormatGameMapCell@@QAE@XZ (10i/22b) equal the
// implicit ones at plain /Od (fwsize SAME, scout/fwsize.txt); retail expands the map header's (two out-of-line
// ??0CFileLargeThemePalette@@QAE@XZ 0x0050EE40 calls at +0x16 / +0x841E, 0x00821AB6..0x00821AC4) and the game-map cell's (version byte 1,
// 0x00822223).
class COldFileFormatMapHeader8;class COldFileFormatHeightMapCell6;
class CFileFormatMapHeader:public CReplaceSerialise<CFileFormatMapHeader,COldFileFormatMapHeader8>{public:
 unsigned __int64 ThingUIDCount;unsigned long WorldSizeX,WorldSizeY;bool UsingExtraMapCells;
 CFileLargeThemePalette LargeThemePalette;unsigned long VersionNumber,NoSoundThemeEntries;CFileLargeThemePalette AtmosThemePalette;
 unsigned long StaticMapChecksum;
};
class CFileFormatHeightMapCell:public CReplaceSerialise<CFileFormatHeightMapCell,COldFileFormatHeightMapCell6>{public:
 float Height;unsigned char TextureBlendAlpha,EngineTheme[3],EngineBlend[2],Passable,CameraPassable;
 signed char SoundLayerPaletteIndex,MinimapZone;bool ShorePoint,NavPreferred;
};
class CFileFormatGameMapCell:public CBaseSerialise<CFileFormatGameMapCell,1>{public:
 unsigned char AtmosThemes[3],AtmosBlends[2];signed char SoundThemePaletteIndex;
};
#endif
#pragma pack(pop)
#ifdef FABLE_RETAIL_CFILEFORMAT_MAPHEADER
FABLE_STATIC_ASSERT(sizeof(CFileFormatMapHeader)==0x10822);
FABLE_STATIC_ASSERT(offsetof(CFileFormatMapHeader,ThingUIDCount)==5);
FABLE_STATIC_ASSERT(offsetof(CFileFormatMapHeader,UsingExtraMapCells)==0x15);
FABLE_STATIC_ASSERT(offsetof(CFileFormatMapHeader,LargeThemePalette)==0x16);
FABLE_STATIC_ASSERT(offsetof(CFileFormatMapHeader,VersionNumber)==0x8416);
FABLE_STATIC_ASSERT(offsetof(CFileFormatMapHeader,NoSoundThemeEntries)==0x841A);
FABLE_STATIC_ASSERT(offsetof(CFileFormatMapHeader,AtmosThemePalette)==0x841E);
FABLE_STATIC_ASSERT(sizeof(CFileFormatHeightMapCell)==21);
FABLE_STATIC_ASSERT(offsetof(CFileFormatHeightMapCell,SoundLayerPaletteIndex)==17);
FABLE_STATIC_ASSERT(offsetof(CFileFormatHeightMapCell,NavPreferred)==20);
FABLE_STATIC_ASSERT(sizeof(CFileFormatGameMapCell)==11);
FABLE_STATIC_ASSERT(offsetof(CFileFormatGameMapCell,SoundThemePaletteIndex)==10);
#endif
FABLE_STATIC_ASSERT(sizeof(CFileFormatLevel)==25);
FABLE_STATIC_ASSERT(sizeof(CFileLargeThemePaletteEntry)==132);
FABLE_STATIC_ASSERT(sizeof(CFileLargeThemePalette)==33792);
FABLE_STATIC_ASSERT(sizeof(CFileFormatBrushHeader)==33801);
FABLE_STATIC_ASSERT(sizeof(CFileFormatBrush)==73);
FABLE_STATIC_ASSERT(sizeof(CFileFormatBrushCell)==14);
FABLE_STATIC_ASSERT(sizeof(CFileFormatBrushThingToMove)==37);
FABLE_STATIC_ASSERT(sizeof(CFileFormatBrushThingToDelete)==13);
FABLE_STATIC_ASSERT(sizeof(CFileFormatBrushThingToAdd)==13);
FABLE_STATIC_ASSERT(offsetof(CFileFormatLevel,BrushDataOffset)==13);
FABLE_STATIC_ASSERT(offsetof(CFileFormatBrush,GameArea)==5);
FABLE_STATIC_ASSERT(offsetof(CFileFormatBrush,ScriptName)==21);
FABLE_STATIC_ASSERT(offsetof(CFileFormatBrushCell,Height)==5);
FABLE_STATIC_ASSERT(offsetof(CFileFormatBrushThingToMove,UniqueID)==5);
FABLE_STATIC_ASSERT(offsetof(CFileFormatBrushThingToMove,Position)==13);
#endif
