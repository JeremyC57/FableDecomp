// <unattributed>::ScaledQuery() -- Source(+0x04)->Query() * Owner(+0x00)->Scale(+0x10)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C46F40, oracle length 18 bytes.
// Grade: candidate -- authored and graded in the Linux lane (Wine + DDK 13.10.4035 cl.exe); the
//        batch README records the lane verdict.
// The manifest label for this body is propagated across byte-identical bodies and is NOT trusted;
// the class is modelled from the bytes only and the name below describes what the code does.
//
// Retail bytes:  push esi ; mov esi,ecx ; mov ecx,[esi+4] ; call Query ; mov eax,[esi] ; fmul dword ptr [eax+10h] ; pop esi ; ret
// A float from a no-argument __thiscall query @ 0x00C38510 on the object at +0x04, scaled by the
// float at +0x10 of the object at +0x00.
struct CScaleOwner {
    unsigned char _pad_0x00[0x10];
    float         Scale;                    // +0x10 (name from use)
};

struct CQuerySource {
    float Query();                          // 0x00C38510
};

struct CScaledQuery_Retail {
    CScaleOwner*  Owner;                    // +0x00
    CQuerySource* Source;                   // +0x04
    float ScaledQuery();
};

float CScaledQuery_Retail::ScaledQuery()
{
    return this->Source->Query() * this->Owner->Scale;
}
