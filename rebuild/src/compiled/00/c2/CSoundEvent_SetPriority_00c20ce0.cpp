// CSoundEvent::SetPriority(CSoundEventManager* manager, long priority) -- move the event to a new priority: take it out of its state's queue (waiting for a physical player: the channel and the manager's queue; waiting for memory: that queue; playing physically: the channel at twice the priority) and out of the manager's priority list, store the priority, then put it back everywhere
// Leaf: ?SetPriority@CSoundEvent@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C20CE0, 261 bytes (thiscall, `ret 8`). The bound spelling
//   ?SetPriority@CSoundEvent@@QAEXPAUCSoundEventManager@@J@Z (reloc_bindings, landed caller ResetLevel 0x00C20DF0, which calls
//   it with priority 1); only caller 0x00C20E02. Descriptive names (no PDB or RTTI covers libpk); the manifest's
//   "EquipmentState_UpdateInventory" is propagated.
// Callees: CheckState 0x00C20720 (-1, same-TU stand-in as in the landed siblings), CSoundChannel::Stop 0x00C26480 /
//   CSoundChannel::Start 0x00C26410 (thiscall: manager, priority), the manager's event lists' Remove / Add (slots 8 / 9)
//   on +0x164 (state 3), +0x194 (state 4) and +0x1C4 (the priority list).
// Layout: the landed CSoundEvent / CSoundEventManager model of ResetLevel 0x00C20DF0, plus the manager's list at +0x1C4.
// Shape: the state is re-read after the list updates; the priority test returns through the final epilogue.
struct CSoundEventManager;
struct CSoundEvent;

struct SEventLevel {                        // the 8-byte level record at +4
    float m_gain;                           // +0
    long m_muted;                           // +4
};

struct SEventRecord {                       // 16 bytes: three words and a flag byte
    void* m_a;
    void* m_b;
    void* m_c;
    unsigned char m_flag;
    SEventRecord();                         // 0x00C20390 (zeroes all four)
};

struct SPlayDesc {                          // 0x28-byte physical play request built on the frame
    long m_mode;                            // +0x00
    SEventRecord m_record;                  // +0x04
    float m_unused14[2];                    // +0x14
    float m_distance;                       // +0x1C
    float m_minDistance;                    // +0x20
    float m_maxDistance;                    // +0x24
    SPlayDesc() : m_mode(0), m_record(), m_minDistance(0.0f), m_maxDistance(0.0f) { }
};

struct CEventList {                         // embedded polymorphic event registry of the manager
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03();
    virtual void v04(); virtual void v05(); virtual void v06(); virtual void v07();
    virtual void Remove(CSoundEvent* event);    // slot 8 (+0x20)
    virtual void Add(CSoundEvent* event);       // slot 9 (+0x24)
};

struct CSoundEventManager {
    unsigned char _pad000[0x134];
    CEventList m_waitingForData;            // +0x134 (state 2)
    unsigned char _pad138[0x164 - 0x138];
    CEventList m_waitingForPlayer;          // +0x164 (state 3)
    unsigned char _pad168[0x194 - 0x168];
    CEventList m_waitingForMemory;          // +0x194 (state 4)
    unsigned char _pad198[0x1c4 - 0x198];
    CEventList m_byPriority;                // +0x1C4 every event, kept in priority order (descriptive)
};

struct CSoundPlayer {                       // polymorphic player: the embedded virtual one (+0x78) or the physical one (+0xA0)
    virtual void v00(); virtual void v01();
    virtual void ReadLevel(CSoundEventManager* manager, SEventLevel* level);   // slot 2 (+8)
    virtual void v03();
    virtual void Update();                  // slot 4 (+0x10)
};

struct CVirtualPlayer : CSoundPlayer {      // embedded at +0x78
    unsigned char _body[0x28 - 4];
    void Start(long sample, SEventLevel* level, float pitch);   // 0x00C351B0
};

struct CPhysicalPlayer : CSoundPlayer {
    void Detach(CSoundEventManager* manager);   // 0x00C27A60
    void Play(CSoundEventManager* manager, SPlayDesc* desc, CSoundEvent* event, SEventLevel* level, float pitch, float gain);   // 0x00C271F0
};

struct CSoundChannel {
    void Start(CSoundEventManager* manager, long priority);  // 0x00C26410
    void Stop(CSoundEventManager* manager, long priority);   // 0x00C26480
};

struct CSampleRef {                         // +0x18
    unsigned char _body[0x20];
    long GetSample();                       // 0x00C25C60 (first word)
};

struct CSoundParams {                       // +0x38
    unsigned char _pad00[0x10];
    long m_mode;                            // +0x10 (event +0x48)
    unsigned char _pad14[0x40 - 0x14];
    float GetPitch(CSoundEventManager* manager);   // 0x00C34410
    float GetGain(CSoundEventManager* manager);    // 0x00C34440
    bool Matches(long a, long b);           // 0x00C34530
    float GetDistance();                    // 0x00C345F0
    SEventRecord GetRecord() const;         // 0x00C34600 (hidden result)
};

struct CSoundEvent {                        // 0xB4 bytes, vtable 0x012B3674 (no RTTI)
    void* __vftable;                        // +0x00
    SEventLevel m_level;                    // +0x04
    unsigned char _pad0c[0x14 - 0x0c];
    long m_priority;                        // +0x14
    CSampleRef m_sample;                    // +0x18
    CSoundParams m_params;                  // +0x38
    CVirtualPlayer m_virtualPlayer;          // +0x78
    CPhysicalPlayer* m_pPhysicalPlayer;      // +0xA0
    CSoundChannel* m_pChannel;              // +0xA4
    unsigned long m_dirty;                  // +0xA8
    unsigned short m_state;                 // +0xAC
    unsigned char _padae[2];
    bool m_listening;                       // +0xB0
    unsigned char _padb1[3];
    void CheckState(long expected);         // 0x00C20720
    void SetPriority(CSoundEventManager* manager, long priority);  // 0x00C20CE0
    void StopListening(CSoundEventManager* manager); // 0x00C20810
    CSoundPlayer* GetActivePlayer();        // 0x00C20750
    void ResetLevel(CSoundEventManager* manager);
};

// Same-TU body of CheckState 0x00C20720 (retail compiles it above its callers): a switch on the state word whose
// cases are all empty. VC7.1 tracks the register use of callees compiled earlier in the TU, so the callers keep `this`
// in ecx across the call. (VC7.1 folds the empty switch to `ret 4` where retail keeps its jump table; only the
// function named on line 1 is graded from this file.)
#pragma auto_inline(off)
__declspec(noinline) void CSoundEvent::CheckState(long expected)
{
    switch (m_state) { case 0: case 1: case 2: case 3: case 4: case 5: case 6: break; }
}
#pragma auto_inline(on)

void CSoundEvent::SetPriority(CSoundEventManager* manager, long priority)
{
    CheckState(-1);
    if (priority == m_priority)
        return;
    if (m_state == 3) {
        m_pChannel->Stop(manager, m_priority);
        manager->m_waitingForPlayer.Remove(this);
    } else if (m_state == 4)
        manager->m_waitingForMemory.Remove(this);
    else if (m_state == 6)
        m_pChannel->Stop(manager, m_priority * 2);
    manager->m_byPriority.Remove(this);
    m_priority = priority;
    manager->m_byPriority.Add(this);
    if (m_state == 3) {
        m_pChannel->Start(manager, m_priority);
        manager->m_waitingForPlayer.Add(this);
    } else if (m_state == 4)
        manager->m_waitingForMemory.Add(this);
    else if (m_state == 6)
        m_pChannel->Start(manager, m_priority * 2);
}