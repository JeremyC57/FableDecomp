# agentSND2 header overlay

Grade every row in this hand-back with `FABLE_HDR_OVERLAY=$A/hdr` (`$A = $S/w22/agentSND2`). The three files are copies of
the HEAD headers in `rebuild/include/engine/`. Every change sits behind a new opt-in, and with none of the new opt-ins
defined each header preprocesses to the HEAD text.

## engine/CASoundManager.h

- **Forward declarations.** `class CLipSyncDataBank;` is under BANKLOOKUP and `class C2DCoordI;` is under ATMOSMAP.
- **`FABLE_RETAIL_CASOUNDMANAGER_SIZE40`**
  - The base is 0x40 bytes: an opaque pad from +0x04 to +0x40, then `sizeof == 0x40`.
  - Evidence:
    - The base ctor 0x00991910 writes its last member at +0x3D.
    - The CSoundManagerLH ctor 0x00A3EF00 assigns +0x40, +0x44 and +0x48.
    - Get2DEmitter 0x00A3E3A0 returns +0x44.
    - The landed LH dtor 0x00A3EDE0 uses the same base size.
  - Used by the CSoundEmitterLH self rows, which derive a `CSoundManagerLH` whose members start at +0x40.
- **`FABLE_RETAIL_CASOUNDMANAGER_BANKLOOKUP`**
  - Declares the non-virtual bank lookups:
    - `GetPSoundBank(const CLipSyncDataBank&) const` 0x00991690, a descriptive name.
    - `GetPLipSyncBank` 0x009916F0.
    - `GetAnyPLipSyncBank` 0x00991720.
    - `UnloadAllBanks` 0x00991750.
    - `GetPSoundBank(const CCharString&) const` 0x009917E0.
    - `GetPSoundBank(long) const` 0x00991840. This one is declared only when BANKREGISTRATION is not defined, because that opt-in already declares it.
    - `CloseAllBanks` 0x009918F0.
  - Names come from the Fable Anniversary 360 PDB publics of lib_sound_manager_base.cpp (same TU order), with the TLC bodies as the second source.
- **`FABLE_RETAIL_CASOUNDMANAGER_ATMOSMAP`**
  - Replaces the forward declaration with the nested abstract `class CAtmosMap`, with two pure virtuals:
    - `long GetBlockValue(const C2DCoordI&)`.
    - `float GetBlockHeight(const C2DCoordI&)`.
  - Evidence: Fable.exe RTTI `.?AVCAtmosMap@CASoundManager@@` is the base of `.?AVCMyAtmosMap@CAtmosProcess@@` (vtable 0x0125D47C, two slots).
  - The slot names are descriptive, taken from the landed overrides 0x006B1580 and 0x006B15C0.
  - Used by 0x006B1960 and 0x006B1550.
- **`FABLE_RETAIL_CASOUNDMANAGER_ENABLEFLAGS`**
  - Adds a protected pad, then the public `bool SoundEnabled` (+0x24), `MusicEnabled` (+0x25) and `AtmosEnabled` (+0x26), then a private pad to 0x4C, or to 0x40 when SIZE40 is defined.
  - Evidence: the base ctor sets the three flags, and EnableSound / EnableMusic / EnableAtmos store them.
  - The names are descriptive, after the setters.
  - Used by 0x006BD8A0 (CMusicManager reads +0x25).

## engine/CThingSoundEmitter.h

- **`FABLE_RETAIL_CTHINGSOUNDEMITTER_SOUNDEMITTER`**
  - Adds `CASoundEmitter* PSoundEmitter` at +0x98. The name is descriptive; it is the object OnCreate gets from `CASoundManager::Create3DEmitter`.
  - Declares the Ego_r.pdb members: PlaySample, PlaySoundEvent, IsSamplePlaying, IsSampleLoadedAndPlaying, StopSpecificSample, ReleaseLoop, SetSpecificSamplePitch / SetSpecificSampleVolume, `protected virtual bool OnCreate()` and `virtual ~CThingSoundEmitter()`.
- **`FABLE_RETAIL_CTHINGSOUNDEMITTER_MEMBERS`** (nested in SOUNDEMITTER)
  - Types the tail members as `CIntelligentPointer<const CThing> PAttachedThing` and `C3DVector Pos`, inside `pack(1)` as the layout requires.
  - Used by the 0x00836xxx rows.

## engine/CSoundInit.h

- **`FABLE_RETAIL_CSOUNDINIT_INLINE_CTOR`**
  - The ctor 0x0041C930 is inline. Init list: Volume 1, Pitch 1, Pan 0, NoPlays 1, PitchFaderBits 0, OverrideID -1. The body sets `PlaybackType = 1; GainFaderBits = 0`.
  - Retail expands it in the rows that use it: 0x00629DE0, 0x00629F50, 0x009BD910.
