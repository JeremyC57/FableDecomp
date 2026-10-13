// CThingFilter_IsHuman::operator()(CThingCreatureBase*)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006A7B90, oracle length 18 bytes.
// Grade: candidate (authored from the retail bytes; byte parity NOT yet proven -- run
//        tools/decomp_pipeline/verify_and_land.py on this batch's payload to grade it).
//
// Retail bytes:  mov ecx,[esp+4] ; call <creature->IsHuman> ; neg eax ; sbb eax,eax ; neg eax ; ret 4
// Thing-search predicate: true for human creatures. The callee at 0x00662000 is __thiscall on
// the creature with no stack arguments (its manifest label is a propagated byte-identical name
// and is not trusted). The neg/sbb/neg idiom is VC7.1's `x != 0` normalisation into a 32-bit
// result, so the predicate returns an int-sized boolean (long), not a `bool` in al.

// Call-surface view of CThingCreatureBase (engine/CThingCreatureBase.h carries the layout only).
struct CThingCreatureBaseCalls {
    long IsHuman();                       // 0x00662000 (__thiscall, no args)
};

struct CThingFilter_IsHuman {
    long operator()(CThingCreatureBaseCalls* creature);
};

long CThingFilter_IsHuman::operator()(CThingCreatureBaseCalls* creature)
{
    return creature->IsHuman() != 0;
}
