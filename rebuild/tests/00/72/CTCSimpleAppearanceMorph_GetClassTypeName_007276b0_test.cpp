#include <cstdio>

struct StringRep;

class CCharString
{ public:
    StringRep* rep;
    CCharString(const char* s, long n);
};

#pragma auto_inline(off)
CCharString::CCharString(const char* s, long n)
{
    rep = (StringRep*)(s ? (void*)(n) : 0);
}
#pragma auto_inline(on)

struct CTCSimpleAppearanceMorph
{
    void* vtbl;
    CCharString GetClassTypeName() const;
};

CCharString CTCSimpleAppearanceMorph::GetClassTypeName() const
{
    return CCharString("ACTION_DO_CREATURE_ACTION", -1);
}

int main()
{
    CTCSimpleAppearanceMorph obj;
    obj.vtbl = 0;
    CCharString s = obj.GetClassTypeName();
    if (s.rep == (StringRep*)(-1))
        printf("007276b0_TEST PASS\n");
    else
        printf("FAIL rep=%p\n", (void*)s.rep);
    return 0;
}