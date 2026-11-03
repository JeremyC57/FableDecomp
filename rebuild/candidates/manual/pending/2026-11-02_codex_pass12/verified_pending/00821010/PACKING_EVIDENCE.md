# Owner-approved serialization alignment exception

The owner explicitly approved this narrow exception: “as long as you can prove that retail did it the same way, that is fine. make sure to properly document it so claude knows when i hand it off to them”. This supersedes the general prohibition on a new packing directive only for the genuine serialized records used by this assigned row.

The new opt-in engine/CFileFormatBrush.h contains exactly one push-1/pop pair. Ordinary CMap, world, vector, string, C3DVector and C3DAngles declarations are included before that scope. No compiler-wide alignment switch is used. No preexisting default header branch is changed.

| Original serialized class | Bytes in both PDBs and native | Named field evidence |
|---|---:|---|
| CReplaceSerialise and CBaseSerialise bases | 5 | ByteLength +0; SerialiseVersionNumber +4 |
| CFileFormatLevel | 25 | MapDataOffset +5; ThingDataOffset +9; BrushDataOffset +13; NavigationDataOffset +17; NavigationSectionsOffset +21 |
| CFileLargeThemePaletteEntry | 132 | ThemePaletteEntryName[128] +0; ThemePaletteThemeDefIndex +128 |
| CFileLargeThemePalette | 33792 | 256 entries, 132-byte stride |
| CFileFormatBrushHeader | 33801 | NoBrushes +5; LargeThemePalette +9 |
| CFileFormatBrush | 73 | GameArea +5; ScriptName[32] +21; CreationTimeSeconds +53; CreationPattern +57; move/delete/add counts +61/+65/+69 |
| CFileFormatBrushCell | 14 | Height +5; Theme[3] +9; ThemeBlend[2] +12 |
| CFileFormatBrushThingToMove | 37 | UniqueID +5; Position +13; FacingAngles +25 |
| CFileFormatBrushThingToDelete | 13 | UniqueID +5 |
| CFileFormatBrushThingToAdd | 13 | UniqueID +5 |

Both raw original PDB type sets retain the identical unaligned offsets and sizes. Some records additionally carry the packed property bit (for example CFileFormatBrush has properties0x203); others, including CBaseSerialise properties0x202, do not. The complete sizes, inherited unaligned member positions and native accesses establish the byte-aligned layout independently of that per-record flag. Native 00821010 accesses these same fields directly and reads length-prefixed records through the original serialization templates; helper 00826a00 allocates 73 bytes and uses current brush version 2, while 0051a240 uses level version 4 and 25 bytes. The selected RTM target is exact across all 2101 bytes, including every stack/record displacement. Compile-time assertions check all record sizes and the critical unaligned offsets. Both test compilers also assert those sizes/offsets at runtime.

This establishes that retail uses byte-aligned serialized records. It does **not** establish the textual spelling of an unavailable original source directive. The scoped push/pop pair is the explicitly approved C++ reconstruction of that proven ABI. No field is replaced by invented byte-buffer accessors to evade the rule.
