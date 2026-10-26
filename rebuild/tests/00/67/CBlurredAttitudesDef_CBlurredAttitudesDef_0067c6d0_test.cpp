// Behaviour test for the CBlurredAttitudesDef copy constructor (0x0067C6D0): the inline
// CParentDefClassBase copy followed by the Attitudes vector copy.
#include <cstdio>

static int g_order[4];
static int g_calls = 0;

class CDefClassBase { public:
    virtual void v00() {}
    unsigned char _body[0x21];
    CDefClassBase() {}
    CDefClassBase(const CDefClassBase& other)
    {
        for (int i = 0; i < 0x21; ++i) _body[i] = other._body[i];
        g_order[g_calls++] = 1;
    }
};

class CDefString { public:
    long TablePos;
};

struct CSubDefInfoVector {
    void* m_first; void* m_last; void* m_end;
    CSubDefInfoVector() : m_first(0), m_last(0), m_end(0) {}
    CSubDefInfoVector(const CSubDefInfoVector& o) : m_first(o.m_first), m_last(o.m_last), m_end(o.m_end)
    {
        g_order[g_calls++] = 2;
    }
};

struct CKeyPairCompareLess {
};

struct CSubDefInfoMap {
    CSubDefInfoVector   Vector;
    CKeyPairCompareLess Compare;
    bool                Dirty;
    CSubDefInfoMap() : Dirty(false) {}
    CSubDefInfoMap(const CSubDefInfoMap& other)
        : Vector(other.Vector), Compare(other.Compare), Dirty(other.Dirty)
    {
    }
};

class CParentDefClassBase : public CDefClassBase { public:
    CDefString     InstantiationName;
    CSubDefInfoMap SubDefInfoMap;
    CParentDefClassBase() {}
    CParentDefClassBase(const CParentDefClassBase& other)
        : CDefClassBase(other), InstantiationName(other.InstantiationName), SubDefInfoMap(other.SubDefInfoMap)
    {
    }
};

struct CAttitudeVector {
    long* m_first; long* m_last; long* m_end;
    CAttitudeVector() : m_first(0), m_last(0), m_end(0) {}
    CAttitudeVector(const CAttitudeVector& o) : m_first(o.m_first), m_last(o.m_last), m_end(o.m_end)
    {
        g_order[g_calls++] = 3;
    }
};

class CBlurredAttitudesDef : public CParentDefClassBase { public:
    CAttitudeVector Attitudes;
    CBlurredAttitudesDef() {}
    CBlurredAttitudesDef(const CBlurredAttitudesDef& other);
};

CBlurredAttitudesDef::CBlurredAttitudesDef(const CBlurredAttitudesDef& other)
    : CParentDefClassBase(other)
    , Attitudes(other.Attitudes)
{
}

int main()
{
    static long attitudes[3] = { 1, 2, 3 };
    CBlurredAttitudesDef src;
    for (int i = 0; i < 0x21; ++i) src._body[i] = 7;
    src.InstantiationName.TablePos = 77;
    src.SubDefInfoMap.Dirty = true;
    src.Attitudes.m_first = attitudes; src.Attitudes.m_last = attitudes + 3; src.Attitudes.m_end = attitudes + 3;
    CBlurredAttitudesDef copy(src);
    if (g_calls != 3 || g_order[0] != 1 || g_order[1] != 2 || g_order[2] != 3 || copy._body[3] != 7
        || copy.InstantiationName.TablePos != 77 || !copy.SubDefInfoMap.Dirty
        || copy.Attitudes.m_first != attitudes || copy.Attitudes.m_last != attitudes + 3) {
        std::printf("0067c6d0_TEST FAIL\n");
        return 1;
    }
    std::printf("0067c6d0_TEST PASS\n");
    return 0;
}