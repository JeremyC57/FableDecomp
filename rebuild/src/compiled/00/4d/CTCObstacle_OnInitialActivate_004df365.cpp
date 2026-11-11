// CTCObstacle::OnInitialActivate() -- creates the obstacle's navigator interface (on the parent thing) and the steering agent that uses it
// Leaf: ?OnInitialActivate@CTCObstacle@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x004DF365, thiscall, `ret`.
// Identity (agentFAM03 proof, re-verified by agentFAM05): slot 6 of CTCObstacle's vtable 0x0123E28C (the slot holds this body in 1 vtable(s), owner CTCObstacle); the Xbox 360 Test PDB virtual order (third source) aligned on 13 TLC-named slots of that vtable puts OnInitialActivate(void) at this slot [gap kind GS]; Ego_r/FableWin name set: ?OnInitialActivate@CTCObstacle@@UAEXXZ; `ret` matches the signature.
// Was: CTCObstacle::vfunc_6 (rebuild/src/compiled/00/4d/CTCObstacle_vfunc6_004df365.cpp).
// No shared header for CTCObstacle, CObstacleNavigatorExternalInterface or CSteeringAgent: minimal local definitions.
// CTCObstacle on its RTTI base CTCBase with the Ego_r.pdb members (SteeringAgent +0x0c, PInterface +0x10);
// CObstacleNavigatorExternalInterface on its RTTI base CNavigatorExternalInterface (shared header) with the Ego_r.pdb members
// (Obstacle +0x04, DummyAgentList +0x08; inline ??0CObstacleNavigatorExternalInterface@@QAE@AAVCThing@@@Z, vtable 0x01241D04);
// CSteeringAgent: only its constructor ??0CSteeringAgent@@QAE@PBVCNavigatorExternalInterface@@_N@Z (0x00A1C700, Ego_r.pdb) and
// its allocation size 0x68 from this body (`push 68h ; call operator new`).
#pragma optimize("s", on)
#define FABLE_RETAIL_CTCBASE
#include "engine/ETCInterfaceType.h"
#include "engine/CTCBase.h"
#include "engine/CNavigatorExternalInterface.h"
class CSteeringAgent {
public:
    CSteeringAgent(const CNavigatorExternalInterface*, bool);   // 0x00A1C700
private:
    unsigned char Body[0x68];
};
class CObstacleNavigatorExternalInterface : public CNavigatorExternalInterface {
public:
    CObstacleNavigatorExternalInterface(CThing& obstacle) : Obstacle(&obstacle) {}
    virtual bool IsMovementBlockedAt(const C3DVector&,const C3DVector&,bool&,bool&) const;
    virtual float GetTerrainDesirabilityAt(const C3DVector&) const;
    virtual float GetTerrainHeightAt(const C3DVector&) const;
    virtual float GetThemeHeightAt(const C3DVector&) const;
    virtual float FindPosZAt(const C3DVector&,bool) const;
    virtual float FindPosZAtIncludingThemes(const C3DVector&,bool) const;
    virtual float PeekNavigatorRadius(bool) const;
    virtual C2DVector PeekNavigatorOrientation() const;
    virtual C2DVector PeekNavigatorPosition2D() const;
    virtual C3DVector PeekNavigatorPosition3D() const;
    virtual float PeekNavigatorMaxSpeed(const C3DVector&) const;
    virtual const std::vector<CSteeringAgent const *,std::allocator<CSteeringAgent const *> >& GetNavigatorObstacles() const;
    virtual std::vector<CSteeringAgent const *,std::allocator<CSteeringAgent const *> >& GetNavigatorObstacles();
    virtual std::vector<CSteeringAgent const *,std::allocator<CSteeringAgent const *> >* GetPNavigatorObstacles();
    virtual const CSteeringAgent* GetPThingToNotAvoid() const;
    virtual float GetProximityForDirectApproach() const;
    virtual long GetNavigationLevelAt(const C3DVector&) const;
    virtual long GetConstantFPS() const;
    virtual float GetMinPathCost() const;
    virtual float GetMaxPathCost() const;
    virtual bool IsPlayer() const;
    virtual bool IsDead() const;
    virtual float GetForwardNavigationWidthExtension() const;
    virtual ~CObstacleNavigatorExternalInterface();
    virtual CNavigatorExternalInterface* Clone() const;
private:
    CThing* Obstacle;                                                        // +0x04
    std::vector<CSteeringAgent const *,std::allocator<CSteeringAgent const *> > DummyAgentList;   // +0x08
};
class CTCObstacle : public CTCBase {
public:
    virtual void OnInitialActivate();                       // slot 6
private:
    CSteeringAgent* SteeringAgent;                          // +0x0c
    CObstacleNavigatorExternalInterface* PInterface;        // +0x10
};
void CTCObstacle::OnInitialActivate()
{
    PInterface = new CObstacleNavigatorExternalInterface(*ParentThing);
    SteeringAgent = new CSteeringAgent(PInterface, true);
}
