# hypothesis: retail does NOT CSE the memset count with the pBytes increment (lea ecx,[esi*4] recomputed), so the two
# length*4 expressions differ in type (signed/unsigned) or operand; probe member / parameter / pointer types.
SIZE_OLD = "    unsigned int m_size;                   // +0x04 floats"
AXES = [
 ("msize", [("", "", []),
            ("ml", "long m_size", [(SIZE_OLD, "    long m_size;                           // +0x04 floats")]),
            ("mi", "int m_size", [(SIZE_OLD, "    int m_size;                            // +0x04 floats")])]),
 ("len", [("", "", []),
          ("ll", "long length parameter", [(">void Allocate(unsigned int length,", "void Allocate(long length,"), ("void CCombFilter::Allocate(unsigned int length,", "void CCombFilter::Allocate(long length,")]),
          ("li", "int length parameter", [(">void Allocate(unsigned int length,", "void Allocate(int length,"), ("void CCombFilter::Allocate(unsigned int length,", "void CCombFilter::Allocate(int length,")])]),
 ("pbytes", [("", "", []),
             ("pl", "long* pBytes", [(">unsigned int* pBytes", "long* pBytes")]),
             ("pi", "int* pBytes", [(">unsigned int* pBytes", "int* pBytes")])]),
 ("count", [("", "", []),
            ("c4", "m_size * 4", [("memset(m_data, 0, m_size * sizeof(float));", "memset(m_data, 0, m_size * 4);")]),
            ("cs", "sizeof(float) * m_size", [("memset(m_data, 0, m_size * sizeof(float));", "memset(m_data, 0, sizeof(float) * m_size);")]),
            ("cu", "(unsigned int)m_size * sizeof(float)", [("memset(m_data, 0, m_size * sizeof(float));", "memset(m_data, 0, (unsigned int)m_size * sizeof(float));")])]),
 ("bytes", [("", "", []),
            ("b4", "*pBytes += length * 4", [("    *pBytes += length * sizeof(float);", "    *pBytes += length * 4;")]),
            ("bl", "*pBytes += (long)length * sizeof(float)", [("    *pBytes += length * sizeof(float);", "    *pBytes += (long)length * sizeof(float);")]),
            ("bu", "*pBytes += (unsigned int)(length * sizeof(float))", [("    *pBytes += length * sizeof(float);", "    *pBytes += (unsigned int)(length * sizeof(float));")])]),
 ("mpos", [("", "", []),
           ("pz", "unsigned m_pos stored as 0u", [("        m_ring.m_pos = 0;", "        m_ring.m_pos = 0u;")])]),
]
