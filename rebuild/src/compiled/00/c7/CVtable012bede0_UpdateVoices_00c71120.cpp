// CVtable_012bede0::UpdateVoices(bool crossfade) -- the filtered (0x2a4-byte record) twin of CVtable_012bedcc::UpdateVoices 0x00C70750: under the effect's lock, record whether the next block cross-fades and rebuild the two voice records of the set being set up (the pending set when cross-fading, else the current one): both take the rear-louder-than-front output swap, then each is configured from its gain and tap descriptor
// Leaf: ?UpdateVoices@CVtable_012bede0
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C71120, length 225 bytes (+ int3 padding), `ret 4`.
// Module: libpk's software mixer, the second resampling voice effect, vtable 0x012BEDE0 (no PDB/RTTI; named by vtable
//   like the landed siblings): the same layout as vtable 0x012BEDCC with 0x2a4-byte voice records (the record fields
//   +0x290..+0x2a2 are the 0x124-byte record's +0x110..+0x122 moved up by 0x180). Its slot-3 SetParameters
//   0x00C71210 and slot-4 0x00C71270 call this; its Process 0x00C70E50 (slot 2) mixes through 0x00C70C10 / 0x00C70CB0 /
//   0x00C70D60. Byte-identical to 0x00C70750 apart from the member offsets and SetupVoice 0x00C70F80.
// Callees: EnterCriticalSection / LeaveCriticalSection (IAT 0x0143FE0C / 0x0143FE10) on the lock at +8,
//   SetupVoice 0x00C70F80 (thiscall, ret 0Ch: voice record, gain, tap descriptor).
// Layout: the 0x28-byte base (vptr, lock owner at +4 with its RTL_CRITICAL_SECTION at +8, volume +0x20, cross-fade
//   flag +0x24); the two 2-record voice sets at +0x1bc / +0x704 (0x2a4-byte records, output swap flag +0x2a1),
//   current / pending set pointers +0xc4c / +0xc50, per-record gains +0xc54, front / rear balance +0xc5c / +0xc60,
//   per-record tap descriptors +0xc64.
// Shape: each arm assigns `voices[0].swap = voices[1].swap = (rear > front)` through the set-pointer member (so the
//   pointer is reloaded after each byte store); VC7.1 hoists the common fld/fcomp/fnstsw above the branch and
//   tail-merges the second SetupVoice call.
struct _RTL_CRITICAL_SECTION;
extern "C" __declspec(dllimport) void __stdcall EnterCriticalSection(_RTL_CRITICAL_SECTION* section);
extern "C" __declspec(dllimport) void __stdcall LeaveCriticalSection(_RTL_CRITICAL_SECTION* section);

struct SVoiceTap;                                      // tap descriptor (word at +0x32: the fractional delay)

struct SFilteredVoice {                                // 0x2a4 bytes
    unsigned char _pad000[0x2a1];
    bool          m_swapOutputs;                       // +0x2a1
    unsigned char _pad2a2[0x2];
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

class CVtable_012bede0 : public CVtable_012beda4 {     // vtable 0x012BEDE0
public:
    unsigned char    _pad028[0x194];
    SFilteredVoice   m_sets[2][2];                     // +0x1bc
    SFilteredVoice*  m_pCurrent;                       // +0xc4c
    SFilteredVoice*  m_pPending;                       // +0xc50
    float            m_gains[2];                       // +0xc54
    float            m_front;                          // +0xc5c
    float            m_rear;                           // +0xc60
    const SVoiceTap* m_taps[2];                        // +0xc64

    virtual void v00();
    void SetupVoice(SFilteredVoice* voice, float gain, const SVoiceTap* tap);   // 0x00C70F80
    void UpdateVoices(bool crossfade);
};

void CVtable_012bede0::UpdateVoices(bool crossfade)
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