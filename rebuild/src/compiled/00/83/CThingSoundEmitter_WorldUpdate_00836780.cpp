// CThingSoundEmitter::WorldUpdate() -- true when the byte state returned by the method @ 0x004C8050 equals 1
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00836780, oracle length 14 bytes.
// Grade: candidate -- generated for a repeated body shape and graded in the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); the batch README records the lane verdict.
//
// Retail bytes:  call 0x4c8050 ; dec al ; neg al ; sbb al, al ; inc al ; ret 
// Tiny-body shape `IsStateOne` synthesised from the instruction pattern (the class comes from the
// manifest label when it is a plain class name, else the method is landed under a descriptive
// placeholder); member names are descriptive. `this` passes through ecx untouched to the callee; the `dec/neg/sbb/inc` tail is VC's `== 1` on a byte.
class CThingSoundEmitter { public:
    unsigned char WorldUpdate();                   // a byte result: a `bool` return would widen through eax
};

// Naming (NF438 gen, 2026-11-25): callee-only rebind of this row's calls to their PDB spellings (T1 tracker; the target
//   self rows' Leaf / PDB name sets), through a bodiless declaration (no layout; rulings r2/r3):
//   0x004C8050 ?WorldUpdate@CThing@@UAE_NXZ (was ?GetState@CThingSoundEmitter@@QAEEXZ)
class CThing { public: virtual bool WorldUpdate(); };
unsigned char CThingSoundEmitter::WorldUpdate()
{
    return reinterpret_cast<CThing*>(this)->CThing::WorldUpdate() == 1;
}