// CNavigatorAStar_0059b2cc::MoveToNavLineOfSightProximity(SParams a, SParams b, SParams c) -- forward three 16-byte parameter records (copied into the argument area with `movsd`) and a trailing 0 to the route helper (0x0059AFD7, `__fastcall` with `this` and an empty tag in the high byte of the last record's slot), returning `this`
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0059B2CC, length 63 bytes.
// Leaf: MoveToNavLineOfSightProximity
// Shape: a frame-pointer module (`optimize("y",off)`), the records copied last to first, `this` kept in ebx for the return, `ret 30h`.
#pragma optimize("s", on)
#pragma optimize("y", off)

struct SParams_0059b2cc { long m_0, m_4, m_8, m_c; };
struct STag_0059b2cc { STag_0059b2cc() { } };
struct CNavigatorAStar_0059b2cc;
void __fastcall MoveWithParams_0059afd7(CNavigatorAStar_0059b2cc* navigator, const STag_0059b2cc& tag, SParams_0059b2cc a, SParams_0059b2cc b, SParams_0059b2cc c, long flags);   // 0x0059AFD7

struct CNavigatorAStar_0059b2cc {
    CNavigatorAStar_0059b2cc* MoveToNavLineOfSightProximity(SParams_0059b2cc a, SParams_0059b2cc b, SParams_0059b2cc c);
};

CNavigatorAStar_0059b2cc* CNavigatorAStar_0059b2cc::MoveToNavLineOfSightProximity(SParams_0059b2cc a, SParams_0059b2cc b, SParams_0059b2cc c)
{
    MoveWithParams_0059afd7(this, STag_0059b2cc(), a, b, c, 0);
    return this;
}