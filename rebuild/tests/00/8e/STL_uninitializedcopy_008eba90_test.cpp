// Behaviour test for 0x008EBA90 std::__uninitialized_copy<CHoverState*>: copy-constructs each element in order.
#include <cstdio>
#include <new>

static int g_stringCopies = 0;

struct CCharString {
    int id;
    CCharString(int i) : id(i) {}
    CCharString(const CCharString& other) : id(other.id) { ++g_stringCopies; }
};

struct CRadialArea { float ThetaLow, ThetaHigh, RadHigh; };

struct CHoverState {
    CCharString AnimName;
    CRadialArea Area;
    int         MovementType;
    float       MovementSpeed;
    CHoverState(int id) : AnimName(id) {}
};

static CHoverState* __uninitialized_copy(CHoverState* first, CHoverState* last, CHoverState* result)
{
    CHoverState* cur = result;
    for ( ; first != last; ++first, ++cur)
        new (cur) CHoverState(*first);
    return cur;
}

int main()
{
    CHoverState src[3] = { CHoverState(7), CHoverState(8), CHoverState(9) };
    for (int i = 0; i < 3; ++i) {
        src[i].Area.ThetaLow = 1.0f * i; src[i].Area.ThetaHigh = 2.0f; src[i].Area.RadHigh = 3.0f;
        src[i].MovementType = 10 + i; src[i].MovementSpeed = 0.5f;
    }
    unsigned char raw[sizeof(CHoverState) * 3 + 8];
    CHoverState* dst = (CHoverState*)(((unsigned long)(raw + 7)) & ~(unsigned long)7);
    int fail = 0;
    g_stringCopies = 0;
    CHoverState* end = __uninitialized_copy(src, src + 3, dst);
    if (end != dst + 3 || g_stringCopies != 3) fail = 1;
    if (dst[0].AnimName.id != 7 || dst[2].AnimName.id != 9 || dst[1].MovementType != 11) fail = 1;
    float d = dst[2].Area.ThetaLow - 2.0f;
    if (d > 0.001f || d < -0.001f) fail = 1;
    if (__uninitialized_copy(src, src, dst) != dst || g_stringCopies != 3) fail = 1;   // empty range
    if (fail) { std::printf("008eba90_TEST FAIL\n"); return 1; }
    std::printf("008eba90_TEST PASS\n");
    return 0;
}