// Behaviour test for 00c6b720: src/00c6b720.cpp copied verbatim; Reset copies the rest vector to the current one and
// zeroes the rate, leaving the rest vector untouched.
#include <cstdio>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
// ---- source (verbatim) ----
// SBodyRecord_00c55030::Reset() -- put the record back at its rest vector (+0x0c -> +0x00) and zero the rate vector at +0x18
// Retry 10b (statement-permutation sweep): the rest-vector copy sits between the rate's X and Y zero stores, which gives retail's register allocation (edx source, esi destination, edi carrier, the zero hoisted into eax).
// Leaf: Reset
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C6B720, length 37 bytes; 13 callers in the 0x00C5xxxx module, e.g. twice from the
// landed CBodyOwner_00c55030::Reset (0x00C55030), which names this callee SBodyRecord_00c55030::Reset on its 0x24-byte records at
// +0xdc / +0x100. The chunk label "CTCPhysicsRigidBody::GetFacingVector" is a propagated BSim name.
// Callees: none.
// Shape: the 12-byte vector copy through a `lea edx,[ecx+0Ch]` source base and a `mov esi,ecx` destination with edi as the
// carrier (the implicit assignment of a POD C3DVector), then one `xor eax,eax` feeding three ascending zero stores.
// Members are descriptive (record of three vectors; the class is unidentified, named after the landed caller).
struct C3DVector { float X, Y, Z; };

struct SBodyRecord_00c55030 {
    C3DVector m_current;                           // +0x00
    C3DVector m_rest;                              // +0x0c
    C3DVector m_rate;                              // +0x18
    void Reset();
};

void SBodyRecord_00c55030::Reset()
{
    m_rate.X = 0.0f;
    m_current = m_rest;
    m_rate.Y = 0.0f;
    m_rate.Z = 0.0f;
}
// ---- end source ----
int main()
{
    SBodyRecord_00c55030 r;
    r.m_current.X = 1; r.m_current.Y = 2; r.m_current.Z = 3;
    r.m_rest.X = 4; r.m_rest.Y = 5; r.m_rest.Z = 6;
    r.m_rate.X = 7; r.m_rate.Y = 8; r.m_rate.Z = 9;
    r.Reset();
    CHECK(r.m_current.X == 4 && r.m_current.Y == 5 && r.m_current.Z == 6);
    CHECK(r.m_rest.X == 4 && r.m_rest.Y == 5 && r.m_rest.Z == 6);
    CHECK(r.m_rate.X == 0 && r.m_rate.Y == 0 && r.m_rate.Z == 0);
    if (fails == 0) printf("00c6b720_TEST PASS\n"); else printf("00c6b720_TEST FAIL\n");
    return fails;
}