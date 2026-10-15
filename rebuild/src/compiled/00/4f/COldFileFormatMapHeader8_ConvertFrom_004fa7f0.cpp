// COldFileFormatMapHeader8::ConvertFrom(const COldFileFormatMapHeader7& old) -- upgrade a version-7 map header: copy every v7 field and mark the new atmosphere theme palette "UNINITIALISED"
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004FA7F0, length 99 bytes (`ret 4`, void: eax is reused as the
// destination and ends as sprintf's result, so this is a conversion member, not a constructor; the real name is
// unknown -- the CReplaceSerialise<H8,H7> upgrade step).
// Label correction: the catalog's "CRadialBlurEntry::CRadialBlurEntry" is propagated. The offsets are the PDB's
// packed COldFileFormatMapHeader7/8 (Ego_r.pdb): a 5-byte CReplaceSerialise<H8,H7> base {ByteLength,
// SerialiseVersionNumber}, ulong64 ThingUIDCount +5, WorldSizeX +0xd, WorldSizeY +0x11, bool UsingExtraMapCells
// +0x15, CFileLargeThemePalette LargeThemePalette +0x16 (256 x {char[128] name, long index} = 0x8400 bytes),
// VersionNumber +0x8416, NoSoundThemeEntries +0x841a, and in v8 the second palette AtmosThemePalette +0x841e.
// Its only caller (0x0051B9F9) builds the v8 header from a v7 one while loading an old map.
// Callees: sprintf @ 0x00BFEB10 (on the first atmosphere palette entry's name, string 0x012442B4 "UNINITIALISED").
// Shape: the headers are 1-byte packed in the PDB (#pragma pack(1) is the real layout); the palette copies
// implicitly as `rep movsd` 0x2100, the 64-bit count as two dwords; the base is left untouched.
// Leaf: ConvertFrom@COldFileFormatMapHeader8
extern "C" int __cdecl sprintf(char* buffer, const char* format, ...);   // 0x00BFEB10

#pragma pack(push, 1)
struct CFileLargeThemePaletteEntry {
    char ThemePaletteEntryName[128];                    // +0x00
    long ThemePaletteThemeDefIndex;                     // +0x80
};

struct CFileLargeThemePalette {
    CFileLargeThemePaletteEntry FileThemePaletteEntry[256];
};

template <class TNew, class TOld> struct CReplaceSerialise {
    unsigned long ByteLength;                           // +0x00
    unsigned char SerialiseVersionNumber;               // +0x04
};

struct COldFileFormatMapHeader6;
struct COldFileFormatMapHeader7 : CReplaceSerialise<COldFileFormatMapHeader7, COldFileFormatMapHeader6> {
    unsigned __int64       ThingUIDCount;               // +0x05
    unsigned long          WorldSizeX;                  // +0x0d
    unsigned long          WorldSizeY;                  // +0x11
    bool                   UsingExtraMapCells;          // +0x15
    CFileLargeThemePalette LargeThemePalette;           // +0x16
    unsigned long          VersionNumber;               // +0x8416
    unsigned long          NoSoundThemeEntries;         // +0x841a
};

struct COldFileFormatMapHeader8 : CReplaceSerialise<COldFileFormatMapHeader8, COldFileFormatMapHeader7> {
    unsigned __int64       ThingUIDCount;               // +0x05
    unsigned long          WorldSizeX;                  // +0x0d
    unsigned long          WorldSizeY;                  // +0x11
    bool                   UsingExtraMapCells;          // +0x15
    CFileLargeThemePalette LargeThemePalette;           // +0x16
    unsigned long          VersionNumber;               // +0x8416
    unsigned long          NoSoundThemeEntries;         // +0x841a
    CFileLargeThemePalette AtmosThemePalette;           // +0x841e
    void ConvertFrom(const COldFileFormatMapHeader7& old);
};
#pragma pack(pop)

typedef char COldFileFormatMapHeader7_size_check[sizeof(COldFileFormatMapHeader7) == 33822 ? 1 : -1];
typedef char COldFileFormatMapHeader8_size_check[sizeof(COldFileFormatMapHeader8) == 67614 ? 1 : -1];

void COldFileFormatMapHeader8::ConvertFrom(const COldFileFormatMapHeader7& old)
{
    ThingUIDCount = old.ThingUIDCount;
    WorldSizeX = old.WorldSizeX;
    WorldSizeY = old.WorldSizeY;
    UsingExtraMapCells = old.UsingExtraMapCells;
    LargeThemePalette = old.LargeThemePalette;
    VersionNumber = old.VersionNumber;
    NoSoundThemeEntries = old.NoSoundThemeEntries;
    sprintf(AtmosThemePalette.FileThemePaletteEntry[0].ThemePaletteEntryName, "UNINITIALISED");
}