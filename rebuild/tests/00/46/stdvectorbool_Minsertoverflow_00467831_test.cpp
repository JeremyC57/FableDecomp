// Behaviour test for 0x00467831 std::vector<bool>::_M_insert_overflow (POD growth path)
// vector<bool> is a one-byte-per-element STLport vector in retail; the body only moves bytes, so the test drives it through
// distinct byte patterns on an unsigned char view of the same storage.
#include <cstdio>
#include <cstdlib>
#include <cstring>
struct ByteVector {
    unsigned char* _M_start;
    unsigned char* _M_finish;
    unsigned char* _M_end_of_storage;
};
static void* CopyTrivial(const void* first, const void* last, void* result)
{
    return (last == first) ? result
        : ((char*)std::memmove(result, first, (const char*)last - (const char*)first)) + ((const char*)last - (const char*)first);
}
static void InsertOverflow(ByteVector* v, unsigned char* position, const unsigned char& x, unsigned int fillLength, bool atEnd)
{
    const unsigned int oldSize = (unsigned int)(v->_M_finish - v->_M_start);
    const unsigned int length = oldSize + (oldSize < fillLength ? fillLength : oldSize);
    unsigned char* newStart = length != 0 ? (unsigned char*)std::malloc(length) : 0;
    unsigned char* newFinish = (unsigned char*)CopyTrivial(v->_M_start, position, newStart);
    for (unsigned int n = fillLength; n > 0; --n, ++newFinish)
        *newFinish = x;
    if (!atEnd)
        newFinish = (unsigned char*)CopyTrivial(position, v->_M_finish, newFinish);
    if (v->_M_start != 0)
        std::free(v->_M_start);
    v->_M_start = newStart; v->_M_finish = newFinish; v->_M_end_of_storage = newStart + length;
}
int main()
{
    ByteVector v;
    v._M_start = (unsigned char*)std::malloc(4);
    for (int i = 0; i < 4; ++i) v._M_start[i] = (unsigned char)(10 + i);
    v._M_finish = v._M_end_of_storage = v._M_start + 4;
    unsigned char x = 0x55;
    InsertOverflow(&v, v._M_start + 1, x, 2, false);       // 10 55 55 11 12 13, capacity 4 + max(4,2) = 8
    const unsigned char want[6] = { 10, 0x55, 0x55, 11, 12, 13 };
    if (v._M_finish - v._M_start != 6 || v._M_end_of_storage - v._M_start != 8 || std::memcmp(v._M_start, want, 6) != 0) {
        std::printf("00467831_TEST FAIL\n"); return 1;
    }
    InsertOverflow(&v, v._M_finish, x, 10, true);         // append 10 at the end: capacity 6 + max(6,10) = 16
    if (v._M_finish - v._M_start != 16 || v._M_end_of_storage - v._M_start != 16 || v._M_start[15] != 0x55 || v._M_start[5] != 13) {
        std::printf("00467831_TEST FAIL\n"); return 1;
    }
    ByteVector e = { 0, 0, 0 };
    InsertOverflow(&e, 0, x, 0, true);                     // empty and nothing to insert: no allocation
    if (e._M_start != 0 || e._M_finish != 0 || e._M_end_of_storage != 0) { std::printf("00467831_TEST FAIL\n"); return 1; }
    std::free(v._M_start);
    std::printf("00467831_TEST PASS\n");
    return 0;
}