// Grade: DIFFER(170v173) -- constant/zero scheduling: retail keeps 0.04f as two immediates (+0x30, +0x44) and stores the shared edx zero in address order (+8/+0xc after +4, +0x40/+0x48 between the floats); ours CSEs 0.04f into edx and groups the zero stores (xor eax) right after the 1.0f run. tried: (float)0.04 literal for the second use, optimize("s") (fld1/fldz/movsd: no)
// CReverbSettings00c6adb0::SetDefaults() -- fill a 0x68-byte audio reverb/environment parameter block with its default values (all-parameters mask 0x1FFFFF last)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C6ADB0, length 173 bytes (`ret`, no `mov eax,ecx`: a void member, not a ctor).
// AUDIO LAYER (replacement work later): the true class is not named by RTTI or the PDBs (no vtable store); the only
// caller (0x00C6B1DE, inside the routine labelled "CTCHeroStats::InitializeAudioEffects", a propagated label) runs it
// on a stack block at [esp+0x14] that it then passes on by address. The values are reverb-style defaults: a -10000
// room level (+4, the "off" millibel floor), 200.0 / 5000.0 (+0x1c/+0x20, reverb level / HF reference style
// values), 0.011 reverb delay (+0x28), 7.5 environment size (+0x64), two zero C3DVector pans (+0x4c, +0x58) copied from
// the global zero vector 0x012B95E4, and a parameter mask 0x1FFFFF at +0 (21 flag bits).
// Shape: plain body assignments in retail's store order; the 1.0f and 0 constants VC shares in eax/edx.
// Leaf: SetDefaults
struct C3DVector { float X, Y, Z; };
extern const C3DVector C3DVector_Zero;                 // 0x012B95E4

struct CReverbSettings00c6adb0 {
    unsigned long m_mask;          // +0x00 which parameters are valid (0x1FFFFF = all 21)
    long  m_room;                  // +0x04 millibels (-10000)
    long  m_roomHF;                // +0x08
    long  m_roomLF;                // +0x0c
    float m_scale10;               // +0x10 1.0
    float m_scale14;               // +0x14 1.0
    float m_ratio18;               // +0x18 0.5
    float m_level1c;               // +0x1c 200.0
    float m_hfReference;           // +0x20 5000.0
    float m_delay24;               // +0x24 0.0498
    float m_reverbDelay;           // +0x28 0.011
    float m_value2c;               // +0x2c 1.259
    float m_value30;               // +0x30 0.04
    float m_scale34;               // +0x34 1.0
    float m_scale38;               // +0x38 1.0
    float m_value3c;               // +0x3c 0.075
    float m_value40;               // +0x40 0
    float m_value44;               // +0x44 0.04
    float m_value48;               // +0x48 0
    C3DVector m_reflectionsPan;    // +0x4c
    C3DVector m_reverbPan;         // +0x58
    float m_environmentSize;       // +0x64 7.5
    void SetDefaults();
};

void CReverbSettings00c6adb0::SetDefaults()
{
    m_scale10 = 1.0f;
    m_scale14 = 1.0f;
    m_scale34 = 1.0f;
    m_scale38 = 1.0f;
    m_room = -10000;
    m_roomHF = 0;
    m_roomLF = 0;
    m_ratio18 = 0.5f;
    m_level1c = 200.0f;
    m_hfReference = 5000.0f;
    m_delay24 = 0.0498f;
    m_reverbDelay = 0.011f;
    m_value2c = 1.259f;
    m_value30 = 0.04f;
    m_value3c = 0.075f;
    m_value40 = 0;
    m_value44 = 0.04f;
    m_value48 = 0;
    m_reflectionsPan = C3DVector_Zero;
    m_reverbPan = C3DVector_Zero;
    m_environmentSize = 7.5f;
    m_mask = 0x1fffff;
}
