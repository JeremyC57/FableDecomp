# hypothesis: retail's IL did not forward the m_size store into the memset count (no CSE with the pointer bump), so the ring
# stores and the Clear() loads went through different base expressions: all ring accesses through a pointer / reference.
RING_OLD = "        m_ring.m_data = block;\n        m_ring.m_size = length;\n        m_ring.m_pos = 0;\n        m_ring.Clear();"
AXES = [
 ("acc", [("rp", "all ring accesses through a CRingBuffer pointer", [(RING_OLD, "        CRingBuffer* ring = &m_ring;\n        ring->m_data = block;\n        ring->m_size = length;\n        ring->m_pos = 0;\n        ring->Clear();")]),
          ("rr", "all ring accesses through a CRingBuffer reference", [(RING_OLD, "        CRingBuffer& ring = m_ring;\n        ring.m_data = block;\n        ring.m_size = length;\n        ring.m_pos = 0;\n        ring.Clear();")]),
          ("rt", "explicit this->m_ring", [(RING_OLD, "        this->m_ring.m_data = block;\n        this->m_ring.m_size = length;\n        this->m_ring.m_pos = 0;\n        this->m_ring.Clear();")]),
          ("rs", "stores through a pointer, Clear() on the member", [(RING_OLD, "        CRingBuffer* ring = &m_ring;\n        ring->m_data = block;\n        ring->m_size = length;\n        ring->m_pos = 0;\n        m_ring.Clear();")]),
          ("rc", "stores on the member, Clear() through a pointer", [(RING_OLD, "        m_ring.m_data = block;\n        m_ring.m_size = length;\n        m_ring.m_pos = 0;\n        CRingBuffer* ring = &m_ring;\n        ring->Clear();")]),
          ("rf", "pointer taken before the test", [("    if (*pMemory) {", "    CRingBuffer* ring = &m_ring;\n    if (*pMemory) {"), (RING_OLD, "        ring->m_data = block;\n        ring->m_size = length;\n        ring->m_pos = 0;\n        ring->Clear();")])]),
 ("clear", [("", "", []),
            ("cs", "Clear() body reads through a local this copy", [("    void Clear() { memset(m_data, 0, m_size * sizeof(float)); }", "    void Clear() { CRingBuffer* self = this; memset(self->m_data, 0, self->m_size * sizeof(float)); }")]),
            ("cn", "Clear() as a static helper taking the ring", [("    void Clear() { memset(m_data, 0, m_size * sizeof(float)); }", "    static void ClearRing(CRingBuffer* ring) { memset(ring->m_data, 0, ring->m_size * sizeof(float)); }\n    void Clear() { ClearRing(this); }")])]),
 ("bump", [("", "", []),
           ("bv", "bump through the block local", [("        *pMemory += length;", "        *pMemory = block + length;")])]),
]
