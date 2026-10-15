#include <cstdio>
// Behaviour test for SwapKeyedRecord @ 0x0086AF10: std::swap of two {key, 12-byte value} records
struct SValue12 { unsigned long m_a, m_b, m_c; };
struct SKeyedRecord {
    long first;
    SValue12 second;
    SKeyedRecord() {}
    SKeyedRecord(const SKeyedRecord& other) : first(other.first), second(other.second) {}
};
void SwapKeyedRecord(SKeyedRecord& left, SKeyedRecord& right)
{
    SKeyedRecord temp = left;
    left = right;
    right = temp;
}
int main()
{
    SKeyedRecord l, r;
    l.first = 1; l.second.m_a = 2; l.second.m_b = 3; l.second.m_c = 4;
    r.first = -5; r.second.m_a = 6; r.second.m_b = 7; r.second.m_c = 8;
    SwapKeyedRecord(l, r);
    if (l.first != -5 || l.second.m_a != 6 || l.second.m_b != 7 || l.second.m_c != 8 ||
        r.first != 1 || r.second.m_a != 2 || r.second.m_b != 3 || r.second.m_c != 4) { std::printf("0086af10_TEST FAIL\n"); return 1; }
    std::printf("0086af10_TEST PASS\n");
    return 0;
}
