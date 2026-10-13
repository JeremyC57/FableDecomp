// <unattributed>::GetScaledDefValue() -- Def(+0x34)->Value(+0x28) * (1/360)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007445D0, oracle length 13 bytes.
// Grade: landed -- byte-exact (MATCH/RELOCATION_MATCH) and behaviour PASS under the Linux lane
//        (Wine + DDK 13.10.4035 cl.exe, 2026-10-13); the lander's winning pragma, if any, heads the file.
// The manifest label for this body is propagated across byte-identical bodies and is NOT trusted;
// the class is modelled from the bytes only and the name below describes what the code does.
//
// Retail bytes:  mov eax,[ecx+34h] ; fld dword ptr [eax+28h] ; fmul dword ptr [01238E00h] ; ret
// The real CTCPhysicsBase::GetAirResistance is landed at 0x00670000; this body reads a def pointer
// at +0x34 (CTCPhysicsBase keeps its def at +0x38), so it belongs to another component. The def's
// float at +0x28 (degrees per something) is scaled by the pool constant 1/360 at 0x01238E00.
// 0x01238E00 holds 0.0027777778f (1/360): a literal, so the compiler loads the def value first
// and multiplies by the constant pool (an extern would be loaded first).
#define DEGREES_TO_TURNS 0.0027777778f

struct CDefWithValue28 {
    unsigned char _pad_0x00[0x28];
    float         Value;                    // +0x28
};

struct CDefUser_Retail {
    unsigned char    _pad_0x00[0x34];
    CDefWithValue28* Def;                   // +0x34
    float GetScaledDefValue();
};

float CDefUser_Retail::GetScaledDefValue()
{
    return this->Def->Value * DEGREES_TO_TURNS;
}
