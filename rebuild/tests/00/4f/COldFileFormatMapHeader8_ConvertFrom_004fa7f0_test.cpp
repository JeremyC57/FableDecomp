// Behaviour test for 0x004FA7F0: v7 -> v8 map header upgrade copies every v7 field and names the atmosphere palette UNINITIALISED.
#include <cstdio>
#include <cstring>
#pragma pack(push, 1)
struct Entry { char ThemePaletteEntryName[128]; int ThemePaletteThemeDefIndex; };
struct Palette { Entry FileThemePaletteEntry[256]; };
struct H7 { unsigned int ByteLength; unsigned char Ver; unsigned long long ThingUIDCount; unsigned int WorldSizeX, WorldSizeY; bool UsingExtraMapCells; Palette LargeThemePalette; unsigned int VersionNumber, NoSoundThemeEntries; };
struct H8 { unsigned int ByteLength; unsigned char Ver; unsigned long long ThingUIDCount; unsigned int WorldSizeX, WorldSizeY; bool UsingExtraMapCells; Palette LargeThemePalette; unsigned int VersionNumber, NoSoundThemeEntries; Palette AtmosThemePalette;
  void ConvertFrom(const H7& o) { ThingUIDCount = o.ThingUIDCount; WorldSizeX = o.WorldSizeX; WorldSizeY = o.WorldSizeY; UsingExtraMapCells = o.UsingExtraMapCells;
    LargeThemePalette = o.LargeThemePalette; VersionNumber = o.VersionNumber; NoSoundThemeEntries = o.NoSoundThemeEntries;
    std::sprintf(AtmosThemePalette.FileThemePaletteEntry[0].ThemePaletteEntryName, "UNINITIALISED"); } };
#pragma pack(pop)
static H7 a; static H8 b;
int main()
{
    a.ThingUIDCount = 0x100000002ull; a.WorldSizeX = 3; a.WorldSizeY = 4; a.UsingExtraMapCells = true; std::strcpy(a.LargeThemePalette.FileThemePaletteEntry[255].ThemePaletteEntryName, "Last");
    a.LargeThemePalette.FileThemePaletteEntry[7].ThemePaletteThemeDefIndex = 9; a.VersionNumber = 7; a.NoSoundThemeEntries = 11; b.ByteLength = 77;
    b.ConvertFrom(a);
    bool ok = sizeof(H7) == 33822 && sizeof(H8) == 67614 && b.ThingUIDCount == 0x100000002ull && b.WorldSizeX == 3 && b.WorldSizeY == 4 && b.UsingExtraMapCells
           && std::strcmp(b.LargeThemePalette.FileThemePaletteEntry[255].ThemePaletteEntryName, "Last") == 0 && b.LargeThemePalette.FileThemePaletteEntry[7].ThemePaletteThemeDefIndex == 9
           && b.VersionNumber == 7 && b.NoSoundThemeEntries == 11 && b.ByteLength == 77 && std::strcmp(b.AtmosThemePalette.FileThemePaletteEntry[0].ThemePaletteEntryName, "UNINITIALISED") == 0;
    if (!ok) { std::printf("004fa7f0_TEST FAIL\n"); return 1; }
    std::printf("004fa7f0_TEST PASS\n");
    return 0;
}