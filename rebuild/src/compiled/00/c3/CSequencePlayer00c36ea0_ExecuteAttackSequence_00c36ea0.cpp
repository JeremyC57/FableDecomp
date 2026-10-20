// CSequencePlayer_00c36ea0::ExecuteAttackSequence(long restart) -- (re)start the step list on the clock, then for every step build its (time, item) key; when the owner has a track for it, play the track at the key's position with Z = track time + track start for (end - start), and on a restart finish the track at its final step; then end the sequence
// Leaf: ?ExecuteAttackSequence@CSequencePlayer_00c36ea0
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C36EA0, length 265 bytes (`ret 4`; 8-aligned ebp frame for the
//          __int64 time stamps). Only caller 0x00C372A5.
// Callees (all labels in the manifest are propagated and unrelated): Restart 0x00C36DB0, Begin(&clock) 0x00C36E00,
//          step count 0x00C463B0 / item 0x00C463C0 / final-step test 0x00C463E0 on the list at +0xc, the clock's time
//          getter 0x00C25790 (hidden result), the __fastcall key builder 0x00C46E70 (out, time, item), the key's position
//          getter 0x00C46EB0 (hidden result), the owner's (+0) track lookup 0x00C2A340 and track time 0x00C2A390 (float),
//          the track's Play(position, duration) 0x00C36220 and Finish(index) 0x00C36490, End 0x00C36E50.
// Identity: the catalog name "CActionSummonerLightningAttackTarget::ExecuteAttackSequence" is propagated (the
//          neighbouring landed rows belong to particle-morph code); the class is named by address and every member is
//          descriptive -- review at landing.
// Shape: the time and the position come from inline getters that return a NAMED local (VC7.1 has no NRVO: the
//          out-of-line result lands in a temporary that is then copied, as retail does); the key is filled through an
//          explicit out pointer (ecx) by a __fastcall helper; the restart flag is a 32-bit argument re-read each pass.
struct CSequenceTime {                          // 8-byte time stamp (8-aligned)
    __int64 Ticks;
};
struct C3DVector { float x, y, z; };
struct CSequenceItem;
struct CSequenceKey {                           // the (time, item) key built per step
    __int64 Packed;
    C3DVector GetPositionImpl() const;          // 0x00C46EB0 (hidden result pointer)
    C3DVector GetPosition() const { C3DVector p = GetPositionImpl(); return p; }
};
void __fastcall MakeSequenceKey(CSequenceKey* out, const CSequenceTime& time, CSequenceItem* item);   // 0x00C46E70
struct CSequenceClock {
    CSequenceTime GetTimeImpl() const;          // 0x00C25790 (hidden result pointer)
    CSequenceTime GetTime() const { CSequenceTime t = GetTimeImpl(); return t; }
};
struct CSequenceList {
    unsigned long GetCount() const;             // 0x00C463B0
    CSequenceItem* GetItem(unsigned long index) const;   // 0x00C463C0
    long IsFinalItem(unsigned long index) const;         // 0x00C463E0
};
struct CSequenceTrack {
    unsigned char _pad[0xc];
    float Start;                                // +0x0c
    float End;                                  // +0x10
    void Play(const C3DVector& position, float duration);   // 0x00C36220
    void Finish(unsigned long index);           // 0x00C36490
};
struct CSequenceOwner {
    CSequenceTrack* FindTrack(const CSequenceKey& key);   // 0x00C2A340
    float GetTrackTime(const CSequenceKey& key);          // 0x00C2A390
};
struct CSequencePlayer_00c36ea0 {
    CSequenceOwner* Owner;                      // +0x00
    unsigned char   _pad04[8];
    CSequenceList*  Steps;                      // +0x0c
    unsigned char   _pad10[0x30];
    CSequenceClock  Clock;                      // +0x40
    void Restart();                             // 0x00C36DB0
    void Begin(CSequenceClock* clock);          // 0x00C36E00
    void End();                                 // 0x00C36E50
    void ExecuteAttackSequence(long restart);
};

void CSequencePlayer_00c36ea0::ExecuteAttackSequence(long restart)
{
    if (restart)
        Restart();
    Begin(&Clock);
    for (unsigned long i = 0; i < Steps->GetCount(); ++i) {
        CSequenceTime time = Clock.GetTime();
        CSequenceKey key;
        MakeSequenceKey(&key, time, Steps->GetItem(i));
        CSequenceTrack* track = Owner->FindTrack(key);
        if (track) {
            C3DVector position = key.GetPosition();
            position.z = Owner->GetTrackTime(key) + track->Start;
            track->Play(position, track->End - track->Start);
            if (restart && Steps->IsFinalItem(i))
                track->Finish(i);
        }
    }
    End();
}