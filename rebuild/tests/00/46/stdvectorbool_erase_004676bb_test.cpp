// Behaviour test for 0x004676BB std::vector<bool>::erase(first, last)
// vector<bool> is a one-byte-per-element STLport vector in retail; the body only moves bytes, so the test drives it through
// distinct byte patterns on an unsigned char view of the same storage.
#include <cstdio>
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
static unsigned char* Erase(ByteVector* v, unsigned char* first, unsigned char* last)
{
    unsigned char* i = (unsigned char*)CopyTrivial(last, v->_M_finish, first);
    v->_M_finish = i;
    return first;
}
int main()
{
    unsigned char buf[8] = { 1, 2, 3, 4, 5, 6, 7, 8 };
    ByteVector v = { buf, buf + 8, buf + 8 };
    unsigned char* r = Erase(&v, buf + 2, buf + 5);       // drop 3,4,5
    if (r != buf + 2 || v._M_finish != buf + 5 || buf[0] != 1 || buf[1] != 2 || buf[2] != 6 || buf[3] != 7 || buf[4] != 8) {
        std::printf("004676bb_TEST FAIL\n"); return 1;
    }
    r = Erase(&v, buf + 3, buf + 5);                      // erase to the end: nothing to move
    if (r != buf + 3 || v._M_finish != buf + 3) { std::printf("004676bb_TEST FAIL\n"); return 1; }
    r = Erase(&v, buf + 3, buf + 3);                      // empty range at the end
    if (r != buf + 3 || v._M_finish != buf + 3) { std::printf("004676bb_TEST FAIL\n"); return 1; }
    std::printf("004676bb_TEST PASS\n");
    return 0;
}