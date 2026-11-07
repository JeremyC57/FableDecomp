BODY_OLD = """    if (*pMemory) {
        float* block = *pMemory;
        *pMemory += length;
        m_ring.m_data = block;
        m_ring.m_size = length;
        m_ring.m_pos = 0;
        m_ring.Clear();
        m_mode = index;
    }
    *pBytes += length * sizeof(float);"""
AXES = [
 ("order", [("", "", []),
            ("o2", "m_data stored before the pMemory bump", [("        float* block = *pMemory;\n        *pMemory += length;\n        m_ring.m_data = block;", "        m_ring.m_data = *pMemory;\n        *pMemory += length;")]),
            ("o3", "pMemory bump after the ring stores", [("        float* block = *pMemory;\n        *pMemory += length;\n        m_ring.m_data = block;\n        m_ring.m_size = length;\n        m_ring.m_pos = 0;", "        m_ring.m_data = *pMemory;\n        m_ring.m_size = length;\n        m_ring.m_pos = 0;\n        *pMemory += length;")]),
            ("o4", "pMemory bump between m_size and m_pos", [("        float* block = *pMemory;\n        *pMemory += length;\n        m_ring.m_data = block;\n        m_ring.m_size = length;\n        m_ring.m_pos = 0;", "        m_ring.m_data = *pMemory;\n        m_ring.m_size = length;\n        *pMemory += length;\n        m_ring.m_pos = 0;")])]),
 ("mode", [("", "", []),
           ("mf", "m_mode stored before the clear", [("        m_ring.Clear();\n        m_mode = index;", "        m_mode = index;\n        m_ring.Clear();")])]),
 ("clear", [("", "", []),
            ("cm", "memset written out at the site", [("        m_ring.Clear();", "        memset(m_ring.m_data, 0, m_ring.m_size * sizeof(float));")]),
            ("cl", "memset with the length parameter", [("        m_ring.Clear();", "        memset(m_ring.m_data, 0, length * sizeof(float));")]),
            ("cp", "memset through a ring pointer", [("        m_ring.Clear();", "        CRingBuffer* ring = &m_ring;\n        memset(ring->m_data, 0, ring->m_size * sizeof(float));")])]),
 ("bump", [("", "", []),
           ("bp", "*pMemory = *pMemory + length", [(">        *pMemory += length;", "        *pMemory = *pMemory + length;")])]),
 ("bytes", [("", "", []),
            ("bs", "sizeof(float) * length", [("    *pBytes += length * sizeof(float);", "    *pBytes += sizeof(float) * length;")]),
            ("bb", "pBytes via a local count", [("    *pBytes += length * sizeof(float);", "    unsigned int bytes = length * sizeof(float);\n    *pBytes += bytes;")])]),
]
