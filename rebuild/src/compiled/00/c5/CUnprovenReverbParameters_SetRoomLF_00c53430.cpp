// CUnprovenReverbParameters::SetRoomLF(long value) -- store the RoomLF level (millibels) and mark it dirty (bit 0x80 of the flags word at +0)
// Leaf: ?SetRoomLF@CUnprovenReverbParameters@@QAEXJ@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C53430, 16 bytes, thiscall `ret 4`: `mov eax,[esp+4] ; mov [ecx+0Ch],eax ; or dword ptr [ecx],80h`.
// Identity: no reference (an out-of-line member nobody calls). It opens the run of setters over the same dirty-flag block in this
//   TU: 0x00C53410/20/30 (+4/+8/+0xc, bits 0x20/0x40/0x80, plain longs), then the landed clamped float setters 0x00C53440
//   (+0x10, bit 0x100, [0.1, 20]), 0x00C534A0 (+0x14, 0x200), 0x00C53500 (+0x18, 0x400), 0x00C53560 (+0x24, 0x1000), the landed
//   CUnprovenReverbParameters::SetClamped28 0x00C535C0 (+0x28, 0x2000) ... SetClamped64 0x00C53A40 (+0x64, 0x1). Each dirty bit is
//   1 << the EAX 3.0 listener property id of the value: bit 0x100 = 1 << DECAYTIME (8) on the float clamped to EAX's decay-time
//   range [0.1, 20], so bit 0x80 = 1 << 7 (DSPROPERTY_EAX30LISTENER_ROOMLF): these three are the integer room levels.
// libpk/GameCODA has no PDB: the class keeps the landed descriptive CUnprovenReverbParameters (0x00C535C0..0x00C53A40); the member
//   and method names follow the EAX property the dirty bit names (descriptive).
class CUnprovenReverbParameters {
public:
    unsigned long DirtyFlags;                       // +0x00
    long          Room;                             // +0x04 (bit 0x20)
    long          RoomHF;                           // +0x08 (bit 0x40)
    long          RoomLF;                           // +0x0c (bit 0x80)
    void SetRoomLF(long value);
};

void CUnprovenReverbParameters::SetRoomLF(long value)
{
    RoomLF = value;
    DirtyFlags |= 0x80;
}