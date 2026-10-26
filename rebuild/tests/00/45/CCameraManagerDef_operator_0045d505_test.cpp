#include <cstdio>

static int g_baseAssigns = 0;
static int g_containerAssigns = 0;
static int g_stringAssigns = 0;

class CParentDefClassBase { public:
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

class CCharString { public:
    long m_id;
    CCharString() : m_id(0) { }
    CCharString& operator=(const CCharString& other)
    {
        m_id = other.m_id;
        ++g_stringAssigns;
        return *this;
    }
};

class CDefString { public: long TablePos; };
class CRGBColour { public: unsigned long m_packed; };
class C2DVector { public: float X, Y; };
class C3DVector { public: float X, Y, Z; };

class CCameraManagerDef : public CParentDefClassBase { public:
    CContainer CameraList;
    CContainer SpecialityCameraList;
    long InitialMode;
    long ZTargetMode;
    long ProjectileWeaponMode;
    float BaseHeightToKeepAboveFlatLandscape;
    float BaseHeightToKeepAboveWater;
    float MaxHeightRelativeToGround;
    float MaxHeightStepForPointAcceptance;
    float AngleBetweenGroundNormalAndVerticalForLandscapeToBeConsideredNotFlat;
    CCameraManagerDef& operator=(const CCameraManagerDef& other);
};

CCameraManagerDef& CCameraManagerDef::operator=(const CCameraManagerDef& other)
{
    CParentDefClassBase::operator=(other);
    CameraList = other.CameraList;
    SpecialityCameraList = other.SpecialityCameraList;
    InitialMode = other.InitialMode;
    ZTargetMode = other.ZTargetMode;
    ProjectileWeaponMode = other.ProjectileWeaponMode;
    BaseHeightToKeepAboveFlatLandscape = other.BaseHeightToKeepAboveFlatLandscape;
    BaseHeightToKeepAboveWater = other.BaseHeightToKeepAboveWater;
    MaxHeightRelativeToGround = other.MaxHeightRelativeToGround;
    MaxHeightStepForPointAcceptance = other.MaxHeightStepForPointAcceptance;
    AngleBetweenGroundNormalAndVerticalForLandscapeToBeConsideredNotFlat = other.AngleBetweenGroundNormalAndVerticalForLandscapeToBeConsideredNotFlat;
    return *this;
}

int main()
{
    CCameraManagerDef a, b;
    a.m_marker = 7;
    a.CameraList.m_id = 201;
    a.SpecialityCameraList.m_id = 202;
    a.InitialMode = 103;
    b.InitialMode = 0;
    a.ZTargetMode = 104;
    b.ZTargetMode = 0;
    a.ProjectileWeaponMode = 105;
    b.ProjectileWeaponMode = 0;
    a.BaseHeightToKeepAboveFlatLandscape = 6.5f;
    b.BaseHeightToKeepAboveFlatLandscape = 0.0f;
    a.BaseHeightToKeepAboveWater = 7.5f;
    b.BaseHeightToKeepAboveWater = 0.0f;
    a.MaxHeightRelativeToGround = 8.5f;
    b.MaxHeightRelativeToGround = 0.0f;
    a.MaxHeightStepForPointAcceptance = 9.5f;
    b.MaxHeightStepForPointAcceptance = 0.0f;
    a.AngleBetweenGroundNormalAndVerticalForLandscapeToBeConsideredNotFlat = 10.5f;
    b.AngleBetweenGroundNormalAndVerticalForLandscapeToBeConsideredNotFlat = 0.0f;
    CCameraManagerDef& r = (b = a);
    if (&r != &b
        || b.m_marker != 7
        || g_baseAssigns != 1
        || g_containerAssigns != 2
        || g_stringAssigns != 0
        || b.CameraList.m_id != 201
        || b.SpecialityCameraList.m_id != 202
        || b.InitialMode != 103
        || b.ZTargetMode != 104
        || b.ProjectileWeaponMode != 105
        || b.BaseHeightToKeepAboveFlatLandscape != 6.5f
        || b.BaseHeightToKeepAboveWater != 7.5f
        || b.MaxHeightRelativeToGround != 8.5f
        || b.MaxHeightStepForPointAcceptance != 9.5f
        || b.AngleBetweenGroundNormalAndVerticalForLandscapeToBeConsideredNotFlat != 10.5f) {
        std::printf("0045d505_TEST FAIL\n");
        return 1;
    }
    std::printf("0045d505_TEST PASS\n");
    return 0;
}