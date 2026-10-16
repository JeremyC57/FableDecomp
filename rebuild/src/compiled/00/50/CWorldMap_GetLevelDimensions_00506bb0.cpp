// CWorldMap::GetLevelDimensions(CAFile&) -- reads a level file's map header and returns the world box (0,0,SizeX,SizeY)
// Retail 0x00506BB0, 0x8E bytes (CWorldMap.cpp TU). Allocates a CFileFormatMapHeader (PDB 67618 = 0x10822 bytes,
// packed: CReplaceSerialise base ByteLength +0 / SerialiseVersionNumber +4, WorldSizeX +13, WorldSizeY +17,
// LargeThemePalette +22, AtmosThemePalette +33822) with operator new 0x00BFEA1A; its constructor only runs the
// CFileLargeThemePalette constructor 0x0050EE40 on both palettes (manifest label CLayer::SetAllPolys is a
// propagation). The file is rewound (CAFile vtable slot 5, +0x14), a CFileFormatLevel (25 bytes) is read with
// CReplaceSerialise<CFileFormatLevel,...>::ReadFromFile 0x0051A240 (manifest names the EditMapBrushHeader twin),
// the file is moved to its MapDataOffset (+5) and the header read with 0x0051CBB0. The header is owned by a small
// scoped holder, so the result box is written into the caller's slot before the plain `operator delete`
// (0x00BFE9BC; the header has a trivial destructor). `this` is unused.
#include <stddef.h>

struct C2DBoxI
{
    long TLX;
    long TLY;
    long BRX;
    long BRY;
    C2DBoxI(long tlx, long tly, long brx, long bry) : TLX(tlx), TLY(tly), BRX(brx), BRY(bry) {}
};

class CASuspendableProcess;

class CAFile
{
public:
    virtual void Slot00();
    virtual void Slot04();
    virtual void Slot08();
    virtual void Slot0C();
    virtual void Slot10();
    virtual void Seek(long position);                  // slot 5 (+0x14)
};

struct CFileLargeThemePalette
{
    unsigned char Data[0x8400];                        // 33792 bytes
    CFileLargeThemePalette();                          // 0x0050EE40
};

#pragma pack(push, 1)
struct CFileFormatLevel
{
    unsigned long ByteLength;                          // +0x00 CReplaceSerialise
    unsigned char SerialiseVersionNumber;              // +0x04
    long          MapDataOffset;                       // +0x05
    long          ThingDataOffset;                     // +0x09
    long          BrushDataOffset;                     // +0x0D
    long          NavigationDataOffset;                // +0x11
    long          NavigationSectionsOffset;            // +0x15
    void ReadFromFile(CAFile* file, CASuspendableProcess* process);  // 0x0051A240
};

struct CFileFormatMapHeader
{
    unsigned long          ByteLength;                 // +0x00 CReplaceSerialise
    unsigned char          SerialiseVersionNumber;     // +0x04
    unsigned long          ThingUIDCount[2];           // +0x05 ulong64
    unsigned long          WorldSizeX;                 // +0x0D
    unsigned long          WorldSizeY;                 // +0x11
    bool                   UsingExtraMapCells;         // +0x15
    CFileLargeThemePalette LargeThemePalette;          // +0x16
    unsigned long          VersionNumber;              // +0x8416
    unsigned long          NoSoundThemeEntries;        // +0x841A
    CFileLargeThemePalette AtmosThemePalette;          // +0x841E
    unsigned long          StaticMapChecksum;          // +0x1081E
    void ReadFromFile(CAFile* file, CASuspendableProcess* process);  // 0x0051CBB0
};
#pragma pack(pop)

template <class T>
struct CScopedPtr
{
    T* ptr;
    CScopedPtr(T* p) : ptr(p) {}
    ~CScopedPtr() { delete ptr; }
    T* operator->() const { return ptr; }
};

class CWorldMap
{
public:
    C2DBoxI GetLevelDimensions(CAFile& file);
};

C2DBoxI CWorldMap::GetLevelDimensions(CAFile& file)
{
    CScopedPtr<CFileFormatMapHeader> header(new CFileFormatMapHeader);
    file.Seek(0);
    CFileFormatLevel level;
    level.ReadFromFile(&file, 0);
    file.Seek(level.MapDataOffset);
    header->ReadFromFile(&file, 0);
    return C2DBoxI(0, 0, header->WorldSizeX, header->WorldSizeY);
}