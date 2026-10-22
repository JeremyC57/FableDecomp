#include <cstdio>
// Behaviour test for 007b5560: copy (through the inlined __copy_aux) forwards to the random-access __copy into a deque back-inserter
// (modelled with a fixed array container); the result iterator still names the container.
struct SPair8 { long first; long second; };
struct Deque { SPair8 items[8]; int count; void push_back(const SPair8& v) { items[count++] = v; } };
struct random_access_iterator_tag { random_access_iterator_tag() { } };
struct __false_type { __false_type() { } };
struct BackInserter { Deque* container; explicit BackInserter(Deque& d) : container(&d) { } };
BackInserter Copy(const SPair8* first, const SPair8* last, BackInserter result, const random_access_iterator_tag&, long*)
{
    for (long n = last - first; n > 0; --n, ++first) result.container->push_back(*first);
    return result;
}
BackInserter CopyAux(const SPair8* first, const SPair8* last, BackInserter result, const __false_type&)
{
    return Copy(first, last, result, random_access_iterator_tag(), (long*)0);
}
int main()
{
    SPair8 src[3] = { { 1, 2 }, { 3, 4 }, { 5, 6 } };
    Deque d; d.count = 0;
    BackInserter r = CopyAux(src, src + 3, BackInserter(d), __false_type());
    int ok = (r.container == &d && d.count == 3 && d.items[0].first == 1 && d.items[2].second == 6);
    r = CopyAux(src, src, BackInserter(d), __false_type());
    ok = ok && d.count == 3;
    if (!ok) { std::printf("007b5560_TEST FAIL\n"); return 1; }
    std::printf("007b5560_TEST PASS\n");
    return 0;
}
