// SCreatureRecord_00c48770::operator=(const SCreatureRecord_00c48770&) -- member-wise copy of a 0x1c-byte record: a long, a 12-byte triple at +4 (its implicit assignment inlined), an intelligent pointer at +0x10 (out of line), a trailing long
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C48770, length 57 bytes.
// Callees: CIntelligentPointer<CThingCreatureBase>::operator= @ 0x00C16900 (on +0x10, the other's +0x10 by reference). Shape: the other stays in edi, `this` in
// esi; the triple is copied with `lea ecx,[edi+4]` / `lea edx,[esi+4]` (the member's implicit operator= with `this` in edx), then the pointer member out of line,
// the trailing long, and `this` is returned. The chunk label "CEnginePrimitive2DText::operator=" is propagated; the record is named by address.
struct CIntelligentPointer_00c48770 {
    void* m_pItem;
    void* m_pLink;
    CIntelligentPointer_00c48770& operator=(const CIntelligentPointer_00c48770& other);   // 0x00C16900
};

struct STriple_00c48770 { long m_x, m_y, m_z; };

struct SCreatureRecord_00c48770 {
    long                         m_a;              // +0x00
    STriple_00c48770             m_position;       // +0x04
    CIntelligentPointer_00c48770 m_creature;       // +0x10
    long                         m_b;              // +0x18
    SCreatureRecord_00c48770& operator=(const SCreatureRecord_00c48770& other);
};

SCreatureRecord_00c48770& SCreatureRecord_00c48770::operator=(const SCreatureRecord_00c48770& other)
{
    m_a = other.m_a;
    m_position = other.m_position;
    m_creature = other.m_creature;
    m_b = other.m_b;
    return *this;
}
