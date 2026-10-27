// behaviour test for 0x00613FE0: Size() returns the element count of the wrapped vector
#include <cstdio>
struct E { unsigned int m[7]; };
struct V { E* _M_start; E* _M_finish; E* _M_end; };
struct CTV { V* TransferableVector; long Size() { return (long)(unsigned int)(TransferableVector->_M_finish - TransferableVector->_M_start); } };
int main()
{
    E buf[5]; V v; v._M_start = buf; v._M_finish = buf + 5; v._M_end = buf + 5; CTV t; t.TransferableVector = &v;
    V e; e._M_start = e._M_finish = e._M_end = 0; CTV u; u.TransferableVector = &e;
    if (t.Size() != 5 || u.Size() != 0 || sizeof(E) != 28) { std::printf("00613fe0_TEST FAIL\n"); return 1; }
    std::printf("00613fe0_TEST PASS\n");
    return 0;
}