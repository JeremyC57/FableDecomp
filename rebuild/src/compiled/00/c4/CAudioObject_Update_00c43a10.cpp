// CAudioObject::Update(__int64 time) -- one audio-manager tick of an active audio object: fold the global dirty flags into its own, ask its voice for the new play flags (start a current buffer, service a queue, poll a pending one, keep a playing or virtual one running), move to the new state and raise the bit-9 (virtual) callback when it appears, flush a queue that still holds buffers on stop; the dirty flags are cleared at the end
// Leaf: ?Update@CAudioObject@@QAEX_J@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C43A10, 274 bytes, thiscall, `ret 8`.
// Callers: the audio manager's update loop 0x00C3D56D / 0x00C3D59E (every object whose flags have a bit of 0x217 set,
//   with the 64-bit elapsed time now - start).
// Callees: CAudioObject::SetState 0x00C43570 (landed), FlushQueue 0x00C437C0 (landed), ReportQueueInfo 0x00C432F0 (landed
//   under the placeholder CMarkedBufferOwner_00c43810::ReportMark; NAME_FIXES.tsv), the voice (+0x50) slots 7..10 and the
//   `__stdcall` state-change callback +0xDC (object, low 16 flag bits, data +0xE0); global g_audioDirtyFlags 0x0143BBFC.
// Name: descriptive (libpk: no PDB or RTTI). The class layout is the landed CAudioObject model (flags +0x10, dirty flags
//   +0x14, voice +0x50, queue +0x54 (capacity +0x5C, stats +0x6C), current buffer +0x78, callback +0xDC / +0xE0).
// Voice: the +0x50 object is the libpk voice base (vtable 0x012B5C10; OpenAL CAudioSource 0x012B8CC8 derives from it):
//   slot 7 (0x00C59720, landed "CCodaVoice::Play") takes the object, slot 8 (CAudioSource::Update 0x00C72FE0) and slot 9
//   (pure in the base) take the object and the time, slot 10 (0x00C59770, landed "CCodaVoice::CheckPosition") the time.
//   The slot names are descriptive (Play / UpdateQueue / UpdatePlaying / UpdatePending), following those landed names.
// Shape: state 1 asks slot 7 (current buffer) or slot 8, state 0x10 slot 10; then a playing state (bits 1..2) of either
//   the new or the old flags, or a virtual object, asks slot 9. The 0x200 store and its callback use the stored register.
//   The stop test is `capacity != 0 && !IsEmpty()` with IsEmpty an inline `capacity != 0 && stats[0] == 0` (retail tests
//   the capacity twice: `je skip ; je do ; cmp [esi+6Ch],0 ; je skip`).
class CSoundBuffer;
class CAudioObject;

class CVoice {                                      // the libpk voice base (vtable 0x012B5C10)
public:
    virtual ~CVoice();                              // slot 0
    virtual long Initialise(void* sound);           // slot 1 (0x00C596E0)
    virtual void Slot2();                           // slot 2 (not called here)
    virtual void Slot3();                           // slot 3
    virtual void Slot4();                           // slot 4
    virtual void Slot5();                           // slot 5
    virtual void Slot6();                           // slot 6
    virtual unsigned long Play(CAudioObject* object);                          // slot 7 (+0x1C)
    virtual unsigned long UpdateQueue(CAudioObject* object, __int64 time);     // slot 8 (+0x20)
    virtual unsigned long UpdatePlaying(CAudioObject* object, __int64 time);   // slot 9 (+0x24)
    virtual unsigned long UpdatePending(__int64 time);                         // slot 10 (+0x28)
};

class CircularQueue {                               // the audio object's buffer queue (+0x54)
public:
    CSoundBuffer**    m_items;                      // +0x00
    unsigned __int64* m_times;                      // +0x04
    unsigned long     m_capacity;                   // +0x08
    unsigned long     m_head;                       // +0x0c
    unsigned long     m_tail;                       // +0x10
    CSoundBuffer*     m_lastItem;                   // +0x14
    unsigned long     m_stats[3];                   // +0x18
    bool IsEmpty() const { return m_capacity != 0 && m_stats[0] == 0; }
};

typedef void (__stdcall *TStateCallback)(CAudioObject* object, unsigned long flags, void* data);

extern unsigned long g_audioDirtyFlags;             // 0x0143BBFC

class CAudioObject {
public:
    void*          m_vptr;                          // +0x00 (vtable 0x012B5A88)
    unsigned long  m_node[2];                       // +0x04
    unsigned long  m_caps;                          // +0x0c
    unsigned long  m_flags;                         // +0x10 state bits 0..6, bit 8, bit 9 virtual, bit 17 keep voice
    unsigned long  m_dirtyFlags;                    // +0x14
    unsigned char  m_pad18[0x50 - 0x18];
    CVoice*        m_pVoice;                        // +0x50
    CircularQueue  m_queue;                         // +0x54
    CSoundBuffer*  m_pCurrentBuffer;                // +0x78
    unsigned char  m_pad7c[0xdc - 0x7c];
    TStateCallback m_stateCallback;                 // +0xdc
    void*          m_stateCallbackData;             // +0xe0
    unsigned long GetState() const { return m_flags & 0x7f; }
    bool IsVirtual() const { return (m_flags >> 9) & 1; }
    void SetState(unsigned long state);             // 0x00C43570
    void FlushQueue();                              // 0x00C437C0
    void ReportQueueInfo();                         // 0x00C432F0
    void Update(__int64 time);
};

void CAudioObject::Update(__int64 time)
{
    unsigned long state = GetState();
    m_dirtyFlags |= g_audioDirtyFlags;
    unsigned long flags = state;
    if (state == 1) {
        if (m_pCurrentBuffer)
            flags = m_pVoice->Play(this);
        else
            flags = m_pVoice->UpdateQueue(this, time);
    } else if (state == 0x10) {
        flags = m_pVoice->UpdatePending(time);
    }
    if ((flags & 6) || (state & 6) || IsVirtual())
        flags = m_pVoice->UpdatePlaying(this, time);
    if (flags != (m_flags & 0xffff)) {
        SetState(flags);
        if ((flags & 0x200) && !IsVirtual()) {
            m_flags |= 0x200;
            if (m_stateCallback)
                m_stateCallback(this, m_flags & 0xffff, m_stateCallbackData);
        }
        if (flags == 0 && m_queue.m_capacity != 0 && !m_queue.IsEmpty()) {
            FlushQueue();
            ReportQueueInfo();
        }
    }
    m_dirtyFlags = 0;
}