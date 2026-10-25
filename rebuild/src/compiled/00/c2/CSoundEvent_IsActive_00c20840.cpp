// CSoundEvent::IsActive() const -- 1 when the event's state word (+0xAC) is non-zero (any state but idle), else 0
// Leaf: ?IsActive@CSoundEvent@@QBEJXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C20840, 13 bytes (`xor eax,eax ; cmp word ptr [ecx+0ACh],ax ; setne al ;
//   ret`), thiscall.
// Identity (sweep1, 2026-10-25): `this` is the 0xB4-byte libpk sound event (vtable 0x012B3674, state word +0xAC; the
//   states 2..6 are the describer 0x00C21040's "Waiting for data" .. "Playing using physical player", so 0 is idle).
//   Its 12 call sites (0x00C11CCE .. 0x00C12C74) are all CSoundEventManager walks over the +0x5C event pool -- e.g.
//   ReleaseTarget 0x00C12140, Shutdown 0x00C123B0, CountWaitingEvents 0x00C125E0, CountPlayingEvents 0x00C12680 --
//   which test or keep the whole of eax (`test eax,eax` at 0x00C11CD3 / 0x00C120F1, `mov esi,eax` at 0x00C12C79): a
//   four-byte result, so the method returns long, not bool. Unlike the other CSoundEvent queries (IsWaiting 0x00C20EF0,
//   IsPlayingPhysical 0x00C20F20) it does not call CheckState 0x00C20720 first. Descriptive name (no PDB covers libpk).
// Label correction: the old row was a free `bool __fastcall GetKeyPressed(S*)` over a padding struct (module CInput);
//   the catalog's "NUISystem::CManager::GetKeyPressed" is a label propagated from a byte-identical body.
struct CSoundEvent {                        // 0xb4 bytes
    unsigned char _pad[0xac];
    unsigned short m_state;                 // +0xAC: 0 idle, 2..4 waiting, 5/6 playing
    long IsActive() const;
};

long CSoundEvent::IsActive() const
{
    return m_state != 0;
}
