// CForwarder_005790e7::Forward(void*, void*) -- frame-pointer forwarder to the __thiscall helper @ 0x00578CF7 with an empty tag (homed in the dead second slot) and a zero; the second argument is unused
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005790E7, oracle length 21 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push ebp ; mov ebp,esp ; push 0 ; lea eax,[ebp+0Bh] ; push eax ; push [ebp+8] ; call Helper ; pop ebp ; ret 8
// A two-argument forwarder in a frame-pointer translation unit: the first argument and a zero
// are passed to the __thiscall helper @ 0x00578CF7 together with a 1-byte empty tag
// temporary that VC homes in the high byte of the (dead) second argument's slot; the helper's
// result rides through eax. The manifest label (`_Copy_impl<CArray<...>>`) is propagated.
// One of 46 bodies of this shape.
#pragma optimize("y", off)              // frame pointer kept in this TU

struct SScalarTag { SScalarTag() { } };   // user constructor: no zero-initialisation of the 1-byte temporary

struct CForwarder_005790e7 {
    void* Helper(void* first, const SScalarTag& tag, long zero);   // 0x00578CF7
    void* Forward_CForwarder_005790e7(void* first, void* unused);
};

void* CForwarder_005790e7::Forward_CForwarder_005790e7(void* first, void* unused)
{
    return Helper(first, SScalarTag(), 0);
}
