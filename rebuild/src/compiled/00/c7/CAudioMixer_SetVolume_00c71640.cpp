// CAudioMixer::SetVolume(float level) -- scale the bound wave-out volume control's [minimum, maximum] range by level and write it through mixerSetControlDetails
// Leaf: ?SetVolume@CAudioMixer@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C71640, 122 bytes (+ int3 padding), __fastcall `ret 4`.
// Identity: the CAudioMixer of the landed EnumerateAndOpenDevice 0x00C71430 / CloseAndCleanup 0x00C715D0 (no PDB class:
//   descriptive names): HMIXER +0, the volume MIXERCONTROLA +4 (dwControlID +8, Bounds.dwMinimum +0x68, Bounds.dwMaximum
//   +0x6C). Only caller 0x00C56C89 calls it with 1.0f right after EnumerateAndOpenDevice succeeds. The catalog's
//   "make_pair<CSkeletalMorphs::CEntry, float&>" label is propagated.
// Callees: _ftol @ 0x00BFEA70 (the float -> DWORD conversion), winmm mixerSetControlDetails (IAT 0x014404C8) with
//   MIXER_OBJECTF_HMIXER | MIXER_SETCONTROLDETAILSF_VALUE (0x80000000).
// Shape: the unsigned span is converted with VC's fild + 2^32 fixup; the MIXERCONTROLDETAILS_UNSIGNED value is homed in the
//   dead `level` argument slot; the control-id store is scheduled ahead of the pushes (as in CloseAndCleanup).
typedef unsigned long DWORD_;
struct MIXERCONTROLA_ {                          // 0x94
    DWORD_ cbStruct, dwControlID, dwControlType, fdwControl, cMultipleItems;
    char szShortName[16];
    char szName[64];
    DWORD_ Bounds[6];                            // +0x60: dwMinimum, dwMaximum, ...
    DWORD_ Metrics[6];
};
struct MIXERCONTROLDETAILS_ {                    // 0x18
    DWORD_ cbStruct, dwControlID, cChannels, cMultipleItems, cbDetails;
    void* paDetails;
};
extern "C" __declspec(dllimport) unsigned int __stdcall mixerSetControlDetails(void* mixer, MIXERCONTROLDETAILS_* details, DWORD_ flags);

struct CAudioMixer {
    void*          Mixer;                        // +0x00 HMIXER
    MIXERCONTROLA_ VolumeControl;                // +0x04
    DWORD_         VolumeValue;                  // +0x98 MIXERCONTROLDETAILS_UNSIGNED
    void SetVolume(float level);
};

void CAudioMixer::SetVolume(float level)
{
    DWORD_ minimum = VolumeControl.Bounds[0];
    DWORD_ value = minimum + (DWORD_)((float)(VolumeControl.Bounds[1] - minimum) * level);
    MIXERCONTROLDETAILS_ details;
    details.cbStruct = sizeof(details);
    details.dwControlID = VolumeControl.dwControlID;
    details.cChannels = 1;
    details.cMultipleItems = 0;
    details.cbDetails = sizeof(DWORD_);
    details.paDetails = &value;
    mixerSetControlDetails(Mixer, &details, 0x80000000);
}