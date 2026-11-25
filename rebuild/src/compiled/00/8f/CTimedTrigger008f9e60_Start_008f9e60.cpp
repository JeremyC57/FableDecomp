// CTimedTrigger_008f9e60::Start(long duration) -- stamp the start time (GetTickCount) at +0x100, store the duration at +0x104, then run the trigger check at once
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008F9E60, length 36 bytes.
// Leaf: Start
// Callees: KERNEL32 GetTickCount through its import slot (call [0x0143FDFC]), the check @ 0x008F9CA0 (__fastcall on the same
// object; manifest "MusicTimer_CheckAndTrigger", it compares the float at +0x108 with 0.0f).
// The chunk label "CTCKraken::StartHighAttackBeam" is propagated: the body has no direct callers and CTCKraken is only 0x78
// bytes, while this object holds members at +0x100..+0x108; the class is named by address. Shape: the import result is
// stored first, then the argument; `this` parked in esi for the final call (no tail jump: retail keeps `call ; ret 4`).
extern "C" __declspec(dllimport) unsigned long __stdcall GetTickCount(void);

struct CTimedTrigger_008f9e60 {
    unsigned char _pad_0x00[0x100];
    unsigned long m_startTick;                     // +0x100
    long          m_duration;                      // +0x104
    float         m_level;                         // +0x108
    void Start(long duration);
};

// Naming (NF443 gen, 2026-11-25): callee-only rebind of this row's calls to their PDB spellings (T1 tracker; the target
//   self rows' Leaf / PDB name sets), through a bodiless declaration or the row's own model (no layout; rulings r2/r3, NF438 q1 friend):
//   0x008F9CA0 ?Stop@LTimer@LHNetworkLib@@QAEXXZ (was ?CheckAndTrigger@CTimedTrigger_008f9e60@@QAEXXZ)
namespace LHNetworkLib { class LTimer { public: void Stop(); }; }
void CTimedTrigger_008f9e60::Start(long duration)
{
    m_startTick = GetTickCount();
    m_duration = duration;
    reinterpret_cast<LHNetworkLib::LTimer*>(this)->Stop();
}