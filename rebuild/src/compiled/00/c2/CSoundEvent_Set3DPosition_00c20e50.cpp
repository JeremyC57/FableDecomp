// CSoundEvent::Set3DPosition(const CUnprovenVector_00c345b0& position) -- settle the state, store the value in the parameter block and mark it dirty (bit 6) when it changed
// Leaf: ?Set3DPosition@CSoundEvent@@QAEXABVCUnprovenVector_00c345b0@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00c20e50, 38 bytes, thiscall, `ret 4`.
// Identity (descriptive): it stores through 0x00C345B0 (compare 0x00C20370, then copy into params+0, event+0x38), the 3D block the describer 0x00C21040 prints as "3D(" through 0x00C34600; dirty bits 6; callers the describer 0x00C21040 and 0x00C213C0. libpk: no PDB, no RTTI; CSoundEvent / ChangeState are the landed descriptive names
//   (CSoundEvent::CountInto 0x00C20F60).
// Callees: CSoundEvent::ChangeState(long) 0x00C20720 (-1 = no change), CUnprovenSoundParams_00c344d0::Set3DPosition 0x00C345B0 (thiscall on the
//   block at +0x38; nonzero when the value changed).
// Shape (family L125): `push -1` before `mov esi,ecx`, the argument passed straight through, `or dword ptr [esi+0A8h],6`.
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
    void Set3DPosition(const CUnprovenVector_00c345b0& position);
};

void CSoundEvent::Set3DPosition(const CUnprovenVector_00c345b0& position)
{
    ChangeState(-1);
    if (m_params.Set3DPosition(position))
        m_dirty |= 6;
}