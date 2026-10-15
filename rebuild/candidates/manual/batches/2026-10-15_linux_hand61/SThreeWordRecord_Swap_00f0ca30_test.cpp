#include <cstdio>
// Behaviour test for SThreeWordRecord::Swap @ 0x00F0CA30
struct SThreeWordRecord {
    unsigned long m_a, m_b, m_c;
    void Swap(SThreeWordRecord& other);
};
template <class T> inline void SwapValues(T& left, T& right) { T temp = left; left = right; right = temp; }
void SThreeWordRecord::Swap(SThreeWordRecord& other)
{
    SwapValues(m_a, other.m_a);
    SwapValues(m_b, other.m_b);
    SwapValues(m_c, other.m_c);
}
int main()
{
    SThreeWordRecord x = { 1, 2, 3 }, y = { 10, 20, 30 };
    x.Swap(y);
    if (x.m_a != 10 || x.m_b != 20 || x.m_c != 30 || y.m_a != 1 || y.m_b != 2 || y.m_c != 3) { std::printf("00f0ca30_TEST FAIL\n"); return 1; }
    x.Swap(x);
    if (x.m_a != 10 || x.m_b != 20 || x.m_c != 30) { std::printf("00f0ca30_TEST FAIL\n"); return 1; }
    std::printf("00f0ca30_TEST PASS\n");
    return 0;
}
