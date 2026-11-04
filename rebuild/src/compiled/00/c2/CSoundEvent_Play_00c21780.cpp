// CSoundEvent::Play(CSoundEventManager* manager) -- start an idle (state 1) event: by its sample's load status stop it (0) or wait for data (2); else play from a cached stream buffer, or allocate pool memory and a new stream buffer, load or start streaming the sample, then play physically / wait for a physical player, wait for memory, or give up and stop
// Leaf: ?Play@CSoundEvent@@QAEXPAUCSoundEventManager@@@Z
// Flags: /EHsc
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C21780, 622 bytes, thiscall `ret 4`, EH-framed (state 0 only around `new CStreamBuffer`).
// Identity: the libpk sound event (vtable 0x012B3674; no RTTI / PDB, descriptive family names). Bound spelling
//   ?Play@CSoundEvent@@QAEXPAUCSoundEventManager@@@Z from the landed CSoundEvent::Update 0x00C21A50 ("Play 0x00C21780 (the
//   state-1 start: wait for data, physical or virtual)"); the other callers 0x00C21A01 / 0x00C21A38 / 0x00C21B53 / 0x00C21D18 (all
//   CSoundEvent bodies, ecx = the event) bind placeholders (NAME_FIXES.tsv). The catalog's
//   "CInputProcessRebootGame_HandleInputEvent" is propagated.
// Callees (the CSoundEvent family): CheckState 0x00C20720 (1), Stop 0x00C21620, WaitForData 0x00C20850, WaitForMemory
//   0x00C20880, WaitForPhysicalPlayer 0x00C20950, PlayPhysical 0x00C20A20; the event's +0x18 sample reference CSampleRef::GetSample
//   0x00C25C60; the sample's GetSize 0x00C1EA80 / LoadData 0x00C1EA60 and virtual slots 1 (start streaming) / 3 (is streamed);
//   the manager's cached-buffer queue (+0x284) slot 17 Find, GetPhysicalPlayer 0x00C10C90 (bound `void*` result) and its event set
//   (+0x2C) AllocateHandle 0x00C22100; CStreamBuffer ctor 0x00C262A0 / Attach 0x00C262E0 / FinishLoading 0x00C26090 / Release
//   0x00C26390 / dtor 0x00C26400; operator new 0x00BFEA1A (0x2C) / operator delete 0x00BFE9BC.
// Type identity: the pointer `new CStreamBuffer` returns is the one PlayPhysical / WaitForPhysicalPlayer take (and store at
//   +0xA4); the landed bindings call it CSoundChannel there, CStreamBuffer at its own methods: one class, CStreamBuffer
//   (NAME_FIXES.tsv). The manager is the same CSoundEventManager pointer at every call (Attach / Release bind CPanel /
//   CPooledResourceManager spellings).
// Shape: each failure test falls through to its handler (`if (r < 0) { ... }`); the sample reference is re-read at each use
//   (retail calls GetSample every time). The two failure exits each spell out `Release; delete; Stop; return` and the two
//   play exits `GetPhysicalPlayer; PlayPhysical or WaitForPhysicalPlayer`: VC7.1 cross-jumps the identical tails into retail's
//   single release block and single WaitForPhysicalPlayer call, and the unmerged copies weight the buffer's register (edi) above
//   the sample reference (ebx) and `this` (ebp), as in retail (one shared release block in the source gives a this/ref/buffer
//   register rotation). The sample pointers for AllocateHandle / Attach are fetched into locals first: retail calls GetSample
//   before it pushes m_priority / the handle.
struct CSoundEventManager;
struct CStreamBuffer;
struct CPhysicalPlayer;

struct CSoundSample {                            // the sample a stream buffer holds (vtable; load status +0x1C)
    virtual void v00();
    virtual long StartStreaming(CSoundEventManager* manager, CStreamBuffer* buffer);   // slot 1 (+0x04)
    virtual void v02();
    virtual long IsStreamed();                   // slot 3 (+0x0C)
    void*          m_pData;                      // +0x04
    unsigned char  m_format[0x14];               // +0x08
    unsigned short m_status;                     // +0x1C 0 failed, 1 loaded, 2 loading
    unsigned short m_refCount;                   // +0x1E
    long GetSize();                              // 0x00C1EA80
    long LoadData(CSoundEventManager* manager, CStreamBuffer* buffer);   // 0x00C1EA60
};

struct CSampleRef {                              // +0x18 of the event, 0x10 bytes
    CSoundSample* m_pSample;
    long m_4, m_8, m_c;
    CSoundSample* GetSample();                   // 0x00C25C60
};

struct CStreamBuffer {                           // 0x2C bytes
    unsigned char _body[0x2C];
    CStreamBuffer();                             // 0x00C262A0
    ~CStreamBuffer();                            // 0x00C26400
    void Attach(CSoundEventManager* manager, CSoundSample* sample, long handle, long users);   // 0x00C262E0
    long FinishLoading(CSoundEventManager* manager, long success);   // 0x00C26090
    void Release(CSoundEventManager* manager);   // 0x00C26390
};

struct CStreamBufferQueue {                      // polymorphic buffer queue of the manager
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03();
    virtual void v04(); virtual void v05(); virtual void v06(); virtual void v07();
    virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15();
    virtual void v16();
    virtual CStreamBuffer* Find(CSoundSample** sample);   // slot 17 (+0x44)
    unsigned char _pad04[0x2C];
};

struct CSoundEventSet {                          // +0x2C of the manager
    long AllocateHandle(CSoundEventManager* manager, long size, long priority);   // 0x00C22100
};

struct CSoundEventManager {
    unsigned char _pad000[0x2C];
    CSoundEventSet m_eventSet;                   // +0x2C
    unsigned char _pad030[0x284 - 0x30];
    CStreamBufferQueue m_cachedBuffers;          // +0x284
    void* GetPhysicalPlayer(unsigned long priority);   // 0x00C10C90
};

struct CSoundEvent {                             // 0xB4 bytes, vtable 0x012B3674 (no RTTI)
    void* __vftable;                             // +0x00
    unsigned char _pad04[0x14 - 0x04];
    long m_priority;                             // +0x14
    CSampleRef m_sample;                         // +0x18
    unsigned char _pad28[0xAC - 0x28];
    unsigned short m_state;                      // +0xAC
    unsigned char _padae[0xB4 - 0xAE];
    void CheckState(long expected);              // 0x00C20720
    void WaitForData(CSoundEventManager* manager);                    // 0x00C20850
    void WaitForMemory(CSoundEventManager* manager);                  // 0x00C20880
    void WaitForPhysicalPlayer(CSoundEventManager* manager, CStreamBuffer* buffer);   // 0x00C20950
    void PlayPhysical(CSoundEventManager* manager, CStreamBuffer* buffer, CPhysicalPlayer* player);   // 0x00C20A20
    void Stop(CSoundEventManager* manager);      // 0x00C21620
    void Play(CSoundEventManager* manager);
};

// Same-TU body of CheckState 0x00C20720 (the landed WaitForData stand-in); only Play is graded from this file.
#pragma auto_inline(off)
__declspec(noinline) void CSoundEvent::CheckState(long expected)
{
    switch (m_state) { case 0: case 1: case 2: case 3: case 4: case 5: case 6: break; }
}
#pragma auto_inline(on)

void CSoundEvent::Play(CSoundEventManager* manager)
{
    CheckState(1);
    if (m_sample.GetSample()->m_status == 0) {
        Stop(manager);
        return;
    }
    if (m_sample.GetSample()->m_status == 2) {
        WaitForData(manager);
        return;
    }
    CSoundSample* sample = m_sample.GetSample();
    CStreamBuffer* cached = manager->m_cachedBuffers.Find(&sample);
    if (cached) {
        CPhysicalPlayer* player = (CPhysicalPlayer*)manager->GetPhysicalPlayer(m_priority);
        if (player)
            PlayPhysical(manager, cached, player);
        else
            WaitForPhysicalPlayer(manager, cached);
        return;
    }
    CSoundSample* toSize = m_sample.GetSample();
    long handle = manager->m_eventSet.AllocateHandle(manager, toSize->GetSize(), m_priority);
    if (handle < 0) {
        WaitForMemory(manager);
        return;
    }
    CStreamBuffer* buffer = new CStreamBuffer;
    CSoundSample* toAttach = m_sample.GetSample();
    buffer->Attach(manager, toAttach, handle, m_priority);
    if (m_sample.GetSample()->IsStreamed()) {
        if (m_sample.GetSample()->StartStreaming(manager, buffer) < 0) {
            buffer->FinishLoading(manager, 0);
            buffer->Release(manager);
            delete buffer;
            Stop(manager);
            return;
        }
        m_sample.GetSample()->m_status = 2;
        WaitForData(manager);
        return;
    }
    if (buffer->FinishLoading(manager, m_sample.GetSample()->LoadData(manager, buffer) >= 0) < 0) {
        m_sample.GetSample()->m_status = 0;
        buffer->Release(manager);
        delete buffer;
        Stop(manager);
        return;
    }
    CPhysicalPlayer* player = (CPhysicalPlayer*)manager->GetPhysicalPlayer(m_priority);
    if (player)
        PlayPhysical(manager, buffer, player);
    else
        WaitForPhysicalPlayer(manager, buffer);
}