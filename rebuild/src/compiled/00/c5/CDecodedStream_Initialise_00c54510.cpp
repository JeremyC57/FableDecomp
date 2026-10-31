// CDecodedStream::Initialise() -- set up the stream's voice: initialise the +0x2F8 voice effect for a 4-unit, 44100 Hz format (false when that fails), reset the current source properties (+0x224) to the static defaults, reset every pending reverb property of the +0x290 block through its clamped setters, then configure the voice effect from that block
// Leaf: ?Initialise@CDecodedStream@@QAE_NXZ
// Flags: /EHsc
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C54510, 456 bytes, thiscall, plain `ret`, EH frame (the format local's inline
//   virtual destructor is the unwind action).
// Identity: the landed caller CDecodedStream::Open 0x00C547A0 (slot 1 of the CDecodedStream vtable 0x012B89C8, whose ctor
//   0x00C54090 / dtor 0x00C54110 store it) binds this address as `?Initialise@CDecodedStream@@QAE_NXZ` and returns
//   `Initialise() ? 0 : -9`. libpk: no PDB/RTTI, all names descriptive.
// Callees: CVoiceEffect::Initialise 0x00C6B050 (new descriptive name: thiscall on +0x2F8, takes the 7-dword format object,
//   fills its zero size fields from the rate and copies it to effect+0x234), CVoiceEffect::Configure 0x00C6AEF0 (landed),
//   and sixteen clamped float setters on the +0x290 block, called with their landed spellings (the block carries seven
//   descriptive class names across the landed rows; see NOTES.md): 0x00C53440 0x00C53500 0x00C534A0 0x00C53560
//   0x00C535C0 0x00C53650 0x00C536B0 0x00C53A40 0x00C539E0 0x00C53980 0x00C53800 0x00C53860 0x00C538C0 0x00C53920
//   0x00C53740 0x00C537A0.
// Globals: the zero vector 0x012B89E8 and the 0x6C-byte default source properties 0x012B89F8 (.rdata, referenced only
//   here), the format vtable 0x012B89C4 (one slot: the scalar deleting destructor 0x00C53AE0).
// Shape: the format local is the 0x012B89C4 class with its inline defaults (rate and channel count overwritten, so only the
//   vptr and the four zero words survive) and an inline empty virtual destructor (EH state 0, no call at the exits);
//   `this + 0x2F8` is CSE'd and spilled; the pans are inline C3DVector assignments through `lea ecx`, the three level
//   words inline `value ; DirtyFlags |=` setters, each reached through the inline accessor GetPendingParameters() (retail
//   reloads the dirty word after every store: `or [esi],800h` / `or [esi],4000h` after the pan copies, then load / or / store
//   per level word); the sixteen out-of-line setters use the `p` reference.
class C3DVector {
public:
    float X, Y, Z;

};

struct CVtable_012b89c4 {                                  // vtable 0x012B89C4, 0x1C bytes: a sound format description
    CVtable_012b89c4()
    {
        m_sampleRate = 48000;
        m_channels = 2;
        m_0c = 0; m_10 = 0; m_14 = 0; m_18 = 0;
    }
    virtual ~CVtable_012b89c4() { }
    long m_channels;                                       // +0x04
    long m_sampleRate;                                     // +0x08
    long m_0c, m_10, m_14, m_18;                           // +0x0c..+0x18 derived sizes (filled by 0x00C6B050 when 0)
};

struct SSourceProperties {                                 // +0x224 current properties, 0x6C bytes
    C3DVector     Position;                                // +0x00
    C3DVector     Velocity;                                // +0x0c
    long          m_levels[3];                             // +0x18
    float         m_values[18];                            // +0x24
};
extern const C3DVector g_zeroVector_012b89e8;              // 0x012B89E8
extern const SSourceProperties g_defaultSourceProperties_012b89f8;   // 0x012B89F8

// The +0x290 pending-property block (dirty word at +0): Configure 0x00C6AEF0 takes it as SVoiceEffectParameters.
struct SVoiceEffectParameters {
    unsigned long DirtyFlags;                              // +0x00
    long          m_room;                                  // +0x04 (dirty 0x20)
    long          m_roomHF;                                // +0x08 (dirty 0x40)
    long          m_roomLF;                                // +0x0c (dirty 0x80)
    float         m_values[15];                            // +0x10..+0x48 (the clamped setters)
    C3DVector     m_reflectionsPan;                        // +0x4c (dirty 0x800)
    C3DVector     m_reverbPan;                             // +0x58 (dirty 0x4000)
    float         m_environmentSize;                       // +0x64
    void SetReflectionsPan(const C3DVector& v) { m_reflectionsPan = v; DirtyFlags |= 0x800; }
    void SetReverbPan(const C3DVector& v) { m_reverbPan = v; DirtyFlags |= 0x4000; }
    void SetRoom(long v) { m_room = v; DirtyFlags |= 0x20; }
    void SetRoomHF(long v) { m_roomHF = v; DirtyFlags |= 0x40; }
    void SetRoomLF(long v) { m_roomLF = v; DirtyFlags |= 0x80; }
};
// The landed definers of the clamped setters (descriptive spellings, one per address).
struct CAttribute_00c53440 { void SetClamped_CAttribute_00c53440(float value); };   // 0x00C53440 +0x10 [0.1,20] 0x100
struct CAttribute_00c53500 { void SetClamped_CAttribute_00c53500(float value); };   // 0x00C53500 +0x18 [0.1,2] 0x400
struct CAttribute_00c534a0 { void SetClamped_CAttribute_00c534a0(float value); };   // 0x00C534A0 +0x14 [0.1,2] 0x200
struct CAttribute_00c53560 { void SetClamped_CAttribute_00c53560(float value); };   // 0x00C53560 +0x24 [0,3.163] 0x1000
class CUnprovenReverbParameters { public:
    void SetClamped28(float value);                        // 0x00C535C0 +0x28 [0,0.3] 0x2000
    void SetClamped30(float value);                        // 0x00C536B0 +0x30 [0,0.1] 0x10000
    void SetClamped64(float value);                        // 0x00C53A40 +0x64 [0.1,100] 0x1
};
struct CAttribute_00c53650 { void SetClamped_CAttribute_00c53650(float value); };   // 0x00C53650 +0x2c [0,10] 0x8000
struct CUnprovenPendingParams_00c53740 {
    void SetField1C(float value);                          // 0x00C53740 +0x1c [20,2000] 8
    void SetField20(float value);                          // 0x00C537A0 +0x20 [100,20000] 0x10
    void SetField38(float value);                          // 0x00C53980 +0x38 [0,1] 2
    void SetField34(float value);                          // 0x00C539E0 +0x34 [0,1] 4
};
struct CAttribute_00c53800 { void SetClamped_CAttribute_00c53800(float value); };   // 0x00C53800 +0x3c [0.075,0.25]
struct CAttribute_00c53860 { void SetClamped_CAttribute_00c53860(float value); };   // 0x00C53860 +0x40 [0,1]
struct CAttribute_00c538c0 { void SetClamped_CAttribute_00c538c0(float value); };   // 0x00C538C0 +0x44 [0.04,5]
struct CAttribute_00c53920 { void SetClamped_CAttribute_00c53920(float value); };   // 0x00C53920 +0x48 [0,1]

struct CVoiceEffect {                                      // +0x2F8 (landed Configure model)
    bool Initialise(CVtable_012b89c4* format);             // 0x00C6B050
    void Configure(const SVoiceEffectParameters* parameters, long mode);   // 0x00C6AEF0
};

class CDecodedStream {                                     // vtable 0x012B89C8
public:
    virtual ~CDecodedStream();
    unsigned char          _pad004[0x224 - 4];
    SSourceProperties      m_properties;                   // +0x224
    SVoiceEffectParameters m_pending;                      // +0x290
    CVoiceEffect           m_effect;                       // +0x2f8
    // Inline accessor (descriptive name, no symbol): each call yields a fresh pointer temp, so VC7.1 reloads the dirty word
    // after every inline setter exactly like retail; through one reference it folds the five updates into one register RMW.
    SVoiceEffectParameters* GetPendingParameters() { return &m_pending; }
    bool Initialise();
};

bool CDecodedStream::Initialise()
{
    CVtable_012b89c4 format;
    format.m_sampleRate = 44100;
    format.m_channels = 4;
    if (!m_effect.Initialise(&format))
        return false;
    m_properties = g_defaultSourceProperties_012b89f8;
    SVoiceEffectParameters& p = m_pending;
    GetPendingParameters()->SetReflectionsPan(g_zeroVector_012b89e8);
    GetPendingParameters()->SetReverbPan(g_zeroVector_012b89e8);
    GetPendingParameters()->SetRoom(-10000);
    GetPendingParameters()->SetRoomLF(0);
    GetPendingParameters()->SetRoomHF(0);
    reinterpret_cast<CAttribute_00c53440&>(p).SetClamped_CAttribute_00c53440(1.0f);
    reinterpret_cast<CAttribute_00c53500&>(p).SetClamped_CAttribute_00c53500(0.5f);
    reinterpret_cast<CAttribute_00c534a0&>(p).SetClamped_CAttribute_00c534a0(1.0f);
    reinterpret_cast<CAttribute_00c53560&>(p).SetClamped_CAttribute_00c53560(0.0f);
    reinterpret_cast<CUnprovenReverbParameters&>(p).SetClamped28(0.02f);
    reinterpret_cast<CAttribute_00c53650&>(p).SetClamped_CAttribute_00c53650(0.0f);
    reinterpret_cast<CUnprovenReverbParameters&>(p).SetClamped30(0.04f);
    reinterpret_cast<CUnprovenReverbParameters&>(p).SetClamped64(7.5f);
    reinterpret_cast<CUnprovenPendingParams_00c53740&>(p).SetField34(1.0f);
    reinterpret_cast<CUnprovenPendingParams_00c53740&>(p).SetField38(1.0f);
    reinterpret_cast<CAttribute_00c53800&>(p).SetClamped_CAttribute_00c53800(0.075f);
    reinterpret_cast<CAttribute_00c53860&>(p).SetClamped_CAttribute_00c53860(0.0f);
    reinterpret_cast<CAttribute_00c538c0&>(p).SetClamped_CAttribute_00c538c0(0.04f);
    reinterpret_cast<CAttribute_00c53920&>(p).SetClamped_CAttribute_00c53920(0.0f);
    reinterpret_cast<CUnprovenPendingParams_00c53740&>(p).SetField1C(200.0f);
    reinterpret_cast<CUnprovenPendingParams_00c53740&>(p).SetField20(5000.0f);
    m_effect.Configure(&p, 1);
    return true;
}