#include <cstdio>
struct SSkillRecord { long m_property; unsigned char _rest[0x18]; };
struct CSkillTable { unsigned char _pad[0x98]; SSkillRecord m_skills[4]; unsigned char _padB[0x3d4 - 0x98 - 0x1c * 4]; long m_lastError;
    long Set(long index, long value) { if (index == 0 || index == 1) { m_lastError = -1; return -3; } SSkillRecord* r = &m_skills[index]; if (value != r->m_property) r->m_property = value; return 0; } };
int main() { CSkillTable t; t.m_lastError = 0; t.m_skills[2].m_property = 5; if (t.Set(0, 1) != -3 || t.m_lastError != -1) { std::printf("00c67470_TEST FAIL\n"); return 1; } t.m_lastError = 0; if (t.Set(1, 1) != -3 || t.m_lastError != -1) { std::printf("00c67470_TEST FAIL\n"); return 1; }
    if (t.Set(2, 9) != 0 || t.m_skills[2].m_property != 9) { std::printf("00c67470_TEST FAIL\n"); return 1; } if (t.Set(3, 4) != 0 || t.m_skills[3].m_property != 4) { std::printf("00c67470_TEST FAIL\n"); return 1; } std::printf("00c67470_TEST PASS\n"); return 0; }