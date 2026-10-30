// CSoundEvent::Update(CSoundEventManager* manager, unsigned long changes) -- per-frame event update: after the any-state check poll the tracked parameter sources, merge the manager's change bits into the event's; with changes pending push gain (bit 2), pitch (bit 1) and the 3D record or distance (bit 4) to the player that carries the event, then on a gain/3D change move between virtual and physical playback (positive gain while virtual: restart; no gain while waiting or physical: go virtual); the change bits are cleared
// Leaf: ?Update@CSoundEvent@@QAEXPAUCSoundEventManager@@K@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C21A50, 325 bytes, thiscall `ret 8`; 0x10-byte frame (the SEventRecord local at
//   [esp+10h]; the distance float homed in the dead `manager` slot).
// Class: CSoundEvent, the 0xB4-byte libpk sound event (vtable 0x012B3674, state word +0xAC, change bits +0xA8, parameter
//   block +0x38, embedded virtual player +0x78 with vtable 0x012B37DC): every callee here is a CSoundEvent method already
//   landed under that class (GetActivePlayer 0x00C20750, HasPositiveGain 0x00C20CB0, IsWaitingOrPlayingPhysical 0x00C207C0,
//   ReturnToIdle 0x00C21560, PlayVirtual 0x00C20B00) or a CSoundParams method on +0x38 (GetPitch 0x00C34410, GetGain
//   0x00C34440, GetDistance 0x00C345F0, GetRecord 0x00C34600). Its only caller is the manager's event loop 0x00C128C0
//   (`push manager->+0x7C8 ; push manager ; mov ecx,event`). Names descriptive (libpk has no PDB/RTTI); the manifest label
//   "Display_UpdateWithFlags" is propagated.
// Callees not landed: UpdateTrackedParameters 0x00C213C0 (polls the manager's +0x7CC source for the +0x60/+0x68/+0x70 values
//   and the record, then calls the per-value setters), Play 0x00C21780 (the state-1 start: wait for data, physical or
//   virtual); IsPlayingVirtual 0x00C207A0 (state == 5, landed under the placeholder CUnprovenStateObject_00C20720).
// Shape: retail's TU holds CheckState / GetActivePlayer / IsPlayingVirtual above this body, so VC7.1 knows they leave ecx
//   and edx alone: `this` stays in ecx after CheckState and IsPlayingVirtual, the merged change word stays in edx across
//   GetActivePlayer. They are same-TU stand-ins here (the GetActivePlayer 0x00C20750 / ReturnToIdle 0x00C21560 pattern);
//   only the Update leaf is this row.
struct CSoundEventManager;

struct SEventRecord {                       // 16 bytes: three words and a flag byte
    void* m_a;
    void* m_b;
    void* m_c;
    unsigned char m_flag;
    SEventRecord();                         // 0x00C20390 (zeroes all four)
};

struct CSoundPlayer {                       // polymorphic player: the embedded virtual one (+0x78) or the physical one (+0xA0)
    virtual void SetPitch(float pitch);                                 // slot 0 (+0x00)
    virtual void SetGain(CSoundEventManager* manager, float gain);      // slot 1 (+0x04)
    virtual void ReadLevel(CSoundEventManager* manager, void* level);   // slot 2 (+0x08)
    virtual void Release();                 // slot 3 (+0x0C; the virtual player's 0x00C35440 clears its +8 handle)
    virtual void Update();                  // slot 4 (+0x10)
    virtual void Finish();                  // slot 5 (+0x14; the virtual player's 0x00C35450 sets +0x10 = 3, clears +8)
    virtual void SetDistance(float distance);       // slot 6 (+0x18)
    virtual void SetRecord(SEventRecord* record);   // slot 7 (+0x1C)
};

struct CSoundParams {                       // +0x38
    unsigned char _pad00[0x10];
    long m_mode;                            // +0x10 (event +0x48)
    unsigned char _pad14[0x40 - 0x14];
    float GetPitch(CSoundEventManager* manager);   // 0x00C34410
    float GetGain(CSoundEventManager* manager);    // 0x00C34440
    float GetDistance();                    // 0x00C345F0
    SEventRecord GetRecord() const;         // 0x00C34600 (hidden result)
};

struct CSoundEvent {                        // 0xB4 bytes, vtable 0x012B3674 (12 slots, no RTTI)
    virtual void* GetListNode();            // slot 0 (0x00C21D50: `lea eax,[ecx+0Ch]`; descriptive; slots 1..11 not modelled)
    unsigned char _pad04[0x38 - 0x04];
    CSoundParams m_params;                  // +0x38
    unsigned char m_virtualPlayer[0x28];    // +0x78 (embedded virtual player, vtable 0x012B37DC)
    void* m_pPhysicalPlayer;                // +0xA0
    void* m_pChannel;                       // +0xA4
    unsigned long m_dirty;                  // +0xA8
    unsigned short m_state;                 // +0xAC
    unsigned short m_trackFlags;            // +0xAE
    bool m_listening;                       // +0xB0
    unsigned char _padb1[3];
    void CheckState(long expected);         // 0x00C20720
    CSoundPlayer* GetActivePlayer();        // 0x00C20750
    long IsPlayingVirtual();                // 0x00C207A0
    long IsWaitingOrPlayingPhysical();      // 0x00C207C0
    void PlayVirtual(CSoundEventManager* manager);              // 0x00C20B00
    long HasPositiveGain(CSoundEventManager* manager);          // 0x00C20CB0
    void UpdateTrackedParameters(CSoundEventManager* manager);  // 0x00C213C0
    void ReturnToIdle(CSoundEventManager* manager);             // 0x00C21560
    void Play(CSoundEventManager* manager);                     // 0x00C21780
    void Update(CSoundEventManager* manager, unsigned long changes);
};

// Same-TU stand-ins of the retail helpers compiled above this body (retail: CheckState is a byte-indexed jump table over
// the state word whose cases are all empty; GetActivePlayer and IsPlayingVirtual are the landed bodies). They let VC7.1
// keep `this` in ecx and the change word in edx across the calls, as retail does.
#pragma auto_inline(off)
__declspec(noinline) void CSoundEvent::CheckState(long expected)
{
    switch (m_state) { case 0: case 1: case 2: case 3: case 4: case 5: case 6: break; }
}
__declspec(noinline) CSoundPlayer* CSoundEvent::GetActivePlayer()
{
    CheckState(-1);
    switch (m_state) {
    case 3: case 4: case 5:
        return (CSoundPlayer*)m_virtualPlayer;
    case 6:
        return (CSoundPlayer*)m_pPhysicalPlayer;
    case 1: case 2:
        return 0;
    }
    return 0;
}
__declspec(noinline) long CSoundEvent::IsPlayingVirtual()
{
    CheckState(-1);
    return m_state == 5;
}
#pragma auto_inline(on)

void CSoundEvent::Update(CSoundEventManager* manager, unsigned long changes)
{
    CheckState(-1);
    UpdateTrackedParameters(manager);
    if ((m_dirty |= changes) == 0)
        return;
    CSoundPlayer* player = GetActivePlayer();
    if (player) {
        if (m_dirty & 2)
            player->SetGain(manager, m_params.GetGain(manager));
        if (m_dirty & 1)
            player->SetPitch(m_params.GetPitch(manager));
        if (m_dirty & 4) {
            if (m_params.m_mode) {
                player->SetGain(manager, m_params.GetGain(manager));
                SEventRecord record = m_params.GetRecord();
                player->SetRecord(&record);
            } else {
                float distance = m_params.GetDistance();
                player->SetDistance(distance);
            }
        }
    }
    if (m_dirty & 6) {
        CheckState(-1);
        if (HasPositiveGain(manager)) {
            if (IsPlayingVirtual()) {
                ReturnToIdle(manager);
                Play(manager);
            }
        } else if (IsWaitingOrPlayingPhysical()) {
            ReturnToIdle(manager);
            PlayVirtual(manager);
        }
    }
    m_dirty = 0;
}