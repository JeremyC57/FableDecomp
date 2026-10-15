#include <cstdio>

static int g_baseAssigns = 0;
static int g_containerAssigns = 0;
static int g_stringAssigns = 0;

struct CParentDefClassBase {
    virtual void v00() { }
    long m_marker;                              // stands in for the out-of-line base part
    CParentDefClassBase() : m_marker(0) { }
    CParentDefClassBase& operator=(const CParentDefClassBase& other)
    {
        m_marker = other.m_marker;
        ++g_baseAssigns;
        return *this;
    }
};

struct CContainer {                             // a retail vector / map: assignment is out of line
    long m_id;
    CContainer() : m_id(0) { }
    CContainer& operator=(const CContainer& other)
    {
        m_id = other.m_id;
        ++g_containerAssigns;
        return *this;
    }
};

struct CCharString {
    long m_id;
    CCharString() : m_id(0) { }
    CCharString& operator=(const CCharString& other)
    {
        m_id = other.m_id;
        ++g_stringAssigns;
        return *this;
    }
};

struct CDefString { long TablePos; };
struct CRGBColour { unsigned long m_packed; };
struct C2DVector { float X, Y; };
struct C3DVector { float X, Y, Z; };

struct CCreatureGenerationFamilyDef : CParentDefClassBase {
    CContainer Creatures;
    long DifficultyLevel;
    long GeneratorObject;
    CContainer GenerationTypes;
    CCreatureGenerationFamilyDef& operator=(const CCreatureGenerationFamilyDef& other);
};

CCreatureGenerationFamilyDef& CCreatureGenerationFamilyDef::operator=(const CCreatureGenerationFamilyDef& other)
{
    CParentDefClassBase::operator=(other);
    Creatures = other.Creatures;
    DifficultyLevel = other.DifficultyLevel;
    GeneratorObject = other.GeneratorObject;
    GenerationTypes = other.GenerationTypes;
    return *this;
}

int main()
{
    CCreatureGenerationFamilyDef a, b;
    a.m_marker = 7;
    a.Creatures.m_id = 201;
    a.DifficultyLevel = 102;
    b.DifficultyLevel = 0;
    a.GeneratorObject = 103;
    b.GeneratorObject = 0;
    a.GenerationTypes.m_id = 204;
    CCreatureGenerationFamilyDef& r = (b = a);
    if (&r != &b
        || b.m_marker != 7
        || g_baseAssigns != 1
        || g_containerAssigns != 2
        || g_stringAssigns != 0
        || b.Creatures.m_id != 201
        || b.DifficultyLevel != 102
        || b.GeneratorObject != 103
        || b.GenerationTypes.m_id != 204) {
        std::printf("0045c614_TEST FAIL\n");
        return 1;
    }
    std::printf("0045c614_TEST PASS\n");
    return 0;
}