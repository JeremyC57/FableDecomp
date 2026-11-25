// Behaviour test for CObstacleNavigatorExternalInterface::CObstacleNavigatorExternalInterface(const CObstacleNavigatorExternalInterface&)
// @ 0x004DF4E7 (the copy constructor Clone 0x004DF4CA calls): the copy takes the source's Obstacle, gets an empty DummyAgentList of its
// own even when the source's list holds agents, and is a CObstacleNavigatorExternalInterface (its own vtable).
#include <cstdio>
#define FABLE_RETAIL_STLP_VECTOR_DTOR_OUTOFLINE
#define FABLE_RETAIL_CNAVIGATOREXTERNALINTERFACE_INLINE_DTOR
#include "engine/CNavigatorExternalInterface.h"
class CThing { public: long id; };
class CObstacleNavigatorExternalInterface : public CNavigatorExternalInterface {
public:
    CObstacleNavigatorExternalInterface(CThing& obstacle) : Obstacle(&obstacle) {}
    CObstacleNavigatorExternalInterface(const CObstacleNavigatorExternalInterface& other);
    virtual bool IsMovementBlockedAt(const C3DVector&,const C3DVector&,bool&,bool&) const { return false; }
    virtual float GetTerrainDesirabilityAt(const C3DVector&) const { return 0.0f; }
    virtual float GetTerrainHeightAt(const C3DVector&) const { return 0.0f; }
    virtual float GetThemeHeightAt(const C3DVector&) const { return 0.0f; }
    virtual float FindPosZAt(const C3DVector&,bool) const { return 0.0f; }
    virtual float FindPosZAtIncludingThemes(const C3DVector&,bool) const { return 0.0f; }
    virtual float PeekNavigatorRadius(bool) const { return 0.0f; }
    virtual C2DVector PeekNavigatorOrientation() const { return C2DVector(0.0f, 0.0f); }
    virtual C2DVector PeekNavigatorPosition2D() const { return C2DVector(0.0f, 0.0f); }
    virtual C3DVector PeekNavigatorPosition3D() const { C3DVector v; return v; }
    virtual float PeekNavigatorMaxSpeed(const C3DVector&) const { return 0.0f; }
    virtual const std::vector<CSteeringAgent const *,std::allocator<CSteeringAgent const *> >& GetNavigatorObstacles() const { return DummyAgentList; }
    virtual std::vector<CSteeringAgent const *,std::allocator<CSteeringAgent const *> >& GetNavigatorObstacles() { return DummyAgentList; }
    virtual std::vector<CSteeringAgent const *,std::allocator<CSteeringAgent const *> >* GetPNavigatorObstacles() { return &DummyAgentList; }
    virtual const CSteeringAgent* GetPThingToNotAvoid() const { return 0; }
    virtual float GetProximityForDirectApproach() const { return 0.0f; }
    virtual long GetNavigationLevelAt(const C3DVector&) const { return 0; }
    virtual long GetConstantFPS() const { return 0; }
    virtual float GetMinPathCost() const { return 0.0f; }
    virtual float GetMaxPathCost() const { return 0.0f; }
    virtual bool IsPlayer() const { return false; }
    virtual bool IsDead() const { return false; }
    virtual float GetForwardNavigationWidthExtension() const { return 0.0f; }
    virtual ~CObstacleNavigatorExternalInterface() { }
    virtual CNavigatorExternalInterface* Clone() const { return new CObstacleNavigatorExternalInterface(*this); }
    CThing* PeekObstacle() const { return Obstacle; }
private:
    CThing* Obstacle;                                                        // +0x04
    std::vector<CSteeringAgent const *,std::allocator<CSteeringAgent const *> > DummyAgentList;   // +0x08
};
// The row's body.
CObstacleNavigatorExternalInterface::CObstacleNavigatorExternalInterface(const CObstacleNavigatorExternalInterface& other)
    : Obstacle(other.Obstacle)
{
}
// ~vector is out of line in the row's TU (0x004DF54F); the fixture gives it the STLport body.
namespace std {
template <> vector<const CSteeringAgent*, allocator<const CSteeringAgent*> >::~vector() { if (_M_start) free(_M_start); }
}
int main()
{
    CThing a; a.id = 7;
    CObstacleNavigatorExternalInterface src(a);
    const CSteeringAgent* agent = (const CSteeringAgent*)&a;
    src.GetNavigatorObstacles().push_back(agent);
    src.GetNavigatorObstacles().push_back(agent);
    CObstacleNavigatorExternalInterface copy(src);
    CNavigatorExternalInterface* cl = src.Clone();
    CObstacleNavigatorExternalInterface* c2 = (CObstacleNavigatorExternalInterface*)cl;
    bool ok = sizeof(CObstacleNavigatorExternalInterface) == 0x14
           && copy.PeekObstacle() == &a && copy.GetNavigatorObstacles().size() == 0 && copy.GetPNavigatorObstacles()->empty()
           && src.GetNavigatorObstacles().size() == 2 && src.PeekObstacle() == &a
           && c2->PeekObstacle() == &a && c2->GetNavigatorObstacles().size() == 0 && !cl->IsDead()
           && *(void**)&copy == *(void**)&src;
    delete cl;
    if (!ok) { std::printf("004df4e7_TEST FAIL\n"); return 1; }
    std::printf("004df4e7_TEST PASS\n");
    return 0;
}
