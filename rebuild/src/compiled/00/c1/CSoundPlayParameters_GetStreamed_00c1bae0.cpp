// CSoundPlayParameters::GetStreamed() const -- bit 1 of the flag byte at +2: whether the event plays streamed
// Leaf: ?GetStreamed@CSoundPlayParameters@@QBE_NXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C1BAE0, `xor eax,eax ; mov al,[ecx+2] ; shr eax,1 ; and eax,1 ; ret`.
// Identity (descriptive; libpk has no PDB): the getter twin of the landed SetStreamed 0x00C1BC30 (sets / clears bit 1 of +2); its
//   only caller CSoundEventManager::StartEvent 0x00C12940 passes it as CSoundEvent::Initialise's last (streamed) argument and
//   already binds this spelling. The catalog's CTCPhysicsStandard::CanCollideWithLandscape was a propagated label.
class CSoundPlayParameters
{ public:
    unsigned char pad0;
    unsigned char pad1;
    unsigned char flags;
    bool GetStreamed() const;
};

bool CSoundPlayParameters::GetStreamed() const
{
    return (flags >> 1) & 1;
}
