// CAudioObject::CanUseHardwareVoice() -- whether the audio object could move to a hardware voice: false when its current voice (+0x50) already is one, otherwise whether the 2D or 3D hardware pool supports every requested capability bit (+0x0C)
// Leaf: ?CanUseHardwareVoice@CAudioObject@@QAE_NXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C43530, 61 bytes (+ int3 padding), __fastcall `ret`, returns bool.
// Library: libpk audio (no PDB/RTTI): descriptive names. No direct call or pointer reference in the image (dead
//   or reached only through code the scan does not see). The manifest's "GetDrawableStateFlags" is propagated.
// Layout: voice slot 4 is IsHardware (the voice manager's Count 0x00C3EAA0 bumps its software / 2D / 3D hardware
//   counters from slots 4 and 5); audio-object slot 1 is Is3D; the capability table 0x0139C988 holds four 0x18-byte
//   records (2D hardware, 3D hardware, 2D software, 3D software) whose +8 word is the supported object caps.
// Shape: `if (m_caps & ~supported) return false; return true;` gives the byte `neg ; sbb al,al ; inc al`; the
//   `return (m_caps & ~supported) == 0` spelling computes in eax (`sbb eax,eax ; inc eax`, 62 bytes).
struct SVoiceCaps {
    unsigned long m_id;                     // +0x00
    unsigned long m_available;              // +0x04
    unsigned long m_objectCaps;             // +0x08
    unsigned long m_formatCaps;             // +0x0c
    unsigned long m_words[2];               // +0x10
};
extern SVoiceCaps g_voiceCaps[4];           // 0x0139C988

class CVoice {
public:
    virtual void v00();
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual bool IsHardware() const;        // slot 4
    virtual bool Is3D() const;              // slot 5
};

class CAudioObject {
public:
    virtual ~CAudioObject();                // slot 0 (0x00C3CFC0)
    virtual bool Is3D() const;              // slot 1
    unsigned long m_node[2];                // +0x04
    unsigned long m_caps;                   // +0x0c requested capability bits
    unsigned char m_pad10[0x50 - 0x10];
    CVoice*       m_pVoice;                 // +0x50
    bool CanUseHardwareVoice();
};

bool CAudioObject::CanUseHardwareVoice()
{
    if (m_pVoice && m_pVoice->IsHardware())
        return false;
    unsigned long supported = Is3D() ? g_voiceCaps[1].m_objectCaps : g_voiceCaps[0].m_objectCaps;
    if (m_caps & ~supported)
        return false;
    return true;
}