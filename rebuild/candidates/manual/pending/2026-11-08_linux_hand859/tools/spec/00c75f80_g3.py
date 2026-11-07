# hypothesis: CRingBuffer is a second base class (this+4 adjustment), so the ring stores/loads go through an adjusted pointer
BASE_OLD = "struct CCombFilter : CDspObject {           // vtable 0x012B9588\n    CRingBuffer m_ring;                     // +0x04\n"
BASE_MI  = "struct CCombFilter : CDspObject, CRingBuffer {   // vtable 0x012B9588; the ring is the second base (+0x04)\n"
INIT_DECL = "    void Clear() { memset(m_data, 0, m_size * sizeof(float)); }\n"
AXES = [
 ("mi", [("mi", "CRingBuffer as a second base class", [(BASE_OLD, BASE_MI), (">m_ring.", "")])]),
 ("body", [("", "", []),
           ("st", "memset at the site", [("        Clear();", "        memset(m_data, 0, m_size * sizeof(float));")]),
           ("in", "base Initialise(data, n) with Clear()", [(INIT_DECL, INIT_DECL + "    void Initialise(float* data, unsigned int n) { m_data = data; m_size = n; m_pos = 0; Clear(); }\n"),
                                                           ("        m_data = block;\n        m_size = length;\n        m_pos = 0;\n        Clear();", "        Initialise(block, length);")]),
           ("ip", "base Initialise(data, n) with a parameter count", [(INIT_DECL, INIT_DECL + "    void Initialise(float* data, unsigned int n) { m_data = data; m_size = n; m_pos = 0; memset(m_data, 0, n * sizeof(float)); }\n"),
                                                                      ("        m_data = block;\n        m_size = length;\n        m_pos = 0;\n        Clear();", "        Initialise(block, length);")]),
           ("ir", "base Initialise(data, n) through a ring reference", [(INIT_DECL, INIT_DECL + "    void Initialise(float* data, unsigned int n) { m_data = data; m_size = n; m_pos = 0; Clear(); }\n"),
                                                                        ("        m_data = block;\n        m_size = length;\n        m_pos = 0;\n        Clear();", "        CRingBuffer& ring = *this;\n        ring.Initialise(block, length);")])]),
 ("order", [("", "", []),
            ("o2", "m_data stored before the pMemory bump", [("        float* block = *pMemory;\n        *pMemory += length;\n        m_data = block;", "        m_data = *pMemory;\n        *pMemory += length;")])]),
 ("bytes", [("", "", []),
            ("bb", "pBytes via a local count", [("    *pBytes += length * sizeof(float);", "    unsigned int bytes = length * sizeof(float);\n    *pBytes += bytes;")])]),
]
