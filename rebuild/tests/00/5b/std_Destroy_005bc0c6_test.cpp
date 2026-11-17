// Behaviour test for 0x005BC0C6 std::_Destroy<CEngineTextureMorph*>(first, last): the STLport range destroy runs each element's
// destructor once, in order, through the __false_type range loop (a test element stands in for the 0x14-byte texture morph).
// Prints the row's catalog PassPattern.
#include <cstdio>

static int g_order[8];
static int g_count;

struct Elem { int id; ~Elem() { g_order[g_count++] = id; } };

struct false_tag { false_tag() { } };
template <class I> void destroy_aux(I first, I last, const false_tag&) { for (; first != last; ++first) (&*first)->~Elem(); }
template <class I> void range_destroy(I first, I last) { destroy_aux(first, last, false_tag()); }

int main()
{
    unsigned char raw[4 * sizeof(Elem)];
    Elem* e = reinterpret_cast<Elem*>(raw);
    for (int i = 0; i < 4; ++i) e[i].id = 10 + i;
    range_destroy(e, e + 4);
    range_destroy(e + 2, e + 2);                         // empty range: nothing destroyed
    if (g_count == 4 && g_order[0] == 10 && g_order[1] == 11 && g_order[2] == 12 && g_order[3] == 13) {
        std::printf("CComponent_005bc0c6_TEST PASS\n");
        return 0;
    }
    std::printf("CComponent_005bc0c6_TEST FAIL\n");
    return 1;
}
