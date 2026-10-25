// C2DAudioObject::Open(unsigned long caps) -- slot-3 override of the 2D audio object: open the base object with the requested caps, clear the 2D mix record (+0xFC; its level +0x104 starts at 0 with cap 0x80, else -10000), pre-allocate and keep a voice (+0x50, flag bit 17) when the 2D hardware pool cannot serve the caps and the 2D software pool is unlimited (-1), then build the speaker tables
// Leaf: ?Open@C2DAudioObject@@UAE_NK@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C449C0, 178 bytes, __fastcall `ret 4`, returns bool.
// Library: libpk audio (no PDB/RTTI): descriptive names. Slot 3 of the 2D vtable 0x012B7B88 (slot 1 the base's
//   `return false` 0x00C3CFA0, slot 4 Close 0x00C44170); the base Open is 0x00C42DD0 (it stores the caps at +0x0C).
//   The manifest's "CTCHeroStats_ProcessAction" is propagated.
// Callees: CAudioObject::Open 0x00C42DD0 (direct), the voice manager's Allocate 0x00C4D520 (owner, &voice; non-zero
//   = error; g_pVoiceManager 0x0143BB5C), SetError 0x00C3D090, Close (slot 4), InitSpeakerMaps 0x00C448D0.
// Shape: the record clear is the memset intrinsic (pointer kept in eax, stores through edx) followed by a store of
//   its first word through the same register; the level is an if/else (branchless `neg ; sbb ; and 2710h ; add`)
//   -- the `? 0 : -10000` ternary picks edi for caps instead of retail's ebx.
#include <string.h>
#pragma intrinsic(memset)

struct SVoiceCaps {
    unsigned long m_id;                     // +0x00
    unsigned long m_available;              // +0x04 (-1: unlimited)
    unsigned long m_objectCaps;             // +0x08
    unsigned long m_formatCaps;             // +0x0c
    unsigned long m_words[2];               // +0x10
};
extern SVoiceCaps g_voiceCaps[4];           // 0x0139C988: 2D/3D hardware, 2D/3D software

class CAudioSystem {
public:
    void SetError(long code);               // 0x00C3D090
};
extern CAudioSystem g_audioSystem;          // 0x0143BB58

class CVoice;
class CAudioObject;

class CVoiceManager {
public:
    long Allocate(CAudioObject* owner, CVoice** ppVoice);   // 0x00C4D520
};
extern CVoiceManager* g_pVoiceManager;      // 0x0143BB5C

struct S2DMixParams {
    long m_value0;                          // +0x00
    long m_value1;                          // +0x04
    long m_level;                           // +0x08 hundredths of a decibel
    long m_reserved[4];                     // +0x0c
};

class CAudioObject {
public:
    virtual ~CAudioObject();                // slot 0
    virtual bool Is3D() const;              // slot 1
    virtual bool IsOpen() const;            // slot 2
    virtual bool Open(unsigned long caps);  // slot 3 (0x00C42DD0)
    virtual void Close();                   // slot 4 (0x00C44170)
    unsigned long m_node[2];                // +0x04
    unsigned long m_caps;                   // +0x0c
    unsigned long m_flags;                  // +0x10 (bit 17: keep voice)
    unsigned char m_pad14[0x50 - 0x14];
    CVoice*       m_pVoice;                 // +0x50
};

class C2DAudioObject : public CAudioObject {
public:
    virtual bool Open(unsigned long caps);
    unsigned char m_pad54[0xfc - 0x54];
    S2DMixParams  m_mix;                    // +0xfc
    bool InitSpeakerMaps();                 // 0x00C448D0
};

bool C2DAudioObject::Open(unsigned long caps)
{
    if (!CAudioObject::Open(caps))
        return false;
    memset(&m_mix, 0, sizeof(m_mix));
    m_mix.m_value0 = 0;
    m_mix.m_value1 = 0;
    if (caps & 0x80)
        m_mix.m_level = 0;
    else
        m_mix.m_level = -10000;
    if ((!g_voiceCaps[0].m_available || (caps & ~g_voiceCaps[0].m_objectCaps)) && g_voiceCaps[2].m_available == -1) {
        long result = g_pVoiceManager->Allocate(this, &m_pVoice);
        if (result) {
            g_audioSystem.SetError(result);
            Close();
            return false;
        }
        m_flags |= 0x20000;
    }
    return InitSpeakerMaps();
}