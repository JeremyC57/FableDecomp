// CSoundEvent::SetUserGain(float gain) -- settle the state, store the value in the parameter block and mark it dirty (bit 2) when it changed
// Leaf: ?SetUserGain@CSoundEvent@@QAEXM@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00c20c80, 38 bytes, thiscall, `ret 4`.
// Identity (descriptive): it stores through 0x00C34500 (float compare-and-set of params+0x30, event+0x68), the field the event describer 0x00C21040 prints as "user gain("; dirty bit 2; caller 0x00C213C0. libpk: no PDB, no RTTI; CSoundEvent / ChangeState are the landed descriptive names
//   (CSoundEvent::CountInto 0x00C20F60).
// Callees: CSoundEvent::ChangeState(long) 0x00C20720 (-1 = no change), CUnprovenSoundParams_00c344d0::SetUserGain 0x00C34500 (thiscall on the
//   block at +0x38; nonzero when the value changed).
// Shape (family L125): `push -1` before `mov esi,ecx`, the argument passed straight through, `or dword ptr [esi+0A8h],2`.
class CUnprovenVector_00c345b0;
struct CUnprovenSoundParams_00c344d0 {                       // the event's parameter block (+0x38)
    long SetFloat38(float value);                         // 0x00C344D0
    long SetUserGain(float gain);                         // 0x00C34500
    long Set2DPosition(float pan);                        // 0x00C34580
    long Set3DPosition(const CUnprovenVector_00c345b0& position);   // 0x00C345B0
};

struct CSoundEvent {                             // 0xB4-byte manager event
    unsigned char  _pad00[0x38];
    CUnprovenSoundParams_00c344d0 m_params;              // +0x38
    unsigned char  _pad38[0xa8 - 0x38 - sizeof(CUnprovenSoundParams_00c344d0)];
    unsigned long  m_dirty;                     // +0xA8
    unsigned short m_state;                     // +0xAC
    void ChangeState(long state);               // 0x00C20720
    void SetUserGain(float gain);
};

void CSoundEvent::SetUserGain(float gain)
{
    ChangeState(-1);
    if (m_params.SetUserGain(gain))
        m_dirty |= 2;
}