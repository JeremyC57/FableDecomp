// Behaviour test for CTCPhysicsLight::FillPhysicsDebugInfoForSelf (0x00723350): counts one moving light object and one physics object.
#include <cstdio>
class CPhysicsDebugInfo { public: int MovingLightObjects, MovingStandardObjects, MovingNavigatorObjects, MovingControlledObjects, TotalNoPhysicsObjects; };
class CTCPhysicsBase { public:
    virtual ~CTCPhysicsBase() {}
    virtual void FillPhysicsDebugInfoForSelf(CPhysicsDebugInfo&) const {}
};
class CTCPhysicsLight : public CTCPhysicsBase { public:
    void FillPhysicsDebugInfoForSelf(CPhysicsDebugInfo& info) const { ++info.MovingLightObjects; ++info.TotalNoPhysicsObjects; }
};
int main()
{
    CPhysicsDebugInfo d = { 1, 2, 3, 4, 5 };
    CTCPhysicsLight l;
    const CTCPhysicsBase& b = l;
    b.FillPhysicsDebugInfoForSelf(d);
    b.FillPhysicsDebugInfoForSelf(d);
    if (d.MovingLightObjects != 3 || d.MovingStandardObjects != 2 || d.MovingNavigatorObjects != 3 || d.MovingControlledObjects != 4 || d.TotalNoPhysicsObjects != 7) { std::printf("00723350_TEST FAIL\n"); return 1; }
    std::printf("00723350_TEST PASS\n");
    return 0;
}