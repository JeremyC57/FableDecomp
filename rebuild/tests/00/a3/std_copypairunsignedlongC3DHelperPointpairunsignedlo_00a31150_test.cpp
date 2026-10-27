// Test for 0x00A31150 std::__copy<pair<unsigned long, C3DHelperPoint>*, ..., int> (random access): assigns last - first
// elements forward and returns result + n.
#include <stdio.h>
class C3DHelperPoint { public: float Pos[3]; int Bone; };
struct Entry { unsigned int first; C3DHelperPoint second; };
static Entry* copy_ra(Entry* first, Entry* last, Entry* result)
{
    for (int n = (int)(last - first); n > 0; --n) { *result = *first; ++first; ++result; }
    return result;
}
int main()
{
    Entry src[3], dst[4]; int i;
    for (i = 0; i < 3; ++i) { src[i].first = i + 10; src[i].second.Bone = i * 2; src[i].second.Pos[1] = (float)i; }
    dst[3].first = 99;
    Entry* r = copy_ra(src, src + 3, dst);
    bool ok = r == dst + 3 && dst[3].first == 99;
    for (i = 0; i < 3; ++i) if (dst[i].first != (unsigned int)(i + 10) || dst[i].second.Bone != i * 2 || dst[i].second.Pos[1] != (float)i) ok = false;
    if (copy_ra(src, src, dst) != dst) ok = false;
    puts(ok ? "00a31150_TEST PASS" : "00a31150_TEST FAIL");
    return ok ? 0 : 1;
}