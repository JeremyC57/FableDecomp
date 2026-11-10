#ifndef FABLE_ENGINE_CNAVIGATOREXTERNALINTERFACE_H
#define FABLE_ENGINE_CNAVIGATOREXTERNALINTERFACE_H
// Retail opt-in (Codex pass18): original Ego_r interface slots0..24;
// native height request uses slot5, with destructor at23.
#include "engine/C3DVector.h"
#include "engine/C2DVector.h"
#include "stlport/fable_containers.h"
#ifdef FABLE_RETAIL_CNAVIGATOREXTERNALINTERFACE_AGENTLISTS
#include "rebuild_abi.h"
#endif
class CSteeringAgent;
class CNavigatorExternalInterface {
public:
 virtual bool IsMovementBlockedAt(const C3DVector&,const C3DVector&,bool&,bool&) const=0;
 virtual float GetTerrainDesirabilityAt(const C3DVector&) const=0;
 virtual float GetTerrainHeightAt(const C3DVector&) const=0;
 virtual float GetThemeHeightAt(const C3DVector&) const=0;
 virtual float FindPosZAt(const C3DVector&,bool) const=0;
 virtual float FindPosZAtIncludingThemes(const C3DVector&,bool) const=0;
 virtual float PeekNavigatorRadius(bool) const=0;
 virtual C2DVector PeekNavigatorOrientation() const=0;
 virtual C2DVector PeekNavigatorPosition2D() const=0;
 virtual C3DVector PeekNavigatorPosition3D() const=0;
 virtual float PeekNavigatorMaxSpeed(const C3DVector&) const=0;
 virtual const std::vector<CSteeringAgent const *,std::allocator<CSteeringAgent const *> >& GetNavigatorObstacles() const=0;
 virtual std::vector<CSteeringAgent const *,std::allocator<CSteeringAgent const *> >& GetNavigatorObstacles()=0;
 virtual std::vector<CSteeringAgent const *,std::allocator<CSteeringAgent const *> >* GetPNavigatorObstacles()=0;
 virtual const CSteeringAgent* GetPThingToNotAvoid() const=0;
 virtual float GetProximityForDirectApproach() const=0;
 virtual long GetNavigationLevelAt(const C3DVector&) const=0;
 virtual long GetConstantFPS() const=0;
 virtual float GetMinPathCost() const=0;
 virtual float GetMaxPathCost() const=0;
 virtual bool IsPlayer() const=0;
 virtual bool IsDead() const=0;
 virtual float GetForwardNavigationWidthExtension() const=0;
 virtual ~CNavigatorExternalInterface();
 virtual CNavigatorExternalInterface* Clone() const=0;
#ifdef FABLE_RETAIL_CNAVIGATOREXTERNALINTERFACE_AGENTLISTS
 // Retail opt-in (chunk 1021, row 0x00A47D10): the static registry of every live navigator's agent vector and its two
 // static __fastcall maintenance functions (Ego_r.pdb: `dynamic initializer for 'CNavigatorExternalInterface::AgentLists'`
 // 0x013D2834; ?RemoveAgentFromLists@CNavigatorExternalInterface@@SIXPBVCSteeringAgent@@@Z,
 // ?RemoveAgentListFromLists@CNavigatorExternalInterface@@SIXPAV?$vector@PBVCSteeringAgent@@V?$allocator@PBVCSteeringAgent@@@std@@@std@@@Z).
 static std::list<std::vector<const CSteeringAgent*>*> AgentLists;
 static void FABLE_FASTCALL RemoveAgentFromLists(const CSteeringAgent* agent);
 static void FABLE_FASTCALL RemoveAgentListFromLists(std::vector<const CSteeringAgent*>* agentList);
#endif
};
#endif
