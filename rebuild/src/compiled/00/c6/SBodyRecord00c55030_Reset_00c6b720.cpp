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
class C3DVector { public: float X, Y, Z; };

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