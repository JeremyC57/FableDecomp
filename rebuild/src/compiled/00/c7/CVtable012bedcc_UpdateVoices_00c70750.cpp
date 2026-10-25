// CVtable_012bedcc::UpdateVoices(bool crossfade) -- under the effect's lock, record whether the next block cross-fades and rebuild the two voice records of the set being set up (the pending set when cross-fading, else the current one): both take the rear-louder-than-front output swap, then each is configured from its gain and tap descriptor
// Leaf: ?UpdateVoices@CVtable_012bedcc
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C70750, length 225 bytes (+ int3 padding), `ret 4`.
// Module: libpk's software mixer, the resampling voice effect of vtable 0x012BEDCC (no PDB/RTTI; named by vtable like
//   the landed siblings CVtable_012beda4 / CVtable_012bedb8). Its slot-3 SetParameters 0x00C70840 and slot-4
//   0x00C708A0 store the new gains / balance / tap descriptors and call this; Process 0x00C70480 (slot 2) fades the
//   current set out and the pending one in when the +0x24 flag is set, then swaps the +0x64c / +0x650 set pointers.
// Callees: EnterCriticalSection / LeaveCriticalSection (IAT 0x0143FE0C / 0x0143FE10) on the lock at +8,
//   SetupVoice 0x00C705B0 (thiscall, ret 0Ch: voice record, gain, tap descriptor).
// Layout: the 0x28-byte base (vptr, lock owner at +4 with its RTL_CRITICAL_SECTION at +8, volume +0x20, cross-fade
//   flag +0x24); the two 2-record voice sets at +0x1bc / +0x404 (0x124-byte records, output swap flag +0x121),
//   current / pending set pointers +0x64c / +0x650, per-record gains +0x654, front / rear balance +0x65c / +0x660,
//   per-record tap descriptors +0x664.
// Shape: each arm assigns `voices[0].swap = voices[1].swap = (rear > front)` through the set-pointer member (so the
//   pointer is reloaded after each byte store); VC7.1 hoists the common fld/fcomp/fnstsw above the branch and
//   tail-merges the second SetupVoice call.
struct _RTL_CRITICAL_SECTION;
extern "C" __declspec(dllimport) void __stdcall EnterCriticalSection(_RTL_CRITICAL_SECTION* section);
extern "C" __declspec(dllimport) void __stdcall LeaveCriticalSection(_RTL_CRITICAL_SECTION* section);

struct SVoiceTap;                                      // tap descriptor (word at +0x32: the fractional delay)

struct SResampleVoice {                                // 0x124 bytes
    unsigned char _pad000[0x121];
    bool          m_swapOutputs;                       // +0x121
    unsigned char _pad122[0x2];
};

struct CVtable_012beda4 {                              // vtable 0x012BEDA4, 0x28 bytes
    virtual void v00();
    void*          m_lockVtable;                       // +0x04
    _RTL_CRITICAL_SECTION* Lock() { return reinterpret_cast<_RTL_CRITICAL_SECTION*>(m_lockSection); }
    unsigned char  m_lockSection[0x18];                // +0x08
    float          m_volume;                           // +0x20
    bool           m_crossfade;                        // +0x24
    unsigned char  _pad25[0x3];
};

class CVtable_012bedcc : public CVtable_012beda4 {     // vtable 0x012BEDCC
public:
    unsigned char    _pad028[0x194];
    SResampleVoice   m_sets[2][2];                     // +0x1bc
    SResampleVoice*  m_pCurrent;                       // +0x64c
    SResampleVoice*  m_pPending;                       // +0x650
    float            m_gains[2];                       // +0x654
    float            m_front;                          // +0x65c
    float            m_rear;                           // +0x660
    const SVoiceTap* m_taps[2];                        // +0x664

    virtual void v00();
    void SetupVoice(SResampleVoice* voice, float gain, const SVoiceTap* tap);   // 0x00C705B0
    void UpdateVoices(bool crossfade);
};

void CVtable_012bedcc::UpdateVoices(bool crossfade)
{
    EnterCriticalSection(Lock());
    m_crossfade = crossfade;
    if (crossfade) {
        m_pPending[0].m_swapOutputs = m_pPending[1].m_swapOutputs = m_rear > m_front;
        SetupVoice(&m_pPending[0], m_gains[0], m_taps[0]);
        SetupVoice(&m_pPending[1], m_gains[1], m_taps[1]);
    } else {
        m_pCurrent[0].m_swapOutputs = m_pCurrent[1].m_swapOutputs = m_rear > m_front;
        SetupVoice(&m_pCurrent[0], m_gains[0], m_taps[0]);
        SetupVoice(&m_pCurrent[1], m_gains[1], m_taps[1]);
    }
    LeaveCriticalSection(Lock());
}