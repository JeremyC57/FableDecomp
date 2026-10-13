#pragma optimize("s",on)
// CTCDHeroSuit::vfunc_28(long) -- true when the argument equals 22
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004E07D6, oracle length 13 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  xor eax,eax ; cmp dword ptr [esp+4],0x16 ; sete al ; ret 4
// Virtual predicate: compares its one argument with the constant 22 (0x16) and returns the
// result as a full-register integer (`xor eax,eax ; sete al`: an int/long result, a bool result
// would drop the xor). `this` is not
// read. The class comes from the vtable-slot label of this address. One of 10 bodies.
struct CTCDHeroSuit {
    virtual void v00();
    virtual long vfunc_28(long value);
};

long CTCDHeroSuit::vfunc_28(long value)
{
    return value == 22;
}