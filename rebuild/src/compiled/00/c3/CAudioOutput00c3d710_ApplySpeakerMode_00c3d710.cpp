// CAudioOutput_00c3d710::ApplySpeakerMode(long mode) -- publish the channel count and channel mask of a speaker mode in the library globals 0x0139CA04 / 0x0139CA08: 0 -> 0/0, 8 -> 4/0x33 (quad), 64, 0x10000 and 0x20000 -> 6/0x3f (5.1), 128 -> 8/0xff (7.1), anything else -> 2/3 (stereo)
// Leaf: ?ApplySpeakerMode@CAudioOutput_00c3d710@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C3D710, 148 bytes (+ int3 padding), __fastcall `ret 4` (`this` unused).
// Library: libpk audio output (no PDB/RTTI); descriptive names. Single caller 0x00C3DA1D (after a successful device
//   open: `ApplySpeakerMode(mode); m_flags |= 0x8E02;`). GetFallbackSpeakerMode 0x00C3F280 walks the same modes. The
//   manifest's "Render_MapQualityLevelToSettings" is propagated.
// Shape: a sparse switch lowered to a compare tree around 0x80 (`jg` upper half, `je` the pivot); case 0 shares the
//   `xor ecx,ecx` compare register with its two stores; every case its own `ret 4`; bodies in source order.
extern long g_speakerChannels_0139ca04;         // 0x0139CA04
extern long g_speakerMask_0139ca08;             // 0x0139CA08

struct CAudioOutput_00c3d710 {
    void ApplySpeakerMode(long mode);
};

void CAudioOutput_00c3d710::ApplySpeakerMode(long mode)
{
    switch (mode) {
    case 8:
        g_speakerChannels_0139ca04 = 4;
        g_speakerMask_0139ca08 = 0x33;
        break;
    case 0:
        g_speakerChannels_0139ca04 = 0;
        g_speakerMask_0139ca08 = 0;
        break;
    case 0x80:
        g_speakerChannels_0139ca04 = 8;
        g_speakerMask_0139ca08 = 0xff;
        break;
    default:
        g_speakerChannels_0139ca04 = 2;
        g_speakerMask_0139ca08 = 3;
        break;
    case 0x40:
    case 0x10000:
    case 0x20000:
        g_speakerChannels_0139ca04 = 6;
        g_speakerMask_0139ca08 = 0x3f;
        break;
    }
}