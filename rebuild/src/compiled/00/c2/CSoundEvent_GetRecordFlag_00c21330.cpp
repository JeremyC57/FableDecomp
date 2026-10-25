// CSoundEvent::GetRecordFlag(long unused) -- false when the event's parameter-block mode (+0x48, CheckState-guarded getter 0x00C20EE0) is zero, else the flag byte (+0xC) of the 16-byte record copied out of the parameter block at +0x38 (CSoundParams::GetRecord 0x00C34600)
// Leaf: ?GetRecordFlag@CSoundEvent@@QBE_NJ@Z
// Naming (sweep1, 2026-10-25): libpk class set CSoundEventManager (vtable 0x012B2748) / CSoundEventSet (its +0x2C
//   member) / CSoundEvent (0xB4 bytes, vtable 0x012B3674): the describer 0x00C21040 called by TraceEvents/
//   TracePlaybacks prints "EventId(", "Priority=", "State='Waiting for data'" / "Playing using physical player";
//   chunk 431/435's CPanel/CPanelItem names came from propagated UI labels (renamed from 00/c2/CPlayer_IsFirstPersonTargeting_00c21330.cpp).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C21330, length 42 bytes.
// Identity: renamed (sweep1, 2026-10-25) from CPlayer::IsFirstPersonTargeting (the catalog label was doubtful: Ego_r.pdb
//   ?IsFirstPersonTargeting@CPlayer@@QBE_NXZ takes no argument while this body ends in `ret 4`). It is slot 8 of the
//   CSoundEvent vtable 0x012B3674 (0x012B3694 -> 0x00C21330), whose class is the libpk sound event (FINDINGS chunk 434,
//   agent434 NOTES: "a CSoundEvent query: parameter-block mode +0x48, then the record flag byte from GetRecord
//   0x00C34600"); the method name is descriptive. Callees: 0x00C20EE0 = `push -1 ; call CheckState 0x00C20720 ;
//   mov eax,[ecx+48h] ; ret` (manifest "CCombatActionBase::GetAnimationTransitionInTime" propagated); 0x00C34600 copies
//   the four dwords of the parameter block's record into its hidden result (manifest "GFToScreen" propagated).
// Callees: the +0x48 getter @ 0x00C20EE0, the record getter @ 0x00C34600 (`__fastcall`, hidden result).
// Shape: the record is a 16-byte local returned by value through the hidden pointer (`lea eax,[esp]
// ; push eax`), its byte +0xC is the result; the early `xor al,al` exit. Retail keeps `this` in ecx across the
// 0x00C20EE0 call: both that getter and its callee 0x00C20720 are compiled in the same TU above the caller and never
// touch ecx (VC7.1 callee register-usage knowledge), so their bodies are written out here.
struct SEventRecord {                            // 16 bytes, copied whole by the getter
    long          m_a;
    long          m_b;
    long          m_c;
    bool          m_flag;                          // +0x0c (the result)
    unsigned char _pad[3];
    SEventRecord() { }
};
struct CSoundParams {
    SEventRecord GetRecord() const;              // 0x00C34600
};

struct CSoundEvent {
    unsigned char        _pad00[0x38];
    CSoundParams m_params;                 // +0x38
    unsigned char        _pad39[0x48 - 0x39];
    long                 m_mode;                // +0x48
    unsigned char        _pad4c[0xac - 0x4c];
    unsigned short       m_state;                  // +0xac
    long GetMode() const;                       // 0x00C20EE0
    void CheckState(long state) const;             // 0x00C20720
    bool GetRecordFlag(long unused) const;
};

// Same-TU bodies of the two callees (retail's TU): 0x00C20720 switches on the word at +0xAC with every case
// returning at once; 0x00C20EE0 calls it with -1 and returns the long at +0x48. Neither touches ecx. GetMode
// reproduces 0x00C20EE0 byte for byte; CheckState is a stand-in (same behaviour, but VC7.1 folds the empty switch
// to `ret 4` where retail keeps its jump table).
__declspec(noinline) void CSoundEvent::CheckState(long state) const
{
    switch (m_state) { case 0: case 1: case 2: case 3: case 4: case 5: case 6: break; }
}
__declspec(noinline) long CSoundEvent::GetMode() const
{
    CheckState(-1);
    return m_mode;
}

bool CSoundEvent::GetRecordFlag(long unused) const
{
    if (!GetMode())
        return false;
    SEventRecord record = m_params.GetRecord();
    return record.m_flag;
}