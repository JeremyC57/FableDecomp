#include <cstdio>
// Behaviour test for 00800c60: vector::erase(first, last) slides the tail down, moves _M_finish and returns first.
struct E { int a, b; char c; };
E* CopyFwd(E* first, E* last, E* result) { for (long n = last - first; n > 0; --n) *result++ = *first++; return result; }
struct Vec {
    E* _M_start; E* _M_finish; E* _M_end_of_storage;
    E* erase(E* first, E* last) { E* i = CopyFwd(last, _M_finish, first); _M_finish = i; return first; }
};
int main()
{
    E buf[5]; for (int i = 0; i < 5; ++i) { buf[i].a = i; buf[i].b = 10 * i; buf[i].c = (char)('a' + i); }
    Vec v; v._M_start = buf; v._M_finish = buf + 5; v._M_end_of_storage = buf + 5;
    E* r = v.erase(buf + 1, buf + 3);
    int ok = (r == buf + 1 && v._M_finish == buf + 3 && buf[1].a == 3 && buf[2].a == 4 && buf[2].c == 'e' && buf[0].a == 0);
    r = v.erase(buf + 3, buf + 3);
    ok = ok && r == buf + 3 && v._M_finish == buf + 3;
    if (!ok) { std::printf("00800c60_TEST FAIL\n"); return 1; }
    std::printf("00800c60_TEST PASS\n");
    return 0;
}
