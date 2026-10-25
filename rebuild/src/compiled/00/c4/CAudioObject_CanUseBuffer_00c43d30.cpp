// CAudioObject::CanUseBuffer(CSoundBuffer* buffer) -- whether the audio object can take a sound buffer: a streamed buffer needs a hardware voice it could use, a 3D object takes only mono buffers, and when the object is stereo-flagged (+0x10 bit 16) or has loop points (+0x24) or volume parameters (+0x28) the buffer's channel count must agree with that flag; a refusal sets error -14
// Leaf: ?CanUseBuffer@CAudioObject@@QAE_NPAUCSoundBuffer@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C43D30, 176 bytes, __fastcall `ret 4`, returns bool.
// Library: libpk audio (no PDB/RTTI): descriptive names. Callers SetBuffer 0x00C43E20 and the queueing path at
//   0x00C43F89 (landed 0x00C43F80). The manifest's "CShaderConstantRange_ShouldClearPrimitives" is propagated.
// Callees: GetDesc 0x00C3C4A0 (desc +4 = channel count, called out of line three times), Is3D (slot 1), the voice's
//   IsHardware (slot 4), SetError 0x00C3D090 (g_audioSystem 0x0143BB58).
// Shape: CanUseHardwareVoice (out of line at 0x00C43530) is an in-class inline here; the constant 1 lives in ebx
//   (`test bl,al`, `cmp [eax+4],ebx`, `test byte ptr [esi+12h],bl`, and the `mov al,bl` true return); the stereo
//   accessor `(m_flags >> 16) & 1` becomes the byte test of +0x12. One shared failure block; the chained
//   `ok = false` / else-if spelling, a single `||` condition and a goto chain all compile to the same bytes.
struct SBufferDesc {
    unsigned long m_mode;                   // +0x00
    unsigned long m_channels;               // +0x04
};

struct CSoundBuffer {
    virtual void v00();
    unsigned char m_pad04[0x2c - 4];
    unsigned long m_state;                  // +0x2c (bit 2: streamed)
    bool IsStreamed() const { return (m_state >> 2) & 1; }
    const SBufferDesc* GetDesc() const;     // 0x00C3C4A0
};

struct SVoiceCaps {
    unsigned long m_id;                     // +0x00
    unsigned long m_available;              // +0x04
    unsigned long m_objectCaps;             // +0x08
    unsigned long m_formatCaps;             // +0x0c
    unsigned long m_words[2];               // +0x10
};
extern SVoiceCaps g_voiceCaps[4];           // 0x0139C988

class CAudioSystem {
public:
    void SetError(long code);               // 0x00C3D090
};
extern CAudioSystem g_audioSystem;          // 0x0143BB58

class CVoice {
public:
    virtual void v00();
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual bool IsHardware() const;        // slot 4
};

struct SLoopPoints;
struct SVolumeParams;

class CAudioObject {
public:
    virtual ~CAudioObject();                // slot 0 (0x00C3CFC0)
    virtual bool Is3D() const;              // slot 1
    unsigned long  m_node[2];               // +0x04
    unsigned long  m_caps;                  // +0x0c
    unsigned long  m_flags;                 // +0x10 (bit 16: stereo)
    unsigned long  m_dirtyFlags;            // +0x14
    float          m_gain;                  // +0x18
    long           m_volume;                // +0x1c
    unsigned long  m_unknown20;             // +0x20
    SLoopPoints*   m_pLoopPoints;           // +0x24
    SVolumeParams* m_pVolumeParams;         // +0x28
    unsigned char  m_pad2c[0x50 - 0x2c];
    CVoice*        m_pVoice;                // +0x50
    bool IsStereo() const { return (m_flags >> 16) & 1; }
    bool CanUseHardwareVoice()              // out of line at 0x00C43530
    {
        if (m_pVoice && m_pVoice->IsHardware())
            return false;
        unsigned long supported = Is3D() ? g_voiceCaps[1].m_objectCaps : g_voiceCaps[0].m_objectCaps;
        if (m_caps & ~supported)
            return false;
        return true;
    }
    bool CanUseBuffer(CSoundBuffer* buffer);
};

bool CAudioObject::CanUseBuffer(CSoundBuffer* buffer)
{
    bool ok = true;
    if (buffer->IsStreamed() && !CanUseHardwareVoice())
        ok = false;
    else if (Is3D() && buffer->GetDesc()->m_channels > 1)
        ok = false;
    else if (IsStereo() || m_pLoopPoints || m_pVolumeParams) {
        if (buffer->GetDesc()->m_channels == 1 && IsStereo())
            ok = false;
        else if (buffer->GetDesc()->m_channels > 1 && !IsStereo())
            ok = false;
    }
    if (!ok) {
        g_audioSystem.SetError(-14);
        return false;
    }
    return true;
}