// Behaviour test for 0x009EE0A0 __uninitialized_copy<const CGenericVar*, CGenericVar*>: every element is
// copy-constructed into the raw destination and the end of the copies is returned.
#include <cstdio>
#include <cstring>
#include <new>

static int gCopies = 0;

class CCharString
{
public:
    const char* Text;
    CCharString(const char* t) : Text(t) {}
    CCharString(const CCharString& o) : Text(o.Text) { ++gCopies; }
};

class CGenericVar
{
public:
    long        Type;
    CCharString ValString;
    long        ValSLONG;
    unsigned long ValULONG;
    bool        ValBool;
    float       ValFloat;
    CGenericVar(long t, const char* s, long sl, unsigned long ul, bool b, float f)
        : Type(t), ValString(s), ValSLONG(sl), ValULONG(ul), ValBool(b), ValFloat(f) {}
    virtual ~CGenericVar() {}
};

struct FalseType { FalseType() {} };

template <class _InputIter, class _ForwardIter>
_ForwardIter UninitializedCopy(_InputIter __first, _InputIter __last, _ForwardIter __result, const FalseType&)
{
    _ForwardIter __cur = __result;
    for (; __first != __last; ++__first, ++__cur)
        new (&*__cur) CGenericVar(*__first);
    return __cur;
}

int main()
{
    const CGenericVar src[3] = { CGenericVar(1, "a", -5, 7, true, 1.5f), CGenericVar(5, "bb", 0, 0, false, 0.0f),
                                 CGenericVar(3, "ccc", 2, 9, true, -2.25f) };
    static double raw[3 * sizeof(CGenericVar) / sizeof(double) + 3];
    CGenericVar* dst = (CGenericVar*)raw;
    gCopies = 0;
    CGenericVar* end = UninitializedCopy(src, src + 3, dst, FalseType());
    bool ok = end == dst + 3 && gCopies == 3;
    for (int i = 0; i < 3; ++i)
        ok = ok && dst[i].Type == src[i].Type && std::strcmp(dst[i].ValString.Text, src[i].ValString.Text) == 0
                && dst[i].ValSLONG == src[i].ValSLONG && dst[i].ValULONG == src[i].ValULONG
                && dst[i].ValBool == src[i].ValBool && dst[i].ValFloat == src[i].ValFloat;
    ok = ok && UninitializedCopy(src, src, dst, FalseType()) == dst;            // empty range
    if (!ok) { std::printf("009ee0a0_TEST FAIL\n"); return 1; }
    std::printf("009ee0a0_TEST PASS\n");
    return 0;
}