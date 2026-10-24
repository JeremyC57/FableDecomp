// Behaviour test for 0x004f9db7 std::__copy_ptrs<CCardPositionsDef*>(first, last, result, __false_type): [first, last) is assigned onto [result, ...) in order;
// each element is assigned through its operator= (the base def part, then the member part); the returned pointer is
// result advanced past the last assignment.
#include <cstdio>

static int g_base_assigns = 0;

struct BaseModel {                                  // CParentDefClassBase stand-in
    long Index;
    BaseModel() : Index(0) { }
    BaseModel& operator=(const BaseModel& o) { ++g_base_assigns; Index = o.Index; return *this; }
};

struct ElemModel : BaseModel {                      // CCardPositionsDef stand-in
    long MemberA;
    long MemberB;
    ElemModel() : MemberA(0), MemberB(0) { }
    ElemModel& operator=(const ElemModel& o) { BaseModel::operator=(o); MemberA = o.MemberA; MemberB = o.MemberB; return *this; }
};

static ElemModel* model(ElemModel* first, ElemModel* last, ElemModel* result)
{
    for (int n = (int)(last - first); n > 0; --n) {
        *result = *first;
        ++first;
        ++result;
    }
    return result;
}

static int fail() { std::printf("004f9db7_TEST FAIL\n"); return 1; }

int main()
{
    ElemModel a[6];
    for (int i = 0; i < 6; ++i) { a[i].Index = i; a[i].MemberA = 10 * i; a[i].MemberB = 100 * i; }
    ElemModel* r = model(a + 2, a + 5, a);              // erase-style copy down of [2, 5) onto [0, 3)
    if (r != a + 3 || g_base_assigns != 3) return fail();
    if (a[0].Index != 2 || a[1].MemberA != 30 || a[2].MemberB != 400 || a[3].Index != 3 || a[5].Index != 5) return fail();
    if (model(a, a, a + 1) != a + 1 || g_base_assigns != 3) return fail();
    std::printf("004f9db7_TEST PASS\n");
    return 0;
}