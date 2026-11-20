// SPKVoiceSettings::~SPKVoiceSettings() -- libpk 0x1c-byte settings record destructor: destroy the embedded two-word member at +4
// Leaf: ??1SPKVoiceSettings@@QAE@XZ
// Retail 0x00c162a0:  add ecx,4 ; jmp 0xc167c0
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C162A0, 8 bytes: the compiler-generated destructor of the record whose
//   constructor is 0x00C2D1A0 (landed ??0SPKVoiceSettings@@QAE@XZ), called out of line only by EH unwind funclets:
//   0x01213AD9 (C3DMeshVolume_00c2d2f0 ctor 0x00C2D2F0: the +0x320 member built by 0x00C2D1A0, state 1),
//   0x01213B0A (`add ecx,320h ; jmp 0x00C162A0`, the matching destructor 0x00C2D3B0) and 0x01211CA9 (0x00C161A0's
//   stack local built by 0x00C2D1A0). The catalog label `std::_Cons_val<...EHeroMorphType...>` is propagated and wrong.
// Identity: libpk (no PDB / RTTI): the class name is the descriptive one the landed constructor row carries (one name per
//   class). The member at +4 is destroyed by 0x00C167C0; it is spelled as the landed 0x00C2D3B0 row spells that member
//   (??1CMember_00c2d3b0@@QAE@XZ), so 0x00C167C0 gains no spelling (NF200, 2026-11-20, q6932).
struct CMember_00c2d3b0 {                       // +0x04: two words (data, count); destructor out of line
    unsigned char _body[8];
    ~CMember_00c2d3b0();                        // 0x00C167C0
};

struct SPKVoiceSettings {                       // 0x1c bytes (constructor 0x00C2D1A0)
    unsigned long    m_id;                      // +0x00
    CMember_00c2d3b0 m_reference;               // +0x04
    long             m_enabled;                 // +0x0c
    long             m_priority;                // +0x10
    long             m_delay;                   // +0x14
    float            m_gain;                    // +0x18
    ~SPKVoiceSettings();
};

SPKVoiceSettings::~SPKVoiceSettings()
{
}
