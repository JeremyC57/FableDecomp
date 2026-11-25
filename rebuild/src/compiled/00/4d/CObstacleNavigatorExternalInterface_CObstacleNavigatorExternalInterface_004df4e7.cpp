// CObstacleNavigatorExternalInterface::CObstacleNavigatorExternalInterface(const CObstacleNavigatorExternalInterface&) -- copy constructor: own vtable (0x01241D04), Obstacle copied from the source's +4, DummyAgentList a new empty vector (not copied)
// Leaf: ??0CObstacleNavigatorExternalInterface@@QAE@ABV0@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004DF4E7, length 32 bytes, thiscall, `ret 4`. No callees.
// Identity (NF403, two sources): FableWin.pdb tc_register.obj names ??0CObstacleNavigatorExternalInterface@@QAE@ABV0@@Z (with the
//   CThing& constructor; no default constructor); its only caller is Clone 0x004DF4CA (`push 14h ; call new ; push esi ; call 0x004DF4E7`,
//   `return new CObstacleNavigatorExternalInterface(*this)`), whose landed row already binds this spelling here (witness 004df4ca).
//   Was the made-up ??0CObstacleNavigatorExternalInterface@@QAE@ABUCObstacleOwner@@@Z on stand-in CObstacleOwner / CRetailVectorAgent types.
// Model: the landed 0x004DF365 class (Ego_r.pdb members Obstacle +0x04, DummyAgentList +0x08, sizeof 0x14 with the STLport vector of
//   stlport/fable_containers.h, the member type 0x004DF507 uses) on the shared engine/CNavigatorExternalInterface.h base, with the
//   destructors in the inline form retail's deleting destructors prove and 0x004DF507 models: the derived one an empty inline
//   `virtual ~CObstacleNavigatorExternalInterface() { }` (0x004DF507 stores 0x01241D04 itself, then calls ~vector 0x004DF54F out of line:
//   FABLE_RETAIL_STLP_VECTOR_DTOR_OUTOFLINE, as 0x004DF507), the base one inline through the header opt-in
//   FABLE_RETAIL_CNAVIGATOREXTERNALINTERFACE_INLINE_DTOR (0x004DF507 / 0x004DF532 store 0x01241D74 themselves). The leaf is the same
//   without either define; they make this TU's vtable-emitted ??_G copies the retail bodies (??_GCObstacleNavigatorExternalInterface =
//   0x004DF507, ??_GCNavigatorExternalInterface = 0x004DF532) instead of 28-byte copies calling out-of-line destructors.
// The copy is the original source's own: Obstacle is copied and DummyAgentList is default-constructed (+8/+0xC/+0x10 zeroed), so it
//   is not the implicit copy constructor (which would copy the vector).
#pragma optimize("s", on)
#define FABLE_RETAIL_STLP_VECTOR_DTOR_OUTOFLINE
#define FABLE_RETAIL_CNAVIGATOREXTERNALINTERFACE_INLINE_DTOR
#include "engine/CNavigatorExternalInterface.h"
class CThing;
class CObstacleNavigatorExternalInterface : public CNavigatorExternalInterface {
public:
    CObstacleNavigatorExternalInterface(CThing& obstacle) : Obstacle(&obstacle) {}
    CObstacleNavigatorExternalInterface(const CObstacleNavigatorExternalInterface& other);
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
    virtual ~CObstacleNavigatorExternalInterface() { }
    virtual CNavigatorExternalInterface* Clone() const;
private:
    CThing* Obstacle;                                                        // +0x04
    std::vector<CSteeringAgent const *,std::allocator<CSteeringAgent const *> > DummyAgentList;   // +0x08
};

CObstacleNavigatorExternalInterface::CObstacleNavigatorExternalInterface(const CObstacleNavigatorExternalInterface& other)
    : Obstacle(other.Obstacle)
{
}
