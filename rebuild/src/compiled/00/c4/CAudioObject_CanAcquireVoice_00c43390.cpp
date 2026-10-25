// CAudioObject::CanAcquireVoice(CSoundBuffer* buffer, unsigned long flags, const SVoiceCaps* hardware, const SVoiceCaps* software) -- whether a voice can be found for the audio object: a hardware voice when the pool's capabilities cover the object's requested caps (+0x0C) and the buffer's format, otherwise the software pool's availability; a streamed buffer must go to hardware (missing caps set error -14)
// Leaf: ?CanAcquireVoice@CAudioObject@@QAE_NPAUCSoundBuffer@@KPBUSVoiceCaps@@1@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C43390, 144 bytes, __fastcall `ret 10h`, returns bool.
// Library: libpk audio (no PDB/RTTI): descriptive names. Only caller CanPlay 0x00C43910, which passes the current or
//   queued buffer, the global flags 0x0139C9FC and the 2D/3D hardware and software capability records of the global
//   table 0x0139C988 (four 0x18-byte records; 0x00C43530 reads their +8 object-caps words). The manifest's
//   "NCreatureMode_EvaluateAnimState" is propagated.
// Callees: GetDesc 0x00C3C4A0 (`lea eax,[ecx+10h]`; the descriptor's first word is the format mode), the error
//   object's SetError 0x00C3D090 (g_audioSystem 0x0143BB58).
// Shape: the buffer's streamed bit is an inline accessor `(m_state >> 2) & 1` (`shr edx,2 ; test dl,1`; a bit-field
//   gives `test byte ptr [ecx+2Ch],4`); `streamed` is set in both arms (`mov bl,1` / `xor bl,bl`), the late
//   streamed-only-hardware failure returns false WITHOUT a second SetError (it jumps into the first error block's
//   `xor al,al` tail); the software fallback written `if (x) return true; return false;` (or `x ? true : false`)
//   computes straight into eax -- `return x != 0` adds an `xor eax,eax` (146 bytes).
struct SBufferDesc {
    unsigned long m_mode;                   // +0x00 format bits
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
    unsigned long m_available;              // +0x04 voices left in the pool
    unsigned long m_objectCaps;             // +0x08 object capability bits the pool supports
    unsigned long m_formatCaps;             // +0x0c buffer format bits the pool supports
    unsigned long m_words[2];               // +0x10
};

class CAudioSystem {
public:
    void SetError(long code);               // 0x00C3D090
};
extern CAudioSystem g_audioSystem;          // 0x0143BB58

class CAudioObject {
public:
    void*         m_vptr;                   // +0x00 (vtable 0x012B5A88)
    unsigned long m_node[2];                // +0x04
    unsigned long m_caps;                   // +0x0c requested capability bits
    bool CanAcquireVoice(CSoundBuffer* buffer, unsigned long flags, const SVoiceCaps* hardware, const SVoiceCaps* software);
};

bool CAudioObject::CanAcquireVoice(CSoundBuffer* buffer, unsigned long flags, const SVoiceCaps* hardware, const SVoiceCaps* software)
{
    unsigned long missing = m_caps & ~hardware->m_objectCaps;
    bool streamed;
    if (buffer && buffer->IsStreamed()) {
        streamed = true;
        if (missing) {
            g_audioSystem.SetError(-14);
            return false;
        }
    } else
        streamed = false;
    if (!missing) {
        bool ok = true;
        if (buffer) {
            if ((flags & 8) && !buffer->IsStreamed() && !(flags & 0x10))
                ok = false;
            else if (buffer->GetDesc()->m_mode & ~hardware->m_formatCaps)
                ok = false;
        }
        if (ok) {
            if (hardware->m_available)
                return true;
            if (streamed)
                return false;
        }
    }
    if (software->m_available)
        return true;
    return false;
}