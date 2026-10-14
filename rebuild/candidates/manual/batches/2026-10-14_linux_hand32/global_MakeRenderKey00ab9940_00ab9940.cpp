// MakeRenderKey_00ab9940(const SKey4*, long, long, long) -- build a seven-word key from the four words of the object in ecx, the edx argument and two stack arguments, returned by value through the hidden pointer
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00AB9940, length 77 bytes.
// Callees: none. Shape: a `__fastcall` function returning a 28-byte POD -- the hidden pointer is the first stack slot, the two remaining words follow; the key is
// assembled field by field in a 0x1c-byte local (no named-return-value optimisation in VC7.1) and copied with `rep movsd` (7 dwords); `ret 0Ch`. The chunk
// label "CEngineInternalPrimitive2DStaticMesh::GetObjectTransform" is propagated; the function is named by address.
struct SKey4_00ab9940 {
    long m_a, m_b, m_c, m_d;
};

struct SRenderKey_00ab9940 {
    long m_a, m_b, m_c, m_d;                       // +0x00 .. +0x0c: the object's four words
    long m_e;                                      // +0x10: the edx argument
    long m_f;                                      // +0x14
    long m_g;                                      // +0x18
};

SRenderKey_00ab9940 __fastcall MakeRenderKey_00ab9940(const SKey4_00ab9940* object, long e, long f, long g)
{
    SRenderKey_00ab9940 key;
    key.m_a = object->m_a;
    key.m_b = object->m_b;
    key.m_c = object->m_c;
    key.m_d = object->m_d;
    key.m_e = e;
    key.m_f = f;
    key.m_g = g;
    return key;
}
