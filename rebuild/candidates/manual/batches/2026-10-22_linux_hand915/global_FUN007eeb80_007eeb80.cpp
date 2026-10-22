// _global::FUN_007eeb80() -- return the constant 0x4E
// Retail: Fable.exe (TLC, VC7.1) @ 0x007EEB80, 6 bytes, `mov eax,4Eh ; ret`. Unreferenced constant getter (sits
//   between CTCSmashableBarrier::Smash and CTCSmashableBase::IsAvailableToHit; likely a TC/type-id accessor);
//   modelled as a member returning a long.
struct COwner_007eeb80 {
    long FUN_007eeb80();
};

long COwner_007eeb80::FUN_007eeb80()
{
    return 0x4E;
}
