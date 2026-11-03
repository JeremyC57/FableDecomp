#if defined(FABLE_RETAIL_MAP_BRUSHES) && !defined(FABLE_ENGINE_CFILEFORMATBRUSH_H)
#define FABLE_ENGINE_CFILEFORMATBRUSH_H
#include "engine/CAFile.h"
#include "engine/C2DBoxI.h"
#include "engine/C3DVector.h"
#include "engine/C3DAngles.h"
// Owner-approved serialized-record exception. Both PDBs retain these sizes
// and unaligned offsets; some records additionally carry the packed property.
// Native accesses confirm byte alignment, not the original directive spelling.
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
