// Behaviour test for the CParentDefClassBase copy constructor (0x0046EE98): base copy, the
// InstantiationName table position, the sub-def vector and the map's Dirty flag.
#include <cstdio>

static int g_baseCopies = 0;
static int g_vectorCopies = 0;

struct CDefClassBase {
    virtual void v00() {}
    unsigned char _body[0x21];
    CDefClassBase() {}
    CDefClassBase(const CDefClassBase& other)
    {
        for (int i = 0; i < 0x21; ++i) _body[i] = other._body[i];
        ++g_baseCopies;
    }
};

struct CDefString {
    long TablePos;
};

struct CSubDefInfoVector {
    void* m_first;
    void* m_last;
    void* m_end;
    CSubDefInfoVector() : m_first(0), m_last(0), m_end(0) {}
    CSubDefInfoVector(const CSubDefInfoVector& other)
        : m_first(other.m_first), m_last(other.m_last), m_end(other.m_end)
    {
        ++g_vectorCopies;
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

struct CParentDefClassBase : CDefClassBase {
    CDefString     InstantiationName;
    CSubDefInfoMap SubDefInfoMap;
    CParentDefClassBase() {}
    CParentDefClassBase(const CParentDefClassBase& other);
};

CParentDefClassBase::CParentDefClassBase(const CParentDefClassBase& other)
    : CDefClassBase(other)
    , InstantiationName(other.InstantiationName)
    , SubDefInfoMap(other.SubDefInfoMap)
{
}

int main()
{
    static int infos[4];
    CParentDefClassBase src;
    for (int i = 0; i < 0x21; ++i) src._body[i] = (unsigned char)(0x40 + i);
    src.InstantiationName.TablePos = 1234;
    src.SubDefInfoMap.Vector.m_first = infos;
    src.SubDefInfoMap.Vector.m_last = infos + 2;
    src.SubDefInfoMap.Vector.m_end = infos + 4;
    src.SubDefInfoMap.Dirty = true;
    CParentDefClassBase copy(src);
    if (g_baseCopies != 1 || g_vectorCopies != 1 || copy._body[0x20] != 0x60 || copy.InstantiationName.TablePos != 1234
        || copy.SubDefInfoMap.Vector.m_first != infos || copy.SubDefInfoMap.Vector.m_last != infos + 2
        || copy.SubDefInfoMap.Vector.m_end != infos + 4 || !copy.SubDefInfoMap.Dirty) {
        std::printf("0046ee98_TEST FAIL\n");
        return 1;
    }
    std::printf("0046ee98_TEST PASS\n");
    return 0;
}
