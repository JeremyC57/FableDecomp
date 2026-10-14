#include <cstdio>
#include <cstdlib>
#include <cstring>
static long g_lists = 0; struct SAllocator { SAllocator() { } };
struct CList { void* m_pNode; CList(const SAllocator&) { m_pNode = (void*)(size_t)++g_lists; } };
struct SRecord16 { long m_a, m_b, m_c, m_d; SRecord16() { m_d = 0; m_c = 0; m_b = 0; m_a = 0; } };
struct CHeroExperienceDef { CList m_list0, m_list1, m_list2, m_list3; long m_10; SRecord16 m_record14, m_record24;
    CHeroExperienceDef() : m_list0(SAllocator()), m_list1(SAllocator()), m_list2(SAllocator()), m_list3(SAllocator()), m_10(0) { } };

int main() {
    CHeroExperienceDef d; if (g_lists != 4 || d.m_list0.m_pNode != (void*)1 || d.m_list3.m_pNode != (void*)4 || d.m_10 != 0 || d.m_record14.m_a || d.m_record14.m_d || d.m_record24.m_a || d.m_record24.m_d) { std::printf("005797a4_TEST FAIL\n"); return 1; }
    std::printf("005797a4_TEST PASS\n"); return 0;
}
