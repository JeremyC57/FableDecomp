#include <cstdio>
struct CRetailLongVector { long* m_pFirst; long* m_pLast; long* m_pEnd; long buf[8];
    long* erase(long* first, long* last) { m_pLast = first; return first; } void push_back(const long& v) { *m_pLast++ = v; } void clear() { erase(m_pFirst, m_pLast); } };
static void GetMapping(CRetailLongVector& v) { v.clear(); long seq[3] = { 2, 3, 1 }; v.push_back(seq[0]); v.push_back(seq[1]); v.push_back(seq[2]); }
int main() { CRetailLongVector v; v.m_pFirst = v.buf; v.m_pLast = v.buf + 5; v.m_pEnd = v.buf + 8; GetMapping(v); long want[3] = { 2, 3, 1 };
    if (v.m_pLast - v.m_pFirst != 3 || v.buf[0] != want[0] || v.buf[1] != want[1] || v.buf[2] != want[2]) { std::printf("0064d593_TEST FAIL\n"); return 1; } std::printf("0064d593_TEST PASS\n"); return 0; }
