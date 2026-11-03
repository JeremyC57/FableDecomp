#ifndef FABLE_ENGINE_CFILEFORMATBRUSH_H
#define FABLE_ENGINE_CFILEFORMATBRUSH_H
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
#include "engine/CAFile.h"
#include "engine/C2DBoxI.h"
#include "engine/C3DVector.h"
#include "engine/C3DAngles.h"
#pragma pack(push,1)
template<class T,unsigned short Version>class CBaseSerialise {public:
 unsigned long ByteLength;unsigned char SerialiseVersionNumber;
 CBaseSerialise():SerialiseVersionNumber(Version){}
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
#pragma pack(pop)
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
