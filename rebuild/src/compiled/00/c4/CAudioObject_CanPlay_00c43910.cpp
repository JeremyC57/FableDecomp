// CAudioObject::CanPlay() -- whether the audio object can play: true once it holds a voice (+0x50), otherwise whether a voice could be acquired for its current buffer (+0x78, else the queued tail buffer) from the 2D or 3D hardware / software capability records
// Leaf: ?CanPlay@CAudioObject@@QAE_NXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C43910, 110 bytes (+ int3 padding), __fastcall `ret`, returns bool.
// Library: libpk audio (no PDB/RTTI): descriptive names. Only caller the `__stdcall` API wrapper at 0x00C35DBD
//   (null handle: error -7). The manifest's "GetDrawableShadowParameters" is propagated.
// Callee: CanAcquireVoice 0x00C43390 (buffer, global flags 0x0139C9FC, hardware record, software record); the queue
//   at +0x54 is the landed CircularQueue (items +0, capacity +8, tail +0x10).
// Shape: the buffer is a local computed first (edi); the queued buffer is `capacity ? Peek() : 0` where Peek repeats
//   its own capacity test (the doubled `test eax,eax ; je`); the arguments evaluate right to left, so the software
//   record's Is3D call (ebx) comes before the hardware one (eax), then the flags load.
struct CSoundBuffer;

struct SVoiceCaps {
    unsigned long m_id;                     // +0x00
    unsigned long m_available;              // +0x04
    unsigned long m_objectCaps;             // +0x08
    unsigned long m_formatCaps;             // +0x0c
    unsigned long m_words[2];               // +0x10
};
extern SVoiceCaps g_voiceCaps[4];           // 0x0139C988: 2D/3D hardware, 2D/3D software
extern unsigned long g_audioFlags;          // 0x0139C9FC

class CVoice;

class CircularQueue {
public:
    CSoundBuffer**    m_items;              // +0x00
    unsigned __int64* m_times;              // +0x04
    unsigned long     m_capacity;           // +0x08
    unsigned long     m_head;               // +0x0c
    unsigned long     m_tail;               // +0x10
    CSoundBuffer*     m_lastItem;           // +0x14
    unsigned long     m_stats[3];           // +0x18
    CSoundBuffer* Peek() const
    {
        if (m_capacity == 0)
            return 0;
        return m_items[m_tail];
    }
};

class CAudioObject {
public:
    virtual ~CAudioObject();                // slot 0 (0x00C3CFC0)
    virtual bool Is3D() const;              // slot 1
    unsigned long m_node[2];                // +0x04
    unsigned long m_caps;                   // +0x0c
    unsigned char m_pad10[0x50 - 0x10];
    CVoice*       m_pVoice;                 // +0x50
    CircularQueue m_queue;                  // +0x54
    CSoundBuffer* m_pCurrentBuffer;         // +0x78
    bool CanAcquireVoice(CSoundBuffer* buffer, unsigned long flags, const SVoiceCaps* hardware, const SVoiceCaps* software);   // 0x00C43390
    bool CanPlay();
};

bool CAudioObject::CanPlay()
{
    if (m_pVoice)
        return true;
    CSoundBuffer* buffer = m_pCurrentBuffer;
    if (!buffer)
        buffer = m_queue.m_capacity != 0 ? m_queue.Peek() : 0;
    return CanAcquireVoice(buffer, g_audioFlags, Is3D() ? &g_voiceCaps[1] : &g_voiceCaps[0],
                           Is3D() ? &g_voiceCaps[3] : &g_voiceCaps[2]);
}