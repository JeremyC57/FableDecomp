// CAudioMixer::CloseAndCleanup() -- when a mixer is open: restore the saved control value (if one was captured), close the mixer and forget the handle
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C715D0, 105 bytes (+7 int3), __fastcall, no arguments.
// Callees: winmm mixerSetControlDetails (IAT 0x014404C8), mixerClose (IAT 0x014404C4).
// Layout (no PDB class): the HMIXER at +0, the volume control id at +8, the "restore on close" flag at
// +0x28 and the saved MIXERCONTROLDETAILS_UNSIGNED value at +0x98.
// Shape: a 0x18-byte MIXERCONTROLDETAILS on the stack (cbStruct 0x18, one channel, no items, 4-byte
// details pointing at the saved value), passed with MIXER_OBJECTF_HMIXER | MIXER_SETCONTROLDETAILSF_VALUE
// (0x80000000); the handle loaded for the null test is reused for the first call and re-read for
// mixerClose. The control id store is scheduled ahead of the pushes.
typedef unsigned long DWORD;
typedef struct HMIXER__* HMIXER;
typedef struct HMIXEROBJ__* HMIXEROBJ;

struct MIXERCONTROLDETAILS {
    DWORD cbStruct;
    DWORD dwControlID;
    DWORD cChannels;
    DWORD cMultipleItems;
    DWORD cbDetails;
    void* paDetails;
};

#define MIXER_OBJECTF_HMIXER            0x80000000L
#define MIXER_SETCONTROLDETAILSF_VALUE  0x00000000L

extern "C" __declspec(dllimport) unsigned int __stdcall mixerSetControlDetails(HMIXEROBJ mixer, MIXERCONTROLDETAILS* details, DWORD flags);
extern "C" __declspec(dllimport) unsigned int __stdcall mixerClose(HMIXER mixer);

struct CAudioMixer {
    HMIXER m_hMixer;                       // +0x00
    DWORD m_04;                            // +0x04
    DWORD m_volumeControlID;               // +0x08
    unsigned char _pad_0x0c[0x1c];         // +0x0C
    bool m_restoreOnClose;                 // +0x28
    unsigned char _pad_0x29[0x6f];         // +0x29
    DWORD m_savedVolume;                   // +0x98 MIXERCONTROLDETAILS_UNSIGNED
    void CloseAndCleanup();
};

void CAudioMixer::CloseAndCleanup()
{
    if (m_hMixer) {
        if (m_restoreOnClose) {
            MIXERCONTROLDETAILS details;
            details.cbStruct = sizeof(details);
            details.dwControlID = m_volumeControlID;
            details.cChannels = 1;
            details.cMultipleItems = 0;
            details.cbDetails = sizeof(DWORD);
            details.paDetails = &m_savedVolume;
            mixerSetControlDetails((HMIXEROBJ)m_hMixer, &details, MIXER_OBJECTF_HMIXER | MIXER_SETCONTROLDETAILSF_VALUE);
        }
        mixerClose(m_hMixer);
        m_hMixer = 0;
    }
}