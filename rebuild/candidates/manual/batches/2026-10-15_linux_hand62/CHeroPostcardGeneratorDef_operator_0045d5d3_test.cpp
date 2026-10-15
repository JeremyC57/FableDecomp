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

struct CHeroPostcardGeneratorDef : CParentDefClassBase {
    C2DVector MeshAreaTLPos;
    C2DVector MeshAreaBRPos;
    C3DVector DollCentreOffset;
    float DollBoundingSphereRadius;
    CHeroPostcardGeneratorDef& operator=(const CHeroPostcardGeneratorDef& other);
};

CHeroPostcardGeneratorDef& CHeroPostcardGeneratorDef::operator=(const CHeroPostcardGeneratorDef& other)
{
    CParentDefClassBase::operator=(other);
    MeshAreaTLPos = other.MeshAreaTLPos;
    MeshAreaBRPos = other.MeshAreaBRPos;
    DollCentreOffset = other.DollCentreOffset;
    DollBoundingSphereRadius = other.DollBoundingSphereRadius;
    return *this;
}

int main()
{
    CHeroPostcardGeneratorDef a, b;
    a.m_marker = 7;
    a.MeshAreaTLPos.X = 1.25f; a.MeshAreaTLPos.Y = 1.75f;
    b.MeshAreaTLPos.X = b.MeshAreaTLPos.Y = 0.0f;
    a.MeshAreaBRPos.X = 2.25f; a.MeshAreaBRPos.Y = 2.75f;
    b.MeshAreaBRPos.X = b.MeshAreaBRPos.Y = 0.0f;
    a.DollCentreOffset.X = 3.25f; a.DollCentreOffset.Y = 3.5f; a.DollCentreOffset.Z = 3.75f;
    b.DollCentreOffset.X = b.DollCentreOffset.Y = b.DollCentreOffset.Z = 0.0f;
    a.DollBoundingSphereRadius = 4.5f;
    b.DollBoundingSphereRadius = 0.0f;
    CHeroPostcardGeneratorDef& r = (b = a);
    if (&r != &b
        || b.m_marker != 7
        || g_baseAssigns != 1
        || g_containerAssigns != 0
        || g_stringAssigns != 0
        || b.MeshAreaTLPos.X != 1.25f || b.MeshAreaTLPos.Y != 1.75f
        || b.MeshAreaBRPos.X != 2.25f || b.MeshAreaBRPos.Y != 2.75f
        || b.DollCentreOffset.X != 3.25f || b.DollCentreOffset.Y != 3.5f || b.DollCentreOffset.Z != 3.75f
        || b.DollBoundingSphereRadius != 4.5f) {
        std::printf("0045d5d3_TEST FAIL\n");
        return 1;
    }
    std::printf("0045d5d3_TEST PASS\n");
    return 0;
}
