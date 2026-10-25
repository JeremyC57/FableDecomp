// CSoundEvent::AssignPhysicalPlayer(CSoundEventManager* manager, CPhysicalPlayer* player) -- state 3 -> 6: a physical player became free for an event waiting for one, so leave the waiting list (keeping the channel) and play through that player on that channel
// Leaf: ?AssignPhysicalPlayer@CSoundEvent@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C21750, 40 bytes (+8 int3), `__fastcall`, `ret 8`. Descriptive names (no PDB or
//   RTTI covers libpk; the class model is chunk 434's CSoundEvent: the 0xB4-byte libpk sound event, vtable 0x012B3674,
//   state word +0xAC, owner CSoundEventManager passed to every method). The manifest's
//   "basic_string<char>::compare" is propagated.
// Caller: the manager's player scheduler 0x00C10CFF (`call 0x00C21E50` hands back a free physical player; when non-null,
//   `event->AssignPhysicalPlayer(manager, player)`).
// Callees: CheckState @ 0x00C20720 (3 = "Waiting for physical player"), StopWaitingForPhysicalPlayer @ 0x00C209C0 (returns
//   the channel), PlayPhysical @ 0x00C20A20 (manager, channel, player; it sets state 6).
// Shape: the channel is a NAMED local, so StopWaitingForPhysicalPlayer runs before any PlayPhysical argument is pushed
//   (the nested-call form pushes the player first). The CheckState stand-in compiled earlier in the TU (chunk 434's lesson) lets VC7.1 keep
//   `this` in ecx across that call into StopWaitingForPhysicalPlayer, as retail's LTCG build does.
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
    CSoundChannel* StopWaitingForPhysicalPlayer(CSoundEventManager* manager);   // 0x00C209C0
    void PlayPhysical(CSoundEventManager* manager, CSoundChannel* channel, CPhysicalPlayer* player);   // 0x00C20A20
    void AssignPhysicalPlayer(CSoundEventManager* manager, CPhysicalPlayer* player);
};


// Same-TU body of CheckState 0x00C20720 (retail compiles it above its callers): a switch on the state word whose
// cases are all empty. VC7.1 tracks the register use of callees compiled earlier in the TU, so the caller keeps `this`
// in ecx across the call. (Only the function named on line 1 is graded from this file.)
#pragma auto_inline(off)
__declspec(noinline) void CSoundEvent::CheckState(long expected)
{
    switch (m_state) { case 0: case 1: case 2: case 3: case 4: case 5: case 6: break; }
}
#pragma auto_inline(on)

void CSoundEvent::AssignPhysicalPlayer(CSoundEventManager* manager, CPhysicalPlayer* player)
{
    CheckState(3);
    CSoundChannel* channel = StopWaitingForPhysicalPlayer(manager);
    PlayPhysical(manager, channel, player);
}