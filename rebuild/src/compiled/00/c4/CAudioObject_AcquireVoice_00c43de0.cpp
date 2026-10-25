// CAudioObject::AcquireVoice() -- ask the concrete audio object (slot 8) for a voice for its current buffer (+0x78, else the queued tail buffer, else none), written into the voice member (+0x50); returns the status (0 = success)
// Leaf: ?AcquireVoice@CAudioObject@@QAEJXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C43DE0, 55 bytes (+ int3 padding), __fastcall `ret`, long status.
// Library: libpk audio (no PDB/RTTI): descriptive names. Only caller the landed 0x00C43FD0 (at 0x00C43FF9, when the
//   object is idle and has no voice; it returns the non-zero status). The manifest's
//   "DrawAutoPickups_SelectAndDrawPrimitive" is propagated.
// Layout: slot 8 of the base vtable 0x012B5A88 is `_purecall`: the derived classes create the voice.
// Shape: the buffer selection is CanPlay's (0x00C43910: `capacity ? Peek() : 0` with Peek repeating the capacity test);
//   VC7.1 tail-duplicates the virtual call into both arms (two `push esi ; lea esi,[ecx+50h]` call blocks). Writing
//   the two calls by hand moves the zero arm (54 bytes).
class CVoice;
struct CSoundBuffer;

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
    virtual ~CAudioObject();                                                // slot 0 (0x00C3CFC0)
    virtual bool Is3D() const;                                              // slot 1
    virtual bool IsOpen() const;                                            // slot 2
    virtual long Open(void* params);                                        // slot 3
    virtual void Close();                                                   // slot 4
    virtual void v05() = 0;                                                 // slot 5
    virtual void v06();                                                     // slot 6 SetLoopPoints
    virtual void v07();                                                     // slot 7 SetVolumeParams
    virtual long CreateVoice(CSoundBuffer* buffer, CVoice** ppVoice) = 0;   // slot 8
    unsigned long m_node[2];                // +0x04
    unsigned long m_caps;                   // +0x0c
    unsigned char m_pad10[0x50 - 0x10];
    CVoice*       m_pVoice;                 // +0x50
    CircularQueue m_queue;                  // +0x54
    CSoundBuffer* m_pCurrentBuffer;         // +0x78
    long AcquireVoice();
};

long CAudioObject::AcquireVoice()
{
    CSoundBuffer* buffer = m_pCurrentBuffer;
    if (!buffer)
        buffer = m_queue.m_capacity != 0 ? m_queue.Peek() : 0;
    return CreateVoice(buffer, &m_pVoice);
}