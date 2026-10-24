// Behaviour test for 0x004f9f62 std::__copy_backward_ptrs<CCardPositionsDef*>(first, last, result, __false_type): [first, last) is assigned onto the range ending at result, last element first (safe for an overlapping upward shift);
// each element is assigned through its operator= (the base def part, then the member part); the returned pointer is
// the start of the destination range.
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
    for (int n = (int)(last - first); n > 0; --n)
        *--result = *--last;
    return result;
}

static int fail() { std::printf("004f9f62_TEST FAIL\n"); return 1; }

int main()
{
    ElemModel a[6];
    for (int i = 0; i < 6; ++i) { a[i].Index = i; a[i].MemberA = 10 * i; a[i].MemberB = 100 * i; }
    ElemModel* r = model(a, a + 3, a + 5);              // insert-style shift of [0, 3) up onto [2, 5), overlapping
    if (r != a + 2 || g_base_assigns != 3) return fail();
    if (a[0].Index != 0 || a[1].Index != 1 || a[2].Index != 0 || a[3].MemberA != 10 || a[4].MemberB != 200 || a[5].Index != 5) return fail();
    if (model(a, a, a + 4) != a + 4 || g_base_assigns != 3) return fail();
    std::printf("004f9f62_TEST PASS\n");
    return 0;
}