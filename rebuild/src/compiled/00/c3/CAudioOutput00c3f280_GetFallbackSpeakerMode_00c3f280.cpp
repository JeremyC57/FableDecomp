// CAudioOutput_00c3f280::GetFallbackSpeakerMode(long mode) -- next speaker mode to try when `mode` cannot be opened: 1, 2 and 4 fall back to 0 (give up), 8 to 4, 64 to 8, 128 to 64, anything else to 128
// Leaf: ?GetFallbackSpeakerMode@CAudioOutput_00c3f280@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C3F280, 212 bytes = 0x40 code + a 5-entry dword jump table + a 128-entry byte
//   index table (row padding after it), __fastcall `ret 4` (`this` unused).
// Library: libpk audio output (no PDB/RTTI); descriptive names. Single caller 0x00C3F710: `for (m = current; m; m =
//   GetFallbackSpeakerMode(m)) if (TryOpen(m)) return true;` (0x00C3F110 opens a mode). The values pair with
//   ApplySpeakerMode 0x00C3D710 (0 none, 8 quad, 64 5.1, 128 7.1). The manifest's "Render_QualityLevelAdjustment" is
//   propagated.
// Jump table (checked by hand against the retail bytes): index 0 -> `xor eax,eax` (cases 1, 2, 4), 1 -> 4 (case 8),
//   2 -> 8 (case 64), 3 -> 0x40 (case 128), 4 -> 0x80 (every other value in 1..128 and the `ja` default).
// Shape: a dense switch over 1..128 with a byte index table; each case its own `ret 4`.
struct CAudioOutput_00c3f280 {
    long GetFallbackSpeakerMode(long mode);
};

long CAudioOutput_00c3f280::GetFallbackSpeakerMode(long mode)
{
    switch (mode) {
    case 1:
    case 2:
    case 4:
        return 0;
    case 8:
        return 4;
    case 64:
        return 8;
    case 128:
        return 64;
    default:
        return 128;
    }
}