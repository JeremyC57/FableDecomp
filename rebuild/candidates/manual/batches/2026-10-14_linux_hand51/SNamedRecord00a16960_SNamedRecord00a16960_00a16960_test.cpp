#include <cstdio>
#include <cstdlib>
#include <cstring>
struct SCountBlock { long m_references; }; static long g_copies = 0; struct CCountedRef { SCountBlock* m_pCount; CCountedRef() : m_pCount(0) { } CCountedRef(const CCountedRef& o) : m_pCount(o.m_pCount) { if (m_pCount) ++m_pCount->m_references; } };
struct CCharString { void* m_p; CCharString() : m_p(0) { } CCharString(const CCharString& o) : m_p(o.m_p) { ++g_copies; } };
struct SNamedRecord { long m_0; CCountedRef m_ref; long m_8, m_c, m_10, m_14, m_18; CCharString m_name; SNamedRecord() { } SNamedRecord(const SNamedRecord& o) : m_0(o.m_0), m_ref(o.m_ref), m_8(o.m_8), m_c(o.m_c), m_10(o.m_10), m_14(o.m_14), m_18(o.m_18), m_name(o.m_name) { } };

int main() {
    SCountBlock c; c.m_references = 1; SNamedRecord src; src.m_0 = 1; src.m_ref.m_pCount = &c; src.m_8 = 8; src.m_c = 12; src.m_10 = 16; src.m_14 = 20; src.m_18 = 24; src.m_name.m_p = (void*)"n";
    SNamedRecord dst(src); if (dst.m_0 != 1 || dst.m_ref.m_pCount != &c || c.m_references != 2 || dst.m_8 != 8 || dst.m_18 != 24 || dst.m_name.m_p != src.m_name.m_p || g_copies != 1) { std::printf("00a16960_TEST FAIL\n"); return 1; } src.m_ref.m_pCount = 0; SNamedRecord d2(src); if (d2.m_ref.m_pCount || c.m_references != 2) { std::printf("00a16960_TEST FAIL\n"); return 1; }
    std::printf("00a16960_TEST PASS\n"); return 0;
}
