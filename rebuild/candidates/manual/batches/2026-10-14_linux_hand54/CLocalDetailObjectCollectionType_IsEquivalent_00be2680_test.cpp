#include <cstdio>
#include <cstdlib>
#include <cstring>
struct SBlock { long m_words[13]; };
struct CType { unsigned char _pad[0x14]; long m_14, m_18, m_1c; SBlock m_block; bool IsEquivalent(const CType& o) { if (m_14 == o.m_14 && m_18 == o.m_18 && m_1c == o.m_1c && std::memcmp(&m_block, &o.m_block, sizeof(m_block)) == 0) return true; return false; } };

int main() {
    CType a, b; std::memset(&a, 0, sizeof a); std::memset(&b, 0, sizeof b); a.m_14 = 1; b.m_14 = 1; a.m_block.m_words[12] = 7; b.m_block.m_words[12] = 7; if (!a.IsEquivalent(b)) { std::printf("00be2680_TEST FAIL\n"); return 1; }
    b.m_block.m_words[5] = 1; if (a.IsEquivalent(b)) { std::printf("00be2680_TEST FAIL\n"); return 1; } b.m_block.m_words[5] = 0; b.m_1c = 2; if (a.IsEquivalent(b)) { std::printf("00be2680_TEST FAIL\n"); return 1; } b.m_1c = 0; b.m_18 = 2; if (a.IsEquivalent(b)) { std::printf("00be2680_TEST FAIL\n"); return 1; } b.m_18 = 0; b.m_14 = 2; if (a.IsEquivalent(b)) { std::printf("00be2680_TEST FAIL\n"); return 1; }
    std::printf("00be2680_TEST PASS\n"); return 0;
}
