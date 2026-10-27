// CSequencePlayer_00c36ea0::Update(long force) -- per-tick update of the libpk sequence player: refresh the bound float variable when flagged, compute the target clock position into a local, and (steps present, owner not paused) re-run the sequence when forced or the target moved -- restarting it when forced or its time key changed -- then post the update state
// Grade: DIFFER(242v255) -- zero register only: VC7.1 here keeps 0 in ebx (pushed at entry; `cmp al,bl`, `mov [esi+4Ch],bl`, three `mov [esp+..],ebx`, `cmp [esi+0Ch],ebx`, `cmp byte [edx+0CDh],bl`) and the force argument in ebp, where retail uses immediates and `test` on loaded values (`mov eax,[esi+0Ch] ; test eax,eax`, `mov al,[edx+0CDh] ; test al,al`) with force in a lazily pushed ebx; every other instruction matches. optimize("g",off) is worse (236).
// Leaf: ?Update@CSequencePlayer_00c36ea0@@QAEXJ@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C371C0, 255 bytes, thiscall `ret 4`, esp frame (0x38 bytes of locals).
// Identity (descriptive; libpk has no PDB or RTTI): the only caller of the landed CSequencePlayer_00c36ea0::
//   ExecuteAttackSequence 0x00C36EA0 (call at 0x00C372A5) and of the landed Post 0x00C36FB0 / the compute step 0x00C36B90;
//   it updates the same +0x40 clock (12 bytes) ExecuteAttackSequence reads. Called once, from 0x00C2A479. The catalog's
//   `CTCTavernGame_OnStateUpdate` is propagated.
// Callees: UpdateFloatVariable 0x00C37050 and Post 0x00C36FB0 (landed on the descriptive classes CScriptVar_00c37050 /
//   CUnprovenPKAgent_00c36fb0 over this same object: reached through casts so each keeps its one spelling), the compute
//   step 0x00C36B90 (unlanded, descriptive), C3DVector::operator!= 0x00C25730 (`__fastcall` member), the clock's time
//   getter 0x00C25790 (CSequenceClock::GetTimeImpl, hidden result) and the time compare 0x00C46DC0 (`__fastcall`),
//   ExecuteAttackSequence 0x00C36EA0.
// Shape: the time keys come from the inline named-local GetTime() of the landed 0x00C36EA0 (the out-of-line result is
//   copied); `operator==(target time, clock time)` evaluates the clock first (right to left).
class C3DVector { public:
    float x, y, z;
    bool __fastcall operator!=(const C3DVector& other) const;  // 0x00C25730 (the caller tests al: bool)
};

struct CSequenceTime {                          // the 8-byte time key (two words)
    long  Index;
    float Value;
};
bool __fastcall operator==(const CSequenceTime& a, const CSequenceTime& b);   // 0x00C46DC0

struct CSequenceClock : public C3DVector {      // the 12-byte clock position (+0x40)
    CSequenceTime GetTimeImpl() const;          // 0x00C25790 (hidden result pointer)
    CSequenceTime GetTime() const { CSequenceTime t = GetTimeImpl(); return t; }
};

struct SSequenceUpdate_00c371c0 {               // the 12-byte state handed to the compute step and posted
    long m_a;
    long m_b;
    long m_c;
    SSequenceUpdate_00c371c0() : m_a(0), m_b(0), m_c(0) { }
};

struct CSequenceOwner {
    unsigned char _pad[0xcd];
    bool          m_bPaused;                    // +0xcd
};
struct CSequenceList;

struct CScriptVar_00c37050 {
    void UpdateFloatVariable();                 // 0x00C37050
};
struct CUnprovenPKAgent_00c36fb0 {
    void Post(void* argument);                  // 0x00C36FB0
};

struct CSequencePlayer_00c36ea0 {
    CSequenceOwner* Owner;                      // +0x00
    unsigned char   _pad04[8];
    CSequenceList*  Steps;                      // +0x0c
    unsigned char   _pad10[0x30];
    CSequenceClock  Clock;                      // +0x40
    bool            m_bVariableDirty;           // +0x4c
    void ComputeTarget_00c36b90(CSequenceClock* target, SSequenceUpdate_00c371c0* state);   // 0x00C36B90
    void ExecuteAttackSequence(long restart);   // 0x00C36EA0
    void Update(long force);
};

void CSequencePlayer_00c36ea0::Update(long force)
{
    if (m_bVariableDirty) {
        m_bVariableDirty = false;
        ((CScriptVar_00c37050*)this)->UpdateFloatVariable();
    }
    SSequenceUpdate_00c371c0 state;
    CSequenceClock target;
    ComputeTarget_00c36b90(&target, &state);
    if (Steps == 0)
        return;
    if (!Owner->m_bPaused) {
        if (force || target != Clock) {
            long restart = 0;
            if (force || !(target.GetTime() == Clock.GetTime()))
                restart = 1;
            Clock = target;
            ExecuteAttackSequence(restart);
        }
    }
    ((CUnprovenPKAgent_00c36fb0*)this)->Post(&state);
}
