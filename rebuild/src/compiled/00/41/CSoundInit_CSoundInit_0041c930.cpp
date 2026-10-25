// CSoundInit::CSoundInit() -- default sound-start parameters: full volume and pitch, centred pan, one play, playback type
// 1, no fader bits, no override id
// Leaf: ??0CSoundInit@@QAE@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x0041C930, 45 bytes, thiscall `ret` (no frame, `mov eax,ecx`, zero in ecx, one in edx).
// Identity (FINDINGS chunk 384; was the address-named SSettings_0041c930 record although the manifest label is
//   CSoundInit::CSoundInit): Ego_r.pdb `??0CSoundInit@@QAE@XZ` (name set) and struct_layouts_egor CSoundInit is exactly
//   this 0x20-byte record -- Volume +0, Pitch +4, Pan +8 (floats), NoPlays +0xC, PlaybackType +0x10, GainFaderBits +0x14,
//   PitchFaderBits +0x18, OverrideID +0x1C; its 23 callers are the sound-start paths (CASound play helpers, CTCVillage
//   crowd sounds, CTCShop::SetAlarm, ...) that pass the record as `const CSoundInit&` to PlayAttachedSample /
//   PlaySoundEvent; the landed CTCSoundPlayer::InternalPlaySoundEvent 0x0066B120 models the same defaults (1, 1, 0, 1,
//   type 1, 0, 0, -1).
// Shape: stores +0..+0xC and +0x14..+0x1C in declaration order, then +0x10: PlaybackType is assigned in the constructor
//   body (scheduled after the initialiser list); the float zero of Pan shares the integer zero register.
enum ESoundPlaybackType { SOUND_PLAYBACK_1 = 1 };   // Ego_r.pdb enum (only the value used here is named)

class CSoundInit {
public:
    float              Volume;                  // +0x00
    float              Pitch;                   // +0x04
    float              Pan;                     // +0x08
    long               NoPlays;                 // +0x0C
    ESoundPlaybackType PlaybackType;            // +0x10
    unsigned long      GainFaderBits;           // +0x14
    unsigned long      PitchFaderBits;          // +0x18
    long               OverrideID;              // +0x1C
    CSoundInit();
};

CSoundInit::CSoundInit()
    : Volume(1.0f), Pitch(1.0f), Pan(0.0f), NoPlays(1), GainFaderBits(0), PitchFaderBits(0), OverrideID(-1)
{
    PlaybackType = SOUND_PLAYBACK_1;
}
