// CAudioContext::SetDefaultSpeakerConfig() -- slot 3 of libpk's OpenAL context: reset the global speaker configuration words (3, 3, 0, 2 channels, mask 0, 1)
// Leaf: ?SetDefaultSpeakerConfig@CAudioContext@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C56060, 48 bytes, __fastcall, plain `ret` (`this` is not read).
// Identity: slot 3 of the CAudioContext vtable 0x012B8C18. The globals 0x0139CA04 / 0x0139CA08 / 0x0139CA1C are bound as
//   g_speakerChannels_0139ca04 / g_speakerMask_0139ca08 / g_SubSystemA1C by landed rows; 0x0139C994 / 0x0139C9AC /
//   0x0139CA00 are unbound and get address names. Descriptive (libpk, no PDB/RTTI).
// Shape: one `mov eax,3` for the two 3s, one `xor eax,eax` for the two zeros, immediates for 2 and 1.
extern long g_speakerWord_0139c994;             // 0x0139C994
extern long g_speakerWord_0139c9ac;             // 0x0139C9AC
extern long g_speakerWord_0139ca00;             // 0x0139CA00
extern long g_speakerChannels_0139ca04;         // 0x0139CA04
extern long g_speakerMask_0139ca08;             // 0x0139CA08
extern long g_SubSystemA1C;                     // 0x0139CA1C

struct CAudioContext {                          // vtable 0x012B8C18
    virtual void v00();
    virtual void v01();
    virtual void v02();
    virtual void SetDefaultSpeakerConfig();     // slot 3
};

void CAudioContext::SetDefaultSpeakerConfig()
{
    g_speakerWord_0139c994 = 3;
    g_speakerWord_0139c9ac = 3;
    g_speakerWord_0139ca00 = 0;
    g_speakerChannels_0139ca04 = 2;
    g_speakerMask_0139ca08 = 0;
    g_SubSystemA1C = 1;
}