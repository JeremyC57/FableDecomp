// CUnprovenReverbParameters::SetRoomHF(long value) -- store the RoomHF level (millibels) and mark it dirty (bit 0x40 of the flags word at +0)
// Leaf: ?SetRoomHF@CUnprovenReverbParameters@@QAEXJ@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C53420, 13 bytes, thiscall `ret 4`: `mov eax,[esp+4] ; mov [ecx+8h],eax ; or dword ptr [ecx],40h`.
// Identity: no reference (an out-of-line member nobody calls). It opens the run of setters over the same dirty-flag block in this
//   TU: 0x00C53410/20/30 (+4/+8/+0xc, bits 0x20/0x40/0x80, plain longs), then the landed clamped float setters 0x00C53440
//   (+0x10, bit 0x100, [0.1, 20]), 0x00C534A0 (+0x14, 0x200), 0x00C53500 (+0x18, 0x400), 0x00C53560 (+0x24, 0x1000), the landed
//   CUnprovenReverbParameters::SetClamped28 0x00C535C0 (+0x28, 0x2000) ... SetClamped64 0x00C53A40 (+0x64, 0x1). Each dirty bit is
//   1 << the EAX 3.0 listener property id of the value: bit 0x100 = 1 << DECAYTIME (8) on the float clamped to EAX's decay-time
//   range [0.1, 20], so bit 0x40 = 1 << 6 (DSPROPERTY_EAX30LISTENER_ROOMHF): these three are the integer room levels.
// libpk/GameCODA has no PDB: the class keeps the landed descriptive CUnprovenReverbParameters (0x00C535C0..0x00C53A40); the member
//   and method names follow the EAX property the dirty bit names (descriptive).
class CUnprovenReverbParameters {
public:
    unsigned long DirtyFlags;                       // +0x00
    long          Room;                             // +0x04 (bit 0x20)
    long          RoomHF;                           // +0x08 (bit 0x40)
    long          RoomLF;                           // +0x0c (bit 0x80)
    void SetRoomHF(long value);
};

void CUnprovenReverbParameters::SetRoomHF(long value)
{
    RoomHF = value;
    DirtyFlags |= 0x40;
}